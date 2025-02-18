## A high-level lobby wrapper for EOSG
class_name HLobby
extends BaseClass


#region Signals

## Emitted when anything about the lobby changes. Except when kicked or lobby is destroyed. Connect to kicked_from_lobby for the latter.
signal lobby_updated

## Emitted when the current user is kicked from lobby or the lobby was destroyed
signal kicked_from_lobby

## Emitted when another member is promoted to the lobby owner. Use get_owner to get the new owner
signal lobby_owner_changed

## Emitted when the current user in the lobby receives data through the RTC data channel
signal rtc_data_received

#endregion


#region Public vars

var lobby_id: String
var owner_product_user_id: String
var bucket_id: String

var max_members: int
var available_slots: int
var members: Array[HLobbyMember] = []
var allow_invites: bool
var permission_level: EOS.Lobby.LobbyPermissionLevel

var presence_enabled: bool
var allow_host_migration: bool

## [Array] of [Dictionary] having {"key": String, "value": Variant, "visibility": [enum EOS.Lobby.LobbyAttributeVisibility] }
var attributes = []

var rtc_room_enabled: bool
var rtc_room_name: String
var rtc_room_connected: bool

var allow_join_by_id: bool
var rejoin_after_kick_requires_invite: bool
var allowed_platform_ids: Array

#endregion


#region Private vars

var _log = HLog.logger("HLobby")

var _attributes_to_add = []
var _lobby_details: EOSGLobbyDetails

var _connected_to_lobby_events = false
var _connected_to_rtc_events = false
var _notif_rtc_parti_status_changed = EOS.NotificationIdInvalid
var _notif_rtc_audio_parti_updated = EOS.NotificationIdInvalid
var _notif_rtc_data_parti_updated = EOS.NotificationIdInvalid
var _notif_rtc_data_data_received = EOS.NotificationIdInvalid

#endregion


#region Built-in methods

func _init() -> void:
	super._init("HLobby")


func _notification(what: int) -> void:
	if what == NOTIFICATION_PREDELETE:
		_lobby_details = null

#endregion


#region Public methods

## Check whether the lobby is valid
func is_valid() -> bool:
	return lobby_id != ""


## Check whether a user is the lobby owner
## By default it will check the current logged in user
func is_owner(product_user_id = HAuth.product_user_id) -> bool:
	return owner_product_user_id == product_user_id


## Returns a [HLobbyMember] based on product user id or null
func get_member_by_product_user_id(product_user_id: String) -> HLobbyMember:
	for mem in members:
		if mem.product_user_id == product_user_id:
			return mem
	return null


## Returns the current lobby member [HLobbyMember] or null
func get_current_member() -> HLobbyMember:
	for mem in members:
		if mem.product_user_id == HAuth.product_user_id:
			return mem
	return null


## Returns [HLobbyMember] or null
func get_owner() -> HLobbyMember:
	for mem in members:
		if mem.product_user_id == owner_product_user_id:
			return mem
	return null


## Intialize the lobby from a lobby_id
func init_from_id(p_lobby_id: String):
	_log.debug("Initializing from lobby id: %s" % p_lobby_id)
	lobby_id = p_lobby_id

	_copy_lobby_data()

	if is_valid() and get_current_member() and not _connected_to_lobby_events:
		_connected_to_lobby_events = true
		IEOS.lobby_interface_lobby_update_received_callback.connect(_on_lobby_update_received_callback)
		IEOS.lobby_interface_lobby_member_update_received_callback.connect(_on_lobby_interface_lobby_member_update_received_callback)
		IEOS.lobby_interface_lobby_member_status_received_callback.connect(_on_lobby_member_status_received_callback)
		IEOS.lobby_interface_rtc_room_connection_changed_callback.connect(_on_lobby_interface_rtc_room_connection_changed_callback)
		IEOS.lobby_interface_leave_lobby_requested_callback.connect(_on_lobby_interface_leave_lobby_requested_callback)

		# RTC
		IEOS.rtc_interface_participant_status_changed.connect(_on_rtc_interface_participant_status_changed)
		IEOS.rtc_audio_participant_updated.connect(_on_rtc_audio_participant_updated)
		IEOS.rtc_data_participant_updated.connect(_on_rtc_data_participant_updated)
		IEOS.rtc_data_data_received.connect(_on_rtc_data_data_received)



## Returns [Dictionary] or empty [Dictionary] if not found.
func get_attribute(key: String):
	for attr in attributes:
		if attr.key == key:
			return attr
	return {}


## Add an attribute to this lobby. Make sure to call update_async to actually update the lobby. Returns true if success.
func add_attribute(key: String, value: Variant, visibility = EOS.Lobby.LobbyAttributeVisibility.Public) -> bool:
	var attr = make_attribute(key, value, visibility)
	_attributes_to_add.append(attr)
	return true


## Returns the current user's attribute as [Dictionary] or empty [Dictionary] if not found.
func get_current_member_attribute(key: String):
	var member = get_member_by_product_user_id(HAuth.product_user_id)
	if not member:
		return {}
	for attr in member.attributes:
		if attr.key == key:
			return attr
	return {}


## Add an attribute to the current user in the lobby. Make sure to call update_async to actually update the lobby. Returns true if success.
func add_current_member_attribute(key: String, value: Variant, visibility = EOS.Lobby.LobbyAttributeVisibility.Public) -> bool:
	var member = get_member_by_product_user_id(HAuth.product_user_id)
	if not member:
		return false
	
	var attr = make_attribute(key, value, visibility)
	member._attributes_to_add.append(attr)
	return true


## Helper function to create an attribute with key, value and visibility
static func make_attribute(key: String, value: Variant, visibility = EOS.Lobby.LobbyAttributeVisibility.Public) -> Dictionary:
	return {
		key = key,
		value = value,
		visibility = visibility
	}


## Update the lobby, if any changes are pending
func update_async() -> bool:
	_log.debug("Updating lobby: lobby_id=%s" % lobby_id)
	if not is_valid():
		_log.error("Cannot update invalid lobby")
		return false

	var opts = EOS.Lobby.UpdateLobbyModificationOptions.new()
	opts.lobby_id = lobby_id

	var lobby_mod_ret = EOS.Lobby.LobbyInterface.update_lobby_modification(opts)
	if not EOS.is_success(lobby_mod_ret):
		_log.error("Failed to create lobby modification: result_code=%s" % EOS.result_str(lobby_mod_ret))
		return false
	
	var lobby_mod: EOSGLobbyModification = lobby_mod_ret.lobby_modification

	if is_owner(HAuth.product_user_id):
		_log.debug("Updating lobby as owner...")
		if allowed_platform_ids.size() > 0:
			var set_allowed_platform_ids_ret = lobby_mod.set_allowed_platform_ids(allowed_platform_ids)
			if not EOS.is_success(set_allowed_platform_ids_ret):
				_log.error("Failed to set allowed platform ids on lobby modification: result_code=%s" % EOS.result_str(set_allowed_platform_ids_ret))
				return false

		var set_bucket_ret = lobby_mod.set_bucket_id(bucket_id)
		if not EOS.is_success(set_bucket_ret):
			_log.error("Failed to set bucket id on lobby modification: result_code=%s" % EOS.result_str(set_bucket_ret))
			return false

		var set_invites_allowed_ret = lobby_mod.set_invites_allowed(allow_invites)
		if not EOS.is_success(set_invites_allowed_ret):
			_log.error("Failed to set invites allowed on lobby modification: result_code=%s" % EOS.result_str(set_invites_allowed_ret))
			return false

		var set_max_members_ret = lobby_mod.set_max_members(max_members)
		if not EOS.is_success(set_max_members_ret):
			_log.error("Failed to set max members on lobby modification: result_code=%s" % EOS.result_str(set_max_members_ret))
			return false
		
		var set_perm_ret = lobby_mod.set_permission_level(permission_level)
		if not EOS.is_success(set_perm_ret):
			_log.error("Failed to set permission level on lobby modification: result_code=%s" % EOS.result_str(set_perm_ret))
			return false

		var new_attrs = attributes.duplicate(true)
		new_attrs.append_array(_attributes_to_add)
		for attr in new_attrs:
			var add_ret = lobby_mod.add_attribute(attr.key, attr.value, attr.visibility)
			if not EOS.is_success(add_ret):
				_log.error("Failed to add attribute on lobby modification. Skipping this attribute: result_code=%s, key=%s, value=%s, visibility=%s" % [EOS.result_str(add_ret), attr.key, attr.value, attr.visibility])
		_attributes_to_add = []
	
	else:
		_log.debug("Updating lobby as user...")

	var member: HLobbyMember = get_member_by_product_user_id(HAuth.product_user_id)
	if member:
		var mem_attrs = member.attributes.duplicate(true)
		mem_attrs.append_array(member._attributes_to_add)
		for attr in mem_attrs:
			var add_ret = lobby_mod.add_member_attribute(attr.key, attr.value, attr.visibility)
			if not EOS.is_success(add_ret):
				_log.error("Failed to add member attribute on lobby modification. Skipping this attribute: result_code=%s, key=%s, value=%s, visibility=%s" % [EOS.result_str(add_ret), attr.key, attr.value, attr.visibility])
		member._attributes_to_add = []

	var update_opts = EOS.Lobby.UpdateLobbyOptions.new()
	update_opts.lobby_modification = lobby_mod
	EOS.Lobby.LobbyInterface.update_lobby(update_opts)
	
	var update_ret = await IEOS.lobby_interface_update_lobby_callback
	if not EOS.is_success(update_ret):
		_log.error("Failed to update lobby: result_code=%s. lobby_id=%s" % [EOS.result_str(update_ret), lobby_id])
		return false
	
	_log.debug("Lobby updated successfully: lobby_id=%s" % lobby_id)

	_copy_lobby_data()
	lobby_updated.emit()

	return true
	

## Leave the lobby
## Returns true if the leave was successful
func leave_async() -> bool:
	if not is_valid():
		return false

	_log.debug("Leaving lobby: lobby_id=%s" % lobby_id)
	var opts = EOS.Lobby.LeaveLobbyOptions.new()
	opts.lobby_id = lobby_id
	EOS.Lobby.LobbyInterface.leave_lobby(opts)

	var ret = await IEOS.lobby_interface_leave_lobby_callback
	if not EOS.is_success(ret):
		_log.error("Failed to leave lobby: result_code=%s" % EOS.result_str(ret))
		return false
	
	_log.debug("Leave lobby successful: lobby_id=%s" % lobby_id)
	_disconnect_from_signals()
	return true


## Destroy the lobby. Only the owner can call this.
## Returns true if the destroy was successful
func destroy_async() -> bool:
	if not is_valid():
		return false
	
	if not is_owner():
		return false

	_log.debug("Destroying lobby: lobby_id=%s" % lobby_id)
	var opts = EOS.Lobby.DestroyLobbyOptions.new()
	opts.lobby_id = lobby_id
	EOS.Lobby.LobbyInterface.destroy_lobby(opts)

	var ret = await IEOS.lobby_interface_destroy_lobby_callback
	if not EOS.is_success(ret):
		_log.error("Failed to destroy lobby: result_code=%s" % EOS.result_str(ret))
		return false
	
	_log.debug("Destroy lobby successful: lobby_id=%s" % lobby_id)

	lobby_id = ""
	_disconnect_from_signals()
	kicked_from_lobby.emit()
	return true


## Returns true if the current user can invite another user
## p_product_user_id: The product user id of the user to invite
func can_invite(p_product_user_id: String) -> bool:
	if not is_valid():
		return false
	
	if not get_current_member():
		return false
	
	if (not allow_invites) or (not available_slots) or (members.size() >= max_members):
		return false
	
	# Check if in lobby
	var mem = get_member_by_product_user_id(p_product_user_id)
	if mem:
		return false
	
	return true


## Send data as a Variant to all users in the lobby's RTC data channel.
## Note: max packet size is [const EOS.RTCData.MAX_PACKET_SIZE_BYTES] bytes
## This does not encode objects by default. Use rtc_send_data_raw
## to encode arbitary data.
func rtc_send_data(data: Variant) -> bool: 
	return rtc_send_data_raw(var_to_bytes(data))


## Send raw bytes as PackedByteArray to all users in the lobby's RTC data channel
## Note: max packet size is [const EOS.RTCData.MAX_PACKET_SIZE_BYTES] bytes
func rtc_send_data_raw(data: PackedByteArray) -> bool:
	var opts = EOS.RTCData.SendDataOptions.new()
	opts.room_name = rtc_room_name
	opts.data = data
	var res = EOS.RTCData.RTCDataInterface.send_data(opts)
	return EOS.is_success(res)

#endregion


#region Private methods

func _copy_lobby_data():
	var copy_opts = EOS.Lobby.CopyLobbyDetailsOptions.new()
	copy_opts.lobby_id = lobby_id
	var copy_ret = EOS.Lobby.LobbyInterface.copy_lobby_details(copy_opts)

	if not EOS.is_success(copy_ret):
		_log.error("Failed to copy lobby details: result_code=%s" % EOS.result_str(copy_ret))
		return

	_init_from_details(copy_ret.lobby_details)


func _init_from_details(lobby_details: EOSGLobbyDetails):
	_log.debug("Initializing from EOSGLobbyDetails")

	_lobby_details = lobby_details
	var copy_ret = lobby_details.copy_info()
	if not EOS.is_success(copy_ret):
		_log.error("Failed to copy lobby info: result_code=%s" % EOS.result_str(copy_ret))
		return

	var details: Dictionary = copy_ret.lobby_details

	HLog._check_diff_and_set(self, "lobby_id", details.lobby_id)
	HLog._check_diff_and_set(self, "owner_product_user_id", details.lobby_owner_user_id)
	HLog._check_diff_and_set(self, "permission_level", details.permission_level)
	HLog._check_diff_and_set(self, "available_slots", details.available_slots)
	HLog._check_diff_and_set(self, "max_members", details.max_members)
	HLog._check_diff_and_set(self, "allow_invites", details.allow_invites)
	HLog._check_diff_and_set(self, "bucket_id", details.bucket_id)
	HLog._check_diff_and_set(self, "allow_host_migration", details.allow_host_migration)
	HLog._check_diff_and_set(self, "rtc_room_enabled", details.rtc_room_enabled)
	HLog._check_diff_and_set(self, "allow_join_by_id", details.allow_join_by_id)
	HLog._check_diff_and_set(self, "rejoin_after_kick_requires_invite", details.rejoin_after_kick_requires_invite)
	HLog._check_diff_and_set(self, "presence_enabled", details.presence_enabled)
	HLog._check_diff_and_set(self, "allowed_platform_ids", details.allowed_platform_ids)

	# Get attributes
	var attr_count = lobby_details.get_attribute_count()
	# _log.verbose("Got attributes: count=%s" % attr_count)
	var orig_attributes = attributes.duplicate(true)
	attributes = []
	for attr_idx in attr_count:
		var copy_attr_ret = lobby_details.copy_attribute_by_index(attr_idx)
		if not EOS.is_success(copy_attr_ret):
			_log.error("Failed to copy EOSGLobbyDetails attribute by index: result_code=%s" % EOS.result_str(copy_attr_ret))
			continue
		var attr = copy_attr_ret.attribute
		attr = make_attribute(attr.data.key, attr.data.value, attr.visibility)
		attributes.append(attr)
	
	HLog._check_attr_diff(orig_attributes, attributes, "lobby")

	# Get members
	var members_count = lobby_details.get_member_count()
	# _log.verbose("Got members: count=%s" % members_count)
	var old_members = members.duplicate()
	
	members = []
	for member_idx in members_count:
		var member_product_user_id = lobby_details.get_member_by_index(member_idx)
		
		var member = old_members.filter(func(m): return m.product_user_id == member_product_user_id)
		if member.size() > 0:
			_log.verbose("Updating existing lobby member")
			member = member[0]
		else:
			_log.verbose("Creating new lobby member")
			member = HLobbyMember.new(self)
			member.product_user_id = member_product_user_id
		
		members.append(member)

		# Member attributes
		var mem_attr_count = lobby_details.get_member_attribute_count(member_product_user_id)
		# _log.verbose("Got member attributes: count=%s" % mem_attr_count)

		var orig_mem_attr = member.attributes.duplicate(true)
		member.attributes = []
		for mem_attr_idx in mem_attr_count:
			var copy_mem_attr_ret = lobby_details.copy_member_attribute_by_index(member_product_user_id, mem_attr_idx)
			if not EOS.is_success(copy_mem_attr_ret):
				_log.error("Failed to copy EOSGLobbyDetails member attribute by index: result_code=%s" % EOS.result_str(copy_mem_attr_ret))
				continue
			var attr = copy_mem_attr_ret.attribute
			member.attributes.append(make_attribute(attr.data.key, attr.data.value, attr.visibility))
		HLog._check_attr_diff(orig_mem_attr, member.attributes, "member:%s" % member.product_user_id)
	
	_subscribe_rtc_events()


func _on_lobby_update_received_callback(data: Dictionary):
	if not is_valid() or data.lobby_id != lobby_id:
		return
	_log.verbose("Got lobby update: lobby_id=%s" % lobby_id)
	_copy_lobby_data()
	lobby_updated.emit()


func _on_lobby_interface_lobby_member_update_received_callback(data: Dictionary):
	if not is_valid() or data.lobby_id != lobby_id:
		return
	_log.verbose("Got lobby member update: lobby_id=%s" % lobby_id)
	_copy_lobby_data()
	lobby_updated.emit()


func _on_lobby_member_status_received_callback(data: Dictionary):
	if not is_valid() or data.lobby_id != lobby_id:
		return
	var member_id = data.target_user_id
	var status = data.current_status

	_log.verbose("Got lobby member status: member_id=%s, status=%s" % [member_id, EOS.Lobby.LobbyMemberStatus.find_key(status)])

	var update_lobby = true
	if HAuth.product_user_id == data.target_user_id:
		if status == EOS.Lobby.LobbyMemberStatus.Closed or status == EOS.Lobby.LobbyMemberStatus.Kicked or status == EOS.Lobby.LobbyMemberStatus.Disconnected:
			_log.debug("Kicked from lobby: lobby_id=%s" % lobby_id)
			lobby_id = ""
			_disconnect_from_signals()
			kicked_from_lobby.emit()
			update_lobby = false

	if update_lobby:
		_copy_lobby_data()
		lobby_updated.emit()
	
	if status == EOS.Lobby.LobbyMemberStatus.Promoted:
		_log.debug("Lobby owner changed: lobby_id=%s" % lobby_id)
		lobby_owner_changed.emit()


func _on_lobby_interface_rtc_room_connection_changed_callback(data: Dictionary):
	if not is_valid() or data.lobby_id != lobby_id:
		return
	
	if data.local_user_id != HAuth.product_user_id:
		return

	var new_rtc_room_connected = data.is_connected
	var disconnect_reason = data.disconnect_reason
	_log.debug("Lobby rtc room connection changed: lobby_id=%s, is_connected=%s, disconnect_reason=%s" % [lobby_id, new_rtc_room_connected, EOS.result_str(disconnect_reason)])

	HLog._check_diff_and_set(self, "rtc_room_connected", new_rtc_room_connected)

	var mem = get_current_member()
	if mem:
		var did_update = HLog._check_diff_and_set_dict(mem.rtc_state, "is_in_rtc_room", new_rtc_room_connected, "member:" + mem.product_user_id)
		if not new_rtc_room_connected:
			did_update = HLog._check_diff_and_set_dict(mem.rtc_state, "is_talking", false, "member:" + mem.product_user_id) or did_update
		if did_update:
			mem.rtc_state_updated.emit()
		
	lobby_updated.emit()


func _on_lobby_interface_leave_lobby_requested_callback(data: Dictionary):
	if not is_valid() or data.lobby_id != lobby_id:
		return

	_log.debug("Lobby leave requested from UI: lobby_id=%s" % lobby_id)
	leave_async()


func _subscribe_rtc_events():
	if not is_valid() or not rtc_room_enabled:
		return
	
	var mem = get_member_by_product_user_id(HAuth.product_user_id)
	if mem == null:
		return
	
	_log.verbose("Subscribing to rtc events")
	
	
	# _log.verbose("Subscribing to lobby rtc events")
	var did_update = false

	# RTC room name
	var opts = EOS.Lobby.GetRtcRoomNameOptions.new()
	opts.lobby_id = lobby_id
	var ret = EOS.Lobby.LobbyInterface.get_rtc_room_name(opts)
	if not EOS.is_success(ret):
		_log.error("Failed to get rtc room name: result_code=%s" % EOS.result_str(ret))
	else:
		did_update = HLog._check_diff_and_set(self, "rtc_room_name", ret.rtc_room_name) or did_update
	
	# RTC connection status
	var is_conn_opts = EOS.Lobby.IsRtcRoomConnectedOptions.new()
	is_conn_opts.lobby_id = lobby_id
	var is_conn_ret = EOS.Lobby.LobbyInterface.is_rtc_room_connected(is_conn_opts)
	if not EOS.is_success(is_conn_ret):
		_log.error("Failed to get rtc room connection status: result_code=%s" % EOS.result_str(is_conn_ret))
	else:
		did_update = HLog._check_diff_and_set(self, "rtc_room_connected", is_conn_ret.is_connected) or did_update
	

	if rtc_room_name and not _connected_to_rtc_events:
		_connected_to_rtc_events = true
	
		# RTC room participant status changes
		var notify_parti_status_opts = EOS.RTC.AddNotifyParticipantStatusChangedOptions.new()
		notify_parti_status_opts.room_name = rtc_room_name
		_notif_rtc_parti_status_changed = EOS.RTC.RTCInterface.add_notify_participant_status_changed(notify_parti_status_opts)
		
		# RTC audio updates
		var notify_audio_parti_updated = EOS.RTCAudio.AddNotifyParticipantUpdatedOptions.new()
		notify_audio_parti_updated.room_name = rtc_room_name
		_notif_rtc_audio_parti_updated = EOS.RTCAudio.RTCAudioInterface.add_notify_participant_updated(notify_audio_parti_updated)


		# RTC Data participant updated
		var notify_rtc_data_parti_updated = EOS.RTCData.AddNotifyParticipantUpdatedOptions.new()
		notify_rtc_data_parti_updated.room_name = rtc_room_name
		_notif_rtc_data_parti_updated = EOS.RTCData.RTCDataInterface.add_notify_participant_updated(notify_rtc_data_parti_updated)

		# RTC Data data received
		var notify_data_received_opts = EOS.RTCData.AddNotifyDataReceivedOptions.new()
		notify_data_received_opts.room_name = rtc_room_name
		_notif_rtc_data_data_received = EOS.RTCData.RTCDataInterface.add_notify_data_received(notify_data_received_opts)
		
	if did_update:
		# _log.verbose("Lobby rtc values updated: lobby_id=%s, rtc_room_name=%s, rtc_room_connected=%s" % [lobby_id, rtc_room_name, rtc_room_connected])
		lobby_updated.emit()


func _on_rtc_interface_participant_status_changed(data: Dictionary):
	if not is_valid() or data.room_name != rtc_room_name:
		return
	
	var participant_id = data.participant_id
	var participant_status = data.participant_status

	var mem = get_member_by_product_user_id(participant_id)
	if not mem:
		_log.error("Got rtc participant status changed for unknown participant: participant_id=%s" % participant_id)
		return

	if participant_status == EOS.RTC.ParticipantStatus.Joined:
		mem.rtc_state.is_in_rtc_room = true
		mem.rtc_state_updated.emit()
	elif participant_status == EOS.RTC.ParticipantStatus.Left:
		mem.rtc_state.is_in_rtc_room = false
		mem.rtc_state_updated.emit()
	
	lobby_updated.emit()


func _on_rtc_audio_participant_updated(data: Dictionary):
	if not is_valid() or data.room_name != rtc_room_name:
		return
	
	var participant_puid = data.participant_id
	var is_talking = data.speaking
	var is_audio_disabled = data.audio_status != EOS.RTCAudio.AudioStatus.Enabled
	var is_hard_muted = data.audio_status == EOS.RTCAudio.AudioStatus.AdminDisabled
	
	var mem = get_member_by_product_user_id(data.participant_id)
	if mem == null:
		_log.error("Got rtc audio participant update for unknown participant: participant_id=%s" % participant_puid)
		return

	# Update talking status
	var did_change = HLog._check_diff_and_set_dict(mem.rtc_state, "is_talking", is_talking, "member:" + mem.product_user_id)

	# Update audio output status for other players
	if mem.product_user_id != HAuth.product_user_id:
		did_change = HLog._check_diff_and_set_dict(mem.rtc_state, "is_audio_output_disabled", is_audio_disabled, "member:" + mem.product_user_id) or did_change
	else:
		# I could have been hard-muted by the lobby owner
		did_change = HLog._check_diff_and_set_dict(mem.rtc_state, "is_hard_muted", is_hard_muted, "member:" + mem.product_user_id) or did_change

	if did_change:
		mem.rtc_state_updated.emit()
		lobby_updated.emit()


func _on_rtc_data_participant_updated(data: Dictionary):
	if not is_valid() or data.room_name != rtc_room_name:
		return
	
	var participant_puid = data.participant_id
	var data_status = data.data_status

	_log.debug("Got rtc data participant update: participant_id=%s, data_status=%s" % [participant_puid, EOS.RTCData.DataStatus.find_key(data_status)])


func _on_rtc_data_data_received(p_data: Dictionary):
	if not is_valid() or p_data.room_name != rtc_room_name:
		return
	
	rtc_data_received.emit(p_data.data)


func _disconnect_from_signals() -> void:
	_disconnect_signal_if_connected(IEOS, "lobby_interface_lobby_update_received_callback", _on_lobby_interface_lobby_member_update_received_callback)
	_disconnect_signal_if_connected(IEOS, "lobby_interface_lobby_member_update_received_callback", _on_lobby_interface_lobby_member_update_received_callback)
	_disconnect_signal_if_connected(IEOS, "lobby_interface_lobby_member_status_received_callback", _on_lobby_member_status_received_callback)
	_disconnect_signal_if_connected(IEOS, "lobby_interface_rtc_room_connection_changed_callback", _on_lobby_interface_rtc_room_connection_changed_callback)
	_disconnect_signal_if_connected(IEOS, "lobby_interface_leave_lobby_requested_callback", _on_lobby_interface_leave_lobby_requested_callback)

	# RTC
	_disconnect_signal_if_connected(IEOS, "rtc_interface_participant_status_changed", _on_rtc_interface_participant_status_changed)
	_disconnect_signal_if_connected(IEOS, "rtc_audio_participant_updated", _on_rtc_audio_participant_updated)
	_disconnect_signal_if_connected(IEOS, "rtc_data_participant_updated", _on_rtc_data_participant_updated)
	_disconnect_signal_if_connected(IEOS, "rtc_data_data_received", _on_rtc_data_data_received)

	if _notif_rtc_parti_status_changed != EOS.NotificationIdInvalid:
		EOS.RTC.RTCInterface.remove_notify_participant_status_changed(_notif_rtc_parti_status_changed)
	if _notif_rtc_audio_parti_updated != EOS.NotificationIdInvalid:
		EOS.RTCAudio.RTCAudioInterface.remove_notify_participant_updated(_notif_rtc_audio_parti_updated)
	if _notif_rtc_data_parti_updated != EOS.NotificationIdInvalid:
		EOS.RTCData.RTCDataInterface.remove_notify_participant_updated(_notif_rtc_data_parti_updated)
	if _notif_rtc_data_data_received != EOS.NotificationIdInvalid:
		EOS.RTCData.RTCDataInterface.remove_notify_data_received(_notif_rtc_data_data_received)


func _disconnect_signal_if_connected(p_obj, p_signal_name, p_callback):
	if p_obj.is_connected(p_signal_name, p_callback):
		p_obj.disconnect(p_signal_name, p_callback)

#endregion

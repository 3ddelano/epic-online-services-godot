class_name HLobby
extends BaseClass


#region Signals

## Emitted when either lobby data, attribute, members, member data or member attribute is updated
signal lobby_updated

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

#endregion


#region Built-in methods

func _init() -> void:
	super._init("HLobby")

	IEOS.lobby_interface_lobby_update_received_callback.connect(_on_lobby_update_received_callback)
	IEOS.lobby_interface_lobby_member_update_received_callback.connect(_on_lobby_interface_lobby_member_update_received_callback)
	IEOS.lobby_interface_lobby_member_status_received_callback.connect(_on_lobby_member_status_received_callback)

#endregion


#region Public methods

## Check whether the lobby is valid
func is_valid() -> bool:
	return lobby_id != ""


## Check whether the current user is the lobby owner
func is_owner(product_user_id: String) -> bool:
	return owner_product_user_id == product_user_id


## Returns [HLobbyMember] or [null]
func get_member_by_product_user_id(product_user_id: String) -> HLobbyMember:
	for mem in members:
		if mem.product_user_id == product_user_id:
			return mem
	return null


## Returns [HLobbyMember] or [null]
func get_owner() -> HLobbyMember:
	for mem in members:
		if mem.product_user_id == owner_product_user_id:
			return mem
	return null


## Intialize the lobby from a lobby_id
func init_from_id(p_lobby_id: String):
	_log.debug("Initializing from lobby id: %s" % p_lobby_id)
	lobby_id = p_lobby_id

	var copy_opts = EOS.Lobby.CopyLobbyDetailsOptions.new()
	copy_opts.lobby_id = p_lobby_id
	var copy_ret = EOS.Lobby.LobbyInterface.copy_lobby_details(copy_opts)

	if not EOS.is_success(copy_ret):
		_log.error("Failed to copy lobby details: result_code=%s" % EOS.result_str(copy_ret))
		return

	var lobby_details = copy_ret.lobby_details
	_init_from_details(lobby_details)


## Returns [Dictionary] or [null]
func get_attribute(key: String):
	for attr in attributes:
		if attr.key == key:
			return attr
	return null


## Add an attribute to this lobby. Returns the added attribute as [Dictionary]
func add_attribute(key: String, value: Variant, visibility = EOS.Lobby.LobbyAttributeVisibility.Public) -> Dictionary:
	var attr = make_attribute(key, value, visibility)
	attributes.append(attr)
	return attr


## Get the current user's attribute or null if not found. Returns [Dictionary] or [null]
func get_member_attribute(key: String):
	var member = get_member_by_product_user_id(HAuth.product_user_id)
	if not member:
		return null
	for attr in member.attributes:
		if attr.key == key:
			return attr
	return null


## Add an attribute to the current user in the lboby. Returns the added attribute as [Dictionary] or [null] if some error
func add_member_attribute(key: String, value: Variant, visibility = EOS.Lobby.LobbyAttributeVisibility.Public):
	var member = get_member_by_product_user_id(HAuth.product_user_id)
	if not member:
		return null
	
	var attr = make_attribute(key, value, visibility)
	member.attributes.append(attr)
	return attr


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

		# TODO: check diff of attributes
		for attr in attributes:
			var add_ret = lobby_mod.add_attribute(attr.key, attr.value, attr.visibility)
			if not EOS.is_success(add_ret):
				_log.error("Failed to add attribute on lobby modification. Skipping this attribute: result_code=%s, key=%s, value=%s, visibility=%s" % [EOS.result_str(add_ret), attr.key, attr.value, attr.visibility])
	
	else:
		_log.debug("Updating lobby as user...")

	# TODO: check diff of member attributes
	var member: HLobbyMember = get_member_by_product_user_id(HAuth.product_user_id)
	if member:
		for attr in member.attributes:
			var add_ret = lobby_mod.add_member_attribute(attr.key, attr.value, attr.visibility)
			if not EOS.is_success(add_ret):
				_log.error("Failed to add member attribute on lobby modification. Skipping this attribute: result_code=%s, key=%s, value=%s, visibility=%s" % [EOS.result_str(add_ret), attr.key, attr.value, attr.visibility])

	var update_opts = EOS.Lobby.UpdateLobbyOptions.new()
	update_opts.lobby_modification = lobby_mod
	EOS.Lobby.LobbyInterface.update_lobby(update_opts)
	
	var update_ret = await IEOS.lobby_interface_update_lobby_callback
	if not EOS.is_success(update_ret):
		_log.error("Failed to update lobby: result_code=%s. lobby_id=%s" % [EOS.result_str(update_ret), lobby_id])
		return false
	
	_log.debug("Lobby updated successfully: lobby_id=%s" % lobby_id)

	# TODO: Should we init from scratch, what about the HLobbyMember object references
	print("before update")
	print(self)
	await init_from_id(lobby_id)
	print("after update")
	print(self)

	# TODO: handle remove attribute
	# TODO: handle add member attribute
	# TODO: handle remove member attribute

	return true
	
#endregion


#region Private methods

func _init_from_details(lobby_details: EOSGLobbyDetails):
	_log.debug("Initializing from EOSGLobbyDetails")

	var copy_ret = lobby_details.copy_info()
	if not EOS.is_success(copy_ret):
		_log.error("Failed to copy lobby info: result_code=%s" % EOS.result_str(copy_ret))
		return

	var details: Dictionary = copy_ret.lobby_details

	lobby_id = details.lobby_id
	owner_product_user_id = details.lobby_owner_user_id
	permission_level = details.permission_level
	available_slots = details.available_slots
	max_members = details.max_members
	allow_invites = details.allow_invites
	bucket_id = details.bucket_id
	allow_host_migration = details.allow_host_migration
	rtc_room_enabled = details.rtc_room_enabled
	allow_join_by_id = details.allow_join_by_id
	rejoin_after_kick_requires_invite = details.rejoin_after_kick_requires_invite
	presence_enabled = details.presence_enabled
	allowed_platform_ids = details.allowed_platform_ids

	# Get attributes
	var attr_count = lobby_details.get_attribute_count()
	_log.verbose("Got attributes: count=%s" % attr_count)
	attributes = []
	for attr_idx in attr_count:
		var copy_attr_ret = lobby_details.copy_attribute_by_index(attr_idx)
		if not EOS.is_success(copy_attr_ret):
			_log.error("Failed to copy EOSGLobbyDetails attribute by index: result_code=%s" % EOS.result_str(copy_attr_ret))
			continue
		var attr = copy_attr_ret.attribute
		add_attribute(attr.data.key, attr.data.value, attr.visibility)

	# Get members
	var members_count = lobby_details.get_member_count()
	_log.verbose("Got members: count=%s" % members_count)
	var old_members = members.duplicate(true)
	members = []

	for member_idx in members_count:
		var member_product_user_id = lobby_details.get_member_by_index(member_idx)
		var member = HLobbyMember.new()
		member.product_user_id = member_product_user_id
		members.append(member)

		# Member attributes
		var mem_attr_count = lobby_details.get_member_attribute_count(member_product_user_id)
		_log.verbose("Got member attributes: count=%s" % mem_attr_count)
		for mem_attr_idx in mem_attr_count:
			var copy_mem_attr_ret = lobby_details.copy_member_attribute_by_index(member_product_user_id, mem_attr_idx)
			if not EOS.is_success(copy_mem_attr_ret):
				_log.error("Failed to copy EOSGLobbyDetails member attribute by index: result_code=%s" % EOS.result_str(copy_mem_attr_ret))
				continue
			var attr = copy_mem_attr_ret.attribute
			member.add_attribute(attr.data.key, attr.data.value, attr.visibility)
			
			# Copy RTC status from old members
			var old_member = old_members.filter(func(m): return m.product_user_id == member.product_user_id)
			if old_member.size() == 0:
				continue

			member.rtc_state = old_member[0].rtc_state
			old_members.erase(old_member[0])


func _on_lobby_update_received_callback(data: Dictionary):
	if data.lobby_id != lobby_id:
		return
	_log.verbose("Got lobby update: lobby_id=%s" % lobby_id)
	init_from_id(lobby_id)
	lobby_updated.emit()


func _on_lobby_interface_lobby_member_update_received_callback(data: Dictionary):
	if data.lobby_id != lobby_id:
		return
	_log.verbose("Got lobby member update: lobby_id=%s" % lobby_id)
	init_from_id(lobby_id)
	lobby_updated.emit()


func _on_lobby_member_status_received_callback(data: Dictionary):
	if data.lobby_id != lobby_id:
		return
	_log.verbose("Got lobby member status: lobby_id=%s" % lobby_id)
	init_from_id(lobby_id)
	lobby_updated.emit()

#endregion
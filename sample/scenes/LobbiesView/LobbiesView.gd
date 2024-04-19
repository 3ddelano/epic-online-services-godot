class_name LobbiesView
extends VBoxContainer

class GameLobby extends BaseClass:
	func _init():
		super._init("GameLobby")

	var id: String

	var owner_id: String

	var members: Array[GameLobbyMember] = []
	var rtc_room_name: String

	var bucket_id: String
	var max_lobby_members: int
	var permission_level: EOS.Lobby.LobbyPermissionLevel
	var presence_enabled: bool
	var enable_rtc_room: bool
	var allow_invites: bool

	var available_slots: int

	## Array of {"visibility": [enum EOS.Lobby.LobbyAttributeVisibility], data: {"key": String, "value": Variant}}
	var attributes = []

	var rtc_room_connected: bool

	func is_valid() -> bool:
		return id != ""


	func is_owner(user_id: String) -> bool:
		return owner_id == user_id

	func get_member_by_product_user_id(product_user_id: String):
		for mem in members:
			if mem.product_id == product_user_id:
				return mem
		return null

	func get_owner() -> GameLobbyMember:
		for mem in members:
			if mem.product_id == owner_id:
				return mem
		return null


	func is_rtc_room_enabled():
		return enable_rtc_room


	func get_attribute(key: String):
		for attr in attributes:
			if attr.data.key == key:
				return attr
		return null


	func init_from_id(lobby_id: String):
		id = lobby_id

		var copy_opts = EOS.Lobby.CopyLobbyDetailsOptions.new()
		copy_opts.lobby_id = id
		var copy_ret = EOS.Lobby.LobbyInterface.copy_lobby_details(copy_opts)

		if not EOS.is_success(copy_ret):
			print("Failed to copy lobby details: ", EOS.result_str(copy_ret))
			return

		var lobby_details = copy_ret.lobby_details
		init_from_details(lobby_details)


	func init_from_details(lobby_details: EOSGLobbyDetails):
		var new_lobby_owner_id = lobby_details.get_lobby_owner()

		if new_lobby_owner_id != owner_id:
			owner_id = new_lobby_owner_id

		var copy_info_ret = lobby_details.copy_info()
		if not EOS.is_success(copy_info_ret):
			print("Failed to copy lobby info: ", EOS.result_str(copy_info_ret))
			return

		var details_copy: Dictionary = copy_info_ret.lobby_details

		id = details_copy.lobby_id
		max_lobby_members = details_copy.max_members
		permission_level = details_copy.permission_level
		allow_invites = details_copy.allow_invites
		available_slots = details_copy.available_slots
		bucket_id = details_copy.bucket_id
		enable_rtc_room = details_copy.rtc_room_enabled
		presence_enabled = details_copy.presence_enabled

		# Get attributes
		attributes = []

		for attr_idx in lobby_details.get_attribute_count():
			var copy_attr_ret = lobby_details.copy_attribute_by_index(attr_idx)
			if not EOS.is_success(copy_attr_ret):
				print("--- Lobby: Failed to copy EOSGLobbyDetails attribute by index: ", EOS.result_str(copy_attr_ret))
				continue
			attributes.append(copy_attr_ret.attribute)

		# Get members
		var old_members = members.duplicate(true)
		members = []

		var member_count = lobby_details.get_member_count()
		for member_idx in member_count:
			var member_id = lobby_details.get_member_by_index(member_idx)
			var member = GameLobbyMember.new()
			member.product_id = member_id
			members.append(member)

			# Member attributes
			var mem_attr_count = lobby_details.get_member_attribute_count(member_id)
			for mem_attr_idx in mem_attr_count:
				var copy_mem_attr_ret = lobby_details.copy_member_attribute_by_index(member_id, mem_attr_idx)
				if not EOS.is_success(copy_mem_attr_ret):
					print("Failed to copy member attribute by index: ", EOS.result_str(copy_mem_attr_ret))
					continue
				var attribute = copy_mem_attr_ret.attribute
				member.member_attributes.append(attribute)
				if attribute.data.key == "SKIN":
					member.init_skin_from_string(attribute.data.value)

				# Copy RTC status from old members
				var old_member = old_members.filter(func (m): return m.product_id == member.product_id)
				if old_member.size() == 0:
					continue

				member.rtc_state = old_member[0].rtc_state
				old_members.erase(old_member[0])


class GameLobbyMember extends BaseClass:
	func _init():
		super._init("GameLobbyMember")

	var product_id: String
	var account_id: String
	var display_name: String
	var current_skin: Skins = Skins.Human

	var member_attributes = []
	var rtc_state = {
		is_in_rtc_room = false,
		is_talking = false,
		is_locally_muted = false,
		is_hard_muted = false,
		is_audio_output_disabled = false,
		mute_action_in_progress = false,
		hard_mute_action_in_progress = false,
	}

	func init_skin_from_string(p_skin: String):
		var skin_names = Skins.keys()
		var idx = skin_names.find(p_skin)
		if idx == -1:
			print("Invalid skin name: ", p_skin)
			current_skin = Skins.Human
			return
		current_skin = Skins.values()[idx]

	static func get_skin_string(skin: Skins) -> String:
		var skin_names = Skins.keys()
		var idx = Skins.values().find(skin)
		if idx == -1:
			print("Invalid skin: ", skin)
			return ""
		return skin_names[idx]

	func shuffle_skin():
		var idx = randi() % Skins.size()
		current_skin = Skins.values()[idx]

	func get_attribute(key: String):
		for attr in member_attributes:
			if attr.data.key == key:
				return attr
		return null




enum Maps {
	Margao,
	Panaji,
	Canacona,
	Chandor
}

enum Skins {
	Human,
	Robot,
	Cyborg,
	Alien
}

const MAX_PLAYERS = 6
const MAP_ATTRIBUTE_KEY = "LEVEL" # To match with EOS Lobbies sample
const SKIN_ATTRIBUTE_KEY = "SKIN"

var current_lobby: GameLobby

@onready var show_create_lobby_btn = %ShowCreateLobbyBtn
@onready var create_lobby_popup = %CreateLobbyPopup

@onready var search_lobby_type = %SearchLobbyType
@onready var search_lobby_line_edit = %SearchLobbyLineEdit
@onready var search_lobby_btn = %SearchLobbyBtn

@onready var current_lobby_node: CurrentLobby = %CurrentLobby
@onready var search_lobby_results: SearchLobbyResults = %SearchLobbyResults

func _ready() -> void:
	IEOS.lobby_interface_create_lobby_callback.connect(_on_create_lobby_callback)
	IEOS.lobby_interface_update_lobby_callback.connect(_on_update_lobby_callback)
	IEOS.lobby_interface_join_lobby_accepted_callback.connect(_on_join_lobby_accepted)

	#IEOS.lobby_interface_lobby_update_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_update_received_callback: ", data))
	#IEOS.lobby_interface_lobby_member_update_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_member_update_received_callback: ", data))
	#IEOS.lobby_interface_rtc_room_connection_changed_callback.connect(func (data: Dictionary): print("--- Lobby: rtc_room_connection_changed: ", data))
	#IEOS.lobby_interface_lobby_member_status_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_member_status_received_callback: ", data))
	#IEOS.lobby_interface_send_invite_callback.connect(func (data: Dictionary): print("--- Lobby: send_invite_callback: ", data))
	#IEOS.lobby_interface_reject_invite_callback.connect(func (data: Dictionary): print("--- Lobby: reject_invite_callback: ", data))
	#IEOS.lobby_interface_query_invites_callback.connect(func (data: Dictionary): print("--- Lobby: query_invites_callback: ", data))
	#IEOS.lobby_interface_lobby_invite_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_received_callback: ", data))
	#IEOS.lobby_interface_lobby_invite_accepted_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_accepted_callback: ", data))
	#IEOS.lobby_interface_lobby_invite_rejected_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_rejected_callback: ", data))
	#IEOS.lobby_interface_join_lobby_by_id_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_by_id_callback: ", data))
	#IEOS.lobby_interface_join_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_callback: ", data))
	#IEOS.lobby_interface_send_lobby_native_invite_requested_callback.connect(func (data: Dictionary): print("--- Lobby: send_lobby_native_invite_requested_callback: ", data))
	#IEOS.lobby_interface_promote_member_callback.connect(func (data: Dictionary): print("--- Lobby: promote_member_callback: ", data))
	#IEOS.lobby_interface_kick_member_callback.connect(func (data: Dictionary): print("--- Lobby: kick_member_callback: ", data))
	#IEOS.lobby_interface_hard_mute_member_callback.connect(func (data: Dictionary): print("--- Lobby: hard_mute_member_callback: ", data))

	show_create_lobby_btn.pressed.connect(_on_show_create_lobby_btn_pressed)
	current_lobby = GameLobby.new()

	hide_search_results()
	hide_current_lobby()


func create_lobby(lobby: GameLobby) -> bool:
	# Check if there is a current lobby. Leave it.
	if current_lobby.is_valid():
		await leave_lobby()

	var opts = EOS.Lobby.CreateLobbyOptions.new()
	opts.bucket_id = lobby.bucket_id
	opts.max_lobby_members = lobby.max_lobby_members
	opts.presence_enabled = lobby.presence_enabled
	opts.enable_rtc_room = lobby.enable_rtc_room
	opts.permission_level = lobby.permission_level
	opts.allow_invites = lobby.allow_invites
	EOS.Lobby.LobbyInterface.create_lobby(opts)

	# Save lobby data for modification
	current_lobby = lobby
	current_lobby.owner_id = Store.product_user_id

	return true


func current_lobby_destroy() -> bool:
	if not current_lobby.is_valid():
		return false

	if not current_lobby.is_owner(Store.product_user_id):
		print("Only lobby owner can destroy lobby")
		return false

	var destroy_opts = EOS.Lobby.DestroyLobbyOptions.new()
	destroy_opts.lobby_id = current_lobby.id
	EOS.Lobby.LobbyInterface.destroy_lobby(destroy_opts)
	await IEOS.lobby_interface_destroy_lobby_callback

	current_lobby = GameLobby.new()

	return true


func leave_lobby() -> void:
	if not current_lobby.is_valid():
		return

	# Set this to true to test flow where owner always destroys lobby
	var owner_always_destroys_lobby = false
	var is_owner = current_lobby.is_owner(Store.product_user_id)
	var destroy_lobby = is_owner and (owner_always_destroys_lobby or current_lobby.members.size() <= 1)

	if destroy_lobby:
		await current_lobby_destroy()
		return

	var leave_opts = EOS.Lobby.LeaveLobbyOptions.new()
	leave_opts.lobby_id = current_lobby.id
	EOS.Lobby.LobbyInterface.leave_lobby(leave_opts)
	await IEOS.lobby_interface_leave_lobby_callback

	current_lobby = GameLobby.new()


func modify_lobby(lobby: GameLobby) -> bool:
	if not current_lobby.is_valid():
		print("Unable to modify lobby. Current lobby is invalid")
		return false

	if not current_lobby.is_owner(Store.product_user_id):
		print("Unable to modify lobby. Only lobby owner can modify lobby.")
		return false

	var modify_opts = EOS.Lobby.UpdateLobbyModificationOptions.new()
	modify_opts.lobby_id = current_lobby.id

	var update_ret = EOS.Lobby.LobbyInterface.update_lobby_modification(modify_opts)
	if not EOS.is_success(update_ret):
		print("Failed to update lobby modification: ", EOS.result_str(update_ret))
		return false

	var lobby_modification: EOSGLobbyModification = update_ret.lobby_modification

	var set_perm_ret = lobby_modification.set_permission_level(lobby.permission_level)
	if not EOS.is_success(set_perm_ret):
		print("Failed to set permission level on lobby modification: ", EOS.result_str(set_perm_ret))
		return false

	if not lobby.bucket_id.is_empty():
		var set_bucket_ret = lobby_modification.set_bucket_id(lobby.bucket_id)
		if not EOS.is_success(set_bucket_ret):
			print("Failed to set bucket id on lobby modification: ", EOS.result_str(set_bucket_ret))
			return false

	var set_max_members_ret = lobby_modification.set_max_members(lobby.max_lobby_members)
	if not EOS.is_success(set_max_members_ret):
		print("Failed to set max members on lobby modification: ", EOS.result_str(set_max_members_ret))
		return false

	var set_invites_allowed_ret = lobby_modification.set_invites_allowed(lobby.allow_invites)
	if not EOS.is_success(set_invites_allowed_ret):
		print("Failed to set invites allowed on lobby modification: ", EOS.result_str(set_invites_allowed_ret))
		return false

	for attr in lobby.attributes:
		var add_ret = lobby_modification.add_attribute(attr.key, attr.value, attr.visibility)
		if not EOS.is_success(add_ret):
			print("--- LobbyModification: add_attribute: error: ", EOS.result_str(add_ret))

	var update_opts = EOS.Lobby.UpdateLobbyOptions.new()
	update_opts.lobby_modification = lobby_modification
	EOS.Lobby.LobbyInterface.update_lobby(update_opts)

	return true


func _on_show_create_lobby_btn_pressed():
	create_lobby_popup.popup_centered()


func _on_create_lobby_callback(data: Dictionary):
	print("--- Lobby: create_lobby_callback: ", EOS.result_str(data))

	if not (EOS.is_operation_complete(data) and EOS.is_success(data)):
		print("Failed to create lobby")
		return

	if data.lobby_id != "":
		current_lobby.id = data.lobby_id

	modify_lobby(current_lobby)

	#if current_lobby.is_rtc_room_enabled():
		#_subscribe_to_rtc_events()


func _on_update_lobby_callback(data: Dictionary):
	print("--- Lobby: update_lobby_callback: ", EOS.result_str(data))

	if not (EOS.is_operation_complete(data) and EOS.is_success(data)):
		print("Failed to update lobby")

	if current_lobby.id != data.lobby_id:
		return

	var copy_details_opts = EOS.Lobby.CopyLobbyDetailsOptions.new()
	copy_details_opts.lobby_id = current_lobby.id

	var copy_details_ret = EOS.Lobby.LobbyInterface.copy_lobby_details(copy_details_opts)
	if not EOS.is_success(copy_details_ret):
		print("Failed to copy lobby details: ", EOS.result_str(copy_details_ret))
		return

	var lobby_details: EOSGLobbyDetails = copy_details_ret.lobby_details
	current_lobby.init_from_details(lobby_details)
	current_lobby_node.update(current_lobby)
	set_initial_member_attributes()


func _on_join_lobby_accepted(data: Dictionary):
	print("--- Lobby: join_lobby_accepted_callback: ", data)

	var puid = data.local_user_id
	var uievent_id = data.ui_event_id

	var copy_details_opts = EOS.Lobby.CopyLobbyDetailsByUiEventIdOptions.new()
	copy_details_opts.ui_event_id = uievent_id
	var copy_ret = EOS.Lobby.LobbyInterface.copy_lobby_details_by_ui_event_id(copy_details_opts)
	if not EOS.is_success(copy_ret):
		print("--- Lobby: copy_lobby_details_by_ui_event_id: error: ", EOS.result_str(copy_ret))
		return

	var lobby_details = copy_ret.lobby_details
	join_lobby(lobby_details.lobby_id)


func set_initial_member_attributes():
	if not current_lobby.is_valid():
		return

	for member in current_lobby.members:
		if member.product_id == Store.product_user_id:
			# Check if skin is already set
			if member.member_attributes.is_empty():
				var attr = {key = SKIN_ATTRIBUTE_KEY, value = GameLobbyMember.get_skin_string(member.current_skin)}
				set_member_attribute(attr)


func set_member_attribute(attr: Dictionary):
	if not current_lobby.is_valid():
		return

	# Modify lobby
	var update_mod_opts = EOS.Lobby.UpdateLobbyModificationOptions.new()
	update_mod_opts.lobby_id = current_lobby.id

	var update_modification_ret = EOS.Lobby.LobbyInterface.update_lobby_modification(update_mod_opts)
	if not EOS.is_success(update_modification_ret):
		print("Failed to update lobby modification: ", EOS.result_str(update_modification_ret))
		return

	var lobby_modification: EOSGLobbyModification = update_modification_ret.lobby_modification
	var add_mem_attr_ret = lobby_modification.add_member_attribute(attr.key, attr.value, EOS.Lobby.LobbyAttributeVisibility.Public)
	if not EOS.is_success(add_mem_attr_ret):
		print("Failed to add member attribute: ", EOS.result_str(add_mem_attr_ret))
		return

	# Trigger lobby update
	var update_opts = EOS.Lobby.UpdateLobbyOptions.new()
	update_opts.lobby_modification = lobby_modification
	EOS.Lobby.LobbyInterface.update_lobby(update_opts)


func _subscribe_to_rtc_events():
	if not current_lobby.is_rtc_room_enabled():
		return

	# Get RTC room name
	var get_name_opts = EOS.Lobby.GetRtcRoomNameOptions.new()
	get_name_opts.lobby_id = current_lobby.id
	var name_ret = EOS.Lobby.LobbyInterface.get_rtc_room_name(get_name_opts)
	if not EOS.is_success(name_ret):
		print("--- Lobby: get_rtc_room_name: error ", EOS.result_str(name_ret))
	else:
		current_lobby.rtc_room_name = name_ret.rtc_room_name

	# Get current RTC room connection status
	var is_conn_opts = EOS.Lobby.IsRtcRoomConnectedOptions.new()
	is_conn_opts.lobby_id = current_lobby.id
	var is_conn_ret = EOS.Lobby.LobbyInterface.is_rtc_room_connected(is_conn_opts)
	if not EOS.is_success(is_conn_ret):
		print("--- Lobby: is_rtc_room_connected: error ", EOS.result_str(is_conn_ret))
	else:
		current_lobby.rtc_room_connected = is_conn_ret.is_connected

	# Register for RTC room participant changes
	var notify_parti_status_opts = EOS.RTC.AddNotifyParticipantStatusChangedOptions.new()
	notify_parti_status_opts.room_name = current_lobby.rtc_room_name
	EOS.RTC.RTCInterface.add_notify_participant_status_changed(notify_parti_status_opts)

	# Register for talking events
	var notify_participant_updated_opts = EOS.RTCAudio.AddNotifyParticipantUpdatedOptions.new()
	notify_participant_updated_opts.room_name = current_lobby.rtc_room_name
	EOS.RTCAudio.RTCAudioInterface.add_notify_participant_updated(notify_participant_updated_opts)


func hide_current_lobby():
	current_lobby_node.hide()


func handle_search_results(search_results: Array[EOSGLobbyDetails]):
	hide_current_lobby()
	search_lobby_results.show()
	search_lobby_results.update_results(search_results)


func hide_search_results():
	search_lobby_results.hide()
	if current_lobby.is_valid():
		current_lobby_node.show()

func join_lobby(lobby_id: String):
	var join_opts = EOS.Lobby.JoinLobbyByIdOptions.new()
	join_opts.lobby_id = lobby_id
	EOS.Lobby.LobbyInterface.join_lobby_by_id(join_opts)

	var join_ret = await IEOS.lobby_interface_join_lobby_by_id_callback
	if not EOS.is_success(join_ret):
		print("--- Lobby: join_lobby_by_id: error: ", EOS.result_str(join_ret))
		return

	# if current_lobby.is_valid() and current_lobby.id != lobby_id:
	# 	await leave_lobby()

	# current_lobby.init_from_id(lobby_id)
	# set_initial_member_attributes()
	# if current_lobby.is_rtc_room_enabled():
	# 	_subscribe_to_rtc_events()

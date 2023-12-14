class_name LobbiesView
extends VBoxContainer

class GameLobby extends BaseClass:
	func _init():
		super._init("GameLobby")

	var id: String
	var owner_id: String
	var owner_epic_account_id: String
	var owner_display_name: String

	var members = []
	var rtc_room_name: String

	var bucket_id: String
	var max_lobby_members: int
	var permission_level: int
	var presence_enabled: bool
	var enable_rtc_room: bool
	var allow_invites: bool

	var available_slots: int
	var attributes = []

	func is_valid() -> bool:
		return id != ""

	func is_owner(user_id: String) -> bool:
		return owner_id == user_id

	func reset() -> void:
		id = ""
		members = []
		owner_id = ""
		rtc_room_name = ""
		bucket_id = ""
		max_lobby_members = 0
		permission_level = 0
		presence_enabled = false
		enable_rtc_room = false
		allow_invites = false
		available_slots = 0
		attributes = []

	func init_from_details(lobby_details: EOSGLobbyDetails):
		var new_lobby_owner_id = lobby_details.get_lobby_owner()

		if new_lobby_owner_id != owner_id:
			owner_id = new_lobby_owner_id
			owner_epic_account_id = ""
			owner_display_name = ""

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

		# Get attributes
		attributes = []

		var attr_count: int = lobby_details.get_attribute_count()

		for attr_idx in attr_count:
			var copy_attr_ret = lobby_details.copy_attribute_by_index(attr_idx)
			if not EOS.is_success(copy_attr_ret):
				print("Failed to copy attribute by index: ", EOS.result_str(copy_attr_ret))
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
			var mem_attr_count: int = lobby_details.get_member_attribute_count(member_id)
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
				var old_member = old_members.filter(func (m): m.product_id == member.product_id)
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

# Local cache of search lobbies result
var lobbies = []

var current_lobby: GameLobby

@onready var show_create_lobby_btn = %ShowCreateLobbyBtn
@onready var create_lobby_popup = %CreateLobbyPopup

@onready var search_lobby_type = %SearchLobbyType
@onready var search_lobby_line_edit = %SearchLobbyLineEdit
@onready var search_lobby_btn = %SearchLobbyBtn

@onready var my_lobby = %MyLobby
@onready var search_lobby_results = %SearchLobbyResults


func _ready() -> void:
	EOS.get_instance().lobby_interface_rtc_room_connection_changed_callback.connect(_on_rtc_room_connection_changed_callback)
	EOS.get_instance().lobby_interface_create_lobby_callback.connect(_on_lobby_create_lobby_callback)
	EOS.get_instance().lobby_interface_leave_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: leave_lobby_callback: ", EOS.result_str(data)))
	EOS.get_instance().lobby_interface_destroy_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: destroy_lobby_callback: ", EOS.result_str(data)))
	EOS.get_instance().lobby_interface_join_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_callback: ", EOS.result_str(data)))

	EOS.get_instance().lobby_interface_lobby_update_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_update_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_member_update_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_member_update_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_member_status_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_member_status_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_invite_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_invite_accepted_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_accepted_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_invite_rejected_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_rejected_callback: ", data))
	EOS.get_instance().lobby_interface_join_lobby_accepted_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_accepted_callback: ", data))
	EOS.get_instance().lobby_interface_send_lobby_native_invite_requested_callback.connect(func (data: Dictionary): print("--- Lobby: send_lobby_native_invite_requested_callback: ", data))
	EOS.get_instance().lobby_interface_join_lobby_by_id_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_by_id_callback: ", data))
	EOS.get_instance().lobby_interface_update_lobby_callback.connect(_on_lobby_update_lobby_callback)
	EOS.get_instance().lobby_interface_promote_member_callback.connect(func (data: Dictionary): print("--- Lobby: promote_member_callback: ", data))
	EOS.get_instance().lobby_interface_kick_member_callback.connect(func (data: Dictionary): print("--- Lobby: kick_member_callback: ", data))
	EOS.get_instance().lobby_interface_hard_mute_member_callback.connect(func (data: Dictionary): print("--- Lobby: hard_mute_member_callback: ", data))
	EOS.get_instance().lobby_interface_send_invite_callback.connect(func (data: Dictionary): print("--- Lobby: send_invite_callback: ", data))
	EOS.get_instance().lobby_interface_reject_invite_callback.connect(func (data: Dictionary): print("--- Lobby: reject_invite_callback: ", data))
	EOS.get_instance().lobby_interface_query_invites_callback.connect(func (data: Dictionary): print("--- Lobby: query_invites_callback: ", data))
	EOS.get_instance().lobby_interface_destroy_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: destroy_lobby_callback: ", EOS.result_str(data)))
	show_create_lobby_btn.pressed.connect(_on_show_create_lobby_btn_pressed)
	current_lobby = GameLobby.new()


func create_lobby(lobby: GameLobby) -> bool:
	# Check if there is a current lobby. Leave it.
	if current_lobby.is_valid():
		leave_lobby()

	var opts = EOS.Lobby.CreateLobbyOptions.new().from_dict({
		local_user_id = Store.product_user_id,
		bucket_id = lobby.bucket_id,
		max_lobby_members = lobby.max_lobby_members,
		presence_enabled = lobby.presence_enabled,
		enable_rtc_room = lobby.enable_rtc_room,
		permission_level = lobby.permission_level,
		allow_invites = lobby.allow_invites,
	})
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
	destroy_opts.local_user_id = Store.product_user_id
	EOS.Lobby.LobbyInterface.destroy_lobby(destroy_opts)

	current_lobby.reset()

	return true


func leave_lobby() -> void:
	if not current_lobby.is_valid():
		return

	# Set this to true to test flow where owner always destroys lobby
	var owner_always_destroys_lobby = false
	var is_owner = current_lobby.is_owner(Store.product_user_id)
	var destroy_lobby = is_owner && (owner_always_destroys_lobby or current_lobby.members.size() <= 1)

	if destroy_lobby:
		current_lobby_destroy()
		return

	var leave_opts = EOS.Lobby.LeaveLobbyOptions.new()
	leave_opts.lobby_id = current_lobby.id
	leave_opts.local_user_id = Store.product_user_id
	EOS.Lobby.LobbyInterface.leave_lobby(leave_opts)
	current_lobby.reset()


func modify_lobby(lobby: GameLobby) -> bool:
	if not current_lobby.is_valid():
		print("Unable to modify lobby. Current lobby is invalid")
		return false

	if not current_lobby.is_owner(Store.product_user_id):
		print("Unable to modify lobby. Only lobby owner can modify lobby.")
		return false

	var modify_opts = EOS.Lobby.UpdateLobbyModificationOptions.new()
	modify_opts.lobby_id = current_lobby.id
	modify_opts.local_user_id = Store.product_user_id

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

	if lobby.max_lobby_members > 0:
		var set_max_members_ret = lobby_modification.set_max_members(lobby.max_lobby_members)
		if not EOS.is_success(set_max_members_ret):
			print("Failed to set max members on lobby modification: ", EOS.result_str(set_max_members_ret))
			return false

	var set_invites_allowed_ret = lobby_modification.set_invites_allowed(lobby.allow_invites)
	if not EOS.is_success(set_invites_allowed_ret):
		print("Failed to set invites allowed on lobby modification: ", EOS.result_str(set_invites_allowed_ret))
		return false

	# TODO: add lobby attributes

	var update_opts = EOS.Lobby.UpdateLobbyOptions.new()
	update_opts.lobby_modification = lobby_modification
	EOS.Lobby.LobbyInterface.update_lobby(update_opts)

	return true


func _on_show_create_lobby_btn_pressed():
	create_lobby_popup.popup_centered()


func _on_lobby_create_lobby_callback(data: Dictionary):
	print("--- Lobby: create_lobby_callback: ", EOS.result_str(data))

	if not (EOS.is_operation_complete(data) and EOS.is_success(data)):
		print("Failed to create lobby")

	if data.lobby_id != "":
		current_lobby.id = data.lobby_id

	modify_lobby(current_lobby)


func _on_lobby_update_lobby_callback(data: Dictionary):
	print("--- Lobby: update_lobby_callback: ", EOS.result_str(data))

	if not (EOS.is_operation_complete(data) and EOS.is_success(data)):
		print("Failed to update lobby")

	if current_lobby.id != data.lobby_id:
		return

	var copy_details_opts = EOS.Lobby.CopyLobbyDetailsOptions.new()
	copy_details_opts.lobby_id = current_lobby.id
	copy_details_opts.local_user_id = Store.product_user_id

	var copy_details_ret = EOS.Lobby.LobbyInterface.copy_lobby_details(copy_details_opts)
	if not EOS.is_success(copy_details_ret):
		print("Failed to copy lobby details: ", EOS.result_str(copy_details_ret))
		return

	var lobby_details: EOSGLobbyDetails = copy_details_ret.lobby_details
	current_lobby.init_from_details(lobby_details)
	set_initial_member_attributes()


func set_initial_member_attributes():
	if not current_lobby.is_valid():
		return

	for member in current_lobby.members:
		if member.product_id == Store.product_user_id:
			# Check if skin is already set
			if member.member_attributes.is_empty():
				var attr = {key = "SKIN", value = GameLobbyMember.get_skin_string(member.current_skin)}
				set_member_attribute(attr)


func set_member_attribute(attr: Dictionary):
	if not current_lobby.is_valid():
		return

	# Modify lobby
	var update_modification_opts = EOS.Lobby.UpdateLobbyModificationOptions.new()
	update_modification_opts.lobby_id = current_lobby.id
	update_modification_opts.local_user_id = Store.product_user_id

	var update_modification_ret = EOS.Lobby.LobbyInterface.update_lobby_modification(update_modification_opts)
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


func _on_rtc_room_connection_changed_callback(data: Dictionary):
	print("--- Lobby: rtc_room_connection_changed_callback: ", data)


func handle_search_results(_search_results: Array[EOSGLobbyDetails]):
	pass

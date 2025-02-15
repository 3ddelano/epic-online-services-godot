class_name LobbiesView
extends VBoxContainer


enum Maps {
	Margao,
	Panaji,
	Canacona,
	Chandor
}

const Skins = {
	Human = "Human",
	Robot = "Robot",
	Cyborg = "Cyborg",
	Alien = "Alien"
}

const MAX_PLAYERS = 6
const MAP_ATTRIBUTE_KEY = "LEVEL" # To match with EOS Lobbies sample
const SKIN_ATTRIBUTE_KEY = "SKIN"
const OWNER_NAME_ATTRIBUTE_KEY = "OWNER_NAME"
const USERNAME_ATTRIBUTE_KEY = "USER_NAME"

var current_lobby: HLobby

@onready var show_create_lobby_btn = %ShowCreateLobbyBtn
@onready var create_lobby_popup = %CreateLobbyPopup

@onready var search_lobby_type = %SearchLobbyType
@onready var search_lobby_line_edit = %SearchLobbyLineEdit
@onready var search_lobby_btn = %SearchLobbyBtn

@onready var current_lobby_node: CurrentLobby = %CurrentLobby
@onready var search_lobby_results: SearchLobbyResults = %SearchLobbyResults


func _ready() -> void:
	show_create_lobby_btn.pressed.connect(_on_show_create_lobby_btn_pressed)
	current_lobby = HLobby.new()

	hide_search_results()
	hide_current_lobby()


# func current_lobby_destroy() -> bool:
# 	if not current_lobby.is_valid():
# 		return false

# 	if not current_lobby.is_owner(HAuth.product_user_id):
# 		print("Only lobby owner can destroy lobby")
# 		return false

# 	var destroy_opts = EOS.Lobby.DestroyLobbyOptions.new()
# 	destroy_opts.lobby_id = current_lobby.id
# 	EOS.Lobby.LobbyInterface.destroy_lobby(destroy_opts)
# 	await IEOS.lobby_interface_destroy_lobby_callback

# 	current_lobby = HLobby.new()

# 	return true

# func leave_lobby() -> void:
# 	if not current_lobby.is_valid():
# 		return

# 	# Set this to true to test flow where owner always destroys lobby
# 	var owner_always_destroys_lobby = false
# 	var is_owner = current_lobby.is_owner(HAuth.product_user_id)
# 	var destroy_lobby = is_owner and (owner_always_destroys_lobby or current_lobby.members.size() <= 1)

# 	if destroy_lobby:
# 		await current_lobby_destroy()
# 		return

# 	var leave_opts = EOS.Lobby.LeaveLobbyOptions.new()
# 	leave_opts.lobby_id = current_lobby.id
# 	EOS.Lobby.LobbyInterface.leave_lobby(leave_opts)
# 	await IEOS.lobby_interface_leave_lobby_callback

# 	current_lobby = HLobby.new()


# func _on_join_lobby_accepted(data: Dictionary):
# 	print("--- Lobby: join_lobby_accepted_callback: ", data)

# 	var puid = data.local_user_id
# 	var uievent_id = data.ui_event_id

# 	var copy_details_opts = EOS.Lobby.CopyLobbyDetailsByUiEventIdOptions.new()
# 	copy_details_opts.ui_event_id = uievent_id
# 	var copy_ret = EOS.Lobby.LobbyInterface.copy_lobby_details_by_ui_event_id(copy_details_opts)
# 	if not EOS.is_success(copy_ret):
# 		print("--- Lobby: copy_lobby_details_by_ui_event_id: error: ", EOS.result_str(copy_ret))
# 		return

# 	var lobby_details = copy_ret.lobby_details
# 	join_lobby(lobby_details.lobby_id)


# func _subscribe_to_rtc_events():
# 	if not current_lobby.is_rtc_room_enabled():
# 		return

# 	# Get RTC room name
# 	var get_name_opts = EOS.Lobby.GetRtcRoomNameOptions.new()
# 	get_name_opts.lobby_id = current_lobby.id
# 	var name_ret = EOS.Lobby.LobbyInterface.get_rtc_room_name(get_name_opts)
# 	if not EOS.is_success(name_ret):
# 		print("--- Lobby: get_rtc_room_name: error ", EOS.result_str(name_ret))
# 	else:
# 		current_lobby.rtc_room_name = name_ret.rtc_room_name

# 	# Get current RTC room connection status
# 	var is_conn_opts = EOS.Lobby.IsRtcRoomConnectedOptions.new()
# 	is_conn_opts.lobby_id = current_lobby.id
# 	var is_conn_ret = EOS.Lobby.LobbyInterface.is_rtc_room_connected(is_conn_opts)
# 	if not EOS.is_success(is_conn_ret):
# 		print("--- Lobby: is_rtc_room_connected: error ", EOS.result_str(is_conn_ret))
# 	else:
# 		current_lobby.rtc_room_connected = is_conn_ret.is_connected

# 	# Register for RTC room participant changes
# 	var notify_parti_status_opts = EOS.RTC.AddNotifyParticipantStatusChangedOptions.new()
# 	notify_parti_status_opts.room_name = current_lobby.rtc_room_name
# 	EOS.RTC.RTCInterface.add_notify_participant_status_changed(notify_parti_status_opts)

# 	# Register for talking events
# 	var notify_participant_updated_opts = EOS.RTCAudio.AddNotifyParticipantUpdatedOptions.new()
# 	notify_participant_updated_opts.room_name = current_lobby.rtc_room_name
# 	EOS.RTCAudio.RTCAudioInterface.add_notify_participant_updated(notify_participant_updated_opts)


func set_current_lobby(lobby: HLobby):
	current_lobby = lobby
	current_lobby_node.update(current_lobby)


func hide_current_lobby():
	current_lobby_node.hide()


func handle_search_results(search_results: Array[HLobby]):
	hide_current_lobby()
	search_lobby_results.show()
	search_lobby_results.update_results(search_results)


func hide_search_results():
	search_lobby_results.hide()
	if current_lobby.is_valid():
		current_lobby_node.show()


func _on_show_create_lobby_btn_pressed():
	create_lobby_popup.popup_centered()
	# if current_lobby.is_rtc_room_enabled():
	# 	_subscribe_to_rtc_events()

class_name LobbiesView
extends VBoxContainer

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

# Local cache of lobbies data
var lobbies = []
var current_lobby = {}

@onready var show_create_lobby_btn = %ShowCreateLobbyBtn
@onready var create_lobby_popup = %CreateLobbyPopup

@onready var search_lobby_type = %SearchLobbyType
@onready var search_lobby_line_edit = %SearchLobbyLineEdit
@onready var search_lobby_btn = %SearchLobbyBtn

@onready var my_lobby = %MyLobby
@onready var search_lobby_results = %SearchLobbyResults


func _ready() -> void:
	EOS.get_instance().lobby_interface_create_lobby_callback.connect(_on_create_lobby_callback)
	EOS.get_instance().lobby_interface_lobby_update_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_update_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_member_update_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_member_update_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_member_status_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_member_status_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_invite_received_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_received_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_invite_accepted_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_accepted_callback: ", data))
	EOS.get_instance().lobby_interface_lobby_invite_rejected_callback.connect(func (data: Dictionary): print("--- Lobby: lobby_invite_rejected_callback: ", data))
	EOS.get_instance().lobby_interface_join_lobby_accepted_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_accepted_callback: ", data))
	EOS.get_instance().lobby_interface_send_lobby_native_invite_requested_callback.connect(func (data: Dictionary): print("--- Lobby: send_lobby_native_invite_requested_callback: ", data))
	EOS.get_instance().lobby_interface_rtc_room_connection_changed_callback.connect(func (data: Dictionary): print("--- Lobby: rtc_room_connection_changed_callback: ", data))
	EOS.get_instance().lobby_interface_destroy_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: destroy_lobby_callback: ", data))
	EOS.get_instance().lobby_interface_join_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_callback: ", data))
	EOS.get_instance().lobby_interface_join_lobby_by_id_callback.connect(func (data: Dictionary): print("--- Lobby: join_lobby_by_id_callback: ", data))
	EOS.get_instance().lobby_interface_leave_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: leave_lobby_callback: ", data))
	EOS.get_instance().lobby_interface_update_lobby_callback.connect(func (data: Dictionary): print("--- Lobby: update_lobby_callback: ", data))
	EOS.get_instance().lobby_interface_promote_member_callback.connect(func (data: Dictionary): print("--- Lobby: promote_member_callback: ", data))
	EOS.get_instance().lobby_interface_kick_member_callback.connect(func (data: Dictionary): print("--- Lobby: kick_member_callback: ", data))
	EOS.get_instance().lobby_interface_hard_mute_member_callback.connect(func (data: Dictionary): print("--- Lobby: hard_mute_member_callback: ", data))
	EOS.get_instance().lobby_interface_send_invite_callback.connect(func (data: Dictionary): print("--- Lobby: send_invite_callback: ", data))
	EOS.get_instance().lobby_interface_reject_invite_callback.connect(func (data: Dictionary): print("--- Lobby: reject_invite_callback: ", data))
	EOS.get_instance().lobby_interface_query_invites_callback.connect(func (data: Dictionary): print("--- Lobby: query_invites_callback: ", data))

	show_create_lobby_btn.pressed.connect(_on_show_create_lobby_btn_pressed)


func create_lobby(data: Dictionary):
	var opts = EOS.Lobby.CreateLobbyOptions.new().from_dict(data)
	EOS.Lobby.LobbyInterface.create_lobby(opts)
	print("--- Lobies: create_lobby_callback: ", await EOS.get_instance().lobby_interface_create_lobby_callback)


func _on_show_create_lobby_btn_pressed():
	create_lobby_popup.popup_centered()


func _on_create_lobby_callback(data: Dictionary):
	print("--- Lobby: create_lobby_callback: ", data)

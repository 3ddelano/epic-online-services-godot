class_name LobbiesView
extends VBoxContainer


enum Maps {
	Margao,
	Panaji,
	Canacona,
	Chandor,
	Bellandur,
	Dubai,
	NewYork,
	Manhattan
}

const Skins = {
	Human = "Human",
	Robot = "Robot",
	Cyborg = "Cyborg",
	Alien = "Alien",
	Martian = "Martian",
	Werewolf = "Werewolf"
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
	
	hide()

	hide_search_results()
	hide_current_lobby()


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

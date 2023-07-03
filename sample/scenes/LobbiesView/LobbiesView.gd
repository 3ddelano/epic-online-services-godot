class_name LobbiesView
extends VBoxContainer

enum Maps {
	Margao,
	Panaji,
	Canacona,
	Chandor
}

const MAX_PLAYERS = 6


# Local cache of lobbies data
var lobbies = []

@onready var _create_lobby_popup: PopupPanel = %CreateLobbyPopup


func _ready() -> void:
	_create_lobby_popup.show()

	var _c
#	_c = Store.login_success.connect(_on_login_success)
#	_c = Store.logout_success.connect(_on_logout_success)
#	_c = view_leaderboard_btn.pressed.connect(_on_view_leaderboard_btn_pressed)


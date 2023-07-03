class_name CreateLobbyPopup
extends StyledPopupWindow


@onready var _bucket_id = $VB/MC/VB/BucketId
@onready var _map_option_btn = $VB/MC/VB/HBoxContainer/MapOptionBtn
@onready var _max_players_options_btn = $VB/MC/VB/HBoxContainer/MaxPlayersOptionsBtn


func _ready() -> void:
	super._ready()
	_map_option_btn.add_item("Choose a map", -1)
	for character in LobbiesView.Maps.keys():
		_map_option_btn.add_item(character, LobbiesView.Maps[character])

	_max_players_options_btn.add_item("Choose max players", -1)
	for player_size in range(1, LobbiesView.MAX_PLAYERS+1):
		_max_players_options_btn.add_item(str(player_size), player_size)

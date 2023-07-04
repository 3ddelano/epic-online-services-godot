class_name CreateLobbyPopup
extends StyledPopupWindow


@onready var _bucket_id: LineEdit = $VB/MC/VB/BucketId
@onready var _map_option_btn: OptionButton = $VB/MC/VB/HBoxContainer/MapOptionBtn
@onready var _max_players_options_btn: OptionButton = $VB/MC/VB/HBoxContainer/MaxPlayersOptionsBtn
@onready var _public_check_box: CheckBox = $VB/MC/VB/HFlowContainer/PublicCheckBox
@onready var _allow_invites_check_box: CheckBox = $VB/MC/VB/HFlowContainer/AllowInvitesCheckBox
@onready var _presence_check_box: CheckBox = $VB/MC/VB/HFlowContainer/PresenceCheckBox
@onready var _rtc_voice_room_check_box: CheckBox = $VB/MC/VB/HFlowContainer/RTCVoiceRoomCheckBox
@onready var _create_lobby_btn: Button = $VB/MC/VB/CreateLobbyBtn


func _ready() -> void:
	super._ready()
	_populate_options_btns()

	_create_lobby_btn.pressed.connect(_on_create_lobby_btn_pressed)
	_bucket_id.text_changed.connect(_on_bucket_id_text_changed)

	visibility_changed.connect(_on_visibility_changed)


func _populate_options_btns():
	var map_names = LobbiesView.Maps.keys()
	for i in range(map_names.size()):
		_map_option_btn.add_item("Map: " + map_names[i])
		_map_option_btn.set_item_metadata(i, LobbiesView.Maps[map_names[i]])

	for i in range(LobbiesView.MAX_PLAYERS):
		_max_players_options_btn.add_item("Max players: " + str(i + 1), i)
		_max_players_options_btn.set_item_metadata(i, i + 1)


func _on_create_lobby_btn_pressed():
	var _view: LobbiesView = Store.get_view("Lobbies")
	_view.create_lobby({
		local_user_id = Store.product_user_id,
		bucket_id = _bucket_id.text.strip_edges(),
		max_lobby_members = _max_players_options_btn.get_selected_metadata(),
		presence_enabled = _presence_check_box.button_pressed,
		enable_rtc_room = _rtc_voice_room_check_box.button_pressed,
		permission_level = EOS.Lobby.LobbyPermissionLevel.PublicAdvertised if _public_check_box.button_pressed else EOS.Lobby.LobbyPermissionLevel.InviteOnly,
		allow_invites = _allow_invites_check_box.button_pressed,
	})
	hide()


func _on_visibility_changed():
	if visible:
		_reset()


func _reset():
	_bucket_id.text = ""
	_create_lobby_btn.disabled = true

	_map_option_btn.select(0)
	_max_players_options_btn.select(0)


func _on_bucket_id_text_changed(new_text: String):
	if new_text.strip_edges() != "":
		_create_lobby_btn.disabled = false
	else:
		_create_lobby_btn.disabled = true

class_name CreateLobbyPopup
extends StyledPopupWindow


@onready var _bucket_id: LineEdit = %BucketId
@onready var _map_option_btn: OptionButton = %MapOptionBtn
@onready var _max_players_options_btn: OptionButton = %MaxPlayersOptionsBtn
@onready var _public_check_box: CheckBox = %PublicCheckBox
@onready var _allow_invites_check_box: CheckBox = %AllowInvitesCheckBox
@onready var _presence_check_box: CheckBox = %PresenceCheckBox
@onready var _rtc_voice_room_check_box: CheckBox = %RTCVoiceRoomCheckBox
@onready var _create_lobby_btn: Button = %CreateLobbyBtn


func _ready() -> void:
	super._ready()
	_populate_options_btns()

	_bucket_id.text_submitted.connect(func (_text): _on_create_lobby_btn_pressed())
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
	var create_opts := EOS.Lobby.CreateLobbyOptions.new()
	create_opts.bucket_id = _bucket_id.text.strip_edges()
	create_opts.max_lobby_members = _max_players_options_btn.get_selected_metadata()
	create_opts.presence_enabled = _presence_check_box.button_pressed
	create_opts.enable_rtc_room = _rtc_voice_room_check_box.button_pressed
	create_opts.permission_level = EOS.Lobby.LobbyPermissionLevel.PublicAdvertised if _public_check_box.button_pressed else EOS.Lobby.LobbyPermissionLevel.InviteOnly
	create_opts.allow_invites = _allow_invites_check_box.button_pressed
	create_opts.local_rtc_options = {
		flags = EOS.RTC.JoinRoomFlags.EnableDataChannel
	}

	_create_lobby_btn.disabled = true
	var lobby := await HLobbies.create_lobby_async(create_opts)
	if not lobby:
		print("Failed to create lobby!")
		_create_lobby_btn.disabled = false
		return


	lobby.add_attribute(LobbiesView.MAP_ATTRIBUTE_KEY, LobbiesView.Maps.keys()[_map_option_btn.get_selected_metadata()])
	lobby.add_attribute(LobbiesView.OWNER_NAME_ATTRIBUTE_KEY, HAuth.display_name)

	lobby.add_current_member_attribute(LobbiesView.USERNAME_ATTRIBUTE_KEY, HAuth.display_name)
	lobby.add_current_member_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY, Store.get_new_random(LobbiesView.Skins.values()))
	
	await lobby.update_async()

	
	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	lobbies_view.set_current_lobby(lobby)
	_create_lobby_btn.disabled = false
	hide()


func _on_visibility_changed():
	if visible:
		_reset()


func _reset():
	_bucket_id.text = "battle_royale"
	_create_lobby_btn.disabled = false

	_map_option_btn.select(0)
	_max_players_options_btn.select(3)


func _on_bucket_id_text_changed(new_text: String):
	if new_text.strip_edges() != "":
		_create_lobby_btn.disabled = false
	else:
		_create_lobby_btn.disabled = true

class_name CurrentLobby
extends VBoxContainer

@onready var id_label: LineEdit = $HBoxContainer/IdLabel
@onready var owner_label: LineEdit = $HBoxContainer/OwnerLabel
@onready var permission_label: LineEdit = $HBoxContainer2/PermissionLabel
@onready var map_label: LineEdit = $HBoxContainer2/MapLabel
@onready var members: GridContainer = $Members
@onready var destroy_lobby_btn: Button = %DestroyLobbyBtn
@onready var random_map_btn: Button = %RandomMapBtn
@onready var leave_lobby_btn: Button = %LeaveLobbyBtn
@onready var random_skin_btn: Button = %RandomSkinBtn
@onready var mute_unmute_btn: Button = %MuteUnmuteBtn


const GRID_CONTAINER_LABELS_COUNT = 5

var cached_lobby := HLobby.new()


func _ready() -> void:
	destroy_lobby_btn.pressed.connect(_on_destroy_lobby_btn_pressed)
	random_map_btn.pressed.connect(_on_random_map_btn_pressed)
	leave_lobby_btn.pressed.connect(_on_leave_lobby_btn_pressed)
	random_skin_btn.pressed.connect(_on_random_skin_btn_pressed)
	mute_unmute_btn.pressed.connect(_on_mute_unmute_btn_pressed)
	

func update(lobby: HLobby):
	cached_lobby = lobby
	cached_lobby.lobby_updated.connect(_update)
	cached_lobby.kicked_from_lobby.connect(func ():
		_reset()
		hide()
	)
	cached_lobby.lobby_owner_changed.connect(func ():
		if not cached_lobby.is_owner():
			return

		var mem = cached_lobby.get_owner()
		if not mem:
			return
		
		var username_attr = mem.get_attribute(LobbiesView.USERNAME_ATTRIBUTE_KEY)
		if not username_attr:
			return
		
		lobby.add_attribute(LobbiesView.OWNER_NAME_ATTRIBUTE_KEY, username_attr.value)
		await lobby.update_async()
	)
	_update()


func _reset():
	id_label.text = "Id: ?"
	owner_label.text = "Owner: ?"
	permission_label.text = "Permission: ?"
	map_label.text = "Map: ?"
	_reset_lobby_members()
	_reset_buttons()


func _reset_lobby_members():
	var idx = members.get_child_count() - 1
	while idx >= GRID_CONTAINER_LABELS_COUNT:
		var node = members.get_child(idx)
		node.queue_free()
		idx -= 1


func _reset_buttons():
	destroy_lobby_btn.hide()
	random_map_btn.hide()
	leave_lobby_btn.hide()
	random_skin_btn.hide()
	mute_unmute_btn.hide()


func _update():
	_reset()

	if cached_lobby.is_valid():
		show()
	else:
		hide()

	if cached_lobby.lobby_id:
		id_label.text = "Id: " + cached_lobby.lobby_id
	
	if cached_lobby.owner_product_user_id:
		var lobby_owner := cached_lobby.get_owner()
		var owner_name_attr = cached_lobby.get_attribute(LobbiesView.OWNER_NAME_ATTRIBUTE_KEY)
		var owner_name = ""
		if owner_name_attr:
			owner_name = owner_name_attr.value
		elif lobby_owner and lobby_owner.display_name:
			owner_name = lobby_owner.display_name
		owner_label.text = "Owner: %s(%s)" % [owner_name, cached_lobby.owner_product_user_id]

	# Lobby permission level
	var perm = "Public"
	if cached_lobby.permission_level == EOS.Lobby.LobbyPermissionLevel.JoinViaPresence:
		perm = "JoinViaPresence"
	elif cached_lobby.permission_level == EOS.Lobby.LobbyPermissionLevel.InviteOnly:
		perm = "InviteOnly"
	permission_label.text = "Permission: " + perm

	# Lobby Attr: Map
	var map_attr = cached_lobby.get_attribute(LobbiesView.MAP_ATTRIBUTE_KEY)
	if map_attr:
		map_label.text = "Map: " + map_attr.value

	# Lobby members
	_update_lobby_members()
	
	# Buttons
	_update_buttons()


func _update_lobby_members():
	var connected_members = 0
	for mem in cached_lobby.members:
		if mem.rtc_state.is_in_rtc_room:
			connected_members += 1

	for mem in cached_lobby.members:
		var name_label = Label.new()
		
		var display_name_attr = mem.get_attribute(LobbiesView.USERNAME_ATTRIBUTE_KEY)
		var mem_name = "User %d" % randi_range(1000, 9999)
		if display_name_attr:
			mem_name = display_name_attr.value
		name_label.text = "%s(%s)" % [mem_name, mem.product_user_id]
		members.add_child(name_label)

		var is_owner_label = Label.new()
		is_owner_label.text = "Owner" if cached_lobby.is_owner(mem.product_user_id) else "Member"
		members.add_child(is_owner_label)

		var skin_label = Label.new()
		skin_label.text = "?"
		var skin_attr = mem.get_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY)
		if skin_attr:
			skin_label.text = skin_attr.value
		members.add_child(skin_label)
		
		var talking_status = "Unknown"
		var text_color = Color.DARK_GRAY
		if not cached_lobby.rtc_room_enabled:
			talking_status = "RTC Disabled"
		else:
			if not mem.rtc_state.is_in_rtc_room:
				talking_status = "Disconnected"
			elif mem.rtc_state.is_hard_muted:
				talking_status = "Hard Muted"
				text_color = Color.RED
			elif mem.rtc_state.is_locally_muted:
				talking_status = "Muted"
				text_color = Color.RED
			elif mem.rtc_state.is_audio_output_disabled:
				talking_status = "Self muted"
				text_color = Color.DARK_RED
			elif mem.rtc_state.is_talking:
				talking_status = "Talking"
				text_color = Color.GREEN
			elif connected_members < 2:
				talking_status = "Connected"
				text_color = Color.WHITE
			else:
				talking_status = "Not Talking"
				text_color = Color.YELLOW

		var talking_node = Label.new()
		talking_node.text = talking_status
		talking_node.modulate = text_color
		members.add_child(talking_node)

		# TODO: add action button to kick, promote, shuffle skin
		var action_label = Label.new()
		members.add_child(action_label)


func _update_buttons():
	if not cached_lobby.is_valid():
		return
		

	if cached_lobby.is_owner():
		destroy_lobby_btn.show()
		random_map_btn.show()

	leave_lobby_btn.show()
	random_skin_btn.show()
	if cached_lobby.rtc_room_enabled:
		mute_unmute_btn.show()


func _on_leave_lobby_btn_pressed():
	var success = await cached_lobby.leave_async()
	if not success:
		print("failed to leave lobby")
		return
	update(HLobby.new())


func _on_destroy_lobby_btn_pressed():
	var success = await cached_lobby.destroy_async()
	if not success:
		print("failed to destroy lobby")
		return


func _on_random_skin_btn_pressed():
	cached_lobby.add_current_member_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY, LobbiesView.Skins.values().pick_random())
	await cached_lobby.update_async()


func _on_random_map_btn_pressed():
	cached_lobby.add_attribute(LobbiesView.MAP_ATTRIBUTE_KEY, LobbiesView.Maps.keys().pick_random())
	await cached_lobby.update_async()


func _on_mute_unmute_btn_pressed():
	var mem = cached_lobby.get_current_member()
	if not mem:
		print("failed to get member")
		return
	
	var success = await mem.mute_member_async()
	if not success:
		print("mute/unmute failed")

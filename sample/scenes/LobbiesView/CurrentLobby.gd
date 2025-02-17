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
@onready var toggle_mute_btn: Button = %MuteUnmuteBtn

const PRIMARY_BUTTON = preload("res://scenes/UI/PrimaryButton.tscn")

const GRID_CONTAINER_LABELS_COUNT = 5

var cached_lobby := HLobby.new()


func _ready() -> void:
	destroy_lobby_btn.pressed.connect(_on_destroy_lobby_btn_pressed)
	random_map_btn.pressed.connect(_on_random_map_btn_pressed)
	leave_lobby_btn.pressed.connect(_on_leave_lobby_btn_pressed)
	random_skin_btn.pressed.connect(_on_random_skin_btn_pressed)
	toggle_mute_btn.pressed.connect(_on_toggle_mute_btn_pressed)
	

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
	toggle_mute_btn.hide()


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

		var actions_hbox = HBoxContainer.new()
		
		if cached_lobby.is_owner() and not mem.is_self():
			if not mem.is_owner():
				# Can promote
				var promote_btn = PRIMARY_BUTTON.instantiate()
				promote_btn.text = "Promote"
				promote_btn.pressed.connect(_on_promote_btn_pressed.bind(mem))
				actions_hbox.add_child(promote_btn)
			
			# Can kick
			var kick_btn = PRIMARY_BUTTON.instantiate()
			kick_btn.text = "Kick"
			kick_btn.pressed.connect(_on_kick_btn_pressed.bind(mem))
			actions_hbox.add_child(kick_btn)

			# Can toggle hard-mute
			var toggle_hardmute_btn = PRIMARY_BUTTON.instantiate()
			if mem.is_hard_muted():
				toggle_hardmute_btn.text = "Un hard-mute"
			else:
				toggle_hardmute_btn.text = "Hard-mute"
			toggle_hardmute_btn.pressed.connect(_on_toggle_hard_mute_btn_pressed.bind(mem))
			actions_hbox.add_child(toggle_hardmute_btn)
				
		members.add_child(actions_hbox)


func _update_buttons():
	if not cached_lobby.is_valid():
		return
		

	if cached_lobby.is_owner():
		destroy_lobby_btn.show()
		random_map_btn.show()

	leave_lobby_btn.show()
	random_skin_btn.show()
	if cached_lobby.rtc_room_enabled:
		toggle_mute_btn.show()
	
	if cached_lobby and cached_lobby.get_current_member():
		toggle_mute_btn.text = "Unmute" if cached_lobby.get_current_member().is_muted() else "Mute"


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
	var skin_attr = cached_lobby.get_current_member_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY)
	var current_skin = skin_attr.get("value", null)
	cached_lobby.add_current_member_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY, Store.get_new_random(LobbiesView.Skins.values(),current_skin))
	await cached_lobby.update_async()


func _on_random_map_btn_pressed():
	var map_attr = cached_lobby.get_current_member_attribute(LobbiesView.MAP_ATTRIBUTE_KEY)
	var current_map = map_attr.get("value", null)
	cached_lobby.add_attribute(LobbiesView.MAP_ATTRIBUTE_KEY, Store.get_new_random(LobbiesView.Maps.values(),current_map))
	await cached_lobby.update_async()


func _on_toggle_mute_btn_pressed():
	var mem = cached_lobby.get_current_member()
	if not mem:
		print("failed to get member")
		return
	
	var success := await mem.toggle_mute_member_async()
	if not success:
		print("mute/unmute failed")


func _on_promote_btn_pressed(mem: HLobbyMember):
	var success := await mem.promote_member_async()
	if not success:
		print("failed to promote member")
		return

func _on_kick_btn_pressed(mem: HLobbyMember):
	var success := await mem.kick_member_async()
	if not success:
		print("failed to kick member")
		return

func _on_toggle_hard_mute_btn_pressed(mem: HLobbyMember):
	var success := await mem.toggle_hard_mute_member_async()
	if not success:
		print("failed to toggle member hard mute")
		return
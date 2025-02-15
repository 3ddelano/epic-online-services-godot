class_name CurrentLobby
extends VBoxContainer

@onready var id_label: LineEdit = $HBoxContainer/IdLabel
@onready var owner_label: LineEdit = $HBoxContainer/OwnerLabel
@onready var permission_label: LineEdit = $HBoxContainer2/PermissionLabel
@onready var map_label: LineEdit = $HBoxContainer2/MapLabel
@onready var members: GridContainer = $Members

const GRID_CONTAINER_LABELS_COUNT = 5

var cached_lobby := HLobby.new()


func _ready() -> void:
	IEOS.rtc_interface_disconnected.connect(func(data: Dictionary):
		print("--- Lobbies: RTC disconnected: ", data)
	)
	IEOS.rtc_interface_participant_status_changed.connect(func(data: Dictionary):
		print("--- Lobbies: RTC participant status changed: ", data)
		# var status_text = "Joined"
		# if data["participant_status"] == EOS.RTC.ParticipantStatus.Left:
		# 	status_text = "Left"

		#if cached_lobby.is_valid() and cached_lobby.is_rtc_room_enabled():
			#if cached_lobby.rtc_room_name == data["room_name"]:
				# TODO: update the member's status
				#pass
	)
	# IEOS.rtc_audio_participant_updated.connect(_on_rtc_audio_participant_updated)


func update(lobby: HLobby):
	cached_lobby = lobby
	cached_lobby.lobby_updated.connect(_update)
	_update()


func _reset():
	id_label.text = "Id: ?"
	owner_label.text = "Owner: ?"
	permission_label.text = "Permission: ?"
	map_label.text = "Map: ?"
	_reset_lobby_members()


func _reset_lobby_members():
	var idx = members.get_child_count() - 1
	while idx >= GRID_CONTAINER_LABELS_COUNT:
		var node = members.get_child(idx)
		node.queue_free()
		idx -= 1


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
	if map_attr != null:
		map_label.text = "Map: " + map_attr.value

	# Lobby members
	_update_lobby_members()


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
		if skin_attr != null:
			skin_label.text = skin_attr.value
		members.add_child(skin_label)

		var talking_status = "Unknown"
		var text_color = Color.DARK_GRAY
		if not cached_lobby.rtc_room_connected:
			pass
		elif not mem.rtc_state.is_in_rtc_room:
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
			text_color = Color.YELLOW
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


# func _on_rtc_audio_participant_updated(data: Dictionary):
# 	print("--- Lobby: RTCAudio: participant_updated: ")
# 	var is_talking = data.speaking
# 	var is_audio_disabled = data.audio_status != EOS.RTCAudio.AudioStatus.Enabled
# 	var is_hard_muted = data.audio_status != EOS.RTCAudio.AudioStatus.AdminDisabled

# 	# Update the member in the current lobby
# 	if cached_lobby.rtc_room_name.is_empty() or cached_lobby.rtc_room_name != data.room_name:
# 		return

# 	# Find the participant in the lobby members
# 	var mem = cached_lobby.get_member_by_product_user_id(data.participant_id)
# 	if mem == null:
# 		return

# 	mem.rtc_state.is_talking = is_talking
# 	if mem.product_user_id != HAuth.product_user_id:
# 		mem.rtc_state.is_audio_output_disabled = is_audio_disabled
# 	else:
# 		mem.rtc_state.is_hard_muted = is_hard_muted

# 	_update()

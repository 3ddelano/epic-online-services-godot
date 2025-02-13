class_name CurrentLobby
extends VBoxContainer

@onready var id_label: LineEdit = $HBoxContainer/IdLabel
@onready var owner_label: LineEdit = $HBoxContainer/OwnerLabel
@onready var permission_label: LineEdit = $HBoxContainer2/PermissionLabel
@onready var map_label: LineEdit = $HBoxContainer2/MapLabel
@onready var members: GridContainer = $Members

const GRID_CONTAINER_LABELS_COUNT = 5

## Handles async querying of values with cache
class QueryCache:
	signal query_finished(key, value)
	
	var query_fn: Callable
	var values = {}
	
	var _current_queries = {}
	
	func _init(p_query_fn: Callable):
		query_fn = p_query_fn
	
	func get_value(key, default=null):
		if values.has(key):
			return values[key]
		
		_query(key)
		
		return default
	
	func _query(key):
		if _current_queries.has(key):
			return
		
		_current_queries[key] = true
		var value = await query_fn.call(key)
		values[key] = value
		_current_queries.erase(key)
		query_finished.emit(key, value)

## Product User Ids to Epic Account Ids
@onready var account_mappings = QueryCache.new(_query_account_mapping)

## Epic Account Ids to display name
@onready var display_names = QueryCache.new(_query_display_name)

var cached_lobby := LobbiesView.GameLobby.new()

func _ready() -> void:
	account_mappings.query_finished.connect(func(key, value):
		_update()
	)
	display_names.query_finished.connect(func(key, value):
		_update()
	)
	
	IEOS.rtc_interface_disconnected.connect(func(data: Dictionary):
		print("--- Lobbies: RTC disconnected: ", data)
	)
	IEOS.rtc_interface_participant_status_changed.connect(func(data: Dictionary):
		print("--- Lobbies: RTC participant status changed: ", data)

		var status_text="Joined"
		if data["participant_status"] == EOS.RTC.ParticipantStatus.Left:
			status_text="Left"

		#if cached_lobby.is_valid() and cached_lobby.is_rtc_room_enabled():
			#if cached_lobby.rtc_room_name == data["room_name"]:
				# TODO: update the member's status
				#pass
	)
	# IEOS.rtc_audio_participant_updated.connect(_on_rtc_audio_participant_updated)

	if not cached_lobby.is_valid():
		hide()
	else:
		show()

func update(lobby: LobbiesView.GameLobby):
	cached_lobby = lobby
	_update()

func _reset_lobby_members():
	var cnt_nodes_to_free = members.get_child_count() - GRID_CONTAINER_LABELS_COUNT
	for i in range(cnt_nodes_to_free):
		var node = members.get_child(GRID_CONTAINER_LABELS_COUNT + i)
		node.queue_free()

func _reset():
	id_label.text = "Id: ?"
	owner_label.text = "Owner: ?"
	permission_label.text = "Permission: ?"
	map_label.text = "Map: ?"
	_reset_lobby_members()

func _update_lobby_members():
	var connected_members = 0
	for mem in cached_lobby.members:
		if mem.rtc_state.is_in_rtc_room:
			connected_members += 1

	for mem in cached_lobby.members:
		var name_label = Label.new()
		name_label.text = "%s(%s)" % [mem.display_name, mem.product_id]
		members.add_child(name_label)

		var isonwer_label = Label.new()
		isonwer_label.text = "Owner" if cached_lobby.is_owner(mem.product_id) else "Member"
		members.add_child(isonwer_label)

		var skin_label = Label.new()
		skin_label.text = "?"
		var skin_attr = mem.get_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY)
		if skin_attr != null:
			skin_label.text = skin_attr.data.value
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

func _update():
	_reset()

	if not cached_lobby.is_valid():
		hide()
	else:
		show()

	if cached_lobby.id:
		id_label.text = "Id: " + cached_lobby.id
	
	if cached_lobby.owner_id:
		var owner_name = cached_lobby.get_owner().display_name if cached_lobby.get_owner() else "? "
		owner_label.text = "Owner: %s(%s)" % [owner_name, cached_lobby.owner_id]

	# Lobby permission level
	var perm = "Public"
	if cached_lobby.permission_level == EOS.Lobby.LobbyPermissionLevel.JoinViaPresence:
		perm = "JoinViaPresence"
	elif cached_lobby.permission_level == EOS.Lobby.LobbyPermissionLevel.InviteOnly:
		perm = "InviteOnly"
	permission_label.text = "Permission: " + perm

	# Lobby Attr: Map
	var map_attr = cached_lobby.get_attribute("LEVEL")
	if map_attr != null:
		map_label.text = "Map: " + map_attr.data.value

	# Lobby members
	for mem: LobbiesView.GameLobbyMember in cached_lobby.members:
		if mem.account_id.is_empty():
			# Get epic account id from cache
			mem.account_id = account_mappings.get_value(mem.product_id, "")

		# Try to get display name
		if not mem.account_id.is_empty() and mem.display_name.is_empty():
			# Get display name from cached
			mem.display_name = display_names.get_value(mem.account_id, "")
		_update_lobby_members()

func _query_account_mapping(puid):
	var query_opts = EOS.Connect.QueryProductUserIdMappingsOptions.new()
	query_opts.product_user_ids = [puid]
	EOS.Connect.ConnectInterface.query_product_user_id_mappings(query_opts)

	var query_ret = await IEOS.connect_interface_query_product_user_id_mappings_callback

	if not EOS.is_success(query_ret):
		print("--- Lobby: Connect: query_product_user_id_mappings_callback: error: ", EOS.result_str(query_ret))
		return ""

	var get_opts = EOS.Connect.GetProductUserIdMappingOptions.new()
	get_opts.target_product_user_id = puid
	get_opts.account_id_type = EOS.ExternalAccountType.Epic
	var get_ret = EOS.Connect.ConnectInterface.get_product_user_id_mapping(get_opts)
	if not EOS.is_success(get_ret):
		print("--- Lobby: Connect: get_product_user_id_mapping: error: ", EOS.result_str(get_ret))
		return ""

	return get_ret["account_id"]

func _query_display_name(acc_id):
	var query_opts = EOS.UserInfo.QueryUserInfoOptions.new()
	query_opts.target_user_id = acc_id
	EOS.UserInfo.UserInfoInterface.query_user_info(query_opts)

	var query_ret = await IEOS.user_info_interface_query_user_info_callback
	if not EOS.is_success(query_ret):
		print("--- Lobby: UserInfo: query_user_info_callback: error: ", EOS.result_str(query_ret))
		return ""

	var copy_best_name_opts = EOS.UserInfo.CopyBestDisplayNameOptions.new()
	copy_best_name_opts.target_user_id = acc_id

	var copy_best_name_ret = EOS.UserInfo.UserInfoInterface.copy_best_display_name(copy_best_name_opts)
	if not EOS.is_success(copy_best_name_ret):
		print("--- Lobby: UserInfo: copy_best_display_name: error: ", EOS.result_str(copy_best_name_ret))
		return ""

	return copy_best_name_ret.best_display_name

func _on_rtc_audio_participant_updated(data: Dictionary):
	print("--- Lobby: RTCAudio: participant_updated: ")
	var is_talking = data.speaking
	var is_audio_disabled = data.audio_status != EOS.RTCAudio.AudioStatus.Enabled
	var is_hard_muted = data.audio_status != EOS.RTCAudio.AudioStatus.AdminDisabled

	# Update the member in the current lobby
	if cached_lobby.rtc_room_name.is_empty() or cached_lobby.rtc_room_name != data.room_name:
		return

	# Find the participant in the lobby members
	var mem = cached_lobby.get_member_by_product_user_id(data.participant_id)
	if mem == null:
		return

	mem.rtc_state.is_talking = is_talking
	if mem.product_id != HAuth.product_user_id:
		mem.rtc_state.is_audio_output_disabled = is_audio_disabled
	else:
		mem.rtc_state.is_hard_muted = is_hard_muted

	_update()

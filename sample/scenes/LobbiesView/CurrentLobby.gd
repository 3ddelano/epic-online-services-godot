class_name CurrentLobby
extends VBoxContainer

@onready var id_label: LineEdit = $HBoxContainer/IdLabel
@onready var owner_label: LineEdit = $HBoxContainer/OwnerLabel
@onready var permission_label: LineEdit = $HBoxContainer2/PermissionLabel
@onready var map_label: LineEdit = $HBoxContainer2/MapLabel
@onready var members: GridContainer = $Members

## Product User Ids to Epic Account Ids
var account_mappings = {}
## Product User Ids which need to be queried
var account_mapping_query = []
## Product User Ids currently being queried
var current_queried_account_mapping = []

## Epic Account Ids to display name
var display_names = {}
var display_names_query = {}
var current_queried_display_names = []

var cached_lobby := LobbiesView.GameLobby.new()

func _ready() -> void:
	IEOS.rtc_interface_disconnected.connect(func (data: Dictionary):
		print("--- Lobbies: RTC disconnected: ", data)
	)
	IEOS.rtc_interface_participant_status_changed.connect(func (data: Dictionary):
		print("--- Lobbies: RTC participant status changed: ", data)

		var status_text = "Joined"
		if data["participant_status"] == EOS.RTC.ParticipantStatus.Left:
			status_text = "Left"

		#if cached_lobby.is_valid() and cached_lobby.is_rtc_room_enabled():
			#if cached_lobby.rtc_room_name == data["room_name"]:
				# TODO: update the member's status
				#pass
	)
	IEOS.rtc_audio_participant_updated.connect(_on_rtc_audio_participant_updated)


	if not cached_lobby.is_valid():
		hide()
	else:
		show()


func update(lobby: LobbiesView.GameLobby):
	cached_lobby = lobby
	_update()


func _reset_lobby_members():
	while members.get_child_count() > 5:
		var node = members.get_child(members.get_child_count()-1)
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

	if cached_lobby.id != "":
		id_label.text = "Id: " + cached_lobby.id
	if cached_lobby.owner_id != "":
		var owner_name = cached_lobby.get_owner().display_name
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
			mem.account_id = account_mappings.get(mem.product_id, "")
			# Still invalid means need to query
			if mem.account_id.is_empty():
				if not mem.product_id in account_mapping_query:
					account_mapping_query.append(mem.product_id)

		# Try to get display name
		if not mem.account_id.is_empty():
			if mem.display_name.is_empty():
				# Get display name from cached
				mem.display_name = display_names.get(mem.account_id, "")

				# Still invalid means need to query
				if mem.display_name.is_empty():
					if not mem.account_id in display_names_query:
						display_names_query.append(mem.account_id)
		_update_lobby_members()

	if account_mapping_query.size() > 0:
		_query_account_mappings()

	if display_names_query.size() > 0:
		_query_display_names()


func _query_account_mappings():
	var filtered_puids = []
	for puid in account_mapping_query:
		if current_queried_account_mapping.has(puid): continue
		if account_mappings.has(puid): continue
		if filtered_puids.has(puid): continue
		filtered_puids.append(puid)

	if filtered_puids.is_empty():
		return

	# Remove the ones that are currently being queried
	for puid in filtered_puids:
		current_queried_account_mapping.append(puid)
		account_mapping_query.erase(puid)

	var query_opts = EOS.Connect.QueryProductUserIdMappingsOptions.new()
	query_opts.product_user_ids = filtered_puids
	EOS.Connect.ConnectInterface.query_product_user_id_mappings(query_opts)

	var query_ret = await IEOS.connect_interface_query_product_user_id_mappings_callback

	if not EOS.is_success(query_ret):
		print("--- Lobby: Connect: query_product_user_id_mappings_callback: error: ", EOS.result_str(query_ret))
		return

	for puid in filtered_puids:
		var get_opts = EOS.Connect.GetProductUserIdMappingOptions.new()
		get_opts.target_product_user_id = puid
		get_opts.account_id_type = EOS.ExternalAccountType.Epic
		var get_ret = EOS.Connect.ConnectInterface.get_product_user_id_mapping(get_opts)
		if not EOS.is_success(get_ret):
			print("--- Lobby: Connect: get_product_user_id_mapping: error: ", EOS.result_str(get_ret))
			continue

		var account_id = get_ret["account_id"]
		account_mappings[puid] = account_id
		current_queried_account_mapping.erase(puid)

	_update()

func _query_display_names():
	var filtered_acc_ids = []
	for acc_id in display_names_query:
		if current_queried_display_names.has(acc_id): continue
		if display_names.has(acc_id): continue
		if filtered_acc_ids.has(acc_id): continue
		filtered_acc_ids.append(acc_id)

	if filtered_acc_ids.is_empty():
		return

	for acc_id in filtered_acc_ids:
		current_queried_display_names.append(acc_id)
		display_names_query.erase(acc_id)

	for acc_id in filtered_acc_ids:
		var query_opts = EOS.UserInfo.QueryUserInfoOptions.new()
		query_opts.target_user_id = acc_id
		EOS.UserInfo.UserInfoInterface.query_user_info(query_opts)

		var query_ret = await IEOS.user_info_interface_query_user_info_callback
		if not EOS.is_success(query_ret):
			print("--- Lobby: UserInfo: query_user_info_callback: error: ", EOS.result_str(query_ret))
			continue

		var copy_best_name_opts = EOS.UserInfo.CopyBestDisplayNameOptions.new()
		copy_best_name_opts.target_user_id = acc_id

		var copy_best_name_ret = EOS.UserInfo.UserInfoInterface.copy_best_display_name(copy_best_name_opts)
		if not EOS.is_success(copy_best_name_ret):
			print("--- Lobby: UserInfo: copy_best_display_name: error: ", EOS.result_str(copy_best_name_ret))
			continue
		display_names[acc_id] = copy_best_name_ret.best_display_name
		current_queried_display_names.erase(acc_id)

	_update()


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
	if mem.product_id != Store.product_user_id:
		mem.rtc_state.is_audio_output_disabled = is_audio_disabled
	else:
		mem.rtc_state.is_hard_muted = is_hard_muted

	_update()

extends HBoxContainer

enum SearchType {
	Search_Map,
	Search_BucketId,
	Search_LobbyId,
	Search_UserId
}

@onready var search_lobby_type: OptionButton = %SearchLobbyType
@onready var search_lobby_line_edit: LineEdit = %SearchLobbyLineEdit
@onready var search_lobby_btn: Button = %SearchLobbyBtn
@onready var clear_search_lobby_btn: Button = %ClearSearchLobbyBtn

var is_searches_shown = false
var lobby_search: EOSGLobbySearch

func _ready() -> void:
	IEOS.lobby_search_find_callback.connect(_on_lobby_search_find_callback)
	search_lobby_btn.pressed.connect(_on_search_lobby_btn_pressed)

	search_lobby_line_edit.text_changed.connect(_on_search_lobby_line_edit_text_changed)
	search_lobby_line_edit.text_submitted.connect(func(_new_text): _on_search_lobby_btn_pressed())

	clear_search_lobby_btn.pressed.connect(_on_clear_search_lobby_btn_pressed)

	_populate_search_lobby_options()

func _populate_search_lobby_options():
	var search_type_names = SearchType.keys()
	var search_type_values = SearchType.values()
	for i in range(len(search_type_names)):
		search_lobby_type.add_item(": ".join(search_type_names[i].split("_")), i)
		search_lobby_type.set_item_metadata(i, search_type_values[i])

func _on_search_lobby_btn_pressed():
	var search_type = search_lobby_type.get_selected_metadata()
	var search_string = search_lobby_line_edit.text.strip_edges()

	var create_search_opts = EOS.Lobby.CreateLobbySearchOptions.new()
	create_search_opts.max_results = 25

	var create_search_ret = EOS.Lobby.LobbyInterface.create_lobby_search(create_search_opts)
	if not EOS.is_success(create_search_ret):
		print("Failed to create lobby search")
		return

	lobby_search = create_search_ret.lobby_search
	if search_type == SearchType.Search_Map:
		lobby_search.set_parameter(LobbiesView.MAP_ATTRIBUTE_KEY, search_string, EOS.ComparisonOp.Equal)
	elif search_type == SearchType.Search_BucketId:
		lobby_search.set_parameter(EOS.Lobby.SEARCH_BUCKET_ID, search_string, EOS.ComparisonOp.Equal)
	elif search_type == SearchType.Search_LobbyId:
		lobby_search.set_lobby_id(search_string)
	elif search_type == SearchType.Search_UserId:
		lobby_search.set_target_user_id(search_string)
	else:
		print("Invalid lobby search type")
		return

	lobby_search.find(Store.product_user_id)

func _on_lobby_search_find_callback(data: Dictionary):
	print("--- Lobby: lobby_search_find_callback: ", EOS.result_str(data))

	if not EOS.is_success(data) or lobby_search == null:
		return

	var count = lobby_search.get_search_result_count()
	var search_results: Array[EOSGLobbyDetails] = []
	for search_idx in count:
		var copy_ret = lobby_search.copy_search_result_by_index(search_idx)
		if not EOS.is_success(copy_ret):
			print("Failed to copy lobby search result by index")
			continue

		var lobby_details: EOSGLobbyDetails = copy_ret.lobby_details
		search_results.append(lobby_details)

	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	lobbies_view.handle_search_results(search_results)

func _on_clear_search_lobby_btn_pressed():
	search_lobby_line_edit.text = ""

	lobby_search = null
	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	var search_results: Array[EOSGLobbyDetails] = []
	lobbies_view.handle_search_results(search_results)
	lobbies_view.hide_search_results()

func _on_search_lobby_line_edit_text_changed(new_text: String):
	if new_text.strip_edges() == "":
		search_lobby_btn.disabled = true
		clear_search_lobby_btn.disabled = true
	else:
		search_lobby_btn.disabled = false
		clear_search_lobby_btn.disabled = false

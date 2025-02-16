extends HBoxContainer

#region Enums

const SearchType = {
	Map = "Map",
	BucketId = "BucketId",
	LobbyId = "LobbyId",
	UserId = "UserId",
}

#endregion


#region Onready vars

@onready var search_lobby_type: OptionButton = %SearchLobbyType
@onready var search_lobby_line_edit: LineEdit = %SearchLobbyLineEdit
@onready var search_lobby_btn: Button = %SearchLobbyBtn
@onready var clear_search_lobby_btn: Button = %ClearSearchLobbyBtn

#endregion


#region Built-in methods

func _ready() -> void:
	search_lobby_btn.pressed.connect(_on_search_lobby_btn_pressed)
	search_lobby_line_edit.text_changed.connect(_on_search_lobby_line_edit_text_changed)
	search_lobby_line_edit.text_submitted.connect(func(_new_text): _on_search_lobby_btn_pressed())
	clear_search_lobby_btn.pressed.connect(_on_clear_search_lobby_btn_pressed)

	_populate_search_lobby_type_options()

#endregion


#region Private methods

func _populate_search_lobby_type_options():
	var idx = 0
	for search_type in SearchType.keys():
		search_lobby_type.add_item(search_type, idx)
		search_lobby_type.set_item_metadata(idx, search_type)
		idx += 1


func _on_search_lobby_btn_pressed():
	var search_type = search_lobby_type.get_selected_metadata()
	var search_string = search_lobby_line_edit.text.strip_edges()

	var create_search_opts = EOS.Lobby.CreateLobbySearchOptions.new()
	create_search_opts.max_results = 25

	var lobby_search: EOSGLobbySearch = HLobbies.create_search(create_search_opts)
	if not lobby_search:
		print("Failed to create lobby search")
		return

	var lobbies = []
	if search_type == SearchType.Map:
		lobbies = await HLobbies.search_by_attribute_async({
			key = LobbiesView.MAP_ATTRIBUTE_KEY, value = search_string, comparison = EOS.ComparisonOp.Equal
		})
	elif search_type == SearchType.BucketId:
		lobbies = await HLobbies.search_by_attribute_async({
			key = EOS.Lobby.SEARCH_BUCKET_ID, value = search_string, comparison = EOS.ComparisonOp.Equal
		})
	elif search_type == SearchType.LobbyId:
		lobbies = await HLobbies.search_by_lobby_id_async(search_string)
	elif search_type == SearchType.UserId:
		lobbies = await HLobbies.search_by_product_user_id_async(search_string)
	else:
		print("Invalid lobby search type")
		return

	if lobbies == null:
		print("Failed to search for lobbies")
		return
	
	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	lobbies_view.handle_search_results(lobbies)


func _on_clear_search_lobby_btn_pressed():
	search_lobby_line_edit.text = ""
	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	lobbies_view.handle_search_results([])
	lobbies_view.hide_search_results()


func _on_search_lobby_line_edit_text_changed(new_text: String):
	if new_text.strip_edges() == "":
		search_lobby_btn.disabled = true
	else:
		search_lobby_btn.disabled = false
	clear_search_lobby_btn.disabled = false

#endregion

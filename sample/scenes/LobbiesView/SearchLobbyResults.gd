class_name SearchLobbyResults
extends VBoxContainer

const GRID_CONTAINER_LABELS_COUNT = 4


var search_results: Array[HLobby] = []


@onready var grid: GridContainer = %GridContainer
@onready var PrimaryButton = preload("res://scenes/UI/PrimaryButton.tscn")


func update_results(p_search_results: Array[HLobby]):
	search_results = p_search_results
	update_ui()


func update_ui():
	_reset()

	for lobby in search_results:
		var owner_name_attr = lobby.get_attribute(LobbiesView.OWNER_NAME_ATTRIBUTE_KEY)
		var owner_label = Label.new()
		var owner_name = "?"
		if owner_name_attr != null:
			owner_name = owner_name_attr.value
		owner_label.text = "%s(%s)" % [owner_name, lobby.owner_product_user_id]
		grid.add_child(owner_label)

		var members_label = Label.new()
		members_label.text = "%s/%s" % [lobby.members.size(), lobby.max_members]
		grid.add_child(members_label)

		var map_attr = lobby.get_attribute(LobbiesView.MAP_ATTRIBUTE_KEY)
		var map_label = Label.new()
		map_label.text = "?"
		if map_attr != null:
			map_label.text = map_attr.value
		grid.add_child(map_label)

		var join_btn = PrimaryButton.instantiate()
		grid.add_child(join_btn)
		join_btn.text = "JOIN"
		join_btn.pressed.connect(_on_lobby_join_pressed.bind(lobby.lobby_id))


func _on_lobby_join_pressed(lobby_id: String):
	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	var lobby = await HLobbies.join_by_id_async(lobby_id)
	if not lobby:
		print("Failed to join lobby")
		return
	
	lobby.add_member_attribute(LobbiesView.SKIN_ATTRIBUTE_KEY, LobbiesView.Skins.values().pick_random())
	lobby.add_member_attribute(LobbiesView.USERNAME_ATTRIBUTE_KEY, HAuth.display_name)
	lobbies_view.hide_search_results()
	
	await lobby.update_async()
	lobbies_view.set_current_lobby(lobby)
	

	search_results = []
	_reset()


func _reset():
	var i = grid.get_child_count() - 1
	while i >= GRID_CONTAINER_LABELS_COUNT:
		var node = grid.get_child(i)
		node.queue_free()
		i -= 1

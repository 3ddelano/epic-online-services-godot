class_name SearchLobbyResults
extends VBoxContainer

@onready var grid: GridContainer = %GridContainer
@onready var PrimaryButton = preload("res://scenes/UI/PrimaryButton.tscn")

func update_results(search_results: Array[EOSGLobbyDetails]):
	_reset()

	var count = len(search_results)
	for details in search_results:
		var info = details.copy_info().lobby_details
		var member_count = details.get_member_count()

		var owner_label = Label.new()
		owner_label.text = details.get_lobby_owner()
		grid.add_child(owner_label)

		var members_label = Label.new()
		members_label.text = "%s/%s" % [member_count, info.max_members]
		grid.add_child(members_label)

		var map_attr = null
		for i in range(details.get_attribute_count()):
			var attr = details.copy_attribute_by_index(i).attribute
			if attr.data.key == LobbiesView.MAP_ATTRIBUTE_KEY:
				map_attr = attr

		var map_label = Label.new()
		map_label.text = "?"
		if map_attr != null:
			map_label.text = map_attr.data.value
		grid.add_child(map_label)

		var join_btn = PrimaryButton.instantiate()
		grid.add_child(join_btn)
		join_btn.text = "JOIN"
		join_btn.pressed.connect(Callable(self, "_on_lobby_join_pressed").bind(info.lobby_id))


func _reset():
	if grid.get_child_count() > 4:
		for i in range(grid.get_child_count()-1, 3, -1):
			var node = grid.get_child(i)
			node.queue_free()

func _on_lobby_join_pressed(lobby_id: String):
	var lobbies_view: LobbiesView = Store.get_view("Lobbies")
	lobbies_view.join_lobby(lobby_id)

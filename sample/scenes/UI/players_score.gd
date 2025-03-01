extends Label


func _ready() -> void:
	Store.player_score_changed.connect(_on_players_score_changed)


func _on_players_score_changed():
	text = ""
	if not Store.current_lobby:
		return
	
	var players = get_tree().get_nodes_in_group(Store.GROUP_PLAYER)
	for player in players:
		var mem = Store.current_lobby.get_member_by_product_user_id(player.puid)
		if not mem: continue
		
		print("player %s has score %s" % [player.peer_id, player.score])

		var display_name_attr = mem.get_attribute(LobbiesView.USERNAME_ATTRIBUTE_KEY)
		var display_name = "User"
		if display_name_attr:
			display_name = display_name_attr.value
		
		text += "%s: %s\n" % [display_name, player.score]

#endregion

class_name MyNetwork
extends Node2D


const PLAYER = preload("res://game/entities/player/player.tscn")
const MAP_BELLANDUR = preload("res://game/maps/map_bellandur.tscn")
const MAP_MARGAO = preload("res://game/maps/map_margao.tscn")
const MAP_NEW_YORK = preload("res://game/maps/map_new_york.tscn")


@onready var _player_spawner: MultiplayerSpawner = $Game/PlayerSpawner
@onready var _network_spawn: Node2D = $Game/NetworkSpawn
@onready var _mc: MarginContainer = $"../MC"
@onready var _canvas_layer: CanvasLayer = $Game/CanvasLayer


var peer = EOSGMultiplayerPeer.new()

func _ready() -> void:
	Store.network = self
	_player_spawner.spawn_function = _spawn_player
	_mc.show()
	_canvas_layer.hide()


func create_server():
	print("Creating server...")
	peer.create_server(Store.GAME_SOCKET_NAME)
	peer.peer_connected.connect(_on_peer_connected)
	peer.peer_disconnected.connect(_on_peer_disconnected)
	multiplayer.multiplayer_peer = peer


func start_game(map: LobbiesView.Maps):
	print("Starting game: map=%s" % LobbiesView.Maps.find_key(map))
	_spawn_map(map)
	_player_spawner.spawn({peer_id = 1, puid = HAuth.product_user_id})
	_show_map()


func join_game(remote_puid: String):
	print("Joining game: remote_puid=%s" % remote_puid)
	peer.create_client(Store.GAME_SOCKET_NAME, remote_puid)
	peer.peer_disconnected.connect(_on_peer_disconnected)
	multiplayer.multiplayer_peer = peer
	_show_map()


@rpc("any_peer", "call_local", "reliable")
func add_score(peer_id: int, amt: int):
	var player = _network_spawn.get_node(str(peer_id))
	if player:
		player.score += amt
	Store.player_score_changed.emit()



func _on_peer_connected(peer_id: int):
	var puid = peer.get_peer_user_id(peer_id)
	print("server:: client connected: peer_id=%s, puid=%s" % [peer_id, puid])
	_player_spawner.spawn({peer_id = peer_id, puid = puid})


func _on_peer_disconnected(peer_id: int):
	if multiplayer.is_server():
		print("server:: client disconnected: %s" % peer_id)
		_remove_player_by_peer_id(peer_id)
	else:
		print("client:: server disconnected")
		multiplayer.multiplayer_peer = null


func _spawn_map(map: LobbiesView.Maps):
	var map_node = null
	match map:
		LobbiesView.Maps.Margao:
			map_node = MAP_MARGAO.instantiate()
		LobbiesView.Maps.Bellandur:
			map_node = MAP_BELLANDUR.instantiate()
		LobbiesView.Maps.NewYork:
			map_node = MAP_NEW_YORK.instantiate()
	if map_node:
		_network_spawn.add_child(map_node, true)


func _spawn_player(data: Dictionary):
	var peer_id = data.peer_id
	var puid = data.puid
	
	var player: Player = PLAYER.instantiate()
	player.name = str(peer_id)
	player.peer_id = peer_id
	player.puid = puid
	
	player.global_position = _get_player_spawn_point().global_position
	player.died.connect(_on_player_died)
	
	return player


func _remove_player_by_peer_id(peer_id: int):
	var players = get_tree().get_nodes_in_group(Store.GROUP_PLAYER)
	for player: Player in players:
		if player.peer_id == peer_id:
			player.queue_free()


func _on_player_died(player: Player, _killing_peer_id: int):
	if player.is_multiplayer_authority():
		print("Respawning player: peer_id=%s, puid=%s" % [player.peer_id, player.puid])
		player.global_position = _get_random_spawn_point().global_position
		player.health = 100


func _show_map():
	show()
	_mc.hide()
	_canvas_layer.show()


func _get_random_spawn_point() -> Node2D:
	var map = get_tree().get_nodes_in_group(Store.GROUP_MAP)[0]
	var total_spawn_points = map.get_node("SpawnPoints").get_child_count()
	return map.get_node("SpawnPoints").get_child(randi_range(0, total_spawn_points - 1))
	
	
func _get_player_spawn_point() -> Node2D:
	var map = get_tree().get_nodes_in_group(Store.GROUP_MAP)[0]
	var total_spawn_points = map.get_node("SpawnPoints").get_child_count()
	var idx = get_tree().get_nodes_in_group(Store.GROUP_PLAYER).size() % total_spawn_points
	return map.get_node("SpawnPoints").get_child(idx)

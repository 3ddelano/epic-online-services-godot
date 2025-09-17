extends Node


@warning_ignore("unused_signal")
signal player_score_changed


const GAME_SOCKET_NAME = "EOSGSAMPLE"
const GROUP_PLAYER = "GRP_PLAYER"
const GROUP_MAP = "GRP_MAP"

var display_name = ""

# For testing things that need a second player
var second_epic_account_id = "e5d94c924f204a63bd343b976ae662cd"
var second_product_user_id = "0002620f4b524b099abf1b48db7137a2"

var _main_node: Control
var network: MyNetwork
var current_lobby: HLobby
var shoot_joystick: TouchScreenJoystick


@onready var is_mobile := OS.get_name() in ["iOS", "Android"]


func get_view(view_name: String):
	return _main_node.get_view_manager().get_view(view_name)


func set_view(view_name: String):
	return _main_node.get_view_manager().set_view(view_name)


# NetworkImage cache
var network_image_cache = {
	# url: Texture
}


func get_new_random(arr: Array, cur = null):
	if arr.size() == 1:
		return arr[0]
	
	var new_elm = arr.pick_random()
	while cur != null and cur == new_elm:
		new_elm = arr.pick_random()
	return new_elm

extends Node

signal logout_success


var product_user_id = ""
var epic_account_id = ""
var display_name = ""

# For testing things that need a second player
var second_epic_account_id = "e5d94c924f204a63bd343b976ae662cd"
var second_product_user_id = "0002620f4b524b099abf1b48db7137a2"

var _main_node: Control


func get_view(view_name: String):
	return _main_node.get_view_manager().get_view(view_name)


func set_view(view_name: String):
	return _main_node.get_view_manager().set_view(view_name)


# NetworkImage cache
var network_image_cache = {
	# url: Texture
}

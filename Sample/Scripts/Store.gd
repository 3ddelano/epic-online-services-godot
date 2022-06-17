extends Node

# warning-ignore:unused_signal
signal platform_create
# warning-ignore:unused_signal
signal login_success
# warning-ignore:unused_signal
signal logout_success


var product_user_id = ""
var epic_account_id = ""

var _main_node: Control

func get_view(view_name: String):
	return _main_node.get_view_manager().get_view(view_name)

func set_view(view_name: String):
	return _main_node.get_view_manager().set_view(view_name)


# NetworkImage cache
var network_image_cache = {
	# url: Texture
}

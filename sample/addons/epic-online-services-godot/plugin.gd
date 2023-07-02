@tool
extends EditorPlugin

var _export_plugin = preload("res://addons/epic-online-services-godot/export_plugin.gd").new()


func _enter_tree():
	add_autoload_singleton("EOS_tick", "res://addons/epic-online-services-godot/tick.gd")
	add_export_plugin(_export_plugin)


func disable_plugin():
	remove_autoload_singleton("EOS_tick")
	remove_export_plugin(_export_plugin)

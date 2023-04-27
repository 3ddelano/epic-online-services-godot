@tool
extends EditorPlugin

func _enter_tree():
	add_autoload_singleton("EOS_tick", "res://addons/epic-online-services-godot/tick.gd")


func disable_plugin():
	remove_autoload_singleton("EOS_tick")

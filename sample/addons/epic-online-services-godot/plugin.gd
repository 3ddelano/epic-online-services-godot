# Copyright (c) 2023-present Delano Lourenco
# https://github.com/3ddelano/epic-online-services-godot/
# MIT License
@tool
extends EditorPlugin

var _export_plugin = preload("res://addons/epic-online-services-godot/export_plugin.gd").new()


func _enable_plugin():
	add_autoload_singleton("EOSGRuntime", "res://addons/epic-online-services-godot/runtime.gd")
	add_export_plugin(_export_plugin)


func _disable_plugin():
	remove_autoload_singleton("EOSGRuntime")
	remove_export_plugin(_export_plugin)

# Copyright (c) 2023-present Delano Lourenco
# https://github.com/3ddelano/epic-online-services-godot/
# MIT License
@tool
extends EditorPlugin

var _export_plugin = preload("res://addons/epic-online-services-godot/export_plugin.gd").new()


func _enable_plugin():
	add_autoload_singleton("EOSGRuntime", "res://addons/epic-online-services-godot/runtime.gd")
	add_autoload_singleton("HPlatform", "res://addons/epic-online-services-godot/heos/hplatform.gd")
	add_autoload_singleton("HAuth", "res://addons/epic-online-services-godot/heos/hauth.gd")
	add_autoload_singleton("HAchievements", "res://addons/epic-online-services-godot/heos/hachievements.gd")
	add_autoload_singleton("HFriends", "res://addons/epic-online-services-godot/heos/hfriends.gd")
	add_autoload_singleton("HStats", "res://addons/epic-online-services-godot/heos/hstats.gd")
	add_autoload_singleton("HLeaderboards", "res://addons/epic-online-services-godot/heos/hleaderboards.gd")
	add_autoload_singleton("HLobbies", "res://addons/epic-online-services-godot/heos/hlobbies.gd")
	add_autoload_singleton("HP2P", "res://addons/epic-online-services-godot/heos/hp2p.gd")
	add_export_plugin(_export_plugin)


func _disable_plugin():
	remove_autoload_singleton("EOSGRuntime")
	remove_export_plugin(_export_plugin)
	remove_autoload_singleton("HPlatform")
	remove_autoload_singleton("HAuth")
	remove_autoload_singleton("HAchievements")
	remove_autoload_singleton("HFriends")
	remove_autoload_singleton("HStats")
	remove_autoload_singleton("HLeaderboards")
	remove_autoload_singleton("HLobbies")
	remove_autoload_singleton("HP2P")

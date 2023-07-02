@tool
extends EditorExportPlugin


func _get_name() -> String:
	return "Epic Online Services Godot 4.x"


func _export_begin(features: PackedStringArray, is_debug: bool, path: String, flags: int):
	var target_platform = features[2]

	match target_platform:
		"windows":
			add_shared_object("res://addons/epic-online-services-godot/bin/EOSSDK-Win64-Shipping.dll", [], "/")
			add_shared_object("res://addons/epic-online-services-godot/bin/x64/xaudio2_9redist.dll", [], "/")
		"linux":
			add_shared_object("res://addons/epic-online-services-godot/bin/libEOSSDK-Linux-Shipping.so", [], "/")
		"macos":
			add_shared_object("res://addons/epic-online-services-godot/bin/libEOSSDK-Mac-Shipping.dylibb", [], "/")

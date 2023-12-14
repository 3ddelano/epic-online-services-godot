@tool
extends EditorExportPlugin


func _get_name() -> String:
	return "Epic Online Services Godot 4.2"


func _export_begin(features: PackedStringArray, is_debug: bool, path: String, flags: int):
	var target_platform = features[2]

	match target_platform:
		"windows":
			add_shared_object("res://addons/epic-online-services-godot/bin/windows/EOSSDK-Win64-Shipping.dll", [], "/")
			add_shared_object("res://addons/epic-online-services-godot/bin/windows/x64/xaudio2_9redist.dll", [], "/")
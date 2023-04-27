extends Node

var _env = {}


func load_env(p_path := "res://.env"):
	if not FileAccess.file_exists(p_path):
		return {}

	var file = FileAccess.open(p_path, FileAccess.READ)
	var ret = {}
	while not file.eof_reached():
		var line = file.get_line()
		var tokens = line.split("=", false, 1)
		if tokens.size() == 2:
			ret[tokens[0]] = tokens[1].lstrip("\"").rstrip("\"");
	return ret


func _ready() -> void:
	_env = load_env()


func get_var(p_name: String):
	if _env.has(p_name):
		return _env[p_name]
	return null

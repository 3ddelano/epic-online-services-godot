extends Node

var _env = {}

func load_env(path = "res://.env"):
	var file = File.new()
	if(!file.file_exists(path)):
		return {}

	file.open(path, File.READ)
	var ret = {}
	while !file.eof_reached():
		var line = file.get_line()
		var tokens = line.split("=", false, 1)
		if tokens.size() == 2:
			ret[tokens[0]] = tokens[1].lstrip("\"").rstrip("\"");
	return ret


func _ready() -> void:
	_env = load_env()

func get_var(name):
	if _env.has(name):
		return _env[name]
	return null

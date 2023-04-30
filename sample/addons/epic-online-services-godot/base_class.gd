## Copyright (c) 2022-present Delano Lourenco
## https://github.com/3ddelano/dataclasses-godot
## MIT License
## See License.md

class_name BaseClass extends Dataclass

func _init(p_name: String, p_options: Dictionary = {}):
	super._init(p_name, p_options)
	p_options["print_newline"] = true

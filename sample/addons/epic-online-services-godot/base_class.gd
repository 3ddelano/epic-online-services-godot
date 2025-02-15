## Copyright (c) 2022-present Delano Lourenco
## https://github.com/3ddelano/dataclasses-godot
## MIT License
## See License.md

class_name BaseClass extends Dataclass

func _init(p_name: String, p_options: Dictionary = {}):
	p_options["print_newline"] = true
	p_options["print_exclude"] = ["base_class.gd", "_log"]
	super._init(p_name, p_options)

# Copyright (c) 2022-present Delano Lourenco
# https://github.com/3ddelano/dataclasses-godot
# MIT License
# See License.md

class_name BaseClass
extends RefCounted

var __name__: String

func _init(_name: String) -> void:
	__name__ = _name

func _to_string() -> String:
	var all_props = get_property_list()
	all_props.pop_front()
	all_props.pop_front()
	all_props.pop_front()
	all_props.pop_back()
	all_props.pop_back()
	var props = PackedStringArray()
	for prop in all_props:
		props.append("%s=%s" % [prop.name, get(prop.name)])
	return __name__ + "(" + ", ".join(props) + ")"

class_name BaseClass
extends Reference

var __name__: String

func _init(_name: String) -> void:
	__name__ = _name

func get_class() -> String:
	return __name__

func _to_string() -> String:
	var all_props = get_property_list()
	all_props.pop_front()
	all_props.pop_front()
	all_props.pop_front()
	all_props.pop_front()
	var props = PoolStringArray()
	for prop in all_props:
		props.append("%s=%s" % [prop.name, get(prop.name)])
	return get_class() + "(" + props.join(", ") + ")"

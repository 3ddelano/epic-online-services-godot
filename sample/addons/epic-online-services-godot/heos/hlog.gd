## Used for logging by HEOS classes
##
## See also [EOS.Logging] for EOS SDK logging
class_name HLog
extends RefCounted

enum LogLevel {
	OFF,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	VERBOSE,
}

## Current log level
static var log_level = LogLevel.INFO


## Returns a new logger for the given class name
static func logger(clazz_name: String) -> HEOSLogger:
	return HEOSLogger.new(clazz_name)


static func _log_level_str(level: LogLevel) -> String:
	match level:
		LogLevel.VERBOSE:
			return "VERBOSE"
		LogLevel.DEBUG:
			return "DEBUG"
		LogLevel.INFO:
			return "INFO"
		LogLevel.WARN:
			return "WARN"
		LogLevel.ERROR:
			return "ERROR"
	return ""


static func _check_diff_and_set(p_obj: Object, p_key, p_value, p_name = "") -> bool:
	if p_key in p_obj:
		var orig = p_obj.get(p_key)
		p_obj.set(p_key, p_value)
		if orig != p_value:
			if log_level >= LogLevel.VERBOSE:
				print_rich("[color=yellow]CHANGED %s[/color]: %s = %s -> %s" % [p_name, p_key, orig, p_value])
			return true

	return false


static func _check_diff_and_set_dict(p_dict: Dictionary, p_key, p_value, p_name = "") -> bool:
	if p_dict.has(p_key):
		var orig = p_dict[p_key]
		p_dict[p_key] = p_value
		if orig != p_value:
			if log_level >= LogLevel.VERBOSE:
				print_rich("[color=yellow]CHANGED %s[/color]: %s = %s -> %s" % [p_name, p_key, orig, p_value])
			return true
	return false


static func _check_attr_diff(orig_attrs: Array, new_attrs: Array, p_name = "") -> void:
	if log_level < LogLevel.VERBOSE:
		return

	var orig_map: Dictionary = {}
	for attr in orig_attrs:
		orig_map[attr.key] = attr
	
	var new_map: Dictionary = {}
	for attr in new_attrs:
		new_map[attr.key] = attr
	
	for id in orig_map.keys():
		if not new_map.has(id):
			print_rich("[color=yellow]CHANGED %s[/color]: attr removed: %s = (%s,%s)" % [p_name, id, orig_map[id].value, orig_map[id].visibility])
		else:
			var old_attr = orig_map[id]
			var new_attr = new_map[id]
			if not (old_attr.value == new_attr.value and old_attr.visibility == new_attr.visibility):
				print_rich("[color=yellow]CHANGED %s[/color]: attr updated: %s = (%s,%s) -> (%s,%s)" % [p_name, id, old_attr.value, old_attr.visibility, new_attr.value, new_attr.visibility])
	
	for id in new_map.keys():
		if not orig_map.has(id):
			print_rich("[color=yellow]CHANGED %s[/color]: attr added: %s = (%s,%s)" % [p_name, id, new_map[id].value, new_map[id].visibility])


## Used internally by HEOS classes for logging
class HEOSLogger extends RefCounted:
	var clazz_name: String

	func _init(p_clazz_name) -> void:
		clazz_name = p_clazz_name

	func verbose(msg: String) -> void:
		_log(LogLevel.VERBOSE, msg)

	func debug(msg: String) -> void:
		_log(LogLevel.DEBUG, msg)

	func info(msg: String) -> void:
		_log(LogLevel.INFO, msg)

	func warn(msg: String) -> void:
		_log(LogLevel.WARN, msg)

	func error(msg: String) -> void:
		_log(LogLevel.ERROR, msg)
	
	func _log(level: LogLevel, msg: String) -> void:
		if level <= HLog.log_level:
			var timestamp = Time.get_datetime_string_from_system(true, true)
			print("%s [%s] [%s] %s" % [timestamp, HLog._log_level_str(level), clazz_name, msg])
	

class_name HLog
extends RefCounted

enum LogLevel {
	VERBOSE = 1,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	OFF
}

static var log_level = LogLevel.VERBOSE

static func logger(clazz_name: String) -> Logger:
	return Logger.new(clazz_name)


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


class Logger extends RefCounted:
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
		if level >= HLog.log_level:
			var timestamp = Time.get_datetime_string_from_system(true, true)
			print("%s [%s] [%s] %s" % [timestamp, HLog._log_level_str(level), clazz_name, msg])
	

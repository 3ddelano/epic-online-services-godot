class_name LogsView
extends VBoxContainer

onready var logs_label = $PC/SC/LogsLabel


func _ready() -> void:
	var _c = EOS.get_instance().connect("logging_interface_callback", self, "_on_logging_interface_callback")
	_c = Store.connect("platform_create", self, "_on_platform_create")


func _on_platform_create():
	# Set logging categories and level
	var res = EOS.Logging.set_log_level(EOS.Logging.LogCategory.AllCategories, EOS.Logging.LogLevel.Info)
	if res != EOS.Result.Success:
		print("Failed to set log level: ", EOS.print_result(res))


func _on_logging_interface_callback(msg):
	msg = EOS.Logging.LogMessage.from(msg) as EOS.Logging.LogMessage
	if msg == null:
		return
	log_msg(msg.level, msg.message, msg.category)


func log_msg(level: int, msg: String, category := ""):
	var color = "#ffffff"
	var level_str = "Info"
	match level:
		EOS.Logging.LogLevel.VeryVerbose:
			color = "#555555"
			level_str = "VeryVerbose"
		EOS.Logging.LogLevel.Verbose:
			color = "#aaaaaa"
			level_str = "Verbose"
		EOS.Logging.LogLevel.Warning:
			color = "#ffff00"
			level_str = "Warning"
		EOS.Logging.LogLevel.Error:
			color = "#bb0000"
			level_str = "Error"
		EOS.Logging.LogLevel.Fatal:
			color = "#ff0000"
			level_str = "Fatal"

	var _category = "LOG"
	if category != "":
		_category = category

	if level <= EOS.Logging.LogLevel.Warning:
		print("%s | %s | %s" % [_category, level_str, msg])

	var darkened_color = Color(color).darkened(0.2).to_html(true)
	logs_label.bbcode_text += "[color=#%s]%s | %s |[/color] [color=%s]%s\n[/color]" % [darkened_color, _category, level_str, color, msg]
	logs_label.get_parent().set_deferred("scroll_vertical", 100000)

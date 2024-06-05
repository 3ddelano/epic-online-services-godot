class_name LogsView
extends VBoxContainer

@onready var logs_label = %LogsLabel

func _ready() -> void:
	IEOS.logging_interface_callback.connect(_on_logging_interface_callback)
	Store.platform_create.connect(_on_platform_create)

func _on_platform_create():
	# Set logging categories and level
	var res: EOS.Result = EOS.Logging.set_log_level(EOS.Logging.LogCategory.AllCategories, EOS.Logging.LogLevel.Info)
	if res != EOS.Result.Success:
		print("Failed to set log level: ", EOS.result_str(res))

func _on_logging_interface_callback(p_msg: Dictionary):
	var msg = EOS.Logging.LogMessage.from(p_msg) as EOS.Logging.LogMessage
	log_msg(msg.level, msg.message, msg.category)

func log_msg(level: int, msg: String, category:=""):
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

#	if level <= EOS.Logging.LogLevel.Warning:
#		print("%s | %s | %s" % [_category, level_str, msg])

	var darkened_color = Color(color).darkened(0.2).to_html(true)
	logs_label.text += "[color=#%s]%s\t%s\t[/color][color=%s]%s\n[/color]" % [darkened_color, _category, level_str, color, msg]

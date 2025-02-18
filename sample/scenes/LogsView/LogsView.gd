class_name LogsView
extends VBoxContainer

@onready var logs_label = %LogsLabel

func _ready() -> void:
	HPlatform.log_msg.connect(func(msg: EOS.Logging.LogMessage):
		log_msg(msg.level, msg.message, msg.category)
	)
	HPlatform.platform_initialized.connect(_on_platform_initialized)


func _on_platform_initialized() -> void:
	# Set logging categories and level
	HPlatform.set_eos_log_level(EOS.Logging.LogCategory.AllCategories, EOS.Logging.LogLevel.Info)


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

	var darkened_color = Color(color).darkened(0.2).to_html(true)
	var to_print = "[color=#%s]%s\t%s\t[/color][color=%s]%s[/color]" % [darkened_color, _category, level_str, color, msg]
	var to_print_nonrich = "[%s] [%s] %s" % [category, level_str, msg]
	logs_label.text += to_print + "\n"
	# Uncomment the below line if you want to see EOS logs in terminal too
	print_rich(to_print)
	# print(to_print_nonrich)

class_name StyledPopupWindow
extends PopupPanel


@onready var _close_btn = %CloseBtn
@onready var _title = %Title


func _ready() -> void:
	hide()
	_close_btn.pressed.connect(hide)

	_title.text = title

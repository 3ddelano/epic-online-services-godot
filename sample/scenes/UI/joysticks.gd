extends Control

@onready var shoot_joystick: TouchScreenJoystick = $ShootJoystick


func _ready() -> void:
	hide()
	
	if Store.is_mobile:
		show()
	
	Store.shoot_joystick = shoot_joystick

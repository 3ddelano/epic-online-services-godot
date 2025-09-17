##  RESPONSIVE TOUCHSCREEN JOYSTICK 1.0.2 ##

# From https://github.com/kntCyc1230/TouchScreenJoystick
# MIT License
# Copyright (c) Kent Aljo G. Coyoca

@tool
extends Control
class_name TouchScreenJoystick

@export var use_textures: bool:
	set(new_bool):
		use_textures = new_bool
		notify_property_list_changed()
@export var knob_color := Color.WHITE
@export var base_color := Color.WHITE
@export var background_color := Color(Color.BLACK, 0.25)

@export_range(0, 100, 0.1, "or_greater") var base_radius := 130.0
@export_range(0, 100, 0.1, "or_greater") var knob_radius := 65.0
@export var base_thickness := 1.8
@export var anti_aliased: bool

@export_group("Textures")
@export var use_custom_max_length: bool:
	set(new_bool):
		use_custom_max_length = new_bool
		notify_property_list_changed()

@export_range(0, 100, 0.1, "or_greater") var max_length := 120.0
@export_subgroup("Base Texture Params")
@export var base_texture: Texture2D
@export_range(-360, 360, 0.01, "or_greater", "or_less", "suffix:°") var base_rotation: float
@export var base_scale := 1.0

@export_subgroup("Knob Texture Params")
@export var knob_texture: Texture2D
@export_range(-360, 360, 0.01, "or_greater", "or_less", "suffix:°") var knob_rotation: float
@export var knob_scale := 1.0

@export_subgroup("Background Texture Params")
@export var background_texture: Texture2D
@export_range(-360, 360, 0.01, "or_greater", "or_less", "suffix:°") var backround_rotation: float
@export var background_scale := 1.0

@export_group("Joystick Params")
@export_enum("FIXED", "DYNAMIC") var mode := 0
@export var deadzone := 10.0:
	set(new_deadzone):
		deadzone = clamp(new_deadzone, 10, get_current_max_length())

@export var smooth_reset: bool:
	set(new_bool):
		smooth_reset = new_bool
		notify_property_list_changed()
@export var smooth_speed := 5.0
@export var change_opacity_when_touched: bool:
	set(new_bool):
		change_opacity_when_touched = new_bool
		notify_property_list_changed()

@export_range(0, 100, 0.01, "suffix:%") var from_opacity := 50.0
@export_range(0, 100, 0.01, "suffix:%") var to_opacity := 100.0
@export var use_input_actions: bool:
	set(new_bool):
		use_input_actions = new_bool
		notify_property_list_changed()

@export_subgroup("Input Actions")
@export var action_left := "ui_left"
@export var action_right := "ui_right"
@export var action_up := "ui_up"
@export var action_down := "ui_down"

@export_group("Debug")
@export var draw_debugs: bool:
	set(new_bool):
		draw_debugs = new_bool
		notify_property_list_changed()
@export var deadzone_color := Color(Color.RED, 0.5)
@export var current_max_length_color := Color(Color.BLUE, 0.5)

var is_pressing: bool
var knob_position: Vector2
var finger_index := -1
var default_pos: Vector2
var current_input_event: InputEvent
var global_touch: Vector2


func _ready() -> void:
	default_pos = position
	change_opacity()


func _process(_delta: float) -> void:
	# checks if currently pressing
	if is_pressing:
		move_knob_pos()
	else:
		reset_knob_pos()
	
	# update necessities
	update_input_actions()
	pivot_offset = size / 2
	queue_redraw()
	

#moves the knob position when pressing
func move_knob_pos() -> void:
	var centered_touch := global_touch - size / 2
	
	if get_distance() <= get_current_max_length():
		knob_position = centered_touch
	else:
		# calculates the angle position of the knob if it's position --
		# -- exceeds from the current max length
		var angle := Vector2.ZERO.angle_to_point(centered_touch)
		knob_position.x = cos(angle) * get_current_max_length()
		knob_position.y = sin(angle) * get_current_max_length()


# resets knob position if not pressing
func reset_knob_pos() -> void:
	if smooth_reset:
		knob_position = lerp(knob_position, Vector2.ZERO, smooth_speed * get_process_delta_time())
	else:
		knob_position = Vector2.ZERO


# triggers a specific input action based on the --
# -- current direction
func trigger_input_actions() -> void:
	var dir := get_deadzoned_vector()
	
	if dir.x > 0:
		Input.action_release(action_left)
		Input.action_press(action_right, dir.x)
	else:
		Input.action_release(action_right)
		Input.action_press(action_left, - dir.x)
	
	if dir.y < 0:
		Input.action_release(action_down)
		Input.action_press(action_up, - dir.y)
	else:
		Input.action_release(action_up)
		Input.action_press(action_down, dir.y)


# releases all input actions
func release_input_actions() -> void:
	Input.action_release(action_right)
	Input.action_release(action_left)
	Input.action_release(action_up)
	Input.action_release(action_down)


func update_input_actions() -> void:
	if use_input_actions and is_pressing:
		trigger_input_actions()


# please ignore these validitate functions it has nothing --
# -- to do with the joystick's functionality 
func _validate_property(property: Dictionary) -> void:
	deadzone = clamp(deadzone, 10, get_current_max_length())
	validitate_default_drawing_properties(property)
	validitate_texture_drawing_properties(property)
	validitate_input_action_properties(property)
	if property.name == "smooth_speed" and not smooth_reset:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "from_opacity" and not change_opacity_when_touched:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "to_opacity" and not change_opacity_when_touched:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "deadzone_color" and not draw_debugs:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "current_max_length_color" and not draw_debugs:
		property.usage = PROPERTY_USAGE_READ_ONLY


func validitate_input_action_properties(property: Dictionary) -> void:
	if property.name == "action_left" and not use_input_actions:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "action_right" and not use_input_actions:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "action_up" and not use_input_actions:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "action_down" and not use_input_actions:
		property.usage = PROPERTY_USAGE_READ_ONLY


func validitate_default_drawing_properties(property: Dictionary) -> void:
	if property.name == "base_color" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "knob_color" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "background_color" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "base_radius" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	
	if property.name == "knob_radius" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "base_thickness" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "anti_aliased" and use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY


func validitate_texture_drawing_properties(property: Dictionary) -> void:
	if property.name == "background_texture" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "use_custom_max_length" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "max_length" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "max_length" and not use_custom_max_length:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "base_texture" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "knob_texture" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "base_scale" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "base_rotation" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "knob_scale" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "knob_rotation" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	
	if property.name == "background_scale" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	
	if property.name == "backround_rotation" and not use_textures:
		property.usage = PROPERTY_USAGE_READ_ONLY
	

# draws the joystick
func _draw() -> void:
	if not use_textures:
		draw_default_joystick()
	else:
		draw_textured_joystick()
	
	if draw_debugs:
		draw_debug()


# default joystick
func draw_default_joystick() -> void:
	draw_set_transform(size / 2)
	# background
	draw_circle(Vector2.ZERO, base_radius, background_color, true, -1.0, anti_aliased)
	
	# base
	draw_circle(Vector2.ZERO, base_radius, base_color, false, base_thickness, anti_aliased)
	var pos := knob_position
	# knob
	draw_circle(pos, knob_radius, knob_color, true, -1.0, anti_aliased)


# draw joystick using custom textures
func draw_textured_joystick() -> void:
	if background_texture:
		var texture_size := background_texture.get_size()
		var centered_base_pos := size / 2
		draw_set_transform(centered_base_pos, deg_to_rad(backround_rotation), Vector2(background_scale, background_scale))
		draw_texture_rect(background_texture, Rect2(Vector2.ZERO - texture_size / 2, base_texture.get_size()), false)
		
	# draw textured base
	if base_texture:
		var centered_base_pos := size / 2
		var texture_size := base_texture.get_size()
		size.x = clamp(size.x, texture_size.x, INF)
		size.y = clamp(size.y, texture_size.y, INF)
		draw_set_transform(centered_base_pos, deg_to_rad(base_rotation), Vector2(base_scale, base_scale))
		draw_texture_rect(base_texture, Rect2(Vector2.ZERO - texture_size / 2, base_texture.get_size()), false)
	
	# draw texture knob
	if knob_texture:
		var centered_knob_pos := size / 2
		var texture_size := knob_texture.get_size()
		
		draw_set_transform(centered_knob_pos, deg_to_rad(knob_rotation), Vector2(knob_scale, knob_scale))
		draw_texture_rect(knob_texture, Rect2((knob_position / knob_scale) - (texture_size / 2), knob_texture.get_size()), false)


# draws the deadzone and the current max length
func draw_debug() -> void:
	var length_string := str(round(get_weight() * 100.0), "%")
	var string_pos := knob_position / 2 + Vector2.UP * 100
	var lerped_color := lerp(Color.RED, Color.GREEN, get_weight()) as Color
	draw_set_transform(size / 2)
	# draw deadzone
	draw_circle(Vector2.ZERO, deadzone, deadzone_color, false, 1.0, true)
	
	# draw current max length
	draw_circle(Vector2.ZERO, get_current_max_length(), current_max_length_color, false, 1.0, true)
	
	draw_line(Vector2.ZERO, knob_position, lerped_color, 3.0, true)
	draw_string_outline(SystemFont.new(), string_pos, length_string, HORIZONTAL_ALIGNMENT_CENTER, -1.0, 18, 10, Color.BLACK)
	@warning_ignore("narrowing_conversion")
	draw_string(SystemFont.new(), string_pos, length_string, HORIZONTAL_ALIGNMENT_CENTER, -1.0, 18, lerped_color, get_weight())


# handles input
func _input(event: InputEvent) -> void:
	# check if touching
	if event is InputEventScreenTouch:
		# checks if touch is pressed
		if event.pressed:
			# if touch is pressed, checks first if there's currently no touch index and the touch position has a point --
			# -- or is inside the rect
			if finger_index == -1 and get_global_rect().has_point(event.position):
				# sets the touch index to the event index
				finger_index = event.index
				if event.index == finger_index:
					on_touched(event)
					# sets the touch position to event position subtracted to the center position
					# so the touch position starts from the center of the rect instead of --
					# the top left corner of the rect, then divide it to scale.x so the 
					# position keeps centered regardless of the current scale
					global_touch = (event.position - global_position) / get_global_transform().get_scale().x
					get_viewport().set_input_as_handled()
		else:
			# resets the touch index and touch position
			if event.index == finger_index:
				finger_index = -1
				global_touch = Vector2.ZERO
				on_touch_released()
				get_viewport().set_input_as_handled()
	
	# update the position when touch is moving
	if event is InputEventScreenDrag:
		if event.index == finger_index:
			global_touch = (event.position - global_position) / get_global_transform().get_scale().x
			get_viewport().set_input_as_handled()


func on_touched(event: InputEventScreenTouch) -> void:
	is_pressing = true
	change_opacity()
	var mouse_pos := event.position - size / 2
	if mode == 1:
		position = mouse_pos


func on_touch_released() -> void:
	is_pressing = false
	if mode == 1:
		position = default_pos
	release_input_actions()
	change_opacity()


# returns the direction
func get_vector() -> Vector2:
	return get_center_pos().direction_to(knob_position + get_center_pos())


# returns the direction affected by the deadzone
func get_deadzoned_vector() -> Vector2:
	var vector: Vector2
	if is_pressing and not is_in_deadzone():
		vector = get_center_pos().direction_to(knob_position + get_center_pos())
	else:
		vector = Vector2.ZERO
	return vector


# returns a position that starts at the center of the rect
func get_center_pos() -> Vector2:
	return position + size / 2


# returns the distance from center of the joystick to the position of the knob
func get_distance() -> float:
	return (global_touch - size / 2).distance_to(Vector2.ZERO)


# get the current max distance of the knob's position. --
# -- if you use textures, the current max length will --
# -- automatically set to the half base texture's width
func get_current_max_length() -> float:
	var curr_max_length: float
	if not use_textures:
		curr_max_length = base_radius
	else:
		if use_custom_max_length:
			curr_max_length = max_length
		elif not use_custom_max_length and base_texture:
			curr_max_length = (base_texture.get_size().x / 2) * base_scale
	
	return curr_max_length


# changes the opacity when touched
func change_opacity() -> void:
	if change_opacity_when_touched and not Engine.is_editor_hint():
		if is_pressing:
			modulate.a = to_opacity / 100.0
		else:
			modulate.a = from_opacity / 100.0
	else:
		modulate.a = 1.0


func is_in_deadzone() -> bool:
	return get_distance() <= deadzone


func is_correct_event_index(event: InputEventScreenTouch) -> bool:
	return event is InputEventScreenTouch and event.index == finger_index


func is_touch_in_rect(touch_pos: Vector2) -> bool:
	return get_global_rect().has_point(touch_pos)


func get_weight() -> float:
	var weight := (get_distance() / get_current_max_length())
	weight = clamp(weight, 0, 1)
	return weight

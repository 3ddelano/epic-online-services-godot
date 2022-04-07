class_name NetworkImage
extends MarginContainer

var LOADING_TEXTURE = preload("res://Scenes/UI/loading-icon.png")

onready var texture_rect = $TR
onready var anim_player = $Anim

enum States {
	Loading,
	Loaded
}

var _state = States.Loading


func _ready() -> void:
	set_state(States.Loading)


func set_state(new_state: int):
	_state = new_state

	if _state == States.Loading:
		texture_rect.texture = LOADING_TEXTURE
		anim_player.play("rotate")
	elif _state == States.Loaded:
		$TR.set_rotation(0)
		anim_player.stop(true)


func fetch_image(url: String):
	set_state(States.Loading)
	var http_request = HTTPRequest.new()
	add_child(http_request)
	http_request.connect("request_completed", self, "_on_request_completed")
	http_request.request(url)


func _on_request_completed(result, response_code, headers, body):
	print("------------ Request completed")
	var image = Image.new()
	var image_error = image.load_png_from_buffer(body)
	if image_error != OK:
		print("An error occurred while trying to display the image.")
		return

	var image_texture = ImageTexture.new()
	image_texture.create_from_image(image)

	texture_rect.texture = image_texture
	set_state(States.Loaded)

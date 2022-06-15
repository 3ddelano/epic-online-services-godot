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
var _url: String

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
	_url = url
	if Store.network_image_cache.has(url):
		# Directly set the image
		texture_rect.texture = Store.network_image_cache[url]
		set_state(States.Loaded)
		return

	set_state(States.Loading)
	var http_request = HTTPRequest.new()
	add_child(http_request)
	http_request.connect("request_completed", self, "_on_request_completed")
	http_request.request(url)


func _on_request_completed(result, response_code, _headers, body):
	if result != OK or response_code != 200:
		print("Network Image Error: result=%s, response_code=%s" % [result, response_code])

	var image = Image.new()
	var image_error = image.load_png_from_buffer(body)
	if image_error != OK:
		print("Network Image Error: image_error=%s: An error occurred while trying to display the image." % image_error)
		return

	var image_texture = ImageTexture.new()
	image_texture.create_from_image(image)
	Store.network_image_cache[_url] = image_texture

	texture_rect.texture = image_texture
	set_state(States.Loaded)

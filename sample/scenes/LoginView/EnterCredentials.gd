extends VBoxContainer

@onready var id_label = $IdLabel
@onready var token_label = $TokenLabel

@onready var id_lineedit = $IdLineEdit
@onready var token_lineedit = $TokenLineEdit


func set_helper_texts(id_help: String, token_help: String):
	if id_help != "":
		id_label.text = id_help
		id_label.visible = true
		id_lineedit.visible = true
	else:
		id_label.visible = false
		id_lineedit.visible = false

	if token_help != "":
		token_label.text = token_help
		token_label.visible = true
		token_lineedit.visible = true
	else:
		token_label.visible = false
		token_lineedit.visible = false


func set_id_text(text: String):
	id_label.text = text


func set_token_text(text: String):
	token_label.text = text


func get_id_value() -> String:
	return id_lineedit.text


func get_token_value() -> String:
	return token_lineedit.text


func reset_lineedits():
	id_lineedit.text = ""
	token_lineedit.text = ""


func _notification(what: int) -> void:
	if what == NOTIFICATION_VISIBILITY_CHANGED:
		if visible:
			reset_lineedits()

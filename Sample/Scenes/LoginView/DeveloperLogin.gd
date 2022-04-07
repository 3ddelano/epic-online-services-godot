class_name DeveloperLogin
extends VBoxContainer

onready var id_label = $IdLabel
onready var token_label = $TokenLabel

onready var id_lineedit = $IdLineEdit
onready var token_lineedit = $TokenLineEdit


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

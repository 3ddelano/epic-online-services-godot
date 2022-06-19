class_name AchievementUnlockNotification
extends Control

var _data: Dictionary

onready var id_label = $MC/HB/VB/IdLabel
onready var name_label = $MC/HB/VB/NameLabel
onready var unlocked_image = $MC/HB/UnlockedImage
onready var anim = $Anim
onready var fade_out_timer = $FadeOutTimer

func _ready() -> void:
	var _c = fade_out_timer.connect("timeout", self, "_on_fade_out_timer_timeout")


func from_achievement_data(data: Dictionary):
	_data = data

	name_label.text = data.unlocked_display_name
	id_label.text = data.achievement_id
	if data.unlocked_icon_url != null:
		unlocked_image.fetch_image(data.unlocked_icon_url)
	else:
		unlocked_image.fetch_image(data.icon_url)


func fade_in():
	anim.play("FadeIn")
	fade_out_timer.start()


func _on_fade_out_timer_timeout():
	anim.play_backwards("FadeIn")
	yield(anim, "animation_finished")
	queue_free()

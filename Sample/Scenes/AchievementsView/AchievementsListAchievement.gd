class_name AchievementsListAchievement
extends MarginContainer

# warning-ignore:unused_signal
signal pressed(_self)

onready var id_label = $MC/HB/VB/IdLabel
onready var unlocked_image = $MC/HB/UnlockedImage as NetworkImage

onready var button = $RR/Button

var _data: Dictionary


func _ready():
	button.connect("pressed", self, "emit_signal", ["pressed", self])


func from_achievement_data(data: Dictionary):
	_data = data

	id_label.text = data.achievement_id
	if data.unlocked_icon_url != null:
		unlocked_image.fetch_image(data.unlocked_icon_url)
	else:
		unlocked_image.fetch_image(data.icon_url)


func get_data() -> Dictionary:
	return _data

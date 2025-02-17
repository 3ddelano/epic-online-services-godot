class_name AchievementsListAchievement
extends MarginContainer

# warning-ignore:unused_signal
signal pressed(_self)

@onready var id_label = $MC/HB/VB/IdLabel
@onready var unlocked_image = $MC/HB/UnlockedImage as NetworkImage

@onready var button = $RR/Button

var _data: HAchievementData


func _ready():
	button.pressed.connect(func ():
		pressed.emit(self)
	)


func from_achievement_data(data: HAchievementData):
	_data = data

	id_label.text = data.achievement_id
	if data.unlocked_icon_url != null:
		unlocked_image.fetch_image(data.unlocked_icon_url)
	else:
		unlocked_image.fetch_image(data.icon_url)


func get_data() -> HAchievementData:
	return _data

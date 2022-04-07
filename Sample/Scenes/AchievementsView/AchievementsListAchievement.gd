extends MarginContainer

onready var id_label = $MC/HB/VB/IdLabel
onready var unlocked_image = $MC/HB/UnlockedImage as NetworkImage


func from_achievement_data(data: Dictionary):
	id_label.text = data.achievement_id
	if data.has("unlocked_icon_url"):
		unlocked_image.fetch_image(data.unlocked_icon_url)
	else:
		unlocked_image.fetch_image(data.icon_url)

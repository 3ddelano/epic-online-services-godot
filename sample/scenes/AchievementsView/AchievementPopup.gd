class_name AchievementPopup
extends StyledPopupWindow

@onready var id_label = $VB/MC/VB/GridContainer/IdLabel
@onready var flavor_label = $VB/MC/VB/GridContainer/FlavorLabel
@onready var is_visible_label = $VB/MC/VB/GridContainer/IsVisibleLabel
@onready var is_unlocked_label = $VB/MC/VB/GridContainer/IsUnlockedLabel
@onready var stat_thresholds_vb = $VB/MC/VB/MC/VB/SC/StatThresholdsVB
@onready var locked_image = $VB/MC/VB/HB/VB/LockedImage
@onready var locked_label = $VB/MC/VB/HB/VB/LockedLabel
@onready var locked_desc_label = $VB/MC/VB/HB/VB/LockedDescLabel
@onready var unlocked_image = $VB/MC/VB/HB/VB2/UnlockedImage
@onready var unlocked_label = $VB/MC/VB/HB/VB2/UnlockedLabel
@onready var unlocked_desc_label = $VB/MC/VB/HB/VB2/UnlockedDescLabel

@onready var unlock_btn = $VB/MC/VB/UnlockBtn

var achievement_node: AchievementsListAchievement


func _ready() -> void:
	super._ready()
	unlock_btn.pressed.connect(_on_unlock_btn_pressed)


func from_achievement_node(node: AchievementsListAchievement):
	achievement_node = node
	var data := achievement_node.get_data()

	id_label.text = data.achievement_id
	flavor_label.text = data.flavor_text
	if data.is_hidden != null:
		if data.is_hidden:
			is_visible_label.text = "False"
		else:
			is_visible_label.text = "True"
	else:
		is_visible_label.text = "NA"
	if not data.is_unlocked():
		is_unlocked_label.text = "No"
		unlock_btn.disabled = false
	else:
		var time_str = Time.get_datetime_string_from_unix_time(data.unlock_time)
		is_unlocked_label.text = "Yes at " + time_str
		unlock_btn.disabled = true

	for child in stat_thresholds_vb.get_children():
		child.queue_free()

	for stat in data.stats:
		var label = Label.new()
		label.text = "%s : %s" % [stat.name, stat.threshold_value]
		stat_thresholds_vb.add_child(label)

	locked_image.fetch_image(data.locked_icon_url)
	locked_label.text = "Locked Name: " + data.locked_display_name
	locked_desc_label.text = "Locked Desc: " + data.locked_description

	unlocked_image.fetch_image(data.unlocked_icon_url)
	unlocked_label.text = "Locked Name: " + data.unlocked_display_name
	unlocked_desc_label.text = "Locked Desc: " + data.unlocked_description


func _on_unlock_btn_pressed():
	var data = achievement_node.get_data()

	unlock_btn.disabled = true
	visible = false
	HAchievements.unlock_achievement_async(data.achievement_id)

class_name AchievementsList
extends MarginContainer

signal achievement_pressed(node)

const ACHIEVEMENTS_LIST_ACHIEVEMENT = preload("res://Scenes/AchievementsView/AchievementsListAchievement.tscn")

func from_achievements_array(arr: Array):
	# Clear existing achievements
	for child in $SC/VB.get_children():
		child.queue_free()

	# Populate new achievements
	for achievement_data in arr:
		var achievement = ACHIEVEMENTS_LIST_ACHIEVEMENT.instance()
		$SC/VB.add_child(achievement)

		achievement.connect("pressed", self, "_on_achievement_pressed")
		achievement.from_achievement_data(achievement_data)


func _on_achievement_pressed(node: AchievementsListAchievement):
	emit_signal("achievement_pressed", node)

class_name AchievementsList
extends MarginContainer

var ACHIEVEMENTS_LIST_ACHIEVEMENT = preload("res://Scenes/AchievementsView/AchievementsListAchievement.tscn")

func from_achievements_data(arr: Array):
	# Clear existing achievements
	for child in $SC/VB.get_children():
		child.queue_free()

	# Populate new achievements
	for achievement_data in arr:
		var achievement = ACHIEVEMENTS_LIST_ACHIEVEMENT.instance()
		$SC/VB.add_child(achievement)

		achievement.from_achievement_data(achievement_data)

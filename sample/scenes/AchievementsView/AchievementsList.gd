class_name AchievementsList
extends MarginContainer

signal achievement_pressed(node)

const ACHIEVEMENTS_LIST_ACHIEVEMENT = preload("res://scenes/AchievementsView/AchievementsListAchievement.tscn")

func _ready() -> void:
	_clear()


func _clear() -> void:
	# Clear existing achievements
	for child in $SC/VB.get_children():
		child.queue_free()


func from_achievements_array(arr: Array):
	_clear()

	# Populate new achievements
	for achievement_data in arr:
		var achievement = ACHIEVEMENTS_LIST_ACHIEVEMENT.instantiate()
		$SC/VB.add_child(achievement)

		achievement.pressed.connect(_on_achievement_pressed)
		achievement.from_achievement_data(achievement_data)


func _on_achievement_pressed(node: AchievementsListAchievement):
	achievement_pressed.emit(node)

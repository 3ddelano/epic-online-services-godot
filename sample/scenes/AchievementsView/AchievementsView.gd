class_name AchievementsView
extends VBoxContainer

# Local cache of achievements
var achievements = {}

@onready var _achievements_list = %AchievementsList
@onready var _achievement_popup = %AchievementPopup
@onready var refresh_btn = %RefreshBtn

@onready var AchievementUnlockNotificationScene = preload ("res://scenes/AchievementsView/AchievementUnlockNotification.tscn")


func _ready() -> void:
	HAchievements.achievement_unlocked.connect(_on_achievement_unlocked)

	visibility_changed.connect(_get_achievements)
	HAuth.logged_out.connect(_on_logged_out)

	_achievements_list.achievement_pressed.connect(_on_achievement_pressed)
	refresh_btn.pressed.connect(_get_achievements)


func _get_achievements():
	if not visible: return

	var achievements_arr := await HAchievements.get_all_achievements_async()
	for achievement in achievements_arr:
		achievements[achievement.achievement_id] = achievement
	
	var player_achievements_arr := await HAchievements.get_player_achievements_async()
	for player_achievement in player_achievements_arr:
		achievements[player_achievement.achievement_id].unlock_time = player_achievement.unlock_time

	_rebuild_ui()


func _on_logged_out():
	achievements = {}
	_rebuild_ui()


func _rebuild_ui():
	_achievements_list.from_achievements_array(achievements.values())


func _on_achievement_pressed(node: AchievementsListAchievement):
	_achievement_popup.from_achievement_node(node)
	_achievement_popup.popup_centered()


func _on_achievement_unlocked(data: Dictionary):
	var achievement_id = data.achievement_id
	if achievements.has(achievement_id):
		achievements[data.achievement_id].unlock_time = data.unlock_time
	
	_rebuild_ui()

	# Show achievement unlocked notification
	var notif = AchievementUnlockNotificationScene.instantiate()
	Store.get_view("Notifications").add_notification(notif)
	notif.from_achievement_data(achievements[data.achievement_id])

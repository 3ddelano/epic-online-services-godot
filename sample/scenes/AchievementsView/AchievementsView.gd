class_name AchievementsView
extends VBoxContainer

# Local cache of achievements
var achievements = {}

@onready var _achievements_list = %AchievementsList
@onready var _achievement_popup = %AchievementPopup
@onready var refresh_btn = %RefreshBtn

@onready var AchievementUnlockNotificationScene = preload ("res://scenes/AchievementsView/AchievementUnlockNotification.tscn")

func _ready() -> void:
	IEOS.achievements_interface_achievements_unlocked_v2_callback.connect(_on_achievements_interface_achievements_unlocked_v2_callback)
	IEOS.achievements_interface_unlock_achievements_callback.connect(_on_achievements_interface_unlock_achievements_callback)

	visibility_changed.connect(_query_achievements)
	Store.logout_success.connect(_on_logout_success)

	_achievements_list.achievement_pressed.connect(_on_achievement_pressed)
	refresh_btn.pressed.connect(_query_achievements)

func _query_achievements():
	if not visible: return

	var query_opts = EOS.Achievements.QueryDefinitionsOptions.new()
	query_opts.local_user_id = Store.product_user_id
	EOS.Achievements.AchievementsInterface.query_definitions(query_opts)
	print("--- Achievements: query_definitions_callback: ", EOS.result_str(await IEOS.achievements_interface_query_definitions_callback))
	_copy_all_achievements()

	var player_query_opts = EOS.Achievements.QueryPlayerAchievementsOptions.new()
	player_query_opts.local_user_id = Store.product_user_id
	player_query_opts.target_user_id = Store.product_user_id
	EOS.Achievements.AchievementsInterface.query_player_achievements(player_query_opts)
	print("--- Achievements: query_player_achievements_callback: ", EOS.result_str(await IEOS.achievements_interface_query_player_achievements_callback))
	_copy_all_player_achievements()
	_rebuild_ui()

func _copy_all_achievements():
	var get_count_opts = EOS.Achievements.GetAchievementDefinitionCountOptions.new()
	var achievement_count: int = EOS.Achievements.AchievementsInterface.get_achievement_definition_count(get_count_opts)

	achievements = {}
	for i in achievement_count:
		var copy_by_index_opts = EOS.Achievements.CopyAchievementDefinitionV2ByIndexOptions.new()
		copy_by_index_opts.achievement_index = i

		var achievement_definition = EOS.Achievements.AchievementsInterface.copy_achievement_definition_v2_by_index(copy_by_index_opts).definition_v2
		achievements[achievement_definition.achievement_id] = achievement_definition
		achievements[achievement_definition.achievement_id].unlock_time = EOS.Achievements.UNLOCK_TIME_UNDEFINED

func _copy_all_player_achievements():
	var get_count_opts = EOS.Achievements.GetPlayerAchievementCountOptions.new()
	get_count_opts.user_id = Store.product_user_id
	var player_achievement_count: int = EOS.Achievements.AchievementsInterface.get_player_achievement_count(get_count_opts)

	for i in player_achievement_count:
		var opts = EOS.Achievements.CopyPlayerAchievementByIndexOptions.new()
		opts.local_user_id = Store.product_user_id
		opts.target_user_id = Store.product_user_id
		opts.achievement_index = i

		var player_achievement = EOS.Achievements.AchievementsInterface.copy_player_achievement_by_index(opts).player_achievement
		achievements[player_achievement.achievement_id].unlock_time = player_achievement.unlock_time

func _on_logout_success():
	achievements = {}
	_rebuild_ui()

func _rebuild_ui():
	_achievements_list.from_achievements_array(achievements.values())

func _on_achievement_pressed(node: AchievementsListAchievement):
	_achievement_popup.from_achievement_node(node)
	_achievement_popup.popup_centered()

func _on_achievements_interface_achievements_unlocked_v2_callback(data: Dictionary):
	print("--- Achievements: achievements_unlocked_v2_callback: ", data)

	if data.user_id == Store.product_user_id:
		achievements[data.achievement_id].unlock_time = data.unlock_time
	_rebuild_ui()

	# Show achivement unlocked notification
	var achievement_unlock_notification = AchievementUnlockNotificationScene.instantiate()
	Store.get_view("Notifications").add_notification(achievement_unlock_notification)
	achievement_unlock_notification.from_achievement_data(achievements[data.achievement_id])

func _on_achievements_interface_unlock_achievements_callback(data: Dictionary):
	print("--- Achievements: unlock_achievements_callback: ", EOS.result_str(data))

extends Node

#region Signals

## Emitted when an achievement is unlocked
## data has the following keys: achievement_id:String, unlock_time:int, user_id:String
signal achievement_unlocked(data: Dictionary)

#endregion


#region Public vars

#endregion


#region Private vars 

var _log = HLog.logger("HAchievements")
var _achievements: Array[HAchievementData]= [] 

#endregion


#region Built-in methods

func _ready() -> void:
	IEOS.achievements_interface_achievements_unlocked_v2_callback.connect(_on_achievements_interface_achievements_unlocked_v2_callback)

#endregion


#region Public methods

## Returns an [Array] of [HAchievementData] representing all the achievements in the game
func get_all_achievements_async() -> Array[HAchievementData]:
	_log.debug("Getting all achievements...")

	var opts = EOS.Achievements.QueryDefinitionsOptions.new()
	EOS.Achievements.AchievementsInterface.query_definitions(opts)

	var ret = await IEOS.achievements_interface_query_definitions_callback
	if not EOS.is_success(ret):
		_log.error("Failed to query achievements: result_code=%s" % EOS.result_str(ret))
		return []
	

	var get_count_opts = EOS.Achievements.GetAchievementDefinitionCountOptions.new()
	var achievement_count: int = EOS.Achievements.AchievementsInterface.get_achievement_definition_count(get_count_opts)
	_log.debug("Got all achievements: count=%s" % achievement_count)
	
	var achievements: Array[HAchievementData] = []
	for i in achievement_count:
		var copy_opts = EOS.Achievements.CopyAchievementDefinitionV2ByIndexOptions.new()
		copy_opts.achievement_index = i

		var copy_ret = EOS.Achievements.AchievementsInterface.copy_achievement_definition_v2_by_index(copy_opts)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy achievement: result_code=%s" % EOS.result_str(copy_ret))
			continue
		var definition: Dictionary = copy_ret.definition_v2
		var data = HAchievementData.new()
		var stat_thresholds = definition.stat_thresholds
		definition.erase("stat_thresholds")
		data.from_dict(definition)
		for threshold_data in stat_thresholds:
			var stat = {name = threshold_data.name, current_value = 0, threshold_value = threshold_data.threshold}
			data.update_stat(stat)

		achievements.append(data)

	_achievements = achievements
	return achievements


## Returns an [Array] of [HAchievementData] representing all the player's achievements
func get_player_achievements_async() -> Array[HAchievementData]:
	_log.debug("Getting player achievements...")
	var opts = EOS.Achievements.QueryPlayerAchievementsOptions.new()

	EOS.Achievements.AchievementsInterface.query_player_achievements(opts)
	var ret = await IEOS.achievements_interface_query_player_achievements_callback

	if not EOS.is_success(ret):
		_log.error("Failed to query player achievements: result_code=%s" % EOS.result_str(ret))
		return []
	
	var get_count_opts = EOS.Achievements.GetPlayerAchievementCountOptions.new()
	get_count_opts.user_id = HAuth.product_user_id
	var achievements_count = EOS.Achievements.AchievementsInterface.get_player_achievement_count(get_count_opts)
	_log.debug("Got player achievements: count=%s" % achievements_count)

	var achievements: Array[HAchievementData] = []
	for i in achievements_count:
		var copy_opts = EOS.Achievements.CopyPlayerAchievementByIndexOptions.new()
		copy_opts.achievement_index = i
		copy_opts.target_user_id = HAuth.product_user_id

		var copy_ret = EOS.Achievements.AchievementsInterface.copy_player_achievement_by_index(copy_opts)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy player achievement: result_code=%s" % EOS.result_str(copy_ret))
			continue
		
		var achievement_data = copy_ret.player_achievement

		var cached_achievement = null
		for achievement in _achievements:
			if achievement.achievement_id == achievement_data.achievement_id:
				cached_achievement = achievement
		var data: HAchievementData = HAchievementData.new() if cached_achievement == null else cached_achievement
		
		data.progress = achievement_data.progress
		data.unlock_time = achievement_data.unlock_time
		for stat in achievement_data.stat_infos:
			data.update_stat(stat)
		
		if cached_achievement == null:
			data.achievement_id = achievement_data.achievement_id
			data.flavor_text = achievement_data.flavor_text
			if data.is_unlocked():
				data.unlocked_display_name = achievement_data.display_name
				data.unlocked_description = achievement_data.description
				data.unlocked_icon_url = achievement_data.icon_url
			else:
				data.locked_display_name = achievement_data.display_name
				data.locked_description = achievement_data.description
				data.locked_icon_url = achievement_data.icon_url
			
		achievements.append(data)

	return achievements
	

func unlock_achievement_async(achievement_id: String) -> bool:
	_log.debug("Unlocking single achievement: achievement_id=%s" % achievement_id)
	return await unlock_achievements_async([achievement_id])


func unlock_achievements_async(achievement_ids: Array) -> bool:
	_log.debug("Unlocking multiple achievement(s): achievement_ids=%s" % achievement_ids)
	
	var opts = EOS.Achievements.UnlockAchievementsOptions.new()
	opts.user_id = HAuth.product_user_id
	opts.achievement_ids = achievement_ids

	EOS.Achievements.AchievementsInterface.unlock_achievements(opts)

	var res: Dictionary = await IEOS.achievements_interface_unlock_achievements_callback

	if not EOS.is_success(res):
		_log.error("Failed to unlock achievement(s): result_code=%s" % EOS.result_str(res))
		return false

	_log.debug("Unlocked %s achievement(s)" % res.achievements_count)

	return true

#endregion


#region Private methods

func _on_achievements_interface_achievements_unlocked_v2_callback(data: Dictionary):
	_log.verbose("Got Achievement unlocked v2 callback")
	achievement_unlocked.emit(data)

#endregion

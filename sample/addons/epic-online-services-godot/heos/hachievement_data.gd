## Achievement data from Epic Online Services
class_name HAchievementData
extends BaseClass


func _init() -> void:
	super._init("HAchievementData")

## Achievement ID that can be used to uniquely identify the achievement.
var achievement_id: String

## Localized display name for the achievement when it has been unlocked.
var unlocked_display_name: String

## Localized description for the achievement when it has been unlocked.
var unlocked_description: String

## URL of an icon to display for the achievement when it is unlocked. This may be null if there is no data configured in the dev portal.
var unlocked_icon_url: String

## Localized display name for the achievement when it is locked or hidden.
var locked_display_name: String

## Localized description for the achievement when it is locked or hidden.
var locked_description: String

## URL of an icon to display for the achievement when it is locked or hidden. This may be null if there is no data configured in the dev portal.
var locked_icon_url: String

## Localized flavor text that can be used by the game in an arbitrary manner. This may be null if there is no data configured in the dev portal.
var flavor_text: String

## Whether the achievement is hidden or not
var is_hidden: bool

## Progress towards completing this achievement (as a percentage).
var progress := 0.0

## Either [EOS.Achievements.UNLOCK_TIME_UNDEFINED] if not unlocked or the unix timestamp of when the achievement was unlocked by the user
var unlock_time := EOS.Achievements.UNLOCK_TIME_UNDEFINED

## Array of [Dictionary] having keys: name:String, current_value:int, threshold_value:int.
var stats = []


## Whether this achievement is unlocked
func is_unlocked() -> bool:
	return unlock_time != EOS.Achievements.UNLOCK_TIME_UNDEFINED


## Get the appropriate display name based on whether the achievement is unlocked or not
func get_display_name() -> String:
	if is_unlocked():
		return unlocked_display_name
	return locked_display_name


## Get the appropriate description based on whether the achievement is unlocked or not
func get_description() -> String:
	if is_unlocked():
		return unlocked_description
	return locked_description


## Get the appropriate icon URL based on whether the achievement is unlocked or not
func get_icon_url() -> String:
	if is_unlocked():
		return unlocked_icon_url
	return locked_icon_url


func update_stat(p_stat: Dictionary) -> void:
	var found = false
	for i in stats.size():
		var stat = stats[i]
		if stat.name == p_stat.name:
			stats[i] = p_stat
			found = true
			break
	
	if not found:
		stats.append(p_stat) 
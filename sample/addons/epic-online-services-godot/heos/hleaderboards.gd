extends Node

#region Signals

#endregion


#region Public vars

#endregion


#region Private vars 

var _log = HLog.logger("HLeaderboards")

#endregion


#region Built-in methods

func _ready() -> void:
	pass

#endregion


#region Public methods

## Returns an [Array] of [Dictionary] of all the leaderboards.
## Each leaderboard has the following data:
##   leaderboard_id: String - unique id
##   stat_name: String - name of stat used to rank the leaderboard
##   aggregation: EOS.Leaderboards.LeaderboardAggregation - aggregation used to sort the leaderboard
##   start_time: int - unix timestamp for the start time or [EOS.Leaderboard.LEADERBOARD_TIME_UNDEFINED]
##   end_time: int - unix timestamp for the end time or [EOS.Leaderboard.LEADERBOARD_TIME_UNDEFINED]
func get_leaderboard_definitions_async() -> Array:
	_log.debug("Getting leaderboards...")
	var query_opts = EOS.Leaderboards.QueryLeaderboardDefinitionsOptions.new()
	EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_definitions(query_opts)

	var ret: Dictionary = await IEOS.leaderboards_interface_query_leaderboard_definitions_callback
	if not EOS.is_success(ret):
		_log.error("Failed to query leaderboards: result_code=%s" % EOS.result_str(ret))
		return []
	
	var count_opts = EOS.Leaderboards.GetLeaderboardDefinitionCountOptions.new()
	var count: int = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_definition_count(count_opts)

	_log.debug("Got leaderboards: count=%s" % count)

	var leaderboards = []
	for i in count:
		var copy_opts = EOS.Leaderboards.CopyLeaderboardDefinitionByIndexOptions.new()
		copy_opts.leaderboard_index = i

		var copy_ret = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_definition_by_index(copy_opts)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy leaderboard: result_code=%s" % EOS.result_str(copy_ret))
			continue
		
		leaderboards.append(copy_ret.definition)

	return leaderboards


## Returns an [Array] of [Dictionary] of top records in the leaderboard.
## Each record has the following data:
##   leaderboard_id: String - unique id
##   user_id: String - product user id of user associated with this record
##   rank: int - sorted position in the leaderboard
##   score: int - leaderboard score
##   user_display_name: String - latest display name for the user or empty string if the user doesn't have one
func get_leaderboard_records_async(leaderboard_id: String) -> Array:
	_log.debug("Getting leaderboard ranks: leaderboard_id=%s" % leaderboard_id)
	var query_opts = EOS.Leaderboards.QueryLeaderboardRanksOptions.new()
	query_opts.leaderboard_id = leaderboard_id
	EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_ranks(query_opts)

	var ret = await IEOS.leaderboards_interface_query_leaderboard_ranks_callback
	if not EOS.is_success(ret):
		_log.error("Failed to query leaderboard ranks: result_code=%s" % EOS.result_str(ret))
		return []
	
	var count_opts = EOS.Leaderboards.GetLeaderboardRecordCountOptions.new()
	var count = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_record_count(count_opts)

	var records = []
	for i in count:
		var copy_opts = EOS.Leaderboards.CopyLeaderboardRecordByIndexOptions.new()
		copy_opts.leaderboard_record_index = i
		var copy_ret = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_record_by_index(copy_opts)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy leaderboard record: result_code=%s" % EOS.result_str(copy_ret))
			continue
		records.append(copy_ret.record)

	return records


#endregion


#region Private methods

#endregion

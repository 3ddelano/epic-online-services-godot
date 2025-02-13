extends Node

#region Signals

#endregion


#region Public vars

#endregion


#region Private vars 

var _log = HLog.logger("HStats")

#endregion


#region Built-in methods

func _ready() -> void:
	pass

#endregion


#region Public methods

## Returns an [Array] of stats for the current user.
## Each stat has:
##  name: String - name of the stat
##  value: int - current value for the stat
##  start_time: int - unix timestamp or [EOS.Stats.STATS_TIME_UNDEFINED]
##  end_time: int - unix timestamp or [EOS.Stats.STATS_TIME_UNDEFINED]
func get_stats_async() -> Array:
	_log.debug("Getting stats for user...")
	var query_opts = EOS.Stats.QueryStatsOptions.new()
	EOS.Stats.StatsInterface.query_stats(query_opts)

	var ret = await IEOS.stats_interface_query_stats_callback
	if not EOS.is_success(ret):
		_log.error("Failed to query stats: result_code=%s" % EOS.result_str(ret))
		return []

	var stats_count: int = EOS.Stats.StatsInterface.get_stats_count(EOS.Stats.GetStatsCountOptions.new())
	_log.debug("Got stats: count=%s" % stats_count)

	var stats = []
	for i in range(stats_count):
		var copy_opts = EOS.Stats.CopyStatByIndexOptions.new()
		copy_opts.stat_index = i
		var copy_ret = EOS.Stats.StatsInterface.copy_stat_by_index(copy_opts)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy stat: result_code=%s" % EOS.result_str(copy_ret))
		else:
			stats.append(copy_ret.stat)
	return stats


## Ingest a stat by the specified amount
func ingest_stat_async(stat_name: String, _ingest_amount: int) -> EOS.Result:
	_log.debug("Ingesting stat: name=%s, amount=%s" % [stat_name, _ingest_amount])
	var opts = EOS.Stats.IngestStatOptions.new()
	opts.local_user_id = HAuth.product_user_id
	opts.target_user_id = HAuth.product_user_id
	opts.stats = [
		{stat_name = stat_name, ingest_amount = _ingest_amount},
	]
	EOS.Stats.StatsInterface.ingest_stat(opts)

	var ret = await IEOS.stats_interface_ingest_stat_callback
	if not EOS.is_success(ret):
		_log.error("Failed to ingest stat: result_code=%s" % EOS.result_str(ret))
	return ret.result_code
	

#endregion


#region Private methods


#endregion

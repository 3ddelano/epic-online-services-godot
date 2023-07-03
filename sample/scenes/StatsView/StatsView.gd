class_name StatsView
extends VBoxContainer

# Local cache of stats
var stats = []

@onready var stat_name = $VB/StatName
@onready var ingest_amount = $VB/IngestAmount
@onready var ingest_btn = $VB/HB/IngestBtn
@onready var refresh_my_stats_btn = $VB/RefreshMyStatsBtn
@onready var my_stats = $VB/VB/MyStatsRichTextLabel
@onready var status_label = $VB/HB/StatusLabel


func _ready() -> void:
	var _c
	_c = Store.login_success.connect(_on_login_success)
	_c = Store.logout_success.connect(_on_logout_success)
	_c = EOS.get_instance().stats_interface_query_stats_callback.connect(_on_query_stats_callback)
	_c = EOS.get_instance().stats_interface_ingest_stat_callback.connect(_on_ingest_stat_callback)
	_c = ingest_btn.pressed.connect(_on_ingest_btn_pressed)
	_c = refresh_my_stats_btn.pressed.connect(_on_refresh_my_stats_btn_pressed)


func _on_login_success():
	var query_stats_options = EOS.Stats.QueryStatsOptions.new()
	query_stats_options.local_user_id = Store.product_user_id
	query_stats_options.target_user_id = Store.product_user_id
	EOS.Stats.StatsInterface.query_stats(query_stats_options)


func _on_logout_success():
	stats = []
	_update_stats()


func _on_ingest_btn_pressed():
	if stat_name.text == "" or ingest_amount.value == 0:
		return

	ingest_stat(stat_name.text, ingest_amount.value)


func _on_refresh_my_stats_btn_pressed():
	_on_login_success()


func _on_query_stats_callback(data: Dictionary):
	print("--- Stats: query_stats_callback: ", EOS.result_str(data) )

	var stats_count_options = EOS.Stats.GetStatsCountOptions.new()
	stats_count_options.target_user_id = Store.product_user_id
	var stats_count = EOS.Stats.StatsInterface.get_stats_count(stats_count_options)

	stats = []
	for i in range(stats_count):
		var copy_stat_options = EOS.Stats.CopyStatByIndexOptions.new()
		copy_stat_options.target_user_id = Store.product_user_id
		copy_stat_options.stat_index = i
		var ret = EOS.Stats.StatsInterface.copy_stat_by_index(copy_stat_options)
		if ret.result_code != EOS.Result.Success:
			print("--- Stats: copy_stat_by_index: Invalid stat: ", ret)
		else:
			stats.append(ret.stat)
	_update_stats()


func ingest_stat(_stat_name: String, _ingest_amount: int):
	status_label.text = "Ingesting stat..."
	print("--- Stats: Ingesting stat: name=%s amount=%s" % [_stat_name, _ingest_amount])

	var ingest_stat_options = EOS.Stats.IngestStatOptions.new()
	ingest_stat_options.local_user_id = Store.product_user_id
	ingest_stat_options.target_user_id = Store.product_user_id
	ingest_stat_options.stats = [
		{stat_name = _stat_name, ingest_amount = _ingest_amount},
	]
	EOS.Stats.StatsInterface.ingest_stat(ingest_stat_options)


func _on_ingest_stat_callback(data: Dictionary):
	print("--- Stats: ingest_stat_callback: ", EOS.result_str(data))

	if data.result_code != EOS.Result.Success:
		status_label.text = "Ingesting Failed: " + EOS.result_str(data)
		return

	print("--- Stats: Stat ingested")
	status_label.text = "Ingesting success"
	_on_login_success() # Query the stats


func _update_stats():
	var base_bbcode = """[table=4]
[cell][b]Stat Name[/b][/cell]
[cell][b]Value[/b][/cell]
[cell][b]Start Time[/b][/cell]
[cell][b]End Time[/b][/cell]
%s
[/table]"""

	var rows_bbcode = ""
	for stat in stats:
		rows_bbcode += "[cell]%s[/cell]" % stat.name
		if stat.value != null:
			rows_bbcode += "[cell]%s[/cell]" % stat.value
		if stat.start_time != null:
			rows_bbcode += "[cell]%s[/cell]" % stat.start_time
		if stat.end_time != null:
			rows_bbcode += "[cell]%s[/cell]" % stat.end_time

	my_stats.text = base_bbcode % rows_bbcode

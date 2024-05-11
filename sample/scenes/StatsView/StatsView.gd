class_name StatsView
extends VBoxContainer

# Local cache of stats
var stats = []

@onready var stat_name = %StatNameLineEdit
@onready var ingest_amount = %IngestAmount
@onready var ingest_btn = %IngestBtn
@onready var refresh_my_stats_btn = %RefreshMyStatsBtn
@onready var my_stats = %MyStatsRichTextLabel
@onready var status_label = %StatusLabel

func _ready() -> void:
	visibility_changed.connect(_on_query_stats)
	Store.logout_success.connect(_on_logout_success)
	IEOS.stats_interface_query_stats_callback.connect(_on_query_stats_callback)
	IEOS.stats_interface_ingest_stat_callback.connect(_on_ingest_stat_callback)

	stat_name.text_changed.connect(_on_stat_name_text_changed)
	ingest_btn.pressed.connect(_on_ingest_btn_pressed)
	refresh_my_stats_btn.pressed.connect(_on_query_stats)

func _on_query_stats():
	if not visible: return

	var query_opts = EOS.Stats.QueryStatsOptions.new()
	query_opts.local_user_id = Store.product_user_id
	query_opts.target_user_id = Store.product_user_id
	EOS.Stats.StatsInterface.query_stats(query_opts)

func _on_logout_success():
	stats = []
	_rebuild_ui()

func _on_stat_name_text_changed(new_text: String):
	if new_text.strip_edges() == "":
		ingest_btn.disabled = true
	else:
		ingest_btn.disabled = false

func _on_ingest_btn_pressed():
	ingest_stat(stat_name.text, ingest_amount.value)

func _on_query_stats_callback(data: Dictionary):
	print("--- Stats: query_stats_callback: ", EOS.result_str(data))

	var count_opts = EOS.Stats.GetStatsCountOptions.new()
	count_opts.target_user_id = Store.product_user_id
	var stats_count: int = EOS.Stats.StatsInterface.get_stats_count(count_opts)

	stats = []
	for i in range(stats_count):
		var copy_opts = EOS.Stats.CopyStatByIndexOptions.new()
		copy_opts.target_user_id = Store.product_user_id
		copy_opts.stat_index = i

		var ret = EOS.Stats.StatsInterface.copy_stat_by_index(copy_opts)
		if ret.result_code != EOS.Result.Success:
			print("--- Stats: copy_stat_by_index: Invalid stat: ", ret)
		else:
			stats.append(ret.stat)
	_rebuild_ui()

func ingest_stat(_stat_name: String, _ingest_amount: int):
	status_label.text = "Ingesting stat..."
	var ingest_opts = EOS.Stats.IngestStatOptions.new()
	ingest_opts.local_user_id = Store.product_user_id
	ingest_opts.target_user_id = Store.product_user_id
	ingest_opts.stats = [
		{stat_name = _stat_name, ingest_amount = _ingest_amount},
	]
	EOS.Stats.StatsInterface.ingest_stat(ingest_opts)

func _on_ingest_stat_callback(data: Dictionary):
	print("--- Stats: ingest_stat_callback: ", EOS.result_str(data))

	if data.result_code != EOS.Result.Success:
		status_label.text = "Ingesting Failed: " + EOS.result_str(data)
		return

	status_label.text = "Ingesting Success"
	_on_query_stats()

func _rebuild_ui():
	var base_bbcode = """[table=4]
[cell][b]Name[/b][/cell]
[cell][b]Value[/b][/cell]
[cell][b]Start Time[/b][/cell]
[cell][b]End Time[/b][/cell]
%s
[/table]"""

	var rows_bbcode = ""
	for stat in stats:
		rows_bbcode += "[cell]%s[/cell]" % stat.name
		rows_bbcode += "[cell]%s[/cell]" % stat.value
		if stat.start_time != EOS.Stats.STATS_TIME_UNDEFINED:
			var time_str = Time.get_datetime_string_from_unix_time(stat.start_time)
			rows_bbcode += "[cell]%s[/cell]" % time_str
		else:
			rows_bbcode += "[cell]Undefined[/cell]"

		if stat.end_time != EOS.Stats.STATS_TIME_UNDEFINED:
			var time_str = Time.get_datetime_string_from_unix_time(stat.end_time)
			rows_bbcode += "[cell]%s[/cell]" % time_str
		else:
			rows_bbcode += "[cell]Undefined[/cell]"

	my_stats.text = base_bbcode % rows_bbcode

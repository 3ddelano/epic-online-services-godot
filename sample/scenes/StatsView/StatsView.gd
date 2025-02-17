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
	HAuth.logged_out.connect(_on_logged_out)

	stat_name.text_changed.connect(_on_stat_name_text_changed)
	ingest_btn.pressed.connect(_on_ingest_btn_pressed)
	refresh_my_stats_btn.pressed.connect(_on_query_stats)


func _on_query_stats():
	if not visible: return
	stats = await HStats.get_stats_async()
	_rebuild_ui()


func _on_logged_out():
	stats = []
	_rebuild_ui()

func _on_stat_name_text_changed(new_text: String):
	if new_text.strip_edges() == "":
		ingest_btn.disabled = true
	else:
		ingest_btn.disabled = false

func _on_ingest_btn_pressed():
	ingest_stat(stat_name.text, ingest_amount.value)


func ingest_stat(_stat_name: String, _ingest_amount: int):
	status_label.text = "Ingesting stat..."
	var ret := await HStats.ingest_stat_async(_stat_name, _ingest_amount)
	if not EOS.is_success(ret):
		status_label.text = "Ingesting Failed: " + EOS.result_str(ret)
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

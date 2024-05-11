class_name LeaderboardsView
extends VBoxContainer

# Local cache of leaderboard data
var leaderboards = {}

@onready var select_leaderboard_btn = %SelectLeaderboardBtn
@onready var view_leaderboard_btn = %ViewLeaderboardBtn
@onready var leaderboard_id = %LeaderboardId
@onready var leaderboard_data_richtextlabel = %LeaderboardDataRichTextLabel

var selected_leaderboard = {
	id = "",
	records = []
}

func _ready() -> void:
	visibility_changed.connect(_on_query_leaderboard)
	Store.logout_success.connect(_on_logout_success)
	IEOS.leaderboards_interface_query_leaderboard_definitions_callback.connect(_on_query_leaderboard_definitions_callback)
	IEOS.leaderboards_interface_query_leaderboard_ranks_callback.connect(_on_query_leaderboard_ranks_callback)
	view_leaderboard_btn.pressed.connect(_on_view_leaderboard_btn_pressed)

	_rebuild_ui()

func _on_query_leaderboard():
	if not visible: return

	var query_leaderboard_opts = EOS.Leaderboards.QueryLeaderboardDefinitionsOptions.new()
	query_leaderboard_opts.local_user_id = Store.product_user_id
	EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_definitions(query_leaderboard_opts)

func _on_logout_success():
	leaderboards.clear()
	_update_select_leaderboard_button()
	selected_leaderboard.id = ""
	selected_leaderboard.records = []
	_rebuild_ui()

func _on_query_leaderboard_definitions_callback(data: Dictionary):
	print("--- Leaderboards: query_leaderboard_definitions_callback: ", EOS.result_str(data))

	var leaderboard_count_opts = EOS.Leaderboards.GetLeaderboardDefinitionCountOptions.new()
	var leaderboard_defs_count: int = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_definition_count(leaderboard_count_opts)

	leaderboards.clear()
	for i in leaderboard_defs_count:
		var copy_leaderboard_opts = EOS.Leaderboards.CopyLeaderboardDefinitionByIndexOptions.new()
		copy_leaderboard_opts.leaderboard_index = i

		var leaderboard_data = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_definition_by_index(copy_leaderboard_opts)
		if leaderboard_data.result_code != EOS.Result.Success:
			print("--- Leaderboards: copy_leaderboard_definition_by_index: ", EOS.result_str(data))
		else:
			leaderboards[leaderboard_data.definition.leaderboard_id] = leaderboard_data.definition

	_update_select_leaderboard_button()

func _update_select_leaderboard_button():
	select_leaderboard_btn.clear()

	var leaderboard_ids = leaderboards.keys()
	for i in leaderboard_ids.size():
		select_leaderboard_btn.add_item(leaderboard_ids[i], i)
		select_leaderboard_btn.set_item_metadata(i, leaderboard_ids[i])

func _on_view_leaderboard_btn_pressed():
	var selected_leaderboard_id = select_leaderboard_btn.get_selected_metadata()
	var query_ranks_opts = EOS.Leaderboards.QueryLeaderboardRanksOptions.new()
	query_ranks_opts.local_user_id = Store.product_user_id
	query_ranks_opts.leaderboard_id = selected_leaderboard_id
	query_ranks_opts.client_data = selected_leaderboard_id
	EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_ranks(query_ranks_opts)

func _on_query_leaderboard_ranks_callback(data: Dictionary):
	print("--- Leaderboards: query_leaderboard_ranks_callback: ", EOS.result_str(data))

	var records_count_opts = EOS.Leaderboards.GetLeaderboardRecordCountOptions.new()
	var records_count = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_record_count(records_count_opts)

	var leaderboard_records = []
	for i in records_count:
		var copy_leaderboard_record_options = EOS.Leaderboards.CopyLeaderboardRecordByIndexOptions.new()
		copy_leaderboard_record_options.leaderboard_record_index = i
		var leaderboard_record_data = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_record_by_index(copy_leaderboard_record_options)
		if leaderboard_record_data.result_code != EOS.Result.Success:
			print("--- Leaderboards: copy_leaderboard_record_by_index: ", EOS.result_str(data))
			continue
		leaderboard_records.append(leaderboard_record_data.record)

	selected_leaderboard.id = data.client_data
	selected_leaderboard.records = leaderboard_records
	_rebuild_ui()

func _rebuild_ui():
	if selected_leaderboard.id == "":
		leaderboard_id.visible = false
		leaderboard_data_richtextlabel.visible = false
	else:
		leaderboard_id.visible = true
		leaderboard_data_richtextlabel.visible = true

	leaderboard_id.text = selected_leaderboard.id
	var base_bbcode = """[table=3]
[cell][b]Rank[/b][/cell]
[cell][b]Score[/b][/cell]
[cell][b]User[/b][/cell]
%s
[/table]"""

	var rows_bbcode = ""
	for record in selected_leaderboard.records:
		rows_bbcode += "[cell]%s[/cell]" % record.rank
		if record.score != null:
			rows_bbcode += "[cell]%s[/cell]" % record.score
		if record.user_display_name != null:
			var display_name = record.user_display_name
			if record.user_id == Store.product_user_id:
				display_name += "[b] (you)[/b]"
			rows_bbcode += "[cell]%s[/cell]" % display_name

	leaderboard_data_richtextlabel.text = base_bbcode % rows_bbcode

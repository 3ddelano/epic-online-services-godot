class_name LeaderboardsView
extends VBoxContainer

# Local cache of leaderboard data
var leaderboards = []

@onready var select_leaderboard_btn = $VB/SelectLeaderboardBtn
@onready var view_leaderboard_btn = $VB/ViewLeaderboardBtn
@onready var leaderboard_name = $VB/LeaderboardName
@onready var leaderboard_data_richtextlabel = $VB/LeaderboardDataRichTextLabel


func _ready() -> void:
	var _c
	_c = Store.connect("login_success", Callable(self, "_on_login_success"))
	_c = Store.connect("logout_success", Callable(self, "_on_logout_success"))
	_c = EOS.get_instance().connect("leaderboards_interface_query_leaderboard_definitions_complete_callback", Callable(self, "_on_query_leaderboard_defs_complete_callback"))
	_c = EOS.get_instance().connect("leaderboards_interface_query_leaderboard_ranks_complete_callback", Callable(self, "_on_query_leaderboard_ranks_complete_callback"))
	_c = view_leaderboard_btn.connect("pressed", Callable(self, "_on_view_leaderboard_btn_pressed"))

	_update_leaderboard("", [])


func _on_login_success():
	#var query_userscores_options = EOS.Leaderboards.QueryLeaderboardUserScoresOptions.new()
	#query_userscores_options.local_user_id = Store.product_user_id
	#query_userscores_options.user_ids = [Store.product_user_id]
	#query_userscores_options.stat_info = [
	#	{
	#		stat_name = "stat_max",
	#		aggregation = EOS.Leaderboards.LeaderboardAggregation.Max
	#	}
	#]
	#EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_user_scores(query_userscores_options)
	#print(yield(EOS.get_instance(), "leaderboards_interface_query_leaderboard_user_scores_complete_callback"))

	#var user_score_count_options = EOS.Leaderboards.GetLeaderboardUserScoreCountOptions.new()
	#user_score_count_options.stat_name = "stat_max"
	#var userscores_count = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_user_score_count(user_score_count_options)
	#for i in userscores_count:
	#	var copy_userscore_options = EOS.Leaderboards.CopyLeaderboardUserScoreByIndexOptions.new()
	#	copy_userscore_options.stat_name = "stat_max"
	#	copy_userscore_options.leaderboard_user_score_index = i
	#	print(EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_user_score_by_index(copy_userscore_options))
	var query_leaderboard_defs_options = EOS.Leaderboards.QueryLeaderboardDefinitionsOptions.new()
	query_leaderboard_defs_options.local_user_id = Store.product_user_id
	EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_definitions(query_leaderboard_defs_options)


func _on_logout_success():
	leaderboards = []
	_update_select_leaderboard_button()
	_update_leaderboard("", [])


func _on_query_leaderboard_defs_complete_callback(data: Dictionary):
	print("--- Leaderboards: query_leaderboard_definitions_complete_callback: ", EOS.result_str(data))

	var leaderboard_defs_count = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_definition_count(EOS.Leaderboards.GetLeaderboardDefinitionCountOptions.new())

	leaderboards = []
	for i in leaderboard_defs_count:
		var copy_leaderboard_def_options = EOS.Leaderboards.CopyLeaderboardDefinitionByIndexOptions.new()
		copy_leaderboard_def_options.leaderboard_index = i
		var leaderboard_def_data = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_definition_by_index(copy_leaderboard_def_options)
		if leaderboard_def_data.result_code != EOS.Result.Success:
			print("--- Leaderboards: copy_leaderboard_definition_by_index: ", EOS.result_str(data))
			continue
		leaderboards.append(leaderboard_def_data.definition)

	#var copy_leaderboard_def_options = EOS.Leaderboards.CopyLeaderboardDefinitionByLeaderboardId.new()
	#copy_leaderboard_def_options.leaderboard_id = "stat_sum_leaderboard"
	#var leaderboard_def_data = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_definition_by_leaderboard_id(copy_leaderboard_def_options)
	#print("--- Leaderboards: copy_leaderboard_definition_by_leaderboard_id: ", leaderboard_def_data)
	_update_select_leaderboard_button()


func _update_select_leaderboard_button():
	select_leaderboard_btn.clear()
	select_leaderboard_btn.text = "Select a leaderboard"

	for i in leaderboards.size():
		var leaderboard = leaderboards[i]
		select_leaderboard_btn.add_item(leaderboard.leaderboard_id, i)
		select_leaderboard_btn.set_item_metadata(i, leaderboard.leaderboard_id)


func _on_view_leaderboard_btn_pressed():
	var selected_leaderboard_id = select_leaderboard_btn.get_selected_metadata()

	if not selected_leaderboard_id:
		return

	var query_leaderboard_ranks_options = EOS.Leaderboards.QueryLeaderboardRanksOptions.new()
	query_leaderboard_ranks_options.local_user_id = Store.product_user_id
	query_leaderboard_ranks_options.leaderboard_id = selected_leaderboard_id
	query_leaderboard_ranks_options.client_data = selected_leaderboard_id
	EOS.Leaderboards.LeaderboardsInterface.query_leaderboard_ranks(query_leaderboard_ranks_options)


func _on_query_leaderboard_ranks_complete_callback(data: Dictionary):
	print("--- Leaderboards: query_leaderboard_ranks_complete_callback: ", EOS.result_str(data))

	var leaderboard_records_count = EOS.Leaderboards.LeaderboardsInterface.get_leaderboard_record_count(EOS.Leaderboards.GetLeaderboardRecordCountOptions.new())

	var leaderboard_records = []
	for i in leaderboard_records_count:
		var copy_leaderboard_record_options = EOS.Leaderboards.CopyLeaderboardRecordByIndexOptions.new()
		copy_leaderboard_record_options.leaderboard_record_index = i
		var leaderboard_record_data = EOS.Leaderboards.LeaderboardsInterface.copy_leaderboard_record_by_index(copy_leaderboard_record_options)
		if leaderboard_record_data.result_code != EOS.Result.Success:
			print("--- Leaderboards: copy_leaderboard_record_by_index: ", EOS.result_str(data))
			continue
		leaderboard_records.append(leaderboard_record_data.leaderboard_record)

	_update_leaderboard(data.client_data, leaderboard_records)


func _update_leaderboard(leaderboard_id: String, records: Array):
	if leaderboard_id == "":
		leaderboard_name.visible = false
		leaderboard_data_richtextlabel.visible = false
	else:
		leaderboard_name.visible = true
		leaderboard_data_richtextlabel.visible = true

	leaderboard_name.text = leaderboard_id
	var base_bbcode = """[table=3]
[cell][b]Rank[/b][/cell]
[cell][b]Score[/b][/cell]
[cell][b]User[/b][/cell]
%s
[/table]"""

	var rows_bbcode = ""
	for record in records:
		rows_bbcode += "[cell]%s[/cell]" % record.rank
		if record.score != null:
			rows_bbcode += "[cell]%s[/cell]" % record.score
		if record.user_display_name != null:
			var display_name = record.user_display_name
			if record.user_id == Store.product_user_id:
				display_name += "[b] (you)[/b]"
			rows_bbcode += "[cell]%s[/cell]" % display_name

	leaderboard_data_richtextlabel.text = base_bbcode % rows_bbcode


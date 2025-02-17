class_name LeaderboardsView
extends VBoxContainer

# Local cache of leaderboard data
var leaderboards = []

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
	HAuth.logged_out.connect(_on_logged_out)
	# IEOS.leaderboards_interface_query_leaderboard_ranks_callback.connect(_on_query_leaderboard_ranks_callback)
	view_leaderboard_btn.pressed.connect(_on_view_leaderboard_btn_pressed)

	_rebuild_ui()


func _on_query_leaderboard():
	if not visible: return

	leaderboards = await HLeaderboards.get_leaderboard_definitions_async()

	_update_select_leaderboard_button()
	

func _on_logged_out():
	leaderboards = []
	_update_select_leaderboard_button()
	selected_leaderboard.id = ""
	selected_leaderboard.records = []
	_rebuild_ui()


func _update_select_leaderboard_button():
	select_leaderboard_btn.clear()

	for i in leaderboards.size():
		var id = leaderboards[i].leaderboard_id
		select_leaderboard_btn.add_item(id, i)
		select_leaderboard_btn.set_item_metadata(i, id)


func _on_view_leaderboard_btn_pressed():
	var selected_leaderboard_id = select_leaderboard_btn.get_selected_metadata()
	var records = await HLeaderboards.get_leaderboard_records_async(selected_leaderboard_id)

	selected_leaderboard.id = selected_leaderboard_id
	selected_leaderboard.records = records
	_rebuild_ui()


func _rebuild_ui():
	if not selected_leaderboard.id:
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
			if record.user_id == HAuth.product_user_id:
				display_name += "[b] (you)[/b]"
			rows_bbcode += "[cell]%s[/cell]" % display_name

	leaderboard_data_richtextlabel.text = base_bbcode % rows_bbcode

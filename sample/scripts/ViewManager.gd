class_name ViewManager
extends VBoxContainer

@onready var auth_btn = $HB/AuthBtn
@onready var achievements_btn = $HB/AchievementsBtn
@onready var custominvites_btn = $HB/CustomInvitesBtn
@onready var stats_btn = $HB/StatsBtn
@onready var leaderboards_btn = $HB/LeaderboardsBtn
@onready var friends_btn = $HB/FriendsBtn
@onready var ui_btn = $HB/UIBtn
@onready var metrics_btn = $HB/MetricsBtn


func _ready() -> void:
	var _c = auth_btn.connect("pressed", Callable(self, "set_view").bind("Login"))
	_c = achievements_btn.connect("pressed", Callable(self, "set_view").bind("Achievements"))
	_c = custominvites_btn.connect("pressed", Callable(self, "set_view").bind("CustomInvites"))
	_c = stats_btn.connect("pressed", Callable(self, "set_view").bind("Stats"))
	_c = leaderboards_btn.connect("pressed", Callable(self, "set_view").bind("Leaderboards"))
	_c = friends_btn.connect("pressed", Callable(self, "set_view").bind("Friends"))
	_c = ui_btn.connect("pressed", Callable(self, "set_view").bind("UI"))
	_c = metrics_btn.connect("pressed", Callable(self, "set_view").bind("Metrics"))


func get_view(view_name: String):
	if view_name == "Logs":
		return $VSC/LogsView

	elif view_name == "Notifications":
		return $VSC/VB/NotificationsLayer/NotificationsView

	return get_node("VSC/VB/" + view_name + "View")


func set_view(view_name: String):
	for child in $VSC/VB.get_children():
		if child is CanvasItem:
			child.visible = false

	get_view(view_name).visible = true

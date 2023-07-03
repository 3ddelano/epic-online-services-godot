class_name ViewManager
extends VBoxContainer

@onready var auth_btn = %AuthBtn
@onready var achievements_btn = %AchievementsBtn
@onready var custominvites_btn = %CustomInvitesBtn
@onready var stats_btn = %StatsBtn
@onready var leaderboards_btn = %LeaderboardsBtn
@onready var friends_btn = %FriendsBtn
@onready var ui_btn = %UIBtn
@onready var metrics_btn = %MetricsBtn
@onready var lobbies_btn = %LobbiesBtn


func _ready() -> void:
	var _c = auth_btn.pressed.connect(Callable(self, "set_view").bind("Login"))
	_c = achievements_btn.pressed.connect(Callable(self, "set_view").bind("Achievements"))
	_c = custominvites_btn.pressed.connect(Callable(self, "set_view").bind("CustomInvites"))
	_c = stats_btn.pressed.connect(Callable(self, "set_view").bind("Stats"))
	_c = leaderboards_btn.pressed.connect(Callable(self, "set_view").bind("Leaderboards"))
	_c = friends_btn.pressed.connect(Callable(self, "set_view").bind("Friends"))
	_c = ui_btn.pressed.connect(Callable(self, "set_view").bind("UI"))
	_c = metrics_btn.pressed.connect(Callable(self, "set_view").bind("Metrics"))
	_c = lobbies_btn.pressed.connect(Callable(self, "set_view").bind("Lobbies"))


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

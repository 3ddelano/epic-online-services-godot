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
@onready var needs_login_btns = %NeedsLoginBtns


func _ready() -> void:
	_disable_needs_login_btns()
	Store.login_success.connect(_enable_needs_login_btns)
	Store.logout_success.connect(_disable_needs_login_btns)

	auth_btn.pressed.connect(Callable(self, "set_view").bind("Login"))
	achievements_btn.pressed.connect(Callable(self, "set_view").bind("Achievements"))
	custominvites_btn.pressed.connect(Callable(self, "set_view").bind("CustomInvites"))
	stats_btn.pressed.connect(Callable(self, "set_view").bind("Stats"))
	leaderboards_btn.pressed.connect(Callable(self, "set_view").bind("Leaderboards"))
	friends_btn.pressed.connect(Callable(self, "set_view").bind("Friends"))
	ui_btn.pressed.connect(Callable(self, "set_view").bind("UI"))
	metrics_btn.pressed.connect(Callable(self, "set_view").bind("Metrics"))
	lobbies_btn.pressed.connect(Callable(self, "set_view").bind("Lobbies"))


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


func _disable_needs_login_btns():
	for child in needs_login_btns.get_children():
		child.disabled = true


func _enable_needs_login_btns():
	for child in needs_login_btns.get_children():
		child.disabled = false

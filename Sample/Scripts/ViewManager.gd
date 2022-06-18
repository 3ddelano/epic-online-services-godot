class_name ViewManager
extends VBoxContainer

onready var auth_btn = $HB/AuthBtn
onready var achievements_btn = $HB/AchievementsBtn
onready var custominvites_btn = $HB/CustomInvitesBtn
onready var stats_btn = $HB/StatsBtn
onready var leaderboards_btn = $HB/LeaderboardsBtn
onready var friends_btn = $HB/FriendsBtn
onready var ui_btn = $HB/UIBtn

func _ready() -> void:
	var _c = auth_btn.connect("pressed", self, "set_view", ["Login"])
	_c = achievements_btn.connect("pressed", self, "set_view", ["Achievements"])
	_c = custominvites_btn.connect("pressed", self, "set_view", ["CustomInvites"])
	_c = stats_btn.connect("pressed", self, "set_view", ["Stats"])
	_c = leaderboards_btn.connect("pressed", self, "set_view", ["Leaderboards"])
	_c = friends_btn.connect("pressed", self, "set_view", ["Friends"])
	_c = ui_btn.connect("pressed", self, "set_view", ["UI"])


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

class_name ViewManager
extends VBoxContainer

onready var auth_btn = $HB/AuthBtn
onready var achievements_btn = $HB/AchievementsBtn
onready var custominvites_btn = $HB/CustomInvitesBtn
onready var stats_btn = $HB/StatsBtn
onready var leaderboards_btn = $HB/LeaderboardsBtn

func _ready() -> void:
	var _c = auth_btn.connect("pressed", self, "_on_auth_btn_pressed")
	_c = achievements_btn.connect("pressed", self, "_on_achievements_btn_pressed")
	_c = custominvites_btn.connect("pressed", self, "_on_custominvites_btn_pressed")
	_c = stats_btn.connect("pressed", self, "_on_stats_btn_pressed")
	_c = leaderboards_btn.connect("pressed", self, "_on_leaderboards_btn_pressed")

func _on_auth_btn_pressed():
	set_view("Login")


func _on_achievements_btn_pressed():
	set_view("Achievements")


func _on_custominvites_btn_pressed():
	set_view("CustomInvites")


func _on_stats_btn_pressed():
	set_view("Stats")


func _on_leaderboards_btn_pressed():
	set_view("Leaderboards")


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

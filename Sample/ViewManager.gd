class_name ViewManager
extends VBoxContainer

onready var auth_btn = $HB/AuthBtn
onready var achievements_btn = $HB/AchievementsBtn
onready var leaderboards_btn = $HB/LeaderboardsBtn
onready var lobbies_btn = $HB/LobbiesBtn

func _ready() -> void:
	var _c = auth_btn.connect("pressed", self, "_on_auth_btn_pressed")
	_c = achievements_btn.connect("pressed", self, "_on_achievements_btn_pressed")
	_c = leaderboards_btn.connect("pressed", self, "_on_leaderboards_btn_pressed")
	_c = lobbies_btn.connect("pressed", self, "_on_lobbies_btn_pressed")

func _on_auth_btn_pressed():
	set_view("Login")


func _on_achievements_btn_pressed():
	set_view("Achievements")


func get_view(view_name: String):
	if view_name == "Logs":
		return $VSC/LogsView

	return get_node("VSC/VB/" + view_name + "View")


func set_view(view_name: String):
	for child in $VSC/VB.get_children():
		child.visible = false

	get_view(view_name).visible = true

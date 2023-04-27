class_name UIView
extends VBoxContainer

@onready var loginwithepic = $LoginWithEpic
@onready var container = $VB

@onready var friends_visible_label = $VB/HB/FriendsVisibleLabel

@onready var notification_location_btn = $VB/HB2/NotificationLocationBtn
@onready var notification_location_label = $VB/HB2/NotificationLocationLabel

@onready var show_friends_btn = $VB/HB3/ShowFriendsBtn


func _ready() -> void:
	var _c
	_c = Store.connect("login_success", Callable(self, "_on_login_success"))
	_c = Store.connect("logout_success", Callable(self, "_on_logout_success"))

	_c = notification_location_btn.connect("pressed", Callable(self, "_on_notification_location_btn_pressed"))
	_c = show_friends_btn.connect("pressed", Callable(self, "_on_show_friends_btn_pressed"))

#	_c = EOS.get_instance().connect("ui_interface_show_friends_callback", self, "_on_friends_overlay_visibility_changed")
#	_c = EOS.get_instance().connect("ui_interface_hide_friends_callback", self, "_on_friends_overlay_visibility_changed")
	_c = EOS.get_instance().connect("ui_interface_display_settings_updated_callback", Callable(self, "_on_display_settings_updated_callback"))

	loginwithepic.visible = true
	container.visible = false


func _on_login_success():
	if Store.epic_account_id == "":
		loginwithepic.visible = true
		container.visible = false
		return

	loginwithepic.visible = false
	container.visible = true


func _on_logout_success():
	loginwithepic.visible = true
	container.visible = false


#func _on_friends_visible_btn_pressed():
#	var options = EOS.UI.GetFriendsVisibleOptions.new()
#	options.local_user_id = Store.epic_account_id
#	var is_friends_visible = EOS.UI.UIInterface.get_friends_visible(options)
#
#	if is_friends_visible:
#		friends_visible_label.text = "Yes"
#	else:
#		friends_visible_label.text = "No"

func _on_notification_location_btn_pressed():
	var notification_location = EOS.UI.UIInterface.get_notification_location_preference()
	notification_location_label.text = EOS.UI.NotificationLocation.keys()[notification_location]


func _on_show_friends_btn_pressed():
	var options = EOS.UI.ShowFriendsOptions.new()
	options.local_user_id = Store.epic_account_id
	EOS.UI.UIInterface.show_friends(options)


#func _on_friends_overlay_visibility_changed(_data: Dictionary):
#	_on_friends_visible_btn_pressed()


func _on_display_settings_updated_callback(data: Dictionary):
#	print("--- UI: display_settings_updated_callback: ", data)

	if data.is_visible:
		friends_visible_label.text = "Overlay Visible: Yes"
	else:
		friends_visible_label.text = "Overlay Visible: No"

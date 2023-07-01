class_name UIView
extends VBoxContainer

@onready var loginwithepic = %LoginWithEpic
@onready var container = $VB

@onready var friends_visible_label = %FriendsVisibleLabel

@onready var notification_location_btn = %NotificationLocationBtn
@onready var notification_location_label = %NotificationLocationLabel

@onready var get_toggle_friends_key_btn = %GetToggleFriendsKeyBtn
@onready var get_toggle_friends_key_label = %GetToggleFriendsKeyLabel

@onready var show_friends_btn = %ShowFriendsBtn


func _ready() -> void:
	var _c
	_c = Store.login_success.connect(_on_login_success)
	_c = Store.logout_success.connect(_on_logout_success)

	_c = notification_location_btn.pressed.connect(_on_notification_location_btn_pressed)
	_c = get_toggle_friends_key_btn.pressed.connect(_on_get_toggle_friends_key_btn)
	_c = show_friends_btn.pressed.connect(_on_show_friends_btn_pressed)

#	_c = EOS.get_instance().ui_interface_show_friends_callback.connect(_on_friends_overlay_visibility_changed)
#	_c = EOS.get_instance().ui_interface_hide_friends_callback.connect(_on_friends_overlay_visibility_changed)
	_c = EOS.get_instance().ui_interface_display_settings_updated_callback.connect(_on_display_settings_updated_callback)

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


func _on_get_toggle_friends_key_btn():
	var opts = EOS.UI.GetToggleFriendsKeyOptions.new()
	var toggle_friends_key = EOS.UI.UIInterface.get_toggle_friends_key(opts)

	var key_combination = []
	var keys = EOS.UI.KeyCombination.keys()
	for key in keys:
		if toggle_friends_key & EOS.UI.KeyCombination[key] == EOS.UI.KeyCombination[key]:
			key_combination.append(key)
	get_toggle_friends_key_label.text = " + ".join(key_combination)


func _on_show_friends_btn_pressed():
	var options = EOS.UI.ShowFriendsOptions.new()
	options.local_user_id = Store.epic_account_id
	EOS.UI.UIInterface.show_friends(options)


#func _on_friends_overlay_visibility_changed(_data: Dictionary):
#	_on_friends_visible_btn_pressed()


func _on_display_settings_updated_callback(data: Dictionary):
#	print("--- UI: display_settings_updated_callback: ", data)

	if data.is_visible:
		friends_visible_label.text = "Friends Overlay Visible: Yes"
	else:
		friends_visible_label.text = "Friends Overlay Visible: No"

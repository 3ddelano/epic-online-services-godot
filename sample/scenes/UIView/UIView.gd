class_name UIView
extends VBoxContainer

@onready var friends_visible_label = %FriendsVisibleLabel

@onready var notification_location_btn = %NotificationLocationBtn
@onready var notification_location_label = %NotificationLocationLabel

@onready var get_toggle_friends_key_btn = %GetToggleFriendsKeyBtn
@onready var get_toggle_friends_key_label = %GetToggleFriendsKeyLabel

@onready var show_friends_btn = %ShowFriendsBtn
@onready var show_block_player_btn = %ShowBlockPlayerBtn
@onready var show_report_player_btn = %ShowReportPlayerBtn


func _ready() -> void:
	notification_location_btn.pressed.connect(_on_notification_location_btn_pressed)
	get_toggle_friends_key_btn.pressed.connect(_on_get_toggle_friends_key_btn)
	show_friends_btn.pressed.connect(_on_show_friends_btn_pressed)
	show_block_player_btn.pressed.connect(_on_show_block_player_btn_pressed)
	show_report_player_btn.pressed.connect(_on_show_report_player_btn_pressed)

	EOS.get_instance().ui_interface_display_settings_updated_callback.connect(_on_display_settings_updated_callback)


func _on_notification_location_btn_pressed():
	var notification_location = EOS.UI.UIInterface.get_notification_location_preference()
	notification_location_label.text = EOS.UI.NotificationLocation.keys()[notification_location]


func _on_get_toggle_friends_key_btn():
	var opts = EOS.UI.GetToggleFriendsKeyOptions.new()
	var toggle_friends_key = EOS.UI.UIInterface.get_toggle_friends_key(opts)

	var key_combination = []
	var keys = EOS.UI.KeyCombination.keys()
	for key in keys:
		if key == "None" or key == "Numpad0": continue
		if toggle_friends_key & EOS.UI.KeyCombination[key] == EOS.UI.KeyCombination[key]:
			key_combination.append(key)
	get_toggle_friends_key_label.text = " + ".join(key_combination)


func _on_show_friends_btn_pressed():
	var options = EOS.UI.ShowFriendsOptions.new()
	options.local_user_id = Store.epic_account_id
	EOS.UI.UIInterface.show_friends(options)


func _on_show_block_player_btn_pressed():
	var opts = EOS.UI.ShowBlockPlayerOptions.new()
	opts.local_user_id = Store.epic_account_id
	opts.target_user_id = Store.epic_account_id
	EOS.UI.UIInterface.show_block_player(opts)
	print("--- UI: show_block_player_callback: ", EOS.result_str(await EOS.get_instance().ui_interface_show_block_player_callback))


func _on_show_report_player_btn_pressed():
	var opts = EOS.UI.ShowReportPlayerOptions.new()
	opts.local_user_id = Store.epic_account_id
	opts.target_user_id = Store.epic_account_id
	EOS.UI.UIInterface.show_report_player(opts)
	print("--- UI: show_report_player_callback: ", EOS.result_str(await EOS.get_instance().ui_interface_show_report_player_callback))



func _on_display_settings_updated_callback(data: Dictionary):
#	print("--- UI: display_settings_updated_callback: ", data)

	if data.is_visible:
		friends_visible_label.text = "Friends Overlay Visible: Yes"
	else:
		friends_visible_label.text = "Friends Overlay Visible: No"

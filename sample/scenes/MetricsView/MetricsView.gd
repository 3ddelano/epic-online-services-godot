class_name MetricView
extends VBoxContainer

@onready var start_player_session_btn = $HB/StartPlayerSessionBtn

@onready var end_player_session_btn = $HB2/EndPlayerSessionBtn


func _ready() -> void:
	start_player_session_btn.pressed.connect(_on_start_player_session_btn_pressed)
	end_player_session_btn.pressed.connect(_on_end_player_session_btn_pressed)


func _on_start_player_session_btn_pressed():
	var options = EOS.Metrics.BeginPlayerSessionOptions.new()
	if Store.epic_account_id != "":
		options.account_id_type = EOS.Metrics.MetricsAccountIdType.Epic
		options.account_id = Store.epic_account_id
	else:
		options.account_id_type = EOS.Metrics.MetricsAccountIdType.External
		options.account_id = Store.product_user_id

	if Store.display_name != "":
		options.display_name = Store.display_name
	else:
		options.display_name = "Unnamed user"

	options.controller_type = EOS.Metrics.UserControllerType.MouseKeyboard
	var ret = EOS.Metrics.MetricsInterface.begin_player_session(options)
	print("--- Metrics: start_player_session: %s" % EOS.result_str(ret))


func _on_end_player_session_btn_pressed():
	var options = EOS.Metrics.EndPlayerSessionOptions.new()

	if Store.epic_account_id != "":
		options.account_id_type = EOS.Metrics.MetricsAccountIdType.Epic
		options.account_id = Store.epic_account_id
	else:
		options.account_id_type = EOS.Metrics.MetricsAccountIdType.External
		options.account_id = Store.product_user_id

	var ret = EOS.Metrics.MetricsInterface.end_player_session(options)
	print("--- Metrics: end_player_session: %s" % EOS.result_str(ret))


class_name MetricView
extends VBoxContainer

@onready var begin_player_session_btn = %BeginPlayerSessionBtn
@onready var end_player_session_btn = %EndPlayerSessionBtn


func _ready() -> void:
	begin_player_session_btn.pressed.connect(_on_begin_player_session_btn_pressed)
	end_player_session_btn.pressed.connect(_on_end_player_session_btn_pressed)


func _on_begin_player_session_btn_pressed():
	var begin_sess_opts = EOS.Metrics.BeginPlayerSessionOptions.new()
	begin_sess_opts.controller_type = EOS.Metrics.UserControllerType.MouseKeyboard

	if HAuth.epic_account_id:
		begin_sess_opts.account_id_type = EOS.Metrics.MetricsAccountIdType.Epic
		begin_sess_opts.account_id = HAuth.epic_account_id
	else:
		begin_sess_opts.account_id_type = EOS.Metrics.MetricsAccountIdType.External
		begin_sess_opts.account_id = HAuth.product_user_id

	if Store.display_name:
		begin_sess_opts.display_name = Store.display_name
	else:
		begin_sess_opts.display_name = "Unnamed user"

	var begin_sess_ret = EOS.Metrics.MetricsInterface.begin_player_session(begin_sess_opts)
	print("--- Metrics: begin_player_session: ",EOS.result_str(begin_sess_ret))


func _on_end_player_session_btn_pressed():
	var end_sess_opts = EOS.Metrics.EndPlayerSessionOptions.new()

	if HAuth.epic_account_id != "":
		end_sess_opts.account_id_type = EOS.Metrics.MetricsAccountIdType.Epic
		end_sess_opts.account_id = HAuth.epic_account_id
	else:
		end_sess_opts.account_id_type = EOS.Metrics.MetricsAccountIdType.External
		end_sess_opts.account_id = HAuth.product_user_id

	var end_sess_ret = EOS.Metrics.MetricsInterface.end_player_session(end_sess_opts)
	print("--- Metrics: end_player_session: ", EOS.result_str(end_sess_ret))


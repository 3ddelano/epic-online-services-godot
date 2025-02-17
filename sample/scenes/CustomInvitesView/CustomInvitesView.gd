class_name CustomInvitesView
extends VBoxContainer

@onready var payload_textedit = %PayloadTextEdit
@onready var send_invite_btn = %SendInviteBtn

func _ready() -> void:
	IEOS.custom_invites_interface_send_custom_invite_callback.connect(_on_send_custom_invite_callback)
	IEOS.custom_invites_interface_custom_invite_accepted_callback.connect(_on_custom_invite_accepted_callback)
	IEOS.custom_invites_interface_custom_invite_received_callback.connect(_on_custom_invite_received_callback)
	IEOS.custom_invites_interface_custom_invite_rejected_callback.connect(_on_custom_invite_rejected_callback)
	IEOS.custom_invites_interface_request_to_join_received_callback.connect(_on_request_to_join_received_callback)
	IEOS.custom_invites_interface_request_to_join_response_received_callback.connect(_on_request_to_join_response_received_callback)
	IEOS.custom_invites_interface_request_to_join_accepted_callback.connect(_on_request_to_join_accepted_callback)
	IEOS.custom_invites_interface_request_to_join_rejected_callback.connect(_on_request_to_join_rejected_callback)

	send_invite_btn.pressed.connect(_on_send_invite_btn_pressed)
	payload_textedit.text_changed.connect(_on_payload_textedit_text_changed)
	send_invite_btn.disabled = true

func _on_send_custom_invite_callback(data: Dictionary):
	print("--- Custom Invites: send_custom_invite_callback: ", EOS.result_str(data))

func _on_custom_invite_accepted_callback(data: Dictionary):
	print("--- Custom Invites: custom_invite_accepted_callback: ", EOS.result_str(data))

	var finalize_opts = EOS.CustomInvites.FinalizeInviteOptions.new()
	finalize_opts.target_user_id = HAuth.product_user_id
	finalize_opts.local_user_id = HAuth.product_user_id
	finalize_opts.custom_invite_id = data.custom_invite_id
	finalize_opts.processing_result = EOS.Result.Success

	var res = EOS.CustomInvites.CustomInvitesInterface.finalize_invite(finalize_opts)
	print("--- Custom Invites: finalize_invite: ", EOS.result_str(res))

func _on_custom_invite_received_callback(data: Dictionary):
	print("--- Custom Invites: custom_invite_received_callback: ", EOS.result_str(data))

func _on_custom_invite_rejected_callback(data: Dictionary):
	print("--- Custom Invites: custom_invite_rejected_callback: ", EOS.result_str(data))

func _on_request_to_join_received_callback(data: Dictionary):
	print("--- Custom Invites: request_to_join_received_callback: ", EOS.result_str(data))

func _on_request_to_join_response_received_callback(data: Dictionary):
	print("--- Custom Invites: request_to_join_response_received_callback: ", EOS.result_str(data))

func _on_request_to_join_accepted_callback(data: Dictionary):
	print("--- Custom Invites: request_to_join_accepted_callback: ", EOS.result_str(data))

func _on_request_to_join_rejected_callback(data: Dictionary):
	print("--- Custom Invites: request_to_join_rejected_callback: ", EOS.result_str(data))

func _on_send_invite_btn_pressed():
	var set_invite_opts = EOS.CustomInvites.SetCustomInviteOptions.new()
	set_invite_opts.local_user_id = HAuth.product_user_id
	set_invite_opts.payload = payload_textedit.text
	var set_res: EOS.Result = EOS.CustomInvites.CustomInvitesInterface.set_custom_invite(set_invite_opts)
	print("--- Custom Invites: set_custom_invite: ", EOS.result_str(set_res))

	var send_invite_opts = EOS.CustomInvites.SendCustomInviteOptions.new()
	send_invite_opts.local_user_id = HAuth.product_user_id
	send_invite_opts.target_user_ids = [Store.second_product_user_id]
	EOS.CustomInvites.CustomInvitesInterface.send_custom_invite(send_invite_opts)
	payload_textedit.text = ""

func _on_payload_textedit_text_changed():
	if payload_textedit.text.strip_edges() == "":
		send_invite_btn.disabled = true
	else:
		send_invite_btn.disabled = false

class_name CustomInvitesView
extends VBoxContainer

@onready var set_payload_btn = $VB/SetPayloadBtn
@onready var payload_textedit = $VB/PayloadTextEdit
@onready var send_invite_btn = $VB/SendInviteBtn


func _ready() -> void:
	var _c
	_c = EOS.get_instance().custom_invites_interface_send_custom_invite_callback.connect(_on_send_custom_invite_callback)
	_c = EOS.get_instance().custom_invites_interface_custom_invite_accepted_callback.connect(_on_custom_invite_accepted_callback)
	_c = EOS.get_instance().custom_invites_interface_custom_invite_received_callback.connect(_on_custom_invite_received_callback)
	_c = EOS.get_instance().custom_invites_interface_custom_invite_rejected_callback.connect(_on_custom_invite_rejected_callback)
	_c = set_payload_btn.pressed.connect(_on_set_payload_btn_pressed)
	_c = send_invite_btn.pressed.connect(_on_send_invite_btn_pressed)


func _on_send_custom_invite_callback(data: Dictionary):
	print("--- Custom Invites: send_custom_invite_callback: ", EOS.result_str(data))


func _on_custom_invite_accepted_callback(data: Dictionary):
	print("--- Custom Invites: custom_invite_accepted_callback: ", EOS.result_str(data))

	var options1 = EOS.CustomInvites.FinalizeInviteOptions.new()
	options1.target_user_id = Store.product_user_id
	options1.local_user_id = Store.product_user_id
	options1.custom_invite_id = data.custom_invite_id
	options1.processing_result = EOS.Result.Success

	var res = EOS.CustomInvites.CustomInvitesInterface.finalize_invite(options1)
	print("--- Custom Invites: finalize_invite: ", EOS.result_str(res))


func _on_custom_invite_received_callback(data: Dictionary):
	print("--- Custom Invites: custom_invite_received_callback: ", EOS.result_str(data))


func _on_custom_invite_rejected_callback(data: Dictionary):
	print("--- Custom Invites: custom_invite_rejected_callback: ", EOS.result_str(data))


func _on_set_payload_btn_pressed():
	var options1 = EOS.CustomInvites.SetCustomInviteOptions.new()
	options1.local_user_id = Store.product_user_id
	options1.payload = payload_textedit.text
	var res = EOS.CustomInvites.CustomInvitesInterface.set_custom_invite(options1)
	print("--- Custom Invites: set_custom_invite: ", EOS.result_str(res))


func _on_send_invite_btn_pressed():
	var options1 = EOS.CustomInvites.SendCustomInviteOptions.new()
	options1.local_user_id = Store.product_user_id
	options1.target_user_ids = [Store.second_product_user_id]
	EOS.CustomInvites.CustomInvitesInterface.send_custom_invite(options1)

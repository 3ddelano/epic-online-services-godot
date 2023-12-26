# Copyright (c) 2023-present Delano Lourenco
# https://github.com/3ddelano/epic-online-services-godot/
# MIT License
## Runtime controller for Epic Online Services. This is available as the EOSGRuntime autoload.
class_name EOSGRuntime_
extends Node


## The Product User ID of the most recent logged in user.
var local_product_user_id: String

## The Epic Account ID of the most recent logged in user.
var local_epic_account_id: String


func _ready() -> void:
	IEOS.auth_interface_login_callback.connect(func (data: Dictionary):
		if data.local_user_id != "":
			local_epic_account_id = data.local_user_id
	)

	IEOS.connect_interface_login_callback.connect(func (data: Dictionary):
		if data.local_user_id != "":
			local_product_user_id = data.local_user_id
	)

	IEOS.auth_interface_logout_callback.connect(_on_logout)


func _on_logout(data: Dictionary):
	local_epic_account_id = ""
	local_product_user_id = ""


func _process(_delta: float):
	IEOS.tick()

class_name LoginView
extends CenterContainer

#region Enums

enum States {
	ChooseMethod,
	EnterCredentials,
	Pending,
	Success,
	Error
}

#endregion


#region Constants

const LOGIN_TYPES = {
	DEVELOPER = {
		type = "DEVELOPER",
		name = "Developer App",
		id_help = "Devtool Server URL",
		token_help = "Credential Name"
	},
	DISCORD = {
		type = "DISCORD",
		name = "Discord",
		id_help = "",
		token_help = "Discord OAuth2 Token"
	},
	DEVICE_ID = {
		type = "DEVICE_ID",
		name = "Device Id",
		id_help = "Display Name",
		token_help = ""
	},
	ACCOUNT_PORTAL = {
		type = "ACCOUNT_PORTAL",
		name = "Account Portal",
		id_help = "",
		token_help = ""
	},
	PERSISTENT_AUTH = {
		type = "PERSISTENT_AUTH",
		name = "Persistent Auth",
		id_help = "",
		token_help = ""
	}
}

#endregion


#region Private vars

var _state = States.ChooseMethod

#endregion


#region Onready vars

@onready var login_type_btn = $HB/Right/ChooseMethod/LoginTypeButton
@onready var login_btn = $HB/Right/LoginButton
@onready var back_btn = $HB/Right/BackButton
@onready var retry_login_btn = $HB/Right/RetryLoginButton
@onready var logout_btn = $HB/Right/LogoutButton

# Sub Views
@onready var choose_method = $HB/Right/ChooseMethod
@onready var enter_credentials = $HB/Right/EnterCredentials
@onready var login_status = $HB/Right/LoginStatus

#endregion


#region Built-in methods

func _ready() -> void:
	_populate_login_type_dropdown()
	
	enter_credentials.perform_login.connect(_on_login_btn_pressed)
	login_btn.pressed.connect(_on_login_btn_pressed)
	back_btn.pressed.connect(_on_back_btn_pressed)
	retry_login_btn.pressed.connect(_on_retry_login_btn_pressed)
	logout_btn.pressed.connect(_on_logout_btn_pressed)

	HAuth.logged_in.connect(_on_logged_in)
	HAuth.display_name_changed.connect(_on_display_name_changed)
	HAuth.login_error.connect(_on_login_error)
	HAuth.logged_out.connect(_on_logged_out)

	await HPlatform.platform_created
	# HAuth.login_account_portal_async()
	# HAuth.login_devtool_async("localhost:4545", "3ddelano")
	
	# TODO: Remove Autologin (PersistentAuth) for debug purpose

	# var cli_args = OS.get_cmdline_user_args()
	# if cli_args.size() > 0:
	# 	var dev_tool_username = cli_args[0]
	# 	_set_login_status("Logging in with devtool using cli arg...")
	# 	_set_login_state(States.Pending)
	# 	HAuth.login_devtool_async("localhost:4545", dev_tool_username)


	# For developement
	# _set_login_status("Logging in...")
	# _set_login_state(States.Pending)
	# HAuth.login_persistent_auth_async()

#endregion


#region Private methods

func _on_logged_in():
	_set_login_state(States.Success)


func _on_display_name_changed():
	print("User display name: ", HAuth.display_name)


func _on_logged_out():
	_set_login_status("Logout successful")
	_set_login_state(States.ChooseMethod)


func _on_login_error(result_code: EOS.Result):
	_set_login_state(States.Error)
	_set_login_status("Login Error: " + EOS.result_str(result_code))


func _on_retry_login_btn_pressed():
	_set_login_state(States.ChooseMethod)


func _on_back_btn_pressed():
	_set_login_state(States.ChooseMethod)


func _on_login_btn_pressed():
	var login_type_data = login_type_btn.get_selected_metadata()
	var login_type = login_type_data.type
	var login_id = enter_credentials.get_id_value()
	var login_token = enter_credentials.get_token_value()

	if _state == States.ChooseMethod:
		# In ChooseMethod state
		if login_type_data.id_help or login_type_data.token_help:
			# User needs to enter some data
			enter_credentials.set_helper_texts(login_type_data.id_help, login_type_data.token_help)
			_set_login_state(States.EnterCredentials)

			if login_type == "DISCORD":
				OS.shell_open("https://discord.com/api/oauth2/authorize?client_id=959047632091762719&redirect_uri=http%3A%2F%2Flocalhost%3A53134&response_type=token&scope=identify%20email")
			elif login_type == "DEVELOPER":
				enter_credentials.set_id_value("localhost:4545")
		else:
			# No data entry is required
			login_status.text = "Logging in..."
			_set_login_state(States.Pending)
			match login_type:
				"PERSISTENT_AUTH":
					HAuth.login_persistent_auth_async()
				"ACCOUNT_PORTAL":
					HAuth.login_account_portal_async()
	else:
		login_status.text = "Logging in..."
		_set_login_state(States.Pending)
		# In EnterCredentials state
		match login_type:
			"DEVICE_ID":
				HAuth.login_anonymous_async(login_id)
			"DISCORD":
				_login_game_services(EOS.ExternalCredentialType.DiscordAccessToken, login_token)
			"DEVELOPER":
				HAuth.login_devtool_async(login_id, login_token)


func _on_logout_btn_pressed():
	login_status.text = "Logout pending..."
	_set_login_state(States.Pending)
	HAuth.logout_async()


func _set_login_state(new_state: States):
	_state = new_state

	for child in $HB/Right.get_children():
		child.visible = false

	if _state == States.ChooseMethod:
		choose_method.visible = true
		login_btn.visible = true

	if _state == States.Error:
		login_status.visible = true
		retry_login_btn.visible = true

	if _state == States.EnterCredentials:
		enter_credentials.visible = true
		back_btn.visible = true
		login_btn.visible = true

	if _state == States.Pending:

		login_status.visible = true

	if _state == States.Success:
		login_status.text = "Login success!"
		login_status.visible = true
		logout_btn.visible = true


func _login_game_services(type: int, token = null, display_name = null):
	_set_login_status("Logging in to Epic Games Services...")

	var login_options = EOS.Connect.LoginOptions.new()
	login_options.credentials = EOS.Connect.Credentials.new()
	login_options.credentials.type = type
	login_options.credentials.token = token
	if display_name:
		login_options.user_login_info = EOS.Connect.UserLoginInfo.new()
		login_options.user_login_info.display_name = display_name
	HAuth.login_game_services_async(login_options)


func _set_login_status(new_login_status: String):
	login_status.text = new_login_status


func _populate_login_type_dropdown():
	var idx = 0
	for login_type in LOGIN_TYPES.keys():
		login_type_btn.add_item(LOGIN_TYPES[login_type].name)
		login_type_btn.set_item_metadata(idx, LOGIN_TYPES[login_type])
		idx += 1

#endregion

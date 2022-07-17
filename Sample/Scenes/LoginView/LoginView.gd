class_name LoginView
extends CenterContainer

onready var login_type_btn = $HB/Right/ChooseMethod/LoginTypeButton
onready var login_btn = $HB/Right/LoginButton
onready var back_btn = $HB/Right/BackButton
onready var retry_login_btn = $HB/Right/RetryLoginButton
onready var logout_btn = $HB/Right/LogoutButton

# Sub Views
onready var choose_method = $HB/Right/ChooseMethod
onready var enter_credentials = $HB/Right/EnterCredentials
onready var login_status = $HB/Right/LoginStatus

enum States {
	ChooseMethod,
	EnterCredentials,
	Pending,
	Success,
	Error
}

var _state = States.ChooseMethod


const login_types = {
	DEVELOPER = {
		name = "Developer App",
		id_help = "Devtool Server URL",
		token_help = "Credential Name"
	},
	DISCORD = {
		name = "Discord",
		id_help = "",
		token_help = "Discord OAuth2 Token"
	},
	DEVICE_ID = {
		name = "Device Id",
		id_help = "Display Name",
		token_help = ""
	},
	ACCOUNT_PORTAL = {
		name = "Account Portal",
		id_help = "",
		token_help = ""
	},
	PERSISTENT_AUTH = {
		name = "Persistent Auth",
		id_help = "",
		token_help = ""
	}
}

func _ready() -> void:
	# Generate login type dropdown
	var c = 0
	for login_type in login_types.keys():
		login_type_btn.add_item(login_types[login_type].name)
		login_types[login_type].type = login_type
		login_type_btn.set_item_metadata(c, login_types[login_type])
		c += 1


	login_btn.connect("pressed", self, "_on_login_btn_pressed")
	back_btn.connect("pressed", self, "_on_back_btn_pressed")
	retry_login_btn.connect("pressed", self, "_on_retry_login_btn_pressed")
	logout_btn.connect("pressed", self, "_on_logout_btn_pressed")

	# Auth Interface callbacks
	var instance = EOS.get_instance()
	c = instance.connect("auth_interface_login_callback", self, "_on_auth_interface_login_callback")
	c = instance.connect("auth_interface_logout_callback", self, "_on_auth_interface_logout_callback")
	# Connect Interface callbacks
	c = instance.connect("connect_interface_login_callback", self, "_on_connect_interface_login_callback")
	c = instance.connect("connect_interface_login_status_changed", self, "_on_connect_interface_login_status_changed")
	c = instance.connect("user_info_interface_query_user_info_callback", self, "_on_query_user_info_callback")

	# TODO: (Remove) Autologin for debug purpose
	if OS.is_debug_build():
		yield(Store, "platform_create")
		perform_auth_login(EOS.Auth.LoginCredentialType.PersistentAuth)


func _on_auth_interface_login_callback(data: Dictionary):
	print("--- Auth: login_callback: ", EOS.print_result(data))

	if data.pending:
		print("--- Auth: Login Pending...")

	if data.local_user_id != null and data.local_user_id != "":
		Store.epic_account_id = data.local_user_id
		print("Epic Account Id: ", Store.epic_account_id)

		var copy_user_auth_token = EOS.Auth.AuthInterface.copy_user_auth_token(EOS.Auth.CopyUserAuthTokenOptions.new(), Store.epic_account_id)
		var token = copy_user_auth_token.token

		# Get user info of logged in user
		var options = EOS.UserInfo.QueryUserInfoOptions.new()
		options.local_user_id = Store.epic_account_id
		options.target_user_id = Store.epic_account_id
		EOS.UserInfo.UserInfoInterface.query_user_info(options)
		# Connect the account to get a Product User Id from the Epic Account Id
		connect_account(EOS.ExternalCredentialType.Epic, token.access_token)

	if data.pin_grant_info != null:
		var pin_grant_info = data.pin_grant_info
		Store.get_view("Logs").log_msg(EOS.Logging.LogLevel.Fatal, "Complete logging in by visiting the link before %s seconds [url]%s[/url]" % [pin_grant_info.expires_in, pin_grant_info.verification_uri_complete])
		var _c = OS.shell_open(pin_grant_info.verification_uri_complete)

	if data.continuance_token != null:
		print("--- Auth: Got continuance_token, use it to link an account")
		print(data.continuance_token)

	if data.account_feature_restricted_info != null:
		var account_feature_restricted_info = data.account_feature_restricted_info
		Store.get_view("Logs").log_msg(EOS.Logging.LogLevel.Fatal, "Complete logging in by visiting the link [url]%s[/url]" % account_feature_restricted_info.verification_uri_complete)
		var _c = OS.shell_open(account_feature_restricted_info.verification_uri_complete)

	if not (data.pending or data.success):
		print("--- Auth: Login Failed: ", data)
		set_login_state(States.Error)
		set_login_status("Login Error: " + EOS.print_result(data.result_code))


func _on_auth_interface_logout_callback(data: Dictionary):
	print("--- Auth: logout_callback: ", EOS.print_result(data))
	Store.epic_account_id = ""
	Store.product_user_id = ""
	set_login_state(States.ChooseMethod)
	Store.emit_signal("logout_success")


func _on_query_user_info_callback(data: Dictionary):
	print("--- LoginView: UserInfo: query_user_info_callback: ", EOS.print_result(data))
	if data.client_data != "login_view":
		# Not the callback for the LoginView
		return

	var copy_user_info_options = EOS.UserInfo.CopyUserInfoOptions.new()
	copy_user_info_options.local_user_id = Store.epic_account_id
	copy_user_info_options.target_user_id = Store.epic_account_id
	var user_info_data = EOS.UserInfo.UserInfoInterface.copy_user_info(copy_user_info_options)
	if user_info_data.result_code == EOS.Result.Success:
		if user_info_data.user_info.display_name != null:
			Store.display_name = user_info_data.user_info.display_name


func _on_connect_interface_login_status_changed(data: Dictionary):
	print("--- Connect: login_status_changed: ", data)


func _on_connect_interface_login_callback(data: Dictionary):
	print("--- Connect: login_callback: ", EOS.print_result(data))

	if data.continuance_token:
		print("--- Connect: Got continuance_token, creating a user...")
		var ctw = data.continuance_token
		# Create user
		var create_user_options = EOS.Connect.CreateUserOptions.new()
		create_user_options.continuance_token = ctw
		var _c = EOS.Connect.ConnectInterface.create_user(create_user_options)
		var ret = yield(EOS.get_instance(), "connect_interface_create_user_callback")
		print("--- Connect: create_user_callback: ", ret)

	if not data.success:
		print("--- Connect: Login Failed: ", data)
		set_login_state(States.Error)
		set_login_status("Login Error: " + EOS.print_result(data.result_code))
		return

	if data.local_user_id != null and data.local_user_id != "":
		Store.product_user_id = data.local_user_id
		print("Product User Id: ", Store.product_user_id)
		set_login_status("Connected account successfully")
		set_login_state(States.Success)

		Store.emit_signal("login_success")


func _on_retry_login_btn_pressed():
	set_login_state(States.ChooseMethod)


func _on_back_btn_pressed():
	set_login_state(States.ChooseMethod)


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
			set_login_state(States.EnterCredentials)

			if login_type == "DISCORD":
				var _c = OS.shell_open("https://discord.com/api/oauth2/authorize?client_id=959047632091762719&redirect_uri=http%3A%2F%2Flocalhost%3A53134&response_type=token&scope=identify%20email")
		else:
			# No data entry is required
			match login_type:
				"PERSISTENT_AUTH":
					perform_auth_login(EOS.Auth.LoginCredentialType.PersistentAuth)
				"ACCOUNT_PORTAL":
					perform_auth_login(EOS.Auth.LoginCredentialType.AccountPortal)
	else:
		# In EnterCredentials state
		match login_type:
			"DEVICE_ID":
				EOS.Auth.AuthInterface.delete_persistent_auth(EOS.Auth.DeletePersistentAuthOptions.new())
				yield(EOS.get_instance(), "auth_interface_delete_persistent_auth_callback")
				var options1 = EOS.Connect.CreateDeviceIdOptions.new()
				options1.device_model = PoolStringArray([OS.get_name(), OS.get_model_name()]).join(" ")
				var _c = EOS.Connect.ConnectInterface.create_device_id(options1)
				yield(EOS.get_instance(), "connect_interface_create_device_id_callback")
				connect_account(EOS.ExternalCredentialType.DeviceidAccessToken, null, login_id)
			"DISCORD":
				connect_account(EOS.ExternalCredentialType.DiscordAccessToken, login_token)
			"DEVELOPER":
				perform_auth_login(EOS.Auth.LoginCredentialType.Developer, login_id, login_token)


func _on_logout_btn_pressed():
	var logout_options = EOS.Auth.LogoutOptions.new()
	logout_options.local_user_id = Store.epic_account_id
	login_status.text = "Logout pending..."
	set_login_state(States.Pending)
	EOS.Auth.AuthInterface.logout(logout_options)


func set_login_state(new_state: int):
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
		login_status.text = "Login sucess!"
		login_status.visible = true
		logout_btn.visible = true


func perform_auth_login(type: int, id = null, token = null, external_type = null):
	login_status.text = "Login pending..."
	set_login_state(States.Pending)

	var credentials = EOS.Auth.Credentials.new()
	credentials.type = type
	credentials.id = id
	credentials.token = token
	if external_type != null:
		credentials.external_type = external_type

	var login_options = EOS.Auth.LoginOptions.new()
	login_options.credentials = credentials
	login_options.scope_flags = EOS.Auth.ScopeFlags.BasicProfile | EOS.Auth.ScopeFlags.Presence | EOS.Auth.ScopeFlags.FriendsList
	EOS.Auth.AuthInterface.login(login_options)


func connect_account(type: int, token = null, display_name = null):
	set_login_status("Connecting your account...")

	var credentials = EOS.Connect.Credentials.new()
	credentials.token = token
	credentials.type = type

	var login_options = EOS.Connect.LoginOptions.new()
	login_options.credentials = credentials
	if display_name:
		login_options.user_login_info = EOS.Connect.UserLoginInfo.new()
		login_options.user_login_info.display_name = display_name

	EOS.Connect.ConnectInterface.login(login_options)


func set_login_status(new_login_status: String):
	login_status.text = new_login_status

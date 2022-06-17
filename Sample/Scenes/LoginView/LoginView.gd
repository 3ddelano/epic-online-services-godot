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
		login_type_btn.add_item(login_types[login_type]["name"])
		login_types[login_type]["type"] = login_type
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
	#c = instance.connect("auth_interface_login_status_changed", self, "_on_auth_interface_login_status_changed")
	# Connect Interface callbacks
	c = instance.connect("connect_interface_login_callback", self, "_on_connect_interface_login_callback")
	c = instance.connect("connect_interface_login_status_changed", self, "_on_connect_interface_login_status_changed")

	# TODO: (Remove) Autologin for debug purpose
#	yield(Store, "platform_create")
#	perform_auth_login(EOS.Auth.LoginCredentialType.PersistentAuth)
	pass


func _on_auth_interface_login_callback(data: Dictionary):
	print("--- Auth: Login Callback: ", data)

	if data.pending:
		print("--- Auth: Login Pending...")
	elif data.success:
		print("--- Auth: Login Success")

	if data.has("local_user_id"):
		Store.epic_account_id = data["local_user_id"]
		print("Epic Account Id: ", Store.epic_account_id)

		var copy_user_auth_token = EOS.Auth.AuthInterface.copy_user_auth_token(EOS.Auth.CopyUserAuthTokenOptions.new(), Store.epic_account_id)
		var user_auth_token = copy_user_auth_token.user_auth_token
		connect_account(EOS.ExternalCredentialType.Epic, user_auth_token.access_token)

		#print("Logged in accounts count: ", EOS.Auth.AuthInterface.get_logged_in_accounts_count())
		#print("Logged in account by index(0): ", EOS.Auth.AuthInterface.get_logged_in_account_by_index(0))
		#print("Get login status: ", EOS.Auth.AuthInterface.get_login_status(Store.epic_account_id))
		#print("Merged accounts count: ", EOS.Auth.AuthInterface.get_merged_accounts_count(Store.epic_account_id))
		#print("Get merged account by index(0): ", EOS.Auth.AuthInterface.get_merged_account_by_index(Store.epic_account_id, 0))
		#print("Selected account id: ", EOS.Auth.AuthInterface.get_selected_account_id(Store.epic_account_id))

		#var verify_user_auth_options = EOS.Auth.VerifyUserAuthOptions.new()
		#verify_user_auth_options.auth_token = user_auth_token
		#EOS.Auth.AuthInterface.verify_user_auth(verify_user_auth_options)

		#print("Active country code: ", EOS.Platform.PlatformInterface.get_active_country_code(Store.epic_account_id))
		#print("Active local code: ", EOS.Platform.PlatformInterface.get_active_locale_code(Store.epic_account_id))
		#print("Override country code: ", EOS.Platform.PlatformInterface.get_override_country_code())
		#print("Override locale code: ", EOS.Platform.PlatformInterface.get_override_locale_code())

		#var copy_id_token_options = EOS.Auth.CopyIdTokenOptions.new()
		#copy_id_token_options.account_id = Store.epic_account_id
		#print("Copy id token: ", EOS.print_result(EOS.Auth.AuthInterface.copy_id_token(copy_id_token_options).result_code))

	if data.has("pin_grant_info"):
		var pin_grant_info = data["pin_grant_info"]
		Store.get_view("Logs").log_msg(EOS.Logging.LogLevel.Fatal, "Complete logging in by visiting the link before %s seconds [url]%s[/url]" % [pin_grant_info["expires_in"], pin_grant_info["verification_uri_complete"]])
		var _c = OS.shell_open(pin_grant_info["verification_uri_complete"])

	if data.has("continuance_token"):
		print("--- Auth: Got continuance token, use it to link an account")
		print(data["continuance_token"])

	if data.has("account_feature_restricted_info"):
		var account_feature_restricted_info = data["account_feature_restricted_info"]
		Store.get_view("Logs").log_msg(EOS.Logging.LogLevel.Fatal, "Complete logging in by visiting the link [url]%s[/url]" % account_feature_restricted_info["verification_uri_complete"])
		var _c = OS.shell_open(account_feature_restricted_info["verification_uri_complete"])

	if not (data.pending or data.success):
		print("--- Auth: Login Failed:: ", data)
		set_login_state(States.Error)
		set_login_status("Login Error: " + EOS.print_result(data.result_code))


func _on_auth_interface_logout_callback(data: Dictionary):
	print("--- Auth: Logout Callback:: ", data)
	Store.epic_account_id = ""
	Store.product_user_id = ""
	set_login_state(States.ChooseMethod)
	Store.emit_signal("logout_success")


func _on_connect_interface_login_status_changed(data: Dictionary):
	print("--- Connect: Login Status Changed:: ", data)


func _on_connect_interface_login_callback(data: Dictionary):
	print("--- Connect: Login Callback:: ", data)

	if data.has("continuance_token"):
		print("Got continuance_token")
		var ctw = data["continuance_token"]
		# Create user
		var create_user_options = EOS.Connect.CreateUserOptions.new()
		create_user_options.continuance_token = ctw
		var _c = EOS.Connect.ConnectInterface.create_user(create_user_options)
		var ret = yield(EOS.get_instance(), "connect_interface_create_user_callback")
		print("create user callback: ", ret)

	if not data.success:
		print("--- Connect: Login Failed:: ", data)
		set_login_state(States.Error)
		set_login_status("Login Error: " + EOS.print_result(data.result_code))
		return

	if data.has("local_user_id"):
		Store.product_user_id = data.local_user_id
		print("Product User Id: ", Store.product_user_id)
		set_login_status("Connected account successfully")
		set_login_state(States.Success)

		Store.emit_signal("login_success")

	#var options1 = EOS.Connect.CopyProductUserExternalAccountByAccountIdOptions.new()
	#options1.target_user_id = Store.product_user_id
	#options1.account_id = Store.product_user_id
	#print("By account Id: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_id(options1))

	#var options2 = EOS.Connect.CopyProductUserExternalAccountByAccountTypeOptions.new()
	#options2.target_user_id = Store.product_user_id
	#options2.account_id_type = EOS.ExternalAccountType.Epic
	#print("By account type: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_type(options2))

	#var options3 = EOS.Connect.CopyProductUserExternalAccountByIndexOptions.new()
	#options3.target_user_id = Store.product_user_id
	#options3.external_account_info_index = 0
	#print("By account index: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_index(options3))

	#var options4 = EOS.Connect.CopyProductUserInfoOptions.new()
	#options4.target_user_id = Store.product_user_id
	#print("User info: ", EOS.Connect.ConnectInterface.copy_product_user_info(options4))

#	var options5 = EOS.Connect.CopyIdTokenOptions.new()
#	options5.local_user_id = Store.product_user_id
#	var ret5 = EOS.Connect.ConnectInterface.copy_id_token(options5)
#	var id_token = EOS.Connect.IdToken.new()
#	id_token.json_web_token = ret5["json_web_token"]
#	id_token.product_user_id = ret5["product_user_id"]
#	print("Id token: ", id_token)

	#var options6 = EOS.Connect.GetExternalAccountMappingsOptions.new()
	#options6.local_user_id = Store.product_user_id
	#options6.account_id_type = EOS.ExternalAccountType.Discord
	#options6.target_external_user_id  = "321233875776962560"
	#print("External account mapping: ", EOS.Connect.ConnectInterface.get_external_account_mapping(options6))

	#print("Logged in user 0: ", EOS.Connect.ConnectInterface.get_logged_in_user_by_index(0))
	#print("Logged in users count: ", EOS.Connect.ConnectInterface.get_logged_in_users_count())
	#print("Login status: ", EOS.Connect.ConnectInterface.get_login_status(Store.product_user_id))

	#var options7 = EOS.Connect.GetProductUserExternalAccountCountOptions.new()
	#options7.target_user_id = Store.product_user_id
	#print("External account count: ", EOS.Connect.ConnectInterface.get_product_user_external_account_count(options7))

#	var options8 = EOS.Connect.GetProductUserIdMappingOptions.new()
#	options8.local_user_id = Store.product_user_id
#	options8.account_id_type = EOS.ExternalAccountType.Discord
#	options8.target_product_user_id  = "321233875776962560"
#	print("Product user id mapping: ", EOS.Connect.ConnectInterface.get_product_user_id_mapping(options8))

#	var options9 = EOS.Connect.LinkAccountOptions.new()
#	options9.local_user_id = Store.product_user_id
#	options9.continuance_token = ContinunanceTokenWrapperHere
#	EOS.Connect.ConnectInterface.link_account(options9)
#	print(yield(EOS.get_instance(), "connect_interface_link_account_callback"))

#	var options10 = EOS.Connect.VerifyIdTokenOptions.new()
#	options10.id_token = id_token
#	print(options10)
#	EOS.Connect.ConnectInterface.verify_id_token(options10)
#	print("Verify Id Token: ", yield(EOS.get_instance(), "connect_interface_verify_id_token_callback"))
	pass


func _on_retry_login_btn_pressed():
	set_login_state(States.ChooseMethod)


func _on_back_btn_pressed():
	set_login_state(States.ChooseMethod)


func _on_login_btn_pressed():
	var login_type_data = login_type_btn.get_selected_metadata()
	var login_type = login_type_data["type"]
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

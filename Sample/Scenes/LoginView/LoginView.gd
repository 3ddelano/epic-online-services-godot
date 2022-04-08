class_name LoginView
extends CenterContainer

onready var login_type_btn = $HB/Right/ChooseMethod/LoginTypeButton
onready var login_btn = $HB/Right/LoginButton
onready var retry_login_btn = $HB/Right/RetryLoginButton
onready var logout_btn = $HB/Right/LogoutButton

# Sub Views
onready var choose_method = $HB/Right/ChooseMethod
onready var dev_login = $HB/Right/DeveloperLogin
onready var login_status = $HB/Right/LoginStatus

enum States {
	ChooseMethod,
	EnterCredentials,
	Pending,
	Success,
	Error
}


var _state = States.ChooseMethod


func _ready() -> void:
	var login_types = [
		{
			name = "Developer App",
			type = EOS.Auth.LoginCredentialType.Developer
		},
		{
			name = "Account Portal",
			type = EOS.Auth.LoginCredentialType.AccountPortal
		},
		{
			name = "Device Code",
			type = EOS.Auth.LoginCredentialType.DeviceCode
		},
		{
			name = "Persistent Auth",
			type = EOS.Auth.LoginCredentialType.PersistentAuth
		}
	]

	for i in login_types.size():
		var login_type = login_types[i]
		login_type_btn.add_item(login_type.name)
		login_type_btn.set_item_metadata(i, login_type.type)

	login_btn.connect("pressed", self, "_on_login_btn_pressed")
	retry_login_btn.connect("pressed", self, "_on_retry_login_btn_pressed")
	logout_btn.connect("pressed", self, "_on_logout_btn_pressed")

	# Auth callbacks
	var _c = EOS.get_instance().connect("auth_interface_login_callback", self, "_on_auth_interface_login_callback")
	_c = EOS.get_instance().connect("auth_interface_logout_callback", self, "_on_auth_interface_logout_callback")
	_c = EOS.get_instance().connect("auth_interface_login_status_changed", self, "_on_auth_interface_login_status_changed")
	_c = EOS.get_instance().connect("auth_interface_delete_persistent_auth_callback", self, "_on_auth_interface_delete_persistent_auth_callback")
	_c = EOS.get_instance().connect("auth_interface_query_id_token_callback", self, "_on_auth_interface_query_id_token_callback")
	_c = EOS.get_instance().connect("auth_interface_verify_user_auth_callback", self, "_on_auth_interface_verify_user_auth_callback")
	# Connect callbacks
	_c = EOS.get_instance().connect("connect_interface_login_callback", self, "_on_connect_interface_login_callback")
	_c = EOS.get_instance().connect("connect_interface_login_status_changed", self, "_on_connect_interface_login_status_changed")

	# TODO: (Remove) Autologin for debug purpose
	# yield(State, "platform_create")
	# perform_login(EOS.Auth.LoginCredentialType.PersistentAuth, null, null)


func _on_auth_interface_login_callback(data: Dictionary):
	print("--- Auth: Login Callback")
	print(data)

	if data.pending:
		print("Auth login pending...")
	if data.success:
		print("Auth login success")
	if data.has("local_user_id"):
		State.epic_account_id = data["local_user_id"]
		print("Epic Account Id: ", State.epic_account_id)

		var copy_user_auth_token_options = EOS.Auth.CopyUserAuthTokenOptions.new()
		var copy_user_auth_token = EOS.Auth.AuthInterface.copy_user_auth_token(copy_user_auth_token_options, State.epic_account_id)

		var user_auth_token = copy_user_auth_token.user_auth_token
		var credentials = EOS.Connect.Credentials.new()
		credentials.token = user_auth_token.access_token
		credentials.type = EOS.ExternalCredentialType.Epic

		set_login_status("Connecting your account...")

		var login_options = EOS.Connect.LoginOptions.new()
		login_options.credentials = credentials
		EOS.Connect.ConnectInterface.login(login_options)

		print("Logged in accounts count: ", EOS.Auth.AuthInterface.get_logged_in_accounts_count())
		print("Logged in account by index(0): ", EOS.Auth.AuthInterface.get_logged_in_account_by_index(0))
		print("Get login status: ", EOS.Auth.AuthInterface.get_login_status(State.epic_account_id))

		print("Merged accounts count: ", EOS.Auth.AuthInterface.get_merged_accounts_count(State.epic_account_id))
		print("Get merged account by index(0): ", EOS.Auth.AuthInterface.get_merged_account_by_index(State.epic_account_id, 0))

		print("Selected account id: ", EOS.Auth.AuthInterface.get_selected_account_id(State.epic_account_id))

		var verify_user_auth_options = EOS.Auth.VerifyUserAuthOptions.new()
		verify_user_auth_options.auth_token = user_auth_token
		# verify_user_auth_options.client_data = Color.red
		EOS.Auth.AuthInterface.verify_user_auth(verify_user_auth_options)

		print("Active country code: ", EOS.Platform.PlatformInterface.get_active_country_code(State.epic_account_id))
		print("Active local code: ", EOS.Platform.PlatformInterface.get_active_locale_code(State.epic_account_id))
		print("Override country code: ", EOS.Platform.PlatformInterface.get_override_country_code())
		print("Override locale code: ", EOS.Platform.PlatformInterface.get_override_locale_code())

		var copy_id_token_options = EOS.Auth.CopyIdTokenOptions.new()
		copy_id_token_options.account_id = State.epic_account_id
		print("Copy id token: ", EOS.print_result(EOS.Auth.AuthInterface.copy_id_token(copy_id_token_options).result_code))

		# var delete_persistent_auth_options = EOS.Auth.DeletePersistentAuthOptions.new()
		# delete_persistent_auth_options.client_data = Vector2(2, 2)
		# EOS.Auth.AuthInterface.delete_persistent_auth(delete_persistent_auth_options)

		# var logout_options = EOS.Auth.LogoutOptions.new()
		# logout_options.local_user_id = State.epic_account_id
		# logout_options.client_data = "logout data here"
		# EOS.Auth.AuthInterface.logout(logout_options)

	if data.has("pin_grant_info"):
		var pin_grant_info = data["pin_grant_info"]
		State.get_view("Logs").log_msg(EOS.Logging.LogLevel.Fatal, "Complete logging in by visiting the link before %s seconds [url]%s[/url]" % [pin_grant_info["expires_in"], pin_grant_info["verification_uri_complete"]])
		var _c = OS.shell_open(pin_grant_info["verification_uri_complete"])

	if data.has("continuance_token"):
		print("Got continuance token, use it to link an account")
		print(data["continuance_token"])

	if data.has("account_feature_restricted_info"):
		var account_feature_restricted_info = data["account_feature_restricted_info"]
		State.get_view("Logs").log_msg(EOS.Logging.LogLevel.Fatal, "Complete logging in by visiting the link [url]%s[/url]" % account_feature_restricted_info["verification_uri_complete"])
		var _c = OS.shell_open(account_feature_restricted_info["verification_uri_complete"])

	if not (data.pending or data.success):
		print("Auth login failed")
		set_login_state(States.Error)
		set_login_status("Login Error: " + EOS.print_result(data.result_code))
		print(data)


func _on_auth_interface_logout_callback(data: Dictionary):
	print("--- Auth: Logout Callback ")
	print(data)

	State.epic_account_id = ""
	State.product_user_id = ""
	set_login_state(States.ChooseMethod)
	State.emit_signal("logout_success")


func _on_auth_interface_delete_persistent_auth_callback(data: Dictionary):
	print("--- Auth: Delete Persistent Auth Callback")
	print(data)


func _on_auth_interface_login_status_changed(data: Dictionary):
	print("--- Auth: Login Status Changed")
	print(data)


func _on_auth_interface_query_id_token_callback(data: Dictionary):
	print("--- Auth: Query Id Token Callback")
	print(data)


func _on_auth_interface_verify_user_auth_callback(data: Dictionary):
	print("--- Auth: Verify User Auth Callback")
	print(data)


func _on_connect_interface_login_status_changed(data: Dictionary):
	print("--- Connect: Login Status Changed")
	print(data)


func _on_connect_interface_login_callback(data: Dictionary):
	print("--- Connect: Login Callback")

	State.product_user_id = data.local_user_id
	print("Product User Id: ", State.product_user_id)
	set_login_status("Connected account successfully")
	set_login_state(States.Success)
	State.emit_signal("login_success")

#	var query_options = EOS.Achievements.QueryDefinitionsOptions.new()
#	query_options.local_user_id = State.product_user_id
#	query_options.client_data = ["arbitary data"]
#	EOS.Achievements.AchievementsInterface.query_definitions(query_options)

#	var copy_id_token_options = EOS.Connect.CopyIdTokenOptions.new()
#	copy_id_token_options.local_user_id = State.product_user_id
#	print(EOS.Connect.ConnectInterface.copy_id_token(copy_id_token_options))

#	var copy_product_user_external_account_by_account_id_options = EOS.Connect.CopyProductUserExternalAccountByAccountIdOptions.new()
#	copy_product_user_external_account_by_account_id_options.target_user_id = State.product_user_id
#	copy_product_user_external_account_by_account_id_options.account_id = logged_in_user_id
#	print(EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_id(copy_product_user_external_account_by_account_id_options))

#	var copy_product_user_external_account_by_account_type_options = EOS.Connect.CopyProductUserExternalAccountByAccountTypeOptions.new()
#	copy_product_user_external_account_by_account_type_options.target_user_id = State.product_user_id
#	copy_product_user_external_account_by_account_type_options.account_id_type = EOS.ExternalAccountType.Epic
#	print(EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_type(copy_product_user_external_account_by_account_type_options))

#	var copy_product_user_external_account_by_index_options = EOS.Connect.CopyProductUserExternalAccountByIndexOptions.new()
#	copy_product_user_external_account_by_index_options.target_user_id = State.product_user_id
#	copy_product_user_external_account_by_index_options.external_account_info_index = 0
#	print(EOS.Connect.ConnectInterface.copy_product_user_external_account_by_index(copy_product_user_external_account_by_index_options))

#	var copy_product_user_info_options = EOS.Connect.CopyProductUserInfoOptions.new()
#	copy_product_user_info_options.target_user_id = State.product_user_id
#	print(EOS.Connect.ConnectInterface.copy_product_user_info(copy_product_user_info_options))


func _on_retry_login_btn_pressed():
	set_login_state(States.ChooseMethod)


func _on_login_btn_pressed():
	var credential_type = login_type_btn.get_selected_metadata()
	if _state == States.ChooseMethod:
		# Login button was clicked after choosing login method
		match credential_type:
			EOS.Auth.LoginCredentialType.Developer:
				# Show enter credentials
				set_login_state(States.EnterCredentials)
			_:
				perform_login(credential_type, null, null)
	elif _state == States.EnterCredentials:
		# Login button was clicked after entering the credentials
		perform_login(credential_type, dev_login.get_id_value(), dev_login.get_token_value())


func _on_logout_btn_pressed():
	var logout_options = EOS.Auth.LogoutOptions.new()
	logout_options.local_user_id = State.epic_account_id
	EOS.Auth.AuthInterface.logout(logout_options)
	login_status.text = "Logout pending..."
	set_login_state(States.Pending)


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
		print("Showing enter credentials for ", EOS.Auth.LoginCredentialType.keys()[login_type_btn.get_selected_metadata()])
		dev_login.visible = true
		login_btn.visible = true

	if _state == States.Pending:

		login_status.visible = true

	if _state == States.Success:
		login_status.text = "Login sucess!"
		login_status.visible = true
		logout_btn.visible = true


func perform_login(type, id = "", token = ""):
	login_status.text = "Login pending..."
	set_login_state(States.Pending)

	var credentials = EOS.Auth.Credentials.new()
	credentials.type = type
	credentials.id = id
	credentials.token = token

	var login_options = EOS.Auth.LoginOptions.new()
	login_options.credentials = credentials
	login_options.scope_flags = EOS.Auth.ScopeFlags.BasicProfile | EOS.Auth.ScopeFlags.Presence | EOS.Auth.ScopeFlags.FriendsList
	# Arbitary data to get back once the callback completes
	# login_options.client_data = {
	# 	"from": "Godot Engine"
	# }
	EOS.Auth.AuthInterface.login(login_options)


func set_login_status(new_login_status: String):
	login_status.text = new_login_status

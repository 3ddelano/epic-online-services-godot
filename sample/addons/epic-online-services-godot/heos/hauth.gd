extends Node

#region Signals

## Emitted when the user logs in to Epic Game Services. If the [auto_connect_account] option is false, this will also be emitted when the user logs in to Epic Account Services
signal logged_in

## Emitted when the user logs out of Epic Game Services
signal logged_out

## Emitted when an error occurs while loggin in
signal login_error(result_code: EOS.Result)

# Epic Account Services
## Related to Epic Account Services
signal logged_in_auth()

## Related to Epic Account Services
signal logged_out_auth()

## Related to Epic Account Services
signal login_auth_error(result_code: EOS.Result)

# Epic Game Services
## Related to Epic Game Services
signal logged_in_connect()

## Related to Epic Game Services
signal logged_out_connect()

## Related to Epic Game Services
signal login_connect_error(result_code: EOS.Result)

#endregion


#region Public vars

## The epic account id of the logged in user (Used for Epic Account Services)
var epic_account_id = ""

## The product user id of the logged in user (Used for Epic Game Services)
var product_user_id = ""

## Whether to automatically link an epic account for external identity provider (default true)
var auto_link_account = true

## Whether to automatically login to Epic Game Services after logging in to Epic Account Services (default true)
var auto_connect_account = true

#endregion


#region Private vars 

var _log = HLog.logger("HAuth")

#endregion


#region Built-in methods

func _ready() -> void:
	IEOS.connect_interface_auth_expiration.connect(_on_connect_interface_auth_expiration)

#endregion


#region Public methods

## Login using EOS Auth by either using an Identity provider or Epic games Account.
## Allows you to use Epic Account Services: Friends, Presence, Social Overlay, ECom, etc.
## This is the recommended way of logging in as you get many additional features compared to [login_game_services_async]
func login_async(opts: EOS.Auth.LoginOptions) -> bool:
	_log.debug("Logging into Epic Account Services (AuthInterface)")
	EOS.Auth.AuthInterface.login(opts)

	var auth_login_ret: Dictionary = await IEOS.auth_interface_login_callback
	var auth_res: EOS.Result = auth_login_ret.result_code
	
	if auth_res == EOS.Result.AuthMFARequired:
		_log.error("Auth requires MFA - This is not supported by the EOS SDK. Please use an account without MFA or use an alternative login method")
		_emit_login_auth_error(auth_res)
		return false
	
	if auth_res == EOS.Result.InvalidUser:
		if not auth_login_ret.continuance_token:
			_log.error("Auth login failed - Continuance token is invalid")
			_emit_login_auth_error(EOS.Result.InvalidState)
			return false
		
		if not auto_link_account:
			_log.error("Auth login failed - External account not connected")
			_emit_login_auth_error(EOS.Result.InvalidUser)
			return false
		
		_log.debug("External account not found. Proceeding to connect account...")
		var continue_success = await _continue_login_async(auth_login_ret.continuance_token)
		if not continue_success:
			return false
	
	elif not EOS.is_success(auth_res):
		_log.error("Failed to login with EOS Auth: result_code=%s" % EOS.result_str(auth_res))
		_emit_login_auth_error(auth_res)
		return false
	
	if auth_login_ret.selected_account_id:
		epic_account_id = auth_login_ret.selected_account_id
	
	if epic_account_id:
		_log.info("Logged in with Epic Account ID: %s" % epic_account_id)
	
	if not auto_connect_account:
		logged_in_auth.emit()
		logged_in.emit()
		return true

	return await _connect_account_async()


## Logout from EOS Auth and or EOS Connect
func logout_async() -> EOS.Result:
	_log.verbose("Logging out from EOS")
	var ret := EOS.Result.InvalidAuth
	var _logged_out = false

	if product_user_id:
		_log.debug("Logging out from EOS Connect")
		var logout_connect_opts = EOS.Connect.LogoutOptions.new()
		EOS.Connect.ConnectInterface.logout(logout_connect_opts)
		var logout_connect_ret = await IEOS.connect_interface_logout_callback
		ret = logout_connect_ret.result_code

		if not EOS.is_success(ret):
			_log.error("Failed to logout of EOS Connect. result_code=%s" % EOS.result_str(ret))
			return ret
		else:
			_log.debug("Logged out from EOS connect: product_user_id=%s" % product_user_id)
			product_user_id = ""
			_logged_out = true
			logged_out_connect.emit()


	if epic_account_id:
		_log.debug("Logging out from EOS Auth")
		var logout_auth_opts = EOS.Auth.LogoutOptions.new()
		EOS.Auth.AuthInterface.logout(logout_auth_opts)
		var logout_auth_ret = await IEOS.auth_interface_logout_callback
		ret = logout_auth_ret.result_code
	
		if not EOS.is_success(ret):
			_log.error("Failed to logout of EOS Auth. result_code=%s" % EOS.result_str(ret))
		else:
			_log.debug("Logged out from EOS Auth: epic_account_id=%s" % epic_account_id)
			epic_account_id = ""
			_logged_out = true
			logged_out_auth.emit()

	if _logged_out:
		logged_out.emit()

	return ret
	

## Login with EOS Connect by using external credentials
func login_game_services_async(opts: EOS.Connect.LoginOptions) -> bool:
	_log.debug("Logging into Epic Game Services (ConnectInterface)")
	EOS.Connect.ConnectInterface.login(opts)

	var login_ret: Dictionary = await IEOS.connect_interface_login_callback
	var login_res: EOS.Result = login_ret.result_code
	
	if login_res == EOS.Result.InvalidUser:
		_log.debug("Epic Game Services user not found. Proceeding to create user...")
		var create_success := await _create_user_async(login_ret.continuance_token)
		if not create_success:
			return false
	
	elif not EOS.is_success(login_ret):
		_log.error("Failed to login to Epic Game Services: result_code=%s" % EOS.result_str(login_res))
		_emit_login_connect_error(login_res)
		return false

	if login_ret.local_user_id:
		product_user_id = login_ret.local_user_id
	
	if product_user_id:
		_log.info("Logged in with Product User Id: %s" % product_user_id)

	logged_in_connect.emit()
	logged_in.emit()
	return true


## Login automatically if an Epic refresh token is stored by a previous login with Epic Account Portal
func login_persistent_auth_async() -> bool:
	_log.debug("Logging in with persistent auth...")
	var opts = EOS.Auth.LoginOptions.new()
	opts.credentials = EOS.Auth.Credentials.new()
	opts.credentials.type = EOS.Auth.LoginCredentialType.PersistentAuth
	opts.scope_flags = EOS.Auth.ScopeFlags.BasicProfile | EOS.Auth.ScopeFlags.Presence | EOS.Auth.ScopeFlags.FriendsList
	opts.login_flags = EOS.Auth.LoginFlags.NoUserInterface

	return await login_async(opts)


## Delete the internally stored Epic refresh token
func delete_persistent_auth_async(refresh_token = "") -> bool:
	var opts = EOS.Auth.DeletePersistentAuthOptions.new()
	opts.refresh_token = refresh_token
	EOS.Auth.AuthInterface.delete_persistent_auth(opts)

	var ret = await IEOS.auth_interface_delete_persistent_auth_callback
	if not EOS.is_success(ret):
		_log.error("Failed to delete persistent auth: result_code=%s" % EOS.result_str(ret))
		return false
	
	return true


## Login to Epic Game Services without any credentials
func login_anonymous_async(user_display_name = "") -> bool:
	_log.debug("Logging in anonymously...")

	EOS.Connect.ConnectInterface.delete_device_id(EOS.Connect.DeleteDeviceIdOptions.new())
	var delete_ret = await IEOS.connect_interface_delete_device_id_callback
	if not EOS.is_success(delete_ret):
		_log.debug("Failed to delete device id: result_code=%s" % EOS.result_str(delete_ret))
	
	var opts = EOS.Connect.CreateDeviceIdOptions.new()
	opts.device_model = " ".join(PackedStringArray([OS.get_name(), OS.get_model_name()]))
	EOS.Connect.ConnectInterface.create_device_id(opts)

	var create_ret = await IEOS.connect_interface_create_device_id_callback
	if not EOS.is_success(create_ret):
		_log.error("Failed to create device id: result_code=%s" % EOS.result_str(create_ret))
		return false
	
	var login_opts = EOS.Connect.LoginOptions.new()
	login_opts.credentials = EOS.Connect.Credentials.new()
	login_opts.credentials.type = EOS.ExternalCredentialType.DeviceidAccessToken
	login_opts.credentials.token = null
	if user_display_name:
		login_opts.user_login_info = EOS.Connect.UserLoginInfo.new()
		login_opts.user_login_info.display_name = user_display_name
	
	return await login_game_services_async(login_opts)


## Returns a Dictionary with the user info or null.
## Following is the data returned:
##   user_id: String
##   country: String
##   display_name: String
##   display_name_sanitized: String
##   preferred_language: String
##   nickname: String
func get_user_info_async():
	_log.verbose("Querying user info")
	var query_opts = EOS.UserInfo.QueryUserInfoOptions.new()
	query_opts.local_user_id = epic_account_id
	query_opts.target_user_id = epic_account_id
	EOS.UserInfo.UserInfoInterface.query_user_info(query_opts)

	var ret: Dictionary = await IEOS.user_info_interface_query_user_info_callback
	if not EOS.is_success(ret):
		_log.error("Failed to query user info: result_code=%s" % EOS.result_str(ret))
		return null
	
	var copy_opts = EOS.UserInfo.CopyUserInfoOptions.new()
	copy_opts.local_user_id = epic_account_id
	copy_opts.target_user_id = epic_account_id
	var copy_ret = EOS.UserInfo.UserInfoInterface.copy_user_info(copy_opts)
	if not EOS.is_success(copy_ret):
		_log.error("Failed to copy user info: result_code=%s" % EOS.result_str(copy_ret))
		return null

	return copy_ret.user_info

#endregion


#region Private methods

func _on_connect_interface_auth_expiration(data: Dictionary):
	_log.debug("Connect Auth Expiring...")
	_connect_account_async()


func _connect_account_async() -> bool:
	_log.debug("Connecting account to Epic Game Services...")

	# Copy the user auth token
	var auth_token_ret = EOS.Auth.AuthInterface.copy_user_auth_token(EOS.Auth.CopyUserAuthTokenOptions.new(), epic_account_id)
	if not EOS.is_success(auth_token_ret):
		_log.error("Failed to get user auth token: result_code=%s" % EOS.result_str(auth_token_ret))
		_emit_login_auth_error(auth_token_ret.result_code)
		return false

	var token = auth_token_ret.token

	var login_options = EOS.Connect.LoginOptions.new()
	login_options.credentials = EOS.Connect.Credentials.new()
	login_options.credentials.type = EOS.ExternalCredentialType.Epic
	login_options.credentials.token = token.access_token

	# Emit signal only if we are not refreshing the login
	if not epic_account_id:
		logged_in_auth.emit()
	return await login_game_services_async(login_options)


func _continue_login_async(continuance_token: EOSGContinuanceToken) -> bool:
	_log.debug("Continuing login...")
	var link_opts = EOS.Auth.LinkAccountOptions.new()
	link_opts.continuance_token = continuance_token
	EOS.Auth.AuthInterface.link_account(link_opts)

	var link_ret: Dictionary = await IEOS.auth_interface_link_account_callback
	if not EOS.is_success(link_ret):
		_log.error("Failed to link account: result_code=%s" % EOS.result_str(link_ret))
		_emit_login_auth_error(link_ret.result_code)
		return false
	
	if link_ret.selected_account_id:
		epic_account_id = link_ret.selected_account_id

	return true
	

func _create_user_async(continuance_token: EOSGContinuanceToken) -> bool:
	_log.debug("Creating user...")
	var create_opts = EOS.Connect.CreateUserOptions.new()
	create_opts.continuance_token = continuance_token
	EOS.Connect.ConnectInterface.create_user(create_opts)

	var create_ret: Dictionary = await IEOS.connect_interface_create_user_callback

	if not EOS.is_success(create_ret):
		_log.error("Failed to create user: result_code=%s" % EOS.result_str(create_ret))
		_emit_login_connect_error(create_ret.result_code)
		return false
	
	if create_ret.local_user_id:
		product_user_id = create_ret.local_user_id
	
	return true
	

func _emit_login_auth_error(result_code: EOS.Result):
	login_auth_error.emit(result_code)
	login_error.emit(result_code)


func _emit_login_connect_error(result_code: EOS.Result):
	login_connect_error.emit(result_code)
	login_error.emit(result_code)

#endregion
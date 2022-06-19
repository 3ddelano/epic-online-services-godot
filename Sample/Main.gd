extends Control

export (NodePath) var _views_path
onready var views = get_node(_views_path) as VBoxContainer

func _ready() -> void:
	print("Ready!")
	Store._main_node = self

	# -----
	# EOS Setup
	# -----

	# Initialize the SDK
	var init_options = EOS.Platform.InitializeOptions.new()
	init_options.product_name = Env.get_var("PRODUCT_NAME")
	init_options.product_version = Env.get_var("PRODUCT_VERSION")
	var init_result: int = EOS.Platform.PlatformInterface.initialize(init_options)
	if init_result != EOS.Result.Success:
		print("Failed to initialize EOS SDK: ", EOS.print_result(init_result))
		return

	# Create platform
	var create_options = EOS.Platform.CreateOptions.new()
	create_options.product_id = Env.get_var("PRODUCT_ID") # Paste your own here instead
	create_options.sandbox_id = Env.get_var("SANDBOX_ID") # Paste your own here instead
	create_options.deployment_id = Env.get_var("DEPLOYMENT_ID") # Paste your own here instead
	create_options.client_id = Env.get_var("CLIENT_ID") # Paste your own here instead
	create_options.client_secret = Env.get_var("CLIENT_SECRET") # Paste your own here instead
	create_options.encryption_key = Env.get_var("ENCRYPTION_KEY") # Paste your own here instead
	if OS.get_name() == "Windows":
		create_options.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl
	#create_options.is_server = true
	#create_options.tick_budget_in_milliseconds = 500

	var create_success: bool = EOS.Platform.PlatformInterface.create(create_options)
	if not create_success:
		print("Failed to create EOS Platform")
		return

	var _c = Store.connect("login_success", self, "_on_login_success")
	yield(get_tree().create_timer(0.5), "timeout")
	Store.emit_signal("platform_create")
	print(EOS.Version.VersionInterface.get_constants())
	print("Version: ", EOS.Version.VersionInterface.get_version())


func get_view_manager():
	return views


func _input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed:
		if event.scancode == KEY_QUOTELEFT: # ` key to toggle Logs
			Store.get_view("Logs").visible = not Store.get_view("Logs").visible




# Dev testing stuff
func _on_login_success():
#	print("--- Auth: get_logged_in_accounts_count: ", EOS.Auth.AuthInterface.get_logged_in_accounts_count())
#	print("--- Auth: Logged in account by index(0): ", EOS.Auth.AuthInterface.get_logged_in_account_by_index(0))
#	print("--- Auth: get_login_status: ", EOS.Auth.AuthInterface.get_login_status(Store.epic_account_id))
#	print("--- Auth: get_merged_accounts_count: ", EOS.Auth.AuthInterface.get_merged_accounts_count(Store.epic_account_id))
#	print("--- Auth: Get merged account by index(0): ", EOS.Auth.AuthInterface.get_merged_account_by_index(Store.epic_account_id, 0))
#	print("--- Auth: get_selected_account_id: ", EOS.Auth.AuthInterface.get_selected_account_id(Store.epic_account_id))
#
#	var copy_user_auth_token = EOS.Auth.AuthInterface.copy_user_auth_token(EOS.Auth.CopyUserAuthTokenOptions.new(), Store.epic_account_id)
#	var token = copy_user_auth_token.token
#
#	var verify_user_auth_options = EOS.Auth.VerifyUserAuthOptions.new()
#	verify_user_auth_options.auth_token = token
#	EOS.Auth.AuthInterface.verify_user_auth(verify_user_auth_options)
#
#	print("--- Platform: get_active_country_code: ", EOS.Platform.PlatformInterface.get_active_country_code(Store.epic_account_id))
#	print("--- Platform: get_active_locale_code: ", EOS.Platform.PlatformInterface.get_active_locale_code(Store.epic_account_id))
#	print("--- Platform: get_override_country_code: ", EOS.Platform.PlatformInterface.get_override_country_code())
#	print("--- Platform: get_override_locale_code: ", EOS.Platform.PlatformInterface.get_override_locale_code())
#
#	var copy_id_token_options = EOS.Auth.CopyIdTokenOptions.new()
#	copy_id_token_options.account_id = Store.epic_account_id
#	print("--- Auth: copy_id_token: ", EOS.print_result(EOS.Auth.AuthInterface.copy_id_token(copy_id_token_options)))
#
#	var options1 = EOS.Connect.CopyProductUserExternalAccountByAccountIdOptions.new()
#	options1.target_user_id = Store.product_user_id
#	options1.account_id = Store.epic_account_id
#	print("copy_product_user_external_account: By account Id: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_id(options1))
#
#	var options2 = EOS.Connect.CopyProductUserExternalAccountByAccountTypeOptions.new()
#	options2.target_user_id = Store.product_user_id
#	options2.account_id_type = EOS.ExternalAccountType.Epic
#	print("copy_product_user_external_account: By account type: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_type(options2))
#
#	var options3 = EOS.Connect.CopyProductUserExternalAccountByIndexOptions.new()
#	options3.target_user_id = Store.product_user_id
#	options3.external_account_info_index = 0
#	print("copy_product_user_external_account: By account index: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_index(options3))
#
#	var options4 = EOS.Connect.CopyProductUserInfoOptions.new()
#	options4.target_user_id = Store.product_user_id
#	print("--- Connect: copy_product_user_info: ", EOS.Connect.ConnectInterface.copy_product_user_info(options4))
#
#	var options5 = EOS.Connect.CopyIdTokenOptions.new()
#	options5.local_user_id = Store.product_user_id
#	var ret5 = EOS.Connect.ConnectInterface.copy_id_token(options5)
#	var id_token = EOS.Connect.IdToken.new()
#	id_token.json_web_token = ret5.id_token.json_web_token
#	id_token.product_user_id = ret5.id_token.product_user_id
#	print("-- Connect: copy_id_token: Id token: ", id_token)
#
#	var options6 = EOS.Connect.GetExternalAccountMappingsOptions.new()
#	options6.local_user_id = Store.product_user_id
#	options6.account_id_type = EOS.ExternalAccountType.Discord
#	options6.target_external_user_id  = "External Account Id Here"
#	print("External account mapping: ", EOS.Connect.ConnectInterface.get_external_account_mapping(options6))
#
#	print("--- Connect: get_logged_in_user_by_index(0): ", EOS.Connect.ConnectInterface.get_logged_in_user_by_index(0))
#	print("--- Connect: get_logged_in_users_count: ", EOS.Connect.ConnectInterface.get_logged_in_users_count())
#	print("--- Connecet: get_login_status: ", EOS.Connect.ConnectInterface.get_login_status(Store.product_user_id))
#
#	var options7 = EOS.Connect.GetProductUserExternalAccountCountOptions.new()
#	options7.target_user_id = Store.product_user_id
#	print("--- Connect: get_product_user_external_account_count: ", EOS.Connect.ConnectInterface.get_product_user_external_account_count(options7))
#
#	var options8 = EOS.Connect.GetProductUserIdMappingOptions.new()
#	options8.local_user_id = Store.product_user_id
#	options8.account_id_type = EOS.ExternalAccountType.Discord
#	options8.target_product_user_id  = "External Account Id Here"
#	print("--- Connect: get_product_user_id_mapping: ", EOS.Connect.ConnectInterface.get_product_user_id_mapping(options8))
#
##	var options9 = EOS.Connect.LinkAccountOptions.new()
##	options9.local_user_id = Store.product_user_id
##	options9.continuance_token = ContinunanceTokenWrapperHere
##	EOS.Connect.ConnectInterface.link_account(options9)
##	print(yield(EOS.get_instance(), "connect_interface_link_account_callback"))
#
#	var options10 = EOS.Connect.VerifyIdTokenOptions.new()
#	options10.id_token = id_token
#	EOS.Connect.ConnectInterface.verify_id_token(options10)
#	print("--- Connect: verify_id_token_callback: ", yield(EOS.get_instance(), "connect_interface_verify_id_token_callback"))
#
#	# Test Ecom stuff
#	var offers_options = EOS.Ecom.QueryOffersOptions.new()
#	offers_options.local_user_id = Store.epic_account_id
#	EOS.Ecom.EcomInterface.query_offers(offers_options)
#	print("--- Ecom: query_offers_callback", EOS.print_result(yield(EOS.get_instance(), "ecom_interface_query_offers_callback")))
#
#	var checkout_options = EOS.Ecom.CheckoutOptions.new()
#	checkout_options.local_user_id = Store.epic_account_id
#	checkout_options.entries = [{
#		offer_id = "1234"
#	}]
#	EOS.Ecom.EcomInterface.checkout(checkout_options)
#	var checkout_data = yield(EOS.get_instance(), "ecom_interface_checkout_callback")
#	print("--- Ecom: checkout_callback", checkout_data)
	pass

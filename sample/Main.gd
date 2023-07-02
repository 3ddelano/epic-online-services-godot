extends Control

@onready var PRODUCT_NAME: String = Env.get_var("PRODUCT_NAME") # Paste your own
@onready var PRODUCT_VERSION: String = Env.get_var("PRODUCT_VERSION") # Paste your own
@onready var PRODUCT_ID: String = Env.get_var("PRODUCT_ID") # Paste your own
@onready var SANDBOX_ID: String = Env.get_var("SANDBOX_ID") # Paste your own
@onready var DEPLOYMENT_ID: String = Env.get_var("DEPLOYMENT_ID") # Paste your own
@onready var CLIENT_ID: String = Env.get_var("CLIENT_ID") # Paste your own
@onready var CLIENT_SECRET: String = Env.get_var("CLIENT_SECRET") # Paste your own
@onready var ENCRYPTION_KEY: String = Env.get_var("ENCRYPTION_KEY") # Paste your own


@export var _views_path: NodePath
@onready var views = get_node(_views_path) as VBoxContainer


func _ready() -> void:
	print("Ready!")
	Store._main_node = self

	# -----
	# EOS Setup
	# -----

	# Initialize the SDK
	var init_options = EOS.Platform.InitializeOptions.new()
	init_options.product_name = PRODUCT_NAME
	init_options.product_version = PRODUCT_VERSION
	var init_result: int = EOS.Platform.PlatformInterface.initialize(init_options)
	if init_result != EOS.Result.Success:
		print("Failed to initialize EOS SDK: ", EOS.result_str(init_result))
		return
	print("Intialized EOS platform")

	# Create platform
	var create_options = EOS.Platform.CreateOptions.new()
	create_options.product_id = PRODUCT_ID
	create_options.sandbox_id = SANDBOX_ID
	create_options.deployment_id = DEPLOYMENT_ID
	create_options.client_id = CLIENT_ID
	create_options.client_secret = CLIENT_SECRET
	create_options.encryption_key = ENCRYPTION_KEY
	if OS.get_name() == "Windows":
		create_options.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl

	var create_success: bool = EOS.Platform.PlatformInterface.create(create_options)
	if not create_success:
		print("Failed to create EOS Platform")
		return
	print("Created EOS platform")

	var sdk_constants = EOS.Version.VersionInterface.get_constants()
	print("EOS SDK Version: %s (%s)" % [EOS.Version.VersionInterface.get_version(), sdk_constants.copyright_string])
	await get_tree().create_timer(0.5).timeout
	Store.emit_signal("platform_create")
	# Debug
	Store.login_success.connect(_on_tab_pressed)


func _input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed:
		if event.keycode == KEY_QUOTELEFT: # ` key to toggle Logs
			Store.get_view("Logs").visible = not Store.get_view("Logs").visible
		elif event.keycode == KEY_TAB:
			#if Store.product_user_id:
			_on_tab_pressed()


# Dev testing stuff
func _on_tab_pressed():
	print("Tab pressed")

#	test_auth_interface()
#	test_platform_interface()
#	test_connect_interface()
#	test_ecom_interface()
#	test_user_info_interface()
#	test_mods_interface()
#	test_reports_interface()



#	# ProgressionSnapshot Interface
#	var begin_snapshot_options = EOS.ProgressionSnapshot.BeginSnapshotOptions.new()
#	begin_snapshot_options.local_user_id = Store.product_user_id
#	var begin_snapshot_ret = EOS.ProgressionSnapshot.ProgressionSnapshotInterface.begin_snapshot(begin_snapshot_options)
#	print("--- ProgressionSnapshot: begin_snapshot: ", begin_snapshot_ret)
#
#	awit get_tree().create_timer(3).timeout
#
#	var add_progression_options = EOS.ProgressionSnapshot.AddProgressionOptions.new()
#	add_progression_options.snapshot_id = begin_snapshot_ret.snapshot_id
#	add_progression_options.key = "test_key"
#	add_progression_options.value = "test_value 0123456789"
#	print("--- ProgressionSnapshot: add_progression: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.add_progression(add_progression_options)))
#
#	await get_tree().create_timer(3).timeout
#
#	var submit_snapshot_options = EOS.ProgressionSnapshot.SubmitSnapshotOptions.new()
#	submit_snapshot_options.snapshot_id = begin_snapshot_ret.snapshot_id
#	print("--- ProgressionSnapshot: submit_snapshot: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.submit_snapshot(submit_snapshot_options)))
#	print("--- ProgressionSnapshot: submit_snapshot: ", await EOS.get_instance().progression_snapshot_interface_submit_snapshot_callback)
#
#	await get_tree().create_timer(3).timeout
#
#	var delete_snapshot_options = EOS.ProgressionSnapshot.DeleteSnapshotOptions.new()
#	delete_snapshot_options.local_user_id = Store.product_user_id
#	print("--- ProgressionSnapshot: delete_snapshot: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.delete_snapshot(delete_snapshot_options)))
#	print("--- ProgressionSnapshot: delete_snapshot: ", await EOS.get_instance().progression_snapshot_interface_delete_snapshot_callback)


#	# Presence Interface
#	var create_presence_modification_options = EOS.Presence.CreatePresenceModificationOptions.new()
#	create_presence_modification_options.local_user_id = Store.epic_account_id
#	var create_presence_modification_ret = EOS.Presence.PresenceInterface.create_presence_modification(create_presence_modification_options)
#	print("--- Presence: create_presence_modification: ", EOS.result_str(create_presence_modification_ret.result_code))
#	var presence_modification = create_presence_modification_ret.presence_modification

#	var set_data_options = EOS.Presence.PresenceModificationSetDataOptions.new()
#	set_data_options.records = [{key = "hello", value="lorem"}]
#	var set_data_ret = presence_modification.set_data(set_data_options)
#	print("--- PresenceModificationWrapper: set_data: ", EOS.result_str(set_data_ret))

#	var delete_data_options = EOS.Presence.PresenceModificationDeleteDataOptions.new()
#	delete_data_options.records = ["awe", "abc"]
#	var delete_data_ret = presence_modification.delete_data(delete_data_options)
#	print("--- PresenceModificationWrapper: delete_data: ", EOS.result_str(delete_data_ret))

#	var set_join_info_options = EOS.Presence.PresenceModificationSetJoinInfoOptions.new()
#	set_join_info_options.join_info = "this is sample join info"
#	var set_join_info_ret = presence_modification.set_join_info(set_join_info_options)
#	print("--- PresenceModificationWrapper: set_join_info: ", EOS.result_str(set_join_info_ret))

#	var set_raw_rich_text_options = EOS.Presence.PresenceModificationSetRawRichTextOptions.new()
#	set_raw_rich_text_options.rich_text = "this is sample join info"
#	var set_raw_rich_text_ret = presence_modification.set_raw_rich_text(set_raw_rich_text_options)
#	print("--- PresenceModificationWrapper: set_raw_rich_text: ", EOS.result_str(set_raw_rich_text_ret))

#	var set_status_options = EOS.Presence.PresenceModificationSetStatusOptions.new()
#	set_status_options.status = EOS.Presence.Status.DoNotDisturb
#	var set_status_ret = presence_modification.set_status(set_status_options)
#	print("--- PresenceModificationWrapper: set_status: ", EOS.result_str(set_status_ret))

#	var get_join_info_options = EOS.Presence.GetJoinInfoOptions.new()
#	get_join_info_options.local_user_id = Store.epic_account_id
#	get_join_info_options.target_user_id = Store.epic_account_id
#	var get_join_info_ret = EOS.Presence.PresenceInterface.get_join_info(get_join_info_options)
#	print("--- Presence: get_join_info: ", EOS.result_str(get_join_info_ret))

#	var has_presence_options = EOS.Presence.HasPresenceOptions.new()
#	has_presence_options.local_user_id = Store.epic_account_id
#	has_presence_options.target_user_id = Store.epic_account_id
#	var has_presence_ret = EOS.Presence.PresenceInterface.has_presence(has_presence_options)
#	print("--- Presence: has_presence: ", EOS.result_str(has_presence_ret))

#	var query_presence_options = EOS.Presence.QueryPresenceOptions.new()
#	query_presence_options.local_user_id = Store.epic_account_id
#	query_presence_options.target_user_id = Store.epic_account_id
#	EOS.Presence.PresenceInterface.query_presence(query_presence_options)
#	print("--- Presence: query_presence: ", EOS.result_str(await EOS.get_instance().presence_interface_query_presence_callback))

#	var copy_presence_options = EOS.Presence.CopyPresenceOptions.new()
#	copy_presence_options.local_user_id = Store.epic_account_id
#	copy_presence_options.target_user_id = Store.epic_account_id
#	var copy_presence_ret = EOS.Presence.PresenceInterface.copy_presence(copy_presence_options)
#	print("--- Presence: copy_presence: ", EOS.result_str(copy_presence_ret))

#	var set_presence_options = EOS.Presence.SetPresenceOptions.new()
#	set_presence_options.local_user_id = Store.epic_account_id
#	set_presence_options.presence_modification = presence_modification
#	var set_presence_ret = EOS.Presence.PresenceInterface.set_presence(set_presence_options)
#	print("--- Presence: set_presence: ", EOS.result_str(await EOS.get_instance().presence_interface_set_presence_callback))
	pass


func test_auth_interface():
	print("--- Auth: get_logged_in_accounts_count: ", EOS.Auth.AuthInterface.get_logged_in_accounts_count())
	print("--- Auth: Logged in account by index(0): ", EOS.Auth.AuthInterface.get_logged_in_account_by_index(0))
	print("--- Auth: get_login_status: ", EOS.Auth.AuthInterface.get_login_status(Store.epic_account_id))
	print("--- Auth: get_merged_accounts_count: ", EOS.Auth.AuthInterface.get_merged_accounts_count(Store.epic_account_id))
	print("--- Auth: Get merged account by index(0): ", EOS.Auth.AuthInterface.get_merged_account_by_index(Store.epic_account_id, 0))
	print("--- Auth: get_selected_account_id: ", EOS.Auth.AuthInterface.get_selected_account_id(Store.epic_account_id))

	var copy_user_auth_token = EOS.Auth.AuthInterface.copy_user_auth_token(EOS.Auth.CopyUserAuthTokenOptions.new(), Store.epic_account_id)
	var token = copy_user_auth_token.token
	var verify_user_auth_options = EOS.Auth.VerifyUserAuthOptions.new()
	verify_user_auth_options.auth_token = token
	EOS.Auth.AuthInterface.verify_user_auth(verify_user_auth_options)
	var verify_user_auth_ret = await EOS.get_instance().auth_interface_verify_user_auth_callback
	print("--- Auth: verify_user_auth: ", verify_user_auth_ret)
	var copy_id_token_options = EOS.Auth.CopyIdTokenOptions.new()
	copy_id_token_options.account_id = Store.epic_account_id
	print("--- Auth: copy_id_token: ", EOS.result_str(EOS.Auth.AuthInterface.copy_id_token(copy_id_token_options)))


func test_platform_interface():
	print("--- Platform: get_active_country_code: ", EOS.Platform.PlatformInterface.get_active_country_code(Store.epic_account_id))
	print("--- Platform: get_active_locale_code: ", EOS.Platform.PlatformInterface.get_active_locale_code(Store.epic_account_id))
	print("--- Platform: get_override_country_code: ", EOS.Platform.PlatformInterface.get_override_country_code())
	print("--- Platform: get_override_locale_code: ", EOS.Platform.PlatformInterface.get_override_locale_code())


func test_connect_interface():
	var opts1 = EOS.Connect.CopyProductUserExternalAccountByAccountIdOptions.new()
	opts1.target_user_id = Store.product_user_id
	opts1.account_id = Store.epic_account_id
	print("--- Connect: copy_product_user_external_account: By account Id: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_id(opts1))

	var opts2 = EOS.Connect.CopyProductUserExternalAccountByAccountTypeOptions.new()
	opts2.target_user_id = Store.product_user_id
	opts2.account_id_type = EOS.ExternalAccountType.Epic
	print("--- Connect: copy_product_user_external_account: By account type: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_type(opts2))

	var opts3 = EOS.Connect.CopyProductUserExternalAccountByIndexOptions.new()
	opts3.target_user_id = Store.product_user_id
	opts3.external_account_info_index = 0
	print("--- Connect: copy_product_user_external_account: By account index: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_index(opts3))

	var opts4 = EOS.Connect.CopyProductUserInfoOptions.new()
	opts4.target_user_id = Store.product_user_id
	print("--- Connect: copy_product_user_info: ", EOS.Connect.ConnectInterface.copy_product_user_info(opts4))

	var opts5 = EOS.Connect.CopyIdTokenOptions.new()
	opts5.local_user_id = Store.product_user_id
	var ret5 = EOS.Connect.ConnectInterface.copy_id_token(opts5)
	var id_token = EOS.Connect.IdToken.new()
	id_token.json_web_token = ret5.id_token.json_web_token
	id_token.product_user_id = ret5.id_token.product_user_id
	print("-- Connect: copy_id_token: Id token: ", id_token)

	var opts6 = EOS.Connect.GetExternalAccountMappingsOptions.new()
	opts6.local_user_id = Store.product_user_id
	opts6.account_id_type = EOS.ExternalAccountType.Discord
	opts6.target_external_user_id  = "External Account Id Here"
	print("--- Connect: get_external_account_mapping: ", EOS.Connect.ConnectInterface.get_external_account_mapping(opts6))

	print("--- Connect: get_logged_in_user_by_index(0): ", EOS.Connect.ConnectInterface.get_logged_in_user_by_index(0))
	print("--- Connect: get_logged_in_users_count: ", EOS.Connect.ConnectInterface.get_logged_in_users_count())
	print("--- Connecet: get_login_status: ", EOS.Connect.ConnectInterface.get_login_status(Store.product_user_id))

	var opts7 = EOS.Connect.GetProductUserExternalAccountCountOptions.new()
	opts7.target_user_id = Store.product_user_id
	print("--- Connect: get_product_user_external_account_count: ", EOS.Connect.ConnectInterface.get_product_user_external_account_count(opts7))

	var opts8 = EOS.Connect.GetProductUserIdMappingOptions.new()
	opts8.local_user_id = Store.product_user_id
	opts8.account_id_type = EOS.ExternalAccountType.Discord
	opts8.target_product_user_id  = "External Account Id Here"
	print("--- Connect: get_product_user_id_mapping: ", EOS.Connect.ConnectInterface.get_product_user_id_mapping(opts8))

	var opts9 = EOS.Connect.LinkAccountOptions.new()
	opts9.local_user_id = Store.product_user_id
#	opts9.continuance_token = ContinunanceTokenEOSG object here
	EOS.Connect.ConnectInterface.link_account(opts9)
	print(await EOS.get_instance().connect_interface_link_account_callback)

	var opts10 = EOS.Connect.VerifyIdTokenOptions.new()
	opts10.id_token = id_token
	EOS.Connect.ConnectInterface.verify_id_token(opts10)
	print("--- Connect: verify_id_token: ", await EOS.get_instance().connect_interface_verify_id_token_callback)


func test_ecom_interface():
	var offers_options = EOS.Ecom.QueryOffersOptions.new()
	offers_options.local_user_id = Store.epic_account_id
	EOS.Ecom.EcomInterface.query_offers(offers_options)
	print("--- Ecom: query_offers: ", EOS.result_str(await EOS.get_instance().ecom_interface_query_offers_callback))

	var checkout_options = EOS.Ecom.CheckoutOptions.new()
	checkout_options.local_user_id = Store.epic_account_id
	checkout_options.entries = [{
		offer_id = "OFFER ID HERE"
	}]
	EOS.Ecom.EcomInterface.checkout(checkout_options)
	var checkout_data = await EOS.get_instance().ecom_interface_checkout_callback
	print("--- Ecom: checkout: ", checkout_data)


func test_user_info_interface():
	var opts1 = EOS.UserInfo.QueryUserInfoOptions.new()
	opts1.local_user_id = Store.epic_account_id
	opts1.target_user_id = Store.epic_account_id
	EOS.UserInfo.UserInfoInterface.query_user_info(opts1)
	print("--- UserInfo: query_user_info: ", EOS.result_str(await EOS.get_instance().user_info_interface_query_user_info_callback))

	var opts2 = EOS.UserInfo.CopyUserInfoOptions.new()
	opts1.local_user_id = Store.epic_account_id
	opts1.target_user_id = Store.epic_account_id
	print("--- UserInfo: copy_user_info: ", EOS.UserInfo.UserInfoInterface.copy_user_info(opts2))

	var opts3 = EOS.UserInfo.QueryUserInfoByDisplayNameOptions.new()
	opts3.local_user_id = Store.epic_account_id
	opts3.display_name = "3ddelano"
	EOS.UserInfo.UserInfoInterface.query_user_info_by_display_name(opts3)
	print("--- UserInfo: query_user_info_by_display_name: ", EOS.result_str(await EOS.get_instance().user_info_interface_query_user_info_by_display_name_callback))

	var opts4 = EOS.UserInfo.QueryUserInfoByExternalAccountOptions.new()
	opts4.local_user_id = Store.epic_account_id
	opts4.account_type = EOS.ExternalAccountType.Epic
	opts4.external_account_id = Store.epic_account_id
	EOS.UserInfo.UserInfoInterface.query_user_info_by_external_account(opts4)
	print("--- UserInfo: query_user_info_by_external_account: ", EOS.result_str(await EOS.get_instance().user_info_interface_query_user_info_by_external_account_callback))

	var opts5 = EOS.UserInfo.CopyExternalUserInfoByAccountIdOptions.new()
	opts5.local_user_id = Store.epic_account_id
	opts5.target_user_id = Store.epic_account_id
	opts5.account_id = Store.epic_account_id
	print("--- UserInfo: copy_external_user_info_by_account_id: ", EOS.UserInfo.UserInfoInterface.copy_external_user_info_by_account_id(opts5))

	var opts6 = EOS.UserInfo.CopyExternalUserInfoByAccountTypeOptions.new()
	opts6.local_user_id = Store.epic_account_id
	opts6.target_user_id = Store.epic_account_id
	opts6.account_type = EOS.ExternalAccountType.Epic
	EOS.UserInfo.UserInfoInterface.copy_external_user_info_by_account_type(opts6)

	var opts7 = EOS.UserInfo.CopyExternalUserInfoByIndexOptions.new()
	opts7.local_user_id = Store.epic_account_id
	opts7.target_user_id = Store.epic_account_id
	opts7.index = 0
	EOS.UserInfo.UserInfoInterface.copy_external_user_info_by_index(opts7)

	var opts8 = EOS.UserInfo.GetExternalUserInfoCountOptions.new()
	opts8.local_user_id = Store.epic_account_id
	opts8.target_user_id = Store.epic_account_id
	EOS.UserInfo.UserInfoInterface.get_external_user_info_count(opts8)


func test_mods_interface():
	var opts1 = EOS.Mods.EnumerateModsOptions.new()
	opts1.local_user_id = Store.epic_account_id
	opts1.type = EOS.Mods.ModEnumerationType.AllAvailable
	EOS.Mods.ModsInterface.enumerate_mods(opts1)
	print("--- Mods: enumerate_mods: ", EOS.result_str(await EOS.get_instance().mods_interface_enumerate_mods_callback))

	var opts2 = EOS.Mods.CopyModInfoOptions.new()
	opts2.local_user_id = Store.epic_account_id
	opts2.type = EOS.Mods.ModEnumerationType.AllAvailable
	var copy_mods_info_data = EOS.Mods.ModsInterface.copy_mod_info(opts2)
	print("--- Mods: copy_mod_info: ", copy_mods_info_data)

	var mod = {}
	if copy_mods_info_data.mods != null:
		mod = copy_mods_info_data.mods.mods[0]
	var opts3 = EOS.Mods.InstallModOptions.new()
	opts3.local_user_id = Store.epic_account_id
	opts3.mod = mod
	EOS.Mods.ModsInterface.install_mod(opts3)
	print("--- Mods: install_mod: ", EOS.result_str(await EOS.get_instance().mods_interface_install_mod_callback))


func test_reports_interface():
	var report_options = EOS.Reports.SendPlayerBehaviorReportOptions.new()
	report_options.reporter_user_id = Store.product_user_id
	report_options.reported_user_id = "e5d94c924f204a63bd343b976ae662cd"
	report_options.category = EOS.Reports.PlayerReportsCategory.Cheating
	report_options.message = "this is a test report from godot"
	report_options.context = JSON.stringify({hello = "testing"})
	EOS.Reports.ReportsInterface.send_player_behavior_report(report_options)
	print("--- Reports: send_player_behavior_report: ", EOS.result_str(await EOS.get_instance().reports_interface_send_player_behavior_report_callback))


func get_view_manager():
	return views


func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		print("Shutting down EOS...")
		EOS.Platform.PlatformInterface.release()
		var _res = EOS.Platform.PlatformInterface.shutdown()

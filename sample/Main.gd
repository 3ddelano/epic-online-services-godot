extends Control

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
	init_options.product_name = Env.get_var("PRODUCT_NAME")
	init_options.product_version = Env.get_var("PRODUCT_VERSION")
	var init_result: int = EOS.Platform.PlatformInterface.initialize(init_options)
	if init_result != EOS.Result.Success:
		print("Failed to initialize EOS SDK: ", EOS.result_str(init_result))
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

#	var _c = Store.connect("login_success", self, "_on_login_success")
	await get_tree().create_timer(0.5).timeout
	Store.emit_signal("platform_create")
	var sdk_constants = EOS.Version.VersionInterface.get_constants()
	print("EOS SDK Version: %s (%s)" % [EOS.Version.VersionInterface.get_version(), sdk_constants.copyright_string])


func get_view_manager():
	return views


func _input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed:
		if event.keycode == KEY_QUOTELEFT: # ` key to toggle Logs
			Store.get_view("Logs").visible = not Store.get_view("Logs").visible
		elif event.keycode == KEY_TAB:
			if Store.product_user_id:
				_on_tab_pressed()


# Dev testing stuff
func _on_tab_pressed():
	# Auth interface
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



#	Platform interface
#	print("--- Platform: get_active_country_code: ", EOS.Platform.PlatformInterface.get_active_country_code(Store.epic_account_id))
#	print("--- Platform: get_active_locale_code: ", EOS.Platform.PlatformInterface.get_active_locale_code(Store.epic_account_id))
#	print("--- Platform: get_override_country_code: ", EOS.Platform.PlatformInterface.get_override_country_code())
#	print("--- Platform: get_override_locale_code: ", EOS.Platform.PlatformInterface.get_override_locale_code())
#
#	var copy_id_token_options = EOS.Auth.CopyIdTokenOptions.new()
#	copy_id_token_options.account_id = Store.epic_account_id
#	print("--- Auth: copy_id_token: ", EOS.result_str(EOS.Auth.AuthInterface.copy_id_token(copy_id_token_options)))



#	Connect interface
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



#	# Ecom interface
#	var offers_options = EOS.Ecom.QueryOffersOptions.new()
#	offers_options.local_user_id = Store.epic_account_id
#	EOS.Ecom.EcomInterface.query_offers(offers_options)
#	print("--- Ecom: query_offers_callback", EOS.result_str(yield(EOS.get_instance(), "ecom_interface_query_offers_callback")))
#
#	var checkout_options = EOS.Ecom.CheckoutOptions.new()
#	checkout_options.local_user_id = Store.epic_account_id
#	checkout_options.entries = [{
#		offer_id = "1234"
#	}]
#	EOS.Ecom.EcomInterface.checkout(checkout_options)
#	var checkout_data = yield(EOS.get_instance(), "ecom_interface_checkout_callback")
#	print("--- Ecom: checkout_callback", checkout_data)



#	# Mod interface
#	var enum_mods_options = EOS.Mods.EnumerateModsOptions.new()
#	enum_mods_options.local_user_id = Store.epic_account_id
#	enum_mods_options.type = EOS.Mods.ModEnumerationType.AllAvailable
#	EOS.Mods.ModsInterface.enumerate_mods(enum_mods_options)
#	var enum_mods_callback_data = yield(EOS.get_instance(), "mods_interface_enumerate_mods_callback")
#	print("--- Mods: enumerate_mods_callback: ", EOS.result_str(enum_mods_callback_data))
#
#	var copy_mod_info_options = EOS.Mods.CopyModInfoOptions.new()
#	copy_mod_info_options.local_user_id = Store.epic_account_id
#	copy_mod_info_options.type = EOS.Mods.ModEnumerationType.AllAvailable
#	var copy_mods_info_data = EOS.Mods.ModsInterface.copy_mod_info(copy_mod_info_options)
#	print("--- Mods: copy_mod_info: ", copy_mods_info_data)



	# Reports interface
#	var report_options = EOS.Reports.SendPlayerBehaviorReportOptions.new()
#	report_options.reporter_user_id = Store.product_user_id
#	report_options.reported_user_id = "e5d94c924f204a63bd343b976ae662cd"
#	report_options.category = EOS.Reports.PlayerReportsCategory.Spamming
#	report_options.message = "this is test message"
#	report_options.context = JSON.print({hello = "testing"})
#	EOS.Reports.ReportsInterface.send_player_behavior_report(report_options)
#	print("--- Reports: send_player_behavior_report: ", EOS.result_str(yield(EOS.get_instance(), "reports_interface_report_callback")))



#	# ProgressionSnapshot Interface
#	var begin_snapshot_options = EOS.ProgressionSnapshot.BeginSnapshotOptions.new()
#	begin_snapshot_options.local_user_id = Store.product_user_id
#	var begin_snapshot_ret = EOS.ProgressionSnapshot.ProgressionSnapshotInterface.begin_snapshot(begin_snapshot_options)
#	print("--- ProgressionSnapshot: begin_snapshot: ", begin_snapshot_ret)
#
#	yield(get_tree().create_timer(3), "timeout")
#
#	var add_progression_options = EOS.ProgressionSnapshot.AddProgressionOptions.new()
#	add_progression_options.snapshot_id = begin_snapshot_ret.snapshot_id
#	add_progression_options.key = "test_key"
#	add_progression_options.value = "test_value 0123456789"
#	print("--- ProgressionSnapshot: add_progression: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.add_progression(add_progression_options)))
#
#	yield(get_tree().create_timer(3), "timeout")
#
#	var submit_snapshot_options = EOS.ProgressionSnapshot.SubmitSnapshotOptions.new()
#	submit_snapshot_options.snapshot_id = begin_snapshot_ret.snapshot_id
#	print("--- ProgressionSnapshot: submit_snapshot: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.submit_snapshot(submit_snapshot_options)))
#	print("--- ProgressionSnapshot: submit_snapshot_callback: ", yield(EOS.get_instance(), "progression_snapshot_interface_submit_snapshot_callback"))
#
#	yield(get_tree().create_timer(3), "timeout")
#
#	var delete_snapshot_options = EOS.ProgressionSnapshot.DeleteSnapshotOptions.new()
#	delete_snapshot_options.local_user_id = Store.product_user_id
#	print("--- ProgressionSnapshot: delete_snapshot: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.delete_snapshot(delete_snapshot_options)))
#	print("--- ProgressionSnapshot: delete_snapshot_callback: ", yield(EOS.get_instance(), "progression_snapshot_interface_delete_snapshot_callback"))


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
#	print("--- Presence: query_presence_callback: ", EOS.result_str(yield(EOS.get_instance(), "presence_interface_query_presence_callback")))

#	var copy_presence_options = EOS.Presence.CopyPresenceOptions.new()
#	copy_presence_options.local_user_id = Store.epic_account_id
#	copy_presence_options.target_user_id = Store.epic_account_id
#	var copy_presence_ret = EOS.Presence.PresenceInterface.copy_presence(copy_presence_options)
#	print("--- Presence: copy_presence: ", EOS.result_str(copy_presence_ret))

#	var set_presence_options = EOS.Presence.SetPresenceOptions.new()
#	set_presence_options.local_user_id = Store.epic_account_id
#	set_presence_options.presence_modification = presence_modification
#	var set_presence_ret = EOS.Presence.PresenceInterface.set_presence(set_presence_options)
#	print("--- Presence: set_presence_callback: ", EOS.result_str(yield(EOS.get_instance(), "presence_interface_set_presence_callback")))



	pass

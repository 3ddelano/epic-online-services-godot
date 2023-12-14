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
	print("Initialized EOS platform")

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
#	test_progression_snapshot_interface()
#	test_presence_interface()
#	test_kws_interface()
#	test_playerdatastorage_interface()





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
	report_options.reported_user_id = Store.second_product_user_id
	report_options.category = EOS.Reports.PlayerReportsCategory.Cheating
	report_options.message = "this is a test report from godot"
	report_options.context = JSON.stringify({hello = "testing"})
	EOS.Reports.ReportsInterface.send_player_behavior_report(report_options)
	print("--- Reports: send_player_behavior_report: ", EOS.result_str(await EOS.get_instance().reports_interface_send_player_behavior_report_callback))





func test_progression_snapshot_interface():
	var opts1 = EOS.ProgressionSnapshot.BeginSnapshotOptions.new()
	opts1.local_user_id = Store.product_user_id
	var begin_snapshot_ret = EOS.ProgressionSnapshot.ProgressionSnapshotInterface.begin_snapshot(opts1)
	print("--- ProgressionSnapshot: begin_snapshot: ", EOS.result_str(begin_snapshot_ret))

	var key_values = {
		test_key = "test_value",
		health = "100",
		pos_x = "-158.6",
		pos_y = "14.7"
	}
	for key in key_values:
		var opts2 = EOS.ProgressionSnapshot.AddProgressionOptions.new()
		opts2.snapshot_id = begin_snapshot_ret.snapshot_id
		opts2.key = key
		opts2.value = str(key_values[key])
		print("--- ProgressionSnapshot: add_progression: ", EOS.result_str(EOS.ProgressionSnapshot.ProgressionSnapshotInterface.add_progression(opts2)))


	var opts3 = EOS.ProgressionSnapshot.SubmitSnapshotOptions.new()
	opts3.snapshot_id = begin_snapshot_ret.snapshot_id
	EOS.ProgressionSnapshot.ProgressionSnapshotInterface.submit_snapshot(opts3)
	print("--- ProgressionSnapshot: submit_snapshot_callback: ", EOS.result_str(await EOS.get_instance().progression_snapshot_interface_submit_snapshot_callback))


	var opts4 = EOS.ProgressionSnapshot.DeleteSnapshotOptions.new()
	opts4.local_user_id = Store.product_user_id
	EOS.ProgressionSnapshot.ProgressionSnapshotInterface.delete_snapshot(opts4)
	print("--- ProgressionSnapshot: delete_snapshot_callback: ", EOS.result_str(await EOS.get_instance().progression_snapshot_interface_delete_snapshot_callback))




func test_presence_interface():
	var opts1 = EOS.Presence.CreatePresenceModificationOptions.new()
	opts1.local_user_id = Store.epic_account_id
	var create_pmod_ret = EOS.Presence.PresenceInterface.create_presence_modification(opts1)
	print("--- Presence: create_presence_modification: ", EOS.result_str(create_pmod_ret))


	var presence_modification: PresenceModificationEOSG = create_pmod_ret.presence_modification
	var set_data_ret = presence_modification.set_data({
		test_key = "test_val",
		hello = "world"
	})
	print("--- PresenceModificationEOSG: set_data: ", EOS.result_str(set_data_ret))


	var delete_data_ret = presence_modification.delete_data(["hello"])
	print("--- PresenceModificationEOSG: delete_data: ", EOS.result_str(delete_data_ret))


	var set_join_info_ret = presence_modification.set_join_info("this is sample join info")
	print("--- PresenceModificationEOSG: set_join_info: ", EOS.result_str(set_join_info_ret))


	var set_raw_rich_text_ret = presence_modification.set_raw_rich_text("this is sample join info")
	print("--- PresenceModificationEOSG: set_raw_rich_text: ", EOS.result_str(set_raw_rich_text_ret))


	var set_status_ret = presence_modification.set_status(EOS.Presence.Status.Online)
	print("--- PresenceModificationEOSG: set_status: ", EOS.result_str(set_status_ret))


	var opts6 = EOS.Presence.SetPresenceOptions.new()
	opts6.local_user_id = Store.epic_account_id
	opts6.presence_modification = presence_modification
	EOS.Presence.PresenceInterface.set_presence(opts6)
	print("--- Presence: set_presence: ", EOS.result_str(await EOS.get_instance().presence_interface_set_presence_callback))


	var opts2 = EOS.Presence.GetJoinInfoOptions.new()
	opts2.local_user_id = Store.epic_account_id
	opts2.target_user_id = Store.epic_account_id
	var get_join_info_ret = EOS.Presence.PresenceInterface.get_join_info(opts2)
	print("--- Presence: get_join_info: ", EOS.result_str(get_join_info_ret))


	var opts3 = EOS.Presence.HasPresenceOptions.new()
	opts3.local_user_id = Store.epic_account_id
	opts3.target_user_id = Store.epic_account_id
	var has_presence_ret: bool = EOS.Presence.PresenceInterface.has_presence(opts3)
	print("--- Presence: has_presence: ", has_presence_ret)


	var opst4 = EOS.Presence.QueryPresenceOptions.new()
	opst4.local_user_id = Store.epic_account_id
	opst4.target_user_id = Store.epic_account_id
	EOS.Presence.PresenceInterface.query_presence(opst4)
	print("--- Presence: query_presence: ", EOS.result_str(await EOS.get_instance().presence_interface_query_presence_callback))


	var opts5 = EOS.Presence.CopyPresenceOptions.new()
	opts5.local_user_id = Store.epic_account_id
	opts5.target_user_id = Store.epic_account_id
	var copy_presence_ret = EOS.Presence.PresenceInterface.copy_presence(opts5)
	print("--- Presence: copy_presence: ", EOS.result_str(copy_presence_ret))
	print(JSON.stringify(copy_presence_ret, "\t", true, true))





func test_kws_interface():
	var opts1 = EOS.KWS.CreateUserOptions.new()
	opts1.local_user_id = Store.product_user_id
	opts1.parent_email = "3ddelano1@gmail.com"
	opts1.date_of_birth = "2002-01-01"
	EOS.KWS.KWSInterface.create_user(opts1)
	var create_user_ret = await EOS.get_instance().kws_interface_create_user_callback
	print("--- KWS: create_user_callback: ", EOS.result_str(create_user_ret))


	var opts2 = EOS.KWS.QueryPermissionsOptions.new()
	opts2.local_user_id = Store.product_user_id
	EOS.KWS.KWSInterface.query_permissions(opts2)
	print("--- KWS: query_permissions_callback: ", await EOS.get_instance().kws_interface_query_permissions_callback)

	# EOS.KWS.KWSInterface.get_permissions_count()
	# EOS.KWS.KWSInterface.get_permission_by_key()
	# KWS.KWSInterface.copy_permission_by_index()

	# KWS.KWSInterface.query_age_gate()
	# EOS.KWS.KWSInterface.request_permissions()
	# EOS.KWS.KWSInterface.update_parent_email()





func test_playerdatastorage_interface():
	# var query_file_opts = EOS.PlayerDataStorage.QueryFileOptions.new()
	# query_file_opts.local_user_id = Store.product_user_id
	# query_file_opts.filename = "testfile-001.txt"
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.query_file(query_file_opts)
	# print("--- PlayerDataStorage: query_file: ", await EOS.get_instance().playerdatastorage_interface_query_file_callback)

	# var query_file_list_opts = EOS.PlayerDataStorage.QueryFileListOptions.new()
	# query_file_list_opts.local_user_id = Store.product_user_id
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.query_file_list(query_file_list_opts)
	# print("--- PlayerDataStorage: query_file_list: ", await EOS.get_instance().playerdatastorage_interface_query_file_list_callback)

	# var get_meta_count_opts = EOS.PlayerDataStorage.GetFileMetadataCountOptions.new()
	# get_meta_count_opts.local_user_id = Store.product_user_id
	# print("--- PlayerDataStorage: get_file_metadata_count: ", EOS.PlayerDataStorage.PlayerDataStorageInterface.get_file_metadata_count(get_meta_count_opts))

	# var copy_meta_at_index_opts = EOS.PlayerDataStorage.CopyFileMetadataAtIndexOptions.new()
	# copy_meta_at_index_opts.local_user_id = Store.product_user_id
	# copy_meta_at_index_opts.index = 0
	# print("--- PlayerDataStorage: copy_file_metadata_at_index: ", EOS.PlayerDataStorage.PlayerDataStorageInterface.copy_file_metadata_at_index(copy_meta_at_index_opts))

	# var copy_meta_by_filename_opts = EOS.PlayerDataStorage.CopyFileMetadataByFilenameOptions.new()
	# copy_meta_by_filename_opts.local_user_id = Store.product_user_id
	# copy_meta_by_filename_opts.filename = "testfile-001.txt"
	# print("--- PlayerDataStorage: copy_file_metadata_by_filename: ", EOS.PlayerDataStorage.PlayerDataStorageInterface.copy_file_metadata_by_filename(copy_meta_by_filename_opts))

	# var duplicate_opts = EOS.PlayerDataStorage.DuplicateFileOptions.new()
	# duplicate_opts.local_user_id = Store.product_user_id
	# duplicate_opts.source_filename = "testfile-001.txt"
	# duplicate_opts.destination_filename = "testfile-001-copy.txt"
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.duplicate_file(duplicate_opts)
	# print("--- PlayerDataStorage: duplicate_file: ", await EOS.get_instance().playerdatastorage_interface_duplicate_file_callback)

	# var delete_file_opts = EOS.PlayerDataStorage.DeleteFileOptions.new()
	# delete_file_opts.local_user_id = Store.product_user_id
	# delete_file_opts.filename = "testfile-001-copy.txt"
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.delete_file(delete_file_opts)
	# print("--- PlayerDataStorage: delete_file: ", await EOS.get_instance().playerdatastorage_interface_delete_file_callback)

	# var delete_cache_opts = EOS.PlayerDataStorage.DeleteCacheOptions.new()
	# delete_cache_opts.local_user_id = Store.product_user_id
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.delete_cache(delete_cache_opts)
	# print("--- PlayerDataStorage: delete_cache: ", await EOS.get_instance().playerdatastorage_interface_delete_cache_callback)

	EOS.get_instance().playerdatastorage_interface_file_transfer_progress_callback.connect(func (data):
		print("--- PlayerDataStorage: file_transfer_progress_callback: ", data)
	)
	EOS.get_instance().playerdatastorage_interface_write_file_callback.connect(func (data):
		print("--- PlayerDataStorage: write_file_callback: ", data)
	)
	EOS.get_instance().playerdatastorage_interface_write_file_data_callback.connect(func (data):
		print("--- PlayerDataStorage: write_file_data_callback: ", data)
	)
	EOS.get_instance().playerdatastorage_interface_read_file_callback.connect(func (data):
		print("--- PlayerDataStorage: read_file_callback: ", data)
	)
	EOS.get_instance().playerdatastorage_interface_read_file_data_callback.connect(func (data):
		print("--- PlayerDataStorage: read_file_data_callback: ", data)
	)

	var write_file_opts = EOS.PlayerDataStorage.WriteFileOptions.new()
	write_file_opts.local_user_id = Store.product_user_id
	write_file_opts.filename = "testfile-002.txt"
	write_file_opts.data = PackedByteArray([65, 66, 67])
	var write_transfer_request: EOSGFileTransferRequest = EOS.PlayerDataStorage.PlayerDataStorageInterface.write_file(write_file_opts)
	# write_transfer_request.cancel_request()
	await EOS.get_instance().playerdatastorage_interface_write_file_callback

	print("Reading file")
	var read_file_opts = EOS.PlayerDataStorage.ReadFileOptions.new()
	read_file_opts.local_user_id = Store.product_user_id
	read_file_opts.filename = "testfile-002.txt"
	var read_transfer_request: EOSGFileTransferRequest = EOS.PlayerDataStorage.PlayerDataStorageInterface.read_file(read_file_opts)
	# print(file_transfer_request.get_filename())
	# print(file_transfer_request.cancel_request())




func get_view_manager():
	return views


func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		print("Shutting down EOS...")
		EOS.Platform.PlatformInterface.release()
		var _res = EOS.Platform.PlatformInterface.shutdown()

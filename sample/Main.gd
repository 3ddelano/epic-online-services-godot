extends Control

@onready var PRODUCT_NAME: String = Env.get_var("PRODUCT_NAME") # Paste your own
@onready var PRODUCT_VERSION: String = Env.get_var("PRODUCT_VERSION") # Paste your own
@onready var PRODUCT_ID: String = Env.get_var("PRODUCT_ID") # Paste your own
@onready var SANDBOX_ID: String = Env.get_var("SANDBOX_ID") # Paste your own
@onready var DEPLOYMENT_ID: String = Env.get_var("DEPLOYMENT_ID") # Paste your own
@onready var CLIENT_ID: String = Env.get_var("CLIENT_ID") # Paste your own
@onready var CLIENT_SECRET: String = Env.get_var("CLIENT_SECRET") # Paste your own
@onready var ENCRYPTION_KEY: String = Env.get_var("ENCRYPTION_KEY") # Paste your own
@onready var IS_ANTICHEAT_SERVER: String = Env.get_var("IS_ANTICHEAT_SERVER", "false")

@export var _views_path: NodePath
@onready var views = get_node(_views_path) as VBoxContainer

var anticheat_server_main = AntiCheatServerMain.new()

func _ready() -> void:
	if IS_ANTICHEAT_SERVER != "false":
		add_child(anticheat_server_main)
		return

	print("Ready!")
	Store._main_node = self

	# -----
	# EOS Setup
	# -----

	# Initialize the SDK
	var init_opts = EOS.Platform.InitializeOptions.new()
	init_opts.product_name = PRODUCT_NAME
	init_opts.product_version = PRODUCT_VERSION
	var init_res := EOS.Platform.PlatformInterface.initialize(init_opts)
	var init_retry_count = 10
	while not EOS.is_success(init_res) and init_retry_count > 0:
		init_res = EOS.Platform.PlatformInterface.initialize(init_opts)
		init_retry_count -= 1
		await get_tree().create_timer(0.2).timeout

	if not EOS.is_success(init_res):
		print("Failed to initialize EOS SDK: ", EOS.result_str(init_res))
		return
	print("Initialized EOS platform")

	# Create platform
	var create_opts = EOS.Platform.CreateOptions.new()
	create_opts.product_id = PRODUCT_ID
	create_opts.sandbox_id = SANDBOX_ID
	create_opts.deployment_id = DEPLOYMENT_ID
	create_opts.client_id = CLIENT_ID
	create_opts.client_secret = CLIENT_SECRET
	create_opts.encryption_key = ENCRYPTION_KEY
	if OS.get_name() == "Windows":
		create_opts.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl

	var create_success: bool = EOS.Platform.PlatformInterface.create(create_opts)
	var create_retry_count = 10
	while not create_success&&create_retry_count > 0:
		create_success = EOS.Platform.PlatformInterface.create(create_opts)
		create_retry_count -= 1
		await get_tree().create_timer(0.2).timeout
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
#	test_connect_interface()
#	test_ecom_interface()
#	test_kws_interface()
#	test_mods_interface()
#	test_platform_interface()
#	test_playerdatastorage_interface()
#	test_presence_interface()
#	test_progression_snapshot_interface()
#	test_reports_interface()
#	test_sanctions_interface()
#	test_sessions_interface()
#	test_titlestorage_interface()
#	test_user_info_interface()
#	test_rtc_interface()
#	test_anticheat_server_interface()
#	test_anticheat_client_interface()

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
	var verify_user_auth_ret = await IEOS.auth_interface_verify_user_auth_callback
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
	opts1.account_id = Store.epic_account_id
	print("--- Connect: copy_product_user_external_account: By account Id: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_id(opts1))

	var opts2 = EOS.Connect.CopyProductUserExternalAccountByAccountTypeOptions.new()
	opts2.account_id_type = EOS.ExternalAccountType.Epic
	print("--- Connect: copy_product_user_external_account: By account type: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_account_type(opts2))

	var opts3 = EOS.Connect.CopyProductUserExternalAccountByIndexOptions.new()
	opts3.external_account_info_index = 0
	print("--- Connect: copy_product_user_external_account: By account index: ", EOS.Connect.ConnectInterface.copy_product_user_external_account_by_index(opts3))

	var opts4 = EOS.Connect.CopyProductUserInfoOptions.new()
	print("--- Connect: copy_product_user_info: ", EOS.Connect.ConnectInterface.copy_product_user_info(opts4))

	var opts5 = EOS.Connect.CopyIdTokenOptions.new()
	var ret5 = EOS.Connect.ConnectInterface.copy_id_token(opts5)
	var id_token = EOS.Connect.IdToken.new()
	id_token.json_web_token = ret5.id_token.json_web_token
	id_token.product_user_id = ret5.id_token.product_user_id
	print("-- Connect: copy_id_token: Id token: ", id_token)

	var opts11 = EOS.Connect.QueryExternalAccountMappingsOptions.new()
	opts11.account_id_type = EOS.ExternalAccountType.Epic
	opts11.external_account_ids = [Store.second_epic_account_id]
	EOS.Connect.ConnectInterface.query_external_account_mappings(opts11)
	print("--- Connect: query_external_account_mappings: ", await IEOS.connect_interface_query_external_account_mappings_callback)

	var opts6 = EOS.Connect.GetExternalAccountMappingsOptions.new()
	opts6.account_id_type = EOS.ExternalAccountType.Epic
	opts6.target_external_user_id = Store.second_epic_account_id
	print("--- Connect: get_external_account_mapping: ", EOS.Connect.ConnectInterface.get_external_account_mapping(opts6))

	print("--- Connect: get_logged_in_user_by_index(0): ", EOS.Connect.ConnectInterface.get_logged_in_user_by_index(0))
	print("--- Connect: get_logged_in_users_count: ", EOS.Connect.ConnectInterface.get_logged_in_users_count())
	print("--- Connecet: get_login_status: ", EOS.Connect.ConnectInterface.get_login_status(Store.product_user_id))

	var opts7 = EOS.Connect.GetProductUserExternalAccountCountOptions.new()
	print("--- Connect: get_product_user_external_account_count: ", EOS.Connect.ConnectInterface.get_product_user_external_account_count(opts7))

	var opts8 = EOS.Connect.GetProductUserIdMappingOptions.new()
	opts8.account_id_type = EOS.ExternalAccountType.Discord
	opts8.target_product_user_id = "External Account Id Here"
	print("--- Connect: get_product_user_id_mapping: ", EOS.Connect.ConnectInterface.get_product_user_id_mapping(opts8))

	var opts9 = EOS.Connect.LinkAccountOptions.new()
#	opts9.continuance_token = ContinunanceTokenEOSG object here
	EOS.Connect.ConnectInterface.link_account(opts9)
	print(await IEOS.connect_interface_link_account_callback)

	var opts10 = EOS.Connect.VerifyIdTokenOptions.new()
	opts10.id_token = id_token
	EOS.Connect.ConnectInterface.verify_id_token(opts10)
	print("--- Connect: verify_id_token: ", await IEOS.connect_interface_verify_id_token_callback)

func test_ecom_interface():
	var offers_options = EOS.Ecom.QueryOffersOptions.new()
	EOS.Ecom.EcomInterface.query_offers(offers_options)
	print("--- Ecom: query_offers: ", await IEOS.ecom_interface_query_offers_callback)

	var checkout_options = EOS.Ecom.CheckoutOptions.new()
	checkout_options.entries = [{
		offer_id = "OFFER ID HERE"
	}]
	EOS.Ecom.EcomInterface.checkout(checkout_options)
	var checkout_data = await IEOS.ecom_interface_checkout_callback
	print("--- Ecom: checkout: ", checkout_data)

func test_user_info_interface():
	var opts1 = EOS.UserInfo.QueryUserInfoOptions.new()
	EOS.UserInfo.UserInfoInterface.query_user_info(opts1)
	print("--- UserInfo: query_user_info: ", EOS.result_str(await IEOS.user_info_interface_query_user_info_callback))

	var opts2 = EOS.UserInfo.CopyUserInfoOptions.new()
	print("--- UserInfo: copy_user_info: ", EOS.UserInfo.UserInfoInterface.copy_user_info(opts2))

	var opts3 = EOS.UserInfo.QueryUserInfoByDisplayNameOptions.new()
	opts3.display_name = "3ddelano"
	EOS.UserInfo.UserInfoInterface.query_user_info_by_display_name(opts3)
	print("--- UserInfo: query_user_info_by_display_name: ", EOS.result_str(await IEOS.user_info_interface_query_user_info_by_display_name_callback))

	var opts4 = EOS.UserInfo.QueryUserInfoByExternalAccountOptions.new()
	opts4.account_type = EOS.ExternalAccountType.Epic
	opts4.external_account_id = Store.epic_account_id
	EOS.UserInfo.UserInfoInterface.query_user_info_by_external_account(opts4)
	print("--- UserInfo: query_user_info_by_external_account: ", EOS.result_str(await IEOS.user_info_interface_query_user_info_by_external_account_callback))

	var opts5 = EOS.UserInfo.CopyExternalUserInfoByAccountIdOptions.new()
	opts5.account_id = Store.epic_account_id
	print("--- UserInfo: copy_external_user_info_by_account_id: ", EOS.UserInfo.UserInfoInterface.copy_external_user_info_by_account_id(opts5))

	var opts6 = EOS.UserInfo.CopyExternalUserInfoByAccountTypeOptions.new()
	opts6.account_type = EOS.ExternalAccountType.Epic
	EOS.UserInfo.UserInfoInterface.copy_external_user_info_by_account_type(opts6)

	var opts7 = EOS.UserInfo.CopyExternalUserInfoByIndexOptions.new()
	opts7.index = 0
	EOS.UserInfo.UserInfoInterface.copy_external_user_info_by_index(opts7)

	var opts8 = EOS.UserInfo.GetExternalUserInfoCountOptions.new()
	EOS.UserInfo.UserInfoInterface.get_external_user_info_count(opts8)

func test_mods_interface():
	var opts1 = EOS.Mods.EnumerateModsOptions.new()
	opts1.type = EOS.Mods.ModEnumerationType.AllAvailable
	EOS.Mods.ModsInterface.enumerate_mods(opts1)
	print("--- Mods: enumerate_mods: ", EOS.result_str(await IEOS.mods_interface_enumerate_mods_callback))

	var opts2 = EOS.Mods.CopyModInfoOptions.new()
	opts2.type = EOS.Mods.ModEnumerationType.AllAvailable
	var copy_mods_info_data = EOS.Mods.ModsInterface.copy_mod_info(opts2)
	print("--- Mods: copy_mod_info: ", copy_mods_info_data)

	var mod = {}
	if copy_mods_info_data.mods != null:
		mod = copy_mods_info_data.mods.mods[0]

	var opts3 = EOS.Mods.InstallModOptions.new()
	opts3.mod = mod
	EOS.Mods.ModsInterface.install_mod(opts3)
	print("--- Mods: install_mod: ", EOS.result_str(await IEOS.mods_interface_install_mod_callback))

func test_reports_interface():
	var report_options = EOS.Reports.SendPlayerBehaviorReportOptions.new()
	report_options.reported_user_id = Store.second_product_user_id
	report_options.category = EOS.Reports.PlayerReportsCategory.Cheating
	report_options.message = "this is a test report from godot"
	report_options.context = JSON.stringify({hello="testing"})
	EOS.Reports.ReportsInterface.send_player_behavior_report(report_options)
	print("--- Reports: send_player_behavior_report: ", EOS.result_str(await IEOS.reports_interface_send_player_behavior_report_callback))

func test_progression_snapshot_interface():
	var opts1 = EOS.ProgressionSnapshot.BeginSnapshotOptions.new()
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
	print("--- ProgressionSnapshot: submit_snapshot_callback: ", EOS.result_str(await IEOS.progression_snapshot_interface_submit_snapshot_callback))

	var opts4 = EOS.ProgressionSnapshot.DeleteSnapshotOptions.new()
	EOS.ProgressionSnapshot.ProgressionSnapshotInterface.delete_snapshot(opts4)
	print("--- ProgressionSnapshot: delete_snapshot_callback: ", EOS.result_str(await IEOS.progression_snapshot_interface_delete_snapshot_callback))

func test_presence_interface():
	var opts1 = EOS.Presence.CreatePresenceModificationOptions.new()
	var create_pmod_ret = EOS.Presence.PresenceInterface.create_presence_modification(opts1)
	print("--- Presence: create_presence_modification: ", EOS.result_str(create_pmod_ret))

	var presence_modification: EOSGPresenceModification = create_pmod_ret.presence_modification
	var set_data_ret = presence_modification.set_data({
		test_key="test_val",
		hello="world"
	})
	print("--- EOSGPresenceModification: set_data: ", EOS.result_str(set_data_ret))

	var delete_data_ret = presence_modification.delete_data(["hello"])
	print("--- EOSGPresenceModification: delete_data: ", EOS.result_str(delete_data_ret))

	var set_join_info_ret = presence_modification.set_join_info("this is sample join info")
	print("--- EOSGPresenceModification: set_join_info: ", EOS.result_str(set_join_info_ret))

	var set_raw_rich_text_ret = presence_modification.set_raw_rich_text("this is sample join info")
	print("--- EOSGPresenceModification: set_raw_rich_text: ", EOS.result_str(set_raw_rich_text_ret))

	var set_status_ret = presence_modification.set_status(EOS.Presence.Status.Online)
	print("--- EOSGPresenceModification: set_status: ", EOS.result_str(set_status_ret))

	var opts6 = EOS.Presence.SetPresenceOptions.new()
	opts6.presence_modification = presence_modification
	EOS.Presence.PresenceInterface.set_presence(opts6)
	print("--- Presence: set_presence: ", EOS.result_str(await IEOS.presence_interface_set_presence_callback))

	var opts2 = EOS.Presence.GetJoinInfoOptions.new()
	var get_join_info_ret = EOS.Presence.PresenceInterface.get_join_info(opts2)
	print("--- Presence: get_join_info: ", EOS.result_str(get_join_info_ret))

	var opts3 = EOS.Presence.HasPresenceOptions.new()
	var has_presence_ret: bool = EOS.Presence.PresenceInterface.has_presence(opts3)
	print("--- Presence: has_presence: ", has_presence_ret)

	var opst4 = EOS.Presence.QueryPresenceOptions.new()
	EOS.Presence.PresenceInterface.query_presence(opst4)
	print("--- Presence: query_presence: ", EOS.result_str(await IEOS.presence_interface_query_presence_callback))

	var opts5 = EOS.Presence.CopyPresenceOptions.new()
	var copy_presence_ret = EOS.Presence.PresenceInterface.copy_presence(opts5)
	print("--- Presence: copy_presence: ", EOS.result_str(copy_presence_ret))
	print(JSON.stringify(copy_presence_ret, "\t", true, true))

func test_kws_interface():
	var opts1 = EOS.KWS.CreateUserOptions.new()
	opts1.parent_email = "3ddelano1@gmail.com"
	opts1.date_of_birth = "2002-01-01"
	EOS.KWS.KWSInterface.create_user(opts1)
	var create_user_ret = await IEOS.kws_interface_create_user_callback
	print("--- KWS: create_user_callback: ", EOS.result_str(create_user_ret))

	var opts2 = EOS.KWS.QueryPermissionsOptions.new()
	EOS.KWS.KWSInterface.query_permissions(opts2)
	print("--- KWS: query_permissions_callback: ", await IEOS.kws_interface_query_permissions_callback)

	# EOS.KWS.KWSInterface.get_permissions_count()
	# EOS.KWS.KWSInterface.get_permission_by_key()
	# KWS.KWSInterface.copy_permission_by_index()

	# KWS.KWSInterface.query_age_gate()
	# EOS.KWS.KWSInterface.request_permissions()
	# EOS.KWS.KWSInterface.update_parent_email()

func test_playerdatastorage_interface():
	# var query_file_opts = EOS.PlayerDataStorage.QueryFileOptions.new()
	# query_file_opts.filename = "testfile-001.txt"
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.query_file(query_file_opts)
	# print("--- PlayerDataStorage: query_file: ", await IEOS.playerdatastorage_interface_query_file_callback)

	# var query_file_list_opts = EOS.PlayerDataStorage.QueryFileListOptions.new()
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.query_file_list(query_file_list_opts)
	# print("--- PlayerDataStorage: query_file_list: ", await IEOS.playerdatastorage_interface_query_file_list_callback)

	# var get_meta_count_opts = EOS.PlayerDataStorage.GetFileMetadataCountOptions.new()
	# print("--- PlayerDataStorage: get_file_metadata_count: ", EOS.PlayerDataStorage.PlayerDataStorageInterface.get_file_metadata_count(get_meta_count_opts))

	# var copy_meta_at_index_opts = EOS.PlayerDataStorage.CopyFileMetadataAtIndexOptions.new()
	# copy_meta_at_index_opts.index = 0
	# print("--- PlayerDataStorage: copy_file_metadata_at_index: ", EOS.PlayerDataStorage.PlayerDataStorageInterface.copy_file_metadata_at_index(copy_meta_at_index_opts))

	# var copy_meta_by_filename_opts = EOS.PlayerDataStorage.CopyFileMetadataByFilenameOptions.new()
	# copy_meta_by_filename_opts.filename = "testfile-001.txt"
	# print("--- PlayerDataStorage: copy_file_metadata_by_filename: ", EOS.PlayerDataStorage.PlayerDataStorageInterface.copy_file_metadata_by_filename(copy_meta_by_filename_opts))

	# var duplicate_opts = EOS.PlayerDataStorage.DuplicateFileOptions.new()
	# duplicate_opts.source_filename = "testfile-001.txt"
	# duplicate_opts.destination_filename = "testfile-001-copy.txt"
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.duplicate_file(duplicate_opts)
	# print("--- PlayerDataStorage: duplicate_file: ", await IEOS.playerdatastorage_interface_duplicate_file_callback)

	# var delete_file_opts = EOS.PlayerDataStorage.DeleteFileOptions.new()
	# delete_file_opts.filename = "testfile-001-copy.txt"
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.delete_file(delete_file_opts)
	# print("--- PlayerDataStorage: delete_file: ", await IEOS.playerdatastorage_interface_delete_file_callback)

	# var delete_cache_opts = EOS.PlayerDataStorage.DeleteCacheOptions.new()
	# EOS.PlayerDataStorage.PlayerDataStorageInterface.delete_cache(delete_cache_opts)
	# print("--- PlayerDataStorage: delete_cache: ", await IEOS.playerdatastorage_interface_delete_cache_callback)

	IEOS.playerdatastorage_interface_file_transfer_progress_callback.connect(func(data):
		print("--- PlayerDataStorage: file_transfer_progress_callback: ", data)
	)
	IEOS.playerdatastorage_interface_write_file_callback.connect(func(data):
		print("--- PlayerDataStorage: write_file_callback: ", data)
	)
	IEOS.playerdatastorage_interface_write_file_data_callback.connect(func(data):
		print("--- PlayerDataStorage: write_file_data_callback: ", data)
	)
	IEOS.playerdatastorage_interface_read_file_callback.connect(func(data):
		print("--- PlayerDataStorage: read_file_callback: ", data)
	)
	IEOS.playerdatastorage_interface_read_file_data_callback.connect(func(data):
		print("--- PlayerDataStorage: read_file_data_callback: ", data)
	)

	var write_file_opts = EOS.PlayerDataStorage.WriteFileOptions.new()
	write_file_opts.filename = "testfile-002.txt"
	write_file_opts.data = PackedByteArray([65, 66, 67])
	var _write_transfer_request: EOSGFileTransferRequest = EOS.PlayerDataStorage.PlayerDataStorageInterface.write_file(write_file_opts)
	# write_transfer_request.cancel_request()
	await IEOS.playerdatastorage_interface_write_file_callback

	print("Reading file")
	var read_file_opts = EOS.PlayerDataStorage.ReadFileOptions.new()
	read_file_opts.filename = "testfile-002.txt"
	var _read_transfer_request: EOSGFileTransferRequest = EOS.PlayerDataStorage.PlayerDataStorageInterface.read_file(read_file_opts)
	# print(read_transfer_request.get_filename())
	# print(read_transfer_request.cancel_request())

func test_titlestorage_interface():
	IEOS.titlestorage_interface_file_transfer_progress_callback.connect(func(data):
		print("--- TitleStorage: file_transfer_progress_callback: ", data)
	)
	IEOS.titlestorage_interface_read_file_callback.connect(func(data):
		print("--- TitleStorage: read_file_callback: ", data)
	)
	IEOS.titlestorage_interface_read_file_data_callback.connect(func(data):
		print("--- TitleStorage: read_file_data_callback: ", data)
	)

	var read_file_opts = EOS.TitleStorage.ReadFileOptions.new()
	read_file_opts.filename = "title-001.txt"
	var _read_transfer_request: EOSGFileTransferRequest = EOS.TitleStorage.TitleStorageInterface.read_file(read_file_opts)
	#print(read_transfer_request.get_filename())
	#print(read_transfer_request.cancel_request())

	var query_file_opts = EOS.TitleStorage.QueryFileOptions.new()
	query_file_opts.filename = "title-001.txt"
	EOS.TitleStorage.TitleStorageInterface.query_file(query_file_opts)
	print("--- TitleStorage: query_file: ", await IEOS.titlestorage_interface_query_file_callback)

	var query_file_list_opts = EOS.TitleStorage.QueryFileListOptions.new()
	query_file_list_opts.list_of_tags = ["hey"]
	EOS.TitleStorage.TitleStorageInterface.query_file_list(query_file_list_opts)
	print("--- TitleStorage: query_file_list: ", await IEOS.titlestorage_interface_query_file_list_callback)

	var get_meta_count_opts = EOS.TitleStorage.GetFileMetadataCountOptions.new()
	print("--- TitleStorage: get_file_metadata_count: ", EOS.TitleStorage.TitleStorageInterface.get_file_metadata_count(get_meta_count_opts))

	var copy_meta_at_index_opts = EOS.TitleStorage.CopyFileMetadataAtIndexOptions.new()
	copy_meta_at_index_opts.index = 0
	print("--- TitleStorage: copy_file_metadata_at_index: ", EOS.TitleStorage.TitleStorageInterface.copy_file_metadata_at_index(copy_meta_at_index_opts))

	var copy_meta_by_filename_opts = EOS.TitleStorage.CopyFileMetadataByFilenameOptions.new()
	copy_meta_by_filename_opts.filename = "title-001.txt"
	print("--- TitleStorage: copy_file_metadata_by_filename: ", EOS.TitleStorage.TitleStorageInterface.copy_file_metadata_by_filename(copy_meta_by_filename_opts))

	var delete_cache_opts = EOS.TitleStorage.DeleteCacheOptions.new()
	EOS.TitleStorage.TitleStorageInterface.delete_cache(delete_cache_opts)
	print("--- TitleStorage: delete_cache: ", await IEOS.titlestorage_interface_delete_cache_callback)

func test_sanctions_interface():
	var query_active_sanctions_opts = EOS.Sanctions.QueryActivePlayerSanctionsOptions.new()
	EOS.Sanctions.SanctionsInterface.query_active_player_sanctions(query_active_sanctions_opts)

	print("--- Sanctions: query_active_player_sanctions: ", await IEOS.sanctions_interface_query_active_player_sanctions_callback)

	var get_player_sanction_count_opts = EOS.Sanctions.GetPlayerSanctionCountOptions.new()
	var sanctions_count = EOS.Sanctions.SanctionsInterface.get_player_sanction_count(get_player_sanction_count_opts)
	print("--- Sanctions: get_active_player_sanction_count: ", sanctions_count)

	for i in range(sanctions_count):
		var copy_active_sanction_by_index_opts = EOS.Sanctions.CopyPlayerSanctionByIndexOptions.new()
		copy_active_sanction_by_index_opts.sanction_index = i
		print("--- Sanctions: copy_active_player_sanction_by_index(%s): " % i, EOS.Sanctions.SanctionsInterface.copy_player_sanction_by_index(copy_active_sanction_by_index_opts))

func test_sessions_interface():
	var bucket_id = "mode:region01:map01"

	var create_sess_mod_opts = EOS.Sessions.CreateSessionModificationOptions.new()
	create_sess_mod_opts.session_name = "TestSession001"
	create_sess_mod_opts.bucket_id = bucket_id
	create_sess_mod_opts.max_players = 16
	create_sess_mod_opts.presence_enabled = true
	create_sess_mod_opts.sanctions_enabled = false
	var create_sess_mod = EOS.Sessions.SessionsInterface.create_session_modification(create_sess_mod_opts)
	print("--- Sessions: create_session_modification: ", create_sess_mod)

	var session_mod: EOSGSessionModification = create_sess_mod.session_modification

	print("--- Sessions: SessionModification: set_permission_level: ", session_mod.set_permission_level(EOS.Sessions.OnlineSessionPermissionLevel.PublicAdvertised))
	print("--- Sessions: SessionModification: set_join_in_progess_allowed: ", session_mod.set_join_in_progress_allowed(true))
	print("--- Sessions: SessionModification: set_invites_allowed: ", session_mod.set_invites_allowed(true))
	print("--- Sessions: SessionModification: add_attribute: ", session_mod.add_attribute(EOS.Sessions.SEARCH_BUCKET_ID, bucket_id, EOS.Sessions.SessionAttributeAdvertisementType.Advertise))

	var update_sess_opts = EOS.Sessions.UpdateSessionOptions.new()
	update_sess_opts.session_modification = session_mod
	EOS.Sessions.SessionsInterface.update_session(update_sess_opts)
	session_mod = null

	var update_sess = await IEOS.sessions_interface_update_session_callback
	print("--- Sessions: update_session: ", update_sess)

	#var dump_sess_opts = EOS.Sessions.DumpSessionStateOptions.new()
	#dump_sess_opts.session_name = "TestSession001"
	#print("--- Sessions: dump_session_state: ", EOS.Sessions.SessionsInterface.dump_session_state(dump_sess_opts))

	#var copy_active_session_opts = EOS.Sessions.CopyActiveSessionDetailsOptions.new()
	#copy_active_session_opts.session_name = "TestSession001"
	#var copy_active_session = EOS.Sessions.SessionsInterface.copy_active_session_details(copy_active_session_opts)
	#print("--- Sessions: copy_active_session_details: ", copy_active_session)

	#var active_session: EOSGActiveSession = copy_active_session.active_session
	#print("--- Sessions: ActiveSession: copy_info:", active_session.copy_info())

	# Modify session
	# var update_sess_mod_opts = EOS.Sessions.UpdateSessionModificationOptions.new()
	# update_sess_mod_opts.session_name = "TestSession001"
	# var update_sess_mod = EOS.Sessions.SessionsInterface.update_session_modification(update_sess_mod_opts)
	# print("--- Sessions: update_session_modification: ", update_sess_mod)
	# session_mod = update_sess_mod.session_modification

	# print("--- Sessions: SessionModification: set_invites_allowed: ", session_mod.set_invites_allowed(false))

	# update_sess_opts = EOS.Sessions.UpdateSessionOptions.new()
	# update_sess_opts.session_modification = session_mod
	# EOS.Sessions.SessionsInterface.update_session(update_sess_opts)

	# update_sess = await IEOS.sessions_interface_update_session_callback
	# print("--- Sessions: update_session: ", update_sess)

	# Search session
	#var create_search_opts = EOS.Sessions.CreateSessionSearchOptions.new()
	#var create_session_search = EOS.Sessions.SessionsInterface.create_session_search(create_search_opts)
	#print("--- Sessions: create_session_search: ", create_session_search)

	#var session_search: EOSGSessionSearch = create_session_search.session_search
	#session_search.set_parameter(EOS.Sessions.SEARCH_BUCKET_ID, bucket_id, EOS.ComparisonOp.Equal)
	#session_search.find(EOSGRuntime.local_product_user_id)
	#print("--- Sessions: SessionSearch: find: ", await IEOS.session_search_find_callback)

	#var search_result_count = session_search.get_search_result_count()
	#print("--- Sessions: SessionSearch: get_search_result_count: ", search_result_count)

	#for i in range(search_result_count):
		#var search_result = session_search.copy_search_result_by_index(i)
		#print("--- Sessions: copy_search_result_by_index(%d): " % i, search_result)

		#var session_details: EOSGSessionDetails = search_result.session_details
		#print("--- Sessions: SessionDetails: copy_info: ", session_details.copy_info())

	# Join session
	# var join_sess_opts = EOS.Sessions.JoinSessionOptions.new()
	# join_sess_opts.session_name = "TestSession001"
	# join_sess_opts.presence_enabled = true
	# join_sess_opts.session_details = EOSGSesssionDetails here
	# EOS.Sessions.SessionsInterface.join_session(join_sess_opts)
	# print("--- Sessions: join_session: ", await IEOS.sessions_interface_join_session_callback)

	# Register players
	var reg_players_opts = EOS.Sessions.RegisterPlayersOptions.new()
	reg_players_opts.session_name = "TestSession001"
	reg_players_opts.players_to_register = [EOSGRuntime.local_product_user_id]
	EOS.Sessions.SessionsInterface.register_players(reg_players_opts)
	print("--- Sessions: register_players: ", await IEOS.sessions_interface_register_players_callback)

	# Start session
	var start_sess_opts = EOS.Sessions.StartSessionOptions.new()
	start_sess_opts.session_name = "TestSession001"

	EOS.Sessions.SessionsInterface.start_session(start_sess_opts)
	print("--- Sessions: start_session: ", await IEOS.sessions_interface_start_session_callback)

	# Destroy session
	var destroy_sess_opts = EOS.Sessions.DestroySessionOptions.new()
	destroy_sess_opts.session_name = "TestSession001"
	EOS.Sessions.SessionsInterface.destroy_session(destroy_sess_opts)
	print("--- Sessions: destroy_session: ", await IEOS.sessions_interface_destroy_session_callback)

func test_rtc_interface():
	#TODO: test rtc interface
	pass

func test_anticheat_server_interface():
	IEOS.anticheatserver_interface_message_to_client_callback.connect(func(data):
		print("--- AntiCheatServer: message_to_client_callback: ", data)
	)

	IEOS.anticheatserver_interface_client_action_required_callback.connect(func(data):
		print("--- AntiCheatServer: client_action_required_callback: ", data)
	)

	IEOS.anticheatserver_interface_client_auth_status_changed_callback.connect(func(data):
		print("--- AntiCheatServer: client_auth_status_changed_callback: ", data)
	)

	var begin_sess_opts = EOS.AntiCheatServer.BeginSessionOptions.new()
	begin_sess_opts.register_timeout_seconds = 60
	begin_sess_opts.server_name = "Godot 4.3 server"
	begin_sess_opts.enable_gameplay_data = false

	print("--- AntiCheatServer: begin_session: ", EOS.AntiCheatServer.AntiCheatServerInterface.begin_session(begin_sess_opts))

	#print("--- AntiCheatServer: end_session: ", EOS.AntiCheatServer.AntiCheatServerInterface.end_session())

	var reg_client_opts = EOS.AntiCheatServer.RegisterClientOptions.new()
	reg_client_opts.client_handle = "client001"
	reg_client_opts.client_type = EOS.AntiCheatCommonClientType.ProtectedClient
	reg_client_opts.client_platform = EOS.AntiCheatCommonClientPlatform.Windows
	reg_client_opts.ip_address = "192.168.4.1"
	reg_client_opts.user_id = EOSGRuntime.local_product_user_id

	print("--- AntiCheatServer: register_client: ", EOS.AntiCheatServer.AntiCheatServerInterface.register_client(reg_client_opts))

func logger(msg: String):
	var logs = Store.get_view("Logs")
	logs.log_msg(EOS.Logging.LogLevel.Info, msg)

func test_anticheat_client_interface():
	var copy_id_token_opts = EOS.Connect.CopyIdTokenOptions.new()
	var id_token_ret = EOS.Connect.ConnectInterface.copy_id_token(copy_id_token_opts)

	var jwt = id_token_ret.id_token.json_web_token
	var product_user_id = id_token_ret.id_token.product_user_id

	IEOS.anticheat_client_interface_message_to_server_callback.connect(func(data):
		logger("--- AntiCheatClient: message_to_server_callback: " + str(data))
	)

	var begin_sess_opts = EOS.AntiCheatClient.BeginSessionOptions.new()
	begin_sess_opts.mode = EOS.AntiCheatClientMode.ClientServer
	begin_sess_opts.local_user_id = product_user_id
	logger("--- AntiCheatClient: begin_session: " + EOS.result_str(EOS.AntiCheatClient.AntiCheatClientInterface.begin_session(begin_sess_opts)))

	multiplayer.connected_to_server.connect(func():
		logger("--- AntiCheatClient:: connected_to_server")
		_send_msg_to_server.rpc_id(1, product_user_id, jwt, EOS.AntiCheatCommonClientPlatform.Windows)
	)
	multiplayer.server_disconnected.connect(func():
		logger("--- AntiCheatClient:: server_disconnected")
	)

	var peer = ENetMultiplayerPeer.new()
	peer.create_client("127.0.0.1", 12345)
	multiplayer.multiplayer_peer = peer

@rpc("any_peer", "reliable", "call_remote")
func _send_msg_to_server(local_user_id: String, jwt: String, mode: int):
	var peer_id = multiplayer.get_remote_sender_id()
	print("--- AntiCheatServer: _send_msg_to_server: got rpc: peer: %s, local_user_id: %d" % [peer_id, local_user_id])
	anticheat_server_main.on_client_message_receive(peer_id, "register", {
		local_user_id=local_user_id,
		jwt=jwt,
		mode=mode
	})

func get_view_manager():
	return views

func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		print("Shutting down EOS...")
		EOS.Platform.PlatformInterface.release()
		var _res = EOS.Platform.PlatformInterface.shutdown()

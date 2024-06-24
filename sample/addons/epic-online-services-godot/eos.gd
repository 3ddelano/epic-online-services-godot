# Copyright (c) 2023-present Delano Lourenco
# https://github.com/3ddelano/epic-online-services-godot/
# MIT License

## The main class to interact with Epic Online Services
class_name EOS
extends RefCounted

static func get_instance():
	return IEOS

## Pretty prints the [enum Result] code and its string representation.[br]
## [code]p_result[/code] is a [enum Result] or a [Dictionary] with a [code]result_code[/code] key
static func print_result(p_result) -> void:
	print_rich("[b]EOS_Result[/b]:%s[code](%s)[/code]" % [result_str(p_result), p_result])

## Returns a string representation of the [enum Result] code.[br]
## [code]p_result[/code] is a [enum Result] or a [Dictionary] with a [code]result_code[/code] key
static func result_str(p_result) -> String:
	if typeof(p_result) == TYPE_DICTIONARY:
		p_result = p_result["result_code"]
	var idx := Result.values().find(p_result)
	return Result.keys()[idx]

## Returns whether the operation was completed.[br]
## [code]p_result[/code] is a [enum Result] or a [Dictionary] with a [code]result_code[/code] key
static func is_operation_complete(p_result) -> bool:
	if typeof(p_result) == TYPE_DICTIONARY:
		p_result = p_result["result_code"]
	return IEOS.is_operation_complete(p_result)

## Returns whether the operation was successful.[br]
## [code]p_result[/code] is a [enum Result] or a [Dictionary] with a [code]result_code[/code] key
static func is_success(p_result) -> bool:
	if typeof(p_result) == TYPE_BOOL:
		return p_result == true
	if typeof(p_result) == TYPE_DICTIONARY:
		p_result = p_result["result_code"]
	return p_result == Result.Success

class Achievements:
	const UNLOCK_TIME_UNDEFINED = -1

	class CopyAchievementDefinitionV2ByAchievementIdOptions extends BaseClass:
		func _init():
			super._init("CopyAchievementDefinitionV2ByAchievementIdOptions")

		var achievement_id: String

	class CopyAchievementDefinitionV2ByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyAchievementDefinitionV2ByIndexOptions")

		var achievement_index: int

	class CopyPlayerAchievementByAchievementIdOptions extends BaseClass:
		func _init():
			super._init("CopyPlayerAchievementByAchievementIdOptions")

		var achievement_id: String
		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id

	class CopyPlayerAchievementByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyPlayerAchievementByIndexOptions")

		var achievement_index: int
		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id: String

	class GetAchievementDefinitionCountOptions extends BaseClass:
		func _init():
			super._init("GetAchievementDefinitionCountOptions")

	class QueryDefinitionsOptions extends BaseClass:
		func _init():
			super._init("QueryDefinitionsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class GetPlayerAchievementCountOptions extends BaseClass:
		func _init():
			super._init("GetPlayerAchievementCountOptions")

		var user_id: String

	class QueryPlayerAchievementsOptions extends BaseClass:
		func _init():
			super._init("QueryPlayerAchievementsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class UnlockAchievementsOptions extends BaseClass:
		func _init():
			super._init("UnlockAchievementsOptions")

		var user_id: String
		var achievement_ids: Array # Array[String]
		var client_data = null

	class AchievementsInterface:
		static func query_definitions(options: QueryDefinitionsOptions) -> void:
			IEOS.achievements_interface_query_definitions(options)

		static func get_achievement_definition_count(options: GetAchievementDefinitionCountOptions) -> int:
			return IEOS.achievements_interface_get_achievement_definition_count(options)

		static func copy_achievement_definition_v2_by_achievement_id(
			options: CopyAchievementDefinitionV2ByAchievementIdOptions
		) -> Dictionary:
			return IEOS.achievements_interface_copy_achievement_definition_v2_by_achievement_id(
				options
			)

		static func copy_achievement_definition_v2_by_index(
			options: CopyAchievementDefinitionV2ByIndexOptions
		) -> Dictionary:
			return IEOS.achievements_interface_copy_achievement_definition_v2_by_index(options)

		static func query_player_achievements(options: QueryPlayerAchievementsOptions) -> void:
			IEOS.achievements_interface_query_player_achievements(options)

		static func get_player_achievement_count(options: GetPlayerAchievementCountOptions) -> int:
			return IEOS.achievements_interface_get_player_achievement_count(options)

		static func copy_player_achievement_by_achievement_id(
			options: CopyPlayerAchievementByAchievementIdOptions
		) -> Dictionary:
			return IEOS.achievements_interface_copy_player_achievement_by_achievement_id(options)

		static func copy_player_achievement_by_index(options: CopyPlayerAchievementByIndexOptions) -> Dictionary:
			return IEOS.achievements_interface_copy_player_achievement_by_index(options)

		static func unlock_achievements(options: UnlockAchievementsOptions) -> void:
			IEOS.achievements_interface_unlock_achievements(options)

class Connect:
	class Credentials extends BaseClass:
		func _init():
			super._init("Credentials")

		var type: ExternalCredentialType = -1
		var token = null

	class UserLoginInfo extends BaseClass:
		func _init():
			super._init("UserLoginInfo")

		var display_name: String
		var nsa_id_token: String

	class LoginOptions extends BaseClass:
		func _init():
			super._init("LoginOptions")

		var credentials: Credentials
		var user_login_info: UserLoginInfo

		var client_data = null

	class LogoutOptions extends BaseClass:
		func _init():
			super._init("LogoutOptions")

		var local_user_id: String

		var client_data = null

	class CopyIdTokenOptions extends BaseClass:
		func _init():
			super._init("CopyIdTokenOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class CopyProductUserExternalAccountByAccountIdOptions extends BaseClass:
		func _init():
			super._init("CopyProductUserExternalAccountByAccountIdOptions")

		var target_user_id = EOSGRuntime.local_product_user_id
		var account_id: String

	class CopyProductUserExternalAccountByAccountTypeOptions extends BaseClass:
		func _init():
			super._init("CopyProductUserExternalAccountByAccountTypeOptions")

		var target_user_id = EOSGRuntime.local_product_user_id
		var account_id_type: ExternalAccountType

	class CopyProductUserExternalAccountByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyProductUserExternalAccountByIndexOptions")

		var target_user_id = EOSGRuntime.local_product_user_id
		var external_account_info_index: int

	class CopyProductUserInfoOptions extends BaseClass:
		func _init():
			super._init("CopyProductUserInfoOptions")

		var target_user_id = EOSGRuntime.local_product_user_id

	class CreateDeviceIdOptions extends BaseClass:
		func _init():
			super._init("CreateDeviceIdOptions")

		var device_model: String

		var client_data = null

	class DeleteDeviceIdOptions extends BaseClass:
		func _init():
			super._init("DeleteDeviceIdOptions")

		var client_data = null

	class CreateUserOptions extends BaseClass:
		func _init():
			super._init("CreateUserOptions")

		var continuance_token: EOSGContinuanceToken

		var client_data = null

	class QueryExternalAccountMappingsOptions extends BaseClass:
		func _init():
			super._init("QueryExternalAccountMappingsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var account_id_type: ExternalAccountType
		var external_account_ids: Array # Array[String]

	class GetExternalAccountMappingsOptions extends BaseClass:
		func _init():
			super._init("GetExternalAccountMappingsOptions")

		var account_id_type: ExternalAccountType
		var local_user_id = EOSGRuntime.local_product_user_id
		var target_external_user_id: String

	class GetProductUserExternalAccountCountOptions extends BaseClass:
		func _init():
			super._init("GetProductUserExternalAccountCountOptions")

		var target_user_id = EOSGRuntime.local_product_user_id

	class QueryProductUserIdMappingsOptions extends BaseClass:
		func _init():
			super._init("QueryProductUserIdMappingsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var product_user_ids: Array # Array[String]

	class GetProductUserIdMappingOptions extends BaseClass:
		func _init():
			super._init("GetProductUserIdMappingOptions")

		var account_id_type: ExternalAccountType
		var local_user_id = EOSGRuntime.local_product_user_id
		var target_product_user_id = EOSGRuntime.local_product_user_id

	class LinkAccountOptions extends BaseClass:
		func _init():
			super._init("LinkAccountOptions")

		var continuance_token = null # ContinuanceTokenWrapper
		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class IdToken extends BaseClass:
		func _init():
			super._init("IdToken")

		var product_user_id: String
		var json_web_token: String

	class VerifyIdTokenOptions extends BaseClass:
		func _init():
			super._init("VerifyIdTokenOptions")

		var id_token: IdToken

		var client_data = null

	class TransferDeviceIdAccountOptions extends BaseClass:
		func _init():
			super._init("TransferDeviceIdAccountOptions")

		var primary_local_user_id = EOSGRuntime.local_product_user_id
		var local_device_user_id = EOSGRuntime.local_product_user_id
		var product_user_id_to_preserve: String

		var client_data = null

	class UnlinkAccountOptions extends BaseClass:
		func _init():
			super._init("UnlinkAccountOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class ConnectInterface:
		static func login(options: LoginOptions) -> void:
			IEOS.connect_interface_login(options)

		static func logout(options: LogoutOptions) -> void:
			IEOS.connect_interface_logout(options)

		static func copy_id_token(options: CopyIdTokenOptions) -> Dictionary:
			return IEOS.connect_interface_copy_id_token(options)

		static func copy_product_user_external_account_by_account_id(options: CopyProductUserExternalAccountByAccountIdOptions) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_external_account_by_account_id(options)

		static func copy_product_user_external_account_by_account_type(options: CopyProductUserExternalAccountByAccountTypeOptions) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_external_account_by_account_type(options)

		static func copy_product_user_external_account_by_index(options: CopyProductUserExternalAccountByIndexOptions) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_external_account_by_index(options)

		static func copy_product_user_info(options: CopyProductUserInfoOptions) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_info(options)

		static func create_device_id(options: CreateDeviceIdOptions) -> void:
			IEOS.connect_interface_create_device_id(options)

		static func create_user(options: CreateUserOptions) -> void:
			IEOS.connect_interface_create_user(options)

		static func delete_device_id(options: DeleteDeviceIdOptions) -> void:
			IEOS.connect_interface_delete_device_id(options)

		static func query_external_account_mappings(options: QueryExternalAccountMappingsOptions) -> void:
			IEOS.connect_interface_query_external_account_mappings(options)

		static func get_external_account_mapping(options: GetExternalAccountMappingsOptions) -> String:
			return IEOS.connect_interface_get_external_account_mapping(options)

		static func get_logged_in_user_by_index(index: int) -> String:
			return IEOS.connect_interface_get_logged_in_user_by_index(index)

		static func get_logged_in_users_count() -> int:
			return IEOS.connect_interface_get_logged_in_users_count()

		static func get_login_status(local_user_id:=EOSGRuntime.local_product_user_id) -> LoginStatus:
			return IEOS.connect_interface_get_login_status(local_user_id)

		static func get_product_user_external_account_count(options: GetProductUserExternalAccountCountOptions=GetProductUserExternalAccountCountOptions.new()) -> int:
			return IEOS.connect_interface_get_product_user_external_account_count(options)

		static func query_product_user_id_mappings(options: QueryProductUserIdMappingsOptions) -> void:
			IEOS.connect_interface_query_product_user_id_mappings(options)

		static func get_product_user_id_mapping(options: GetProductUserIdMappingOptions) -> Dictionary:
			return IEOS.connect_interface_get_product_user_id_mapping(options)

		static func link_account(options: LinkAccountOptions) -> void:
			IEOS.connect_interface_link_account(options)

		static func verify_id_token(options: VerifyIdTokenOptions) -> void:
			IEOS.connect_interface_verify_id_token(options)

		static func transfer_device_id_account(options: TransferDeviceIdAccountOptions) -> void:
			IEOS.connect_interface_transfer_device_id_account(options)

		static func unlink_account(options: UnlinkAccountOptions) -> void:
			IEOS.connect_interface_unlink_account(options)

class Auth:
	enum ScopeFlags {
		NoFlags = 0x0,
		BasicProfile = 0x1,
		FriendsList = 0x2,
		Presence = 0x4,
		FriendsManagement = 0x8,
		Email = 0x10,
		Country = 0x20,
	}

	enum AuthTokenType {Client = 0, User = 1}

	enum LinkAccountFlags {NoFlags = 0x0, NintendoNsaId = 0x1}

	enum LoginCredentialType {
		None = -1,
		Password = 0,
		ExchangeCode = 1,
		PersistentAuth = 2,
		DeviceCode = 3,
		Developer = 4,
		RefreshToken = 5,
		AccountPortal = 6,
		ExternalAuth = 7
	}

	enum LoginFlags {
		None = 0,
		NO_USER_INTERFACE = 1
	}

	class Credentials extends BaseClass:
		func _init():
			super._init("Credentials")

		var external_type: ExternalCredentialType = -1
		var id: String
		var token: String
		var type: LoginCredentialType = -1

	class LoginOptions extends BaseClass:
		func _init():
			super._init("LoginOptions")

		var credentials: Credentials
		var login_flags: LoginFlags = LoginFlags.None
		var scope_flags: ScopeFlags = -1

		var client_data = null

	class LogoutOptions extends BaseClass:
		func _init():
			super._init("LogoutOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class CopyIdTokenOptions extends BaseClass:
		func _init():
			super._init("CopyIdTokenOptions")

		var account_id = EOSGRuntime.local_epic_account_id

	class CopyUserAuthTokenOptions extends BaseClass:
		func _init():
			super._init("CopyUserAuthTokenOptions")

	class DeletePersistentAuthOptions extends BaseClass:
		func _init():
			super._init("DeletePersistentAuthOptions")

		var refresh_token = null

		var client_data = null

	class LinkAccountOptions extends BaseClass:
		func _init():
			super._init("LinkAccountOptions")

		var continuance_token: EOSGContinuanceToken
		var link_account_flags: LinkAccountFlags
		var local_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class QueryIdTokenOptions extends BaseClass:
		func _init():
			super._init("QueryIdTokenOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_account_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class IdToken extends BaseClass:
		func _init():
			super._init("IdToken")

		var account_id: String
		var json_web_token: String

	class VerifyIdTokenOptions extends BaseClass:
		func _init():
			super._init("VerifyIdTokenOptions")

		var id_token: IdToken

		var client_data = null

	class Token extends BaseClass:
		func _init():
			super._init("Token")

		var access_token: String
		var account_id: String
		var app: String
		var auth_type: int
		var client_id: String
		var expires_at: String
		var expires_in: float
		var refresh_expires_at: String
		var refresh_expires_in: float
		var refresh_token: String

	class VerifyUserAuthOptions extends BaseClass:
		func _init():
			super._init("VerifyUserAuthOptions")

		var auth_token: Token

		var client_data = null

	class AuthInterface:
		static func login(options: LoginOptions) -> void:
			IEOS.auth_interface_login(options)

		static func logout(options: LogoutOptions) -> void:
			IEOS.auth_interface_logout(options)

		static func copy_id_token(options: CopyIdTokenOptions) -> Dictionary:
			return IEOS.auth_interface_copy_id_token(options)

		static func copy_user_auth_token(options: CopyUserAuthTokenOptions, local_user_id:=EOSGRuntime.local_epic_account_id) -> Dictionary:
			var func_result: Dictionary = IEOS.auth_interface_copy_user_auth_token(
				options, local_user_id
			)
			var token: Token = Token.new()
			if func_result.token:
				var token_dict = func_result.token
				token.access_token = token_dict.access_token
				token.account_id = token_dict.account_id
				token.app = token_dict.app
				token.auth_type = token_dict.auth_type
				token.client_id = token_dict.client_id
				token.expires_at = token_dict.expires_at
				token.expires_in = token_dict.expires_in
				token.refresh_expires_at = token_dict.refresh_expires_at
				token.refresh_expires_in = token_dict.refresh_expires_in
				token.refresh_token = token_dict.refresh_token

			return {result_code = func_result.result_code, token = token}

		static func delete_persistent_auth(options: DeletePersistentAuthOptions) -> void:
			IEOS.auth_interface_delete_persistent_auth(options)

		static func get_logged_in_account_by_index(index: int) -> String:
			return IEOS.auth_interface_get_logged_in_account_by_index(index)

		static func get_logged_in_accounts_count() -> int:
			return IEOS.auth_interface_get_logged_in_accounts_count()

		static func get_login_status(local_user_id:=EOSGRuntime.local_epic_account_id) -> LoginStatus:
			return IEOS.auth_interface_get_login_status(local_user_id)

		static func get_merged_account_by_index(local_user_id: String, index: int) -> String:
			return IEOS.auth_interface_get_merged_account_by_index(local_user_id, index)

		static func get_merged_accounts_count(local_user_id:=EOSGRuntime.local_epic_account_id) -> int:
			return IEOS.auth_interface_get_merged_accounts_count(local_user_id)

		static func get_selected_account_id(local_user_id:=EOSGRuntime.local_epic_account_id) -> Dictionary:
			return IEOS.auth_interface_get_selected_account_id(local_user_id)

		static func link_account(options: LinkAccountOptions) -> void:
			IEOS.auth_interface_link_account(options)

		static func query_id_token(options: QueryIdTokenOptions) -> void:
			IEOS.auth_interface_query_id_token(options)

		static func verify_id_token(options: VerifyIdTokenOptions) -> void:
			IEOS.auth_interface_verify_id_token(options)

		static func verify_user_auth(options: VerifyUserAuthOptions) -> void:
			IEOS.auth_interface_verify_user_auth(options)

class CustomInvites:
	enum ResquestToJoinResponse {
		Accepted = 0,
		Rejected = 1,
	}

	class SetCustomInviteOptions extends BaseClass:
		func _init():
			super._init("SetCustomInviteOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var payload: String

	class SendCustomInviteOptions extends BaseClass:
		func _init():
			super._init("SendCustomInviteOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_ids: Array # Array[String]

		var client_data = null

	class FinalizeInviteOptions extends BaseClass:
		func _init():
			super._init("FinalizeInviteOptions")

		var custom_invite_id: String
		var local_user_id = EOSGRuntime.local_product_user_id
		var processing_result: Result
		var target_user_id = EOSGRuntime.local_product_user_id

	class SendRequestToJoinOptions extends BaseClass:
		func _init():
			super._init("SendRequestToJoinOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class AcceptRequestToJoinOptions extends BaseClass:
		func _init():
			super._init("AcceptRequestToJoinOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class RejectRequestToJoinOptions extends BaseClass:
		func _init():
			super._init("RejectRequestToJoinOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class CustomInvitesInterface:
		static func set_custom_invite(options: SetCustomInviteOptions) -> Result:
			return IEOS.custom_invites_interface_set_custom_invite(options)

		static func send_custom_invite(options: SendCustomInviteOptions) -> void:
			IEOS.custom_invites_interface_send_custom_invite(options)

		static func finalize_invite(options: FinalizeInviteOptions) -> Result:
			return IEOS.custom_invites_interface_finalize_invite(options)

		static func send_request_to_join(options: SendRequestToJoinOptions) -> void:
			IEOS.custom_invites_interface_send_request_to_join(options)

		static func accept_request_to_join(options: SendRequestToJoinOptions) -> void:
			IEOS.custom_invites_interface_accept_request_to_join(options)

		static func reject_request_to_join(options: SendRequestToJoinOptions) -> void:
			IEOS.custom_invites_interface_reject_request_to_join(options)

class Stats:
	const STATS_TIME_UNDEFINED = -1

	class CopyStatByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyStatByIndexOptions")

		var target_user_id = EOSGRuntime.local_product_user_id
		var stat_index: int

	class CopyStatByNameOptions extends BaseClass:
		func _init():
			super._init("CopyStatByNameOptions")

		var target_user_id = EOSGRuntime.local_product_user_id
		var name: String

	class GetStatsCountOptions extends BaseClass:
		func _init():
			super._init("GetStatsCountOptions")

		var target_user_id = EOSGRuntime.local_product_user_id

	class IngestStatOptions extends BaseClass:
		func _init():
			super._init("IngestStatOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var stats: Array # Array[Dictionary] {stat_name: String, ingest_amount: int}
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class QueryStatsOptions extends BaseClass:
		func _init():
			super._init("QueryStatsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id
		var stat_names: Array # Array[String]
		var start_time = STATS_TIME_UNDEFINED
		var end_time = STATS_TIME_UNDEFINED

		var client_data = null

	class StatsInterface:
		static func copy_stat_by_index(options: CopyStatByIndexOptions) -> Dictionary:
			return IEOS.stats_interface_copy_stat_by_index(options)

		static func copy_stat_by_name(options: CopyStatByNameOptions) -> Dictionary:
			return IEOS.stats_interface_copy_stat_by_name(options)

		static func get_stats_count(options: GetStatsCountOptions) -> int:
			return IEOS.stats_interface_get_stats_count(options)

		static func ingest_stat(options: IngestStatOptions) -> void:
			IEOS.stats_interface_ingest_stat(options)

		static func query_stats(options: QueryStatsOptions) -> void:
			IEOS.stats_interface_query_stats(options)

class Platform:
	enum PlatformFlags {
		None = 0x0,
		LoadingInEditor = 0x00001,
		DisableOverlay = 0x00002,
		DisableSocialOverlay = 0x00004,
		Reserved1 = 0x00008,
		WindowsEnableOverlayD3D9 = 0x00010,
		WindowsEnableOverlayD3D10 = 0x00020,
		WindowsEnableOverlayOpengl = 0x00040,
		ConsoleEnableOverlayAutomaticUnloading = 0x00080,
	}

	enum ApplicationStatus {
		BackgroundConstrained = 0,
		BackgroundUnconstrained = 1,
		BackgroundSuspended = 2,
		Foreground = 3
	}

	enum NetworkStatus {
		Disabled = 0,
		Offline = 1,
		Online = 2
	}

	enum DesktopCrossplayStatus {
		EOS_DCS_OK = 0,
		EOS_DCS_ApplicationNotBootstrapped = 1,
		EOS_DCS_ServiceNotInstalled = 2,
		EOS_DCS_ServiceStartFailed = 3,
		EOS_DCS_ServiceNotRunning = 4,
		EOS_DCS_OverlayDisabled = 5,
		EOS_DCS_OverlayNotInstalled = 6,
		EOS_DCS_OverlayTrustCheckFailed = 7,
		EOS_DCS_OverlayLoadFailed = 8
	}

	enum RTCBackgroundMode {
		LeaveRooms = 0,
		KeepRoomsAlive = 1,
	}

	class InitializeOptions extends BaseClass:
		func _init():
			super._init("InitializeOptions")

		var product_name: String
		var product_version: String

	class RTCOptions extends BaseClass:
		func _init():
			super._init("RTCOptions")

		var background_mode = null # # See [enum EOS.Platform.RTCBackgroundMode]

	class CreateOptions extends BaseClass:
		func _init():
			super._init("CreateOptions")

		var client_id: String
		var client_secret: String
		var deployment_id: String
		var encryption_key: String
		var product_id: String
		var sandbox_id: String

		var cache_directory = ProjectSettings.globalize_path("user://eosg-cache")
		var flags: int = -1
		var is_server: bool
		var override_country_code: String
		var override_locale_code: String
		var tick_budget_in_milliseconds: int
		var task_network_timeout_seconds = null # float

		var rtc_options := RTCOptions.new()

	class PlatformInterface:
		static func create(options: CreateOptions) -> bool:
			return IEOS.platform_interface_create(options)

		static func get_active_country_code(user_id: String) -> Dictionary:
			return IEOS.platform_interface_get_active_country_code(user_id)

		static func get_active_locale_code(user_id: String) -> Dictionary:
			return IEOS.platform_interface_get_active_locale_code(user_id)

		static func get_override_country_code() -> Dictionary:
			return IEOS.platform_interface_get_override_country_code()

		static func get_override_locale_code() -> Dictionary:
			return IEOS.platform_interface_get_override_locale_code()

		static func set_override_country_code(country_code: String) -> Result:
			return IEOS.platform_interface_set_override_country_code(country_code)

		static func set_override_locale_code(locale_code: String) -> Result:
			return IEOS.platform_interface_set_override_locale_code(locale_code)

		static func check_for_launcher_and_restart() -> Result:
			return IEOS.platform_interface_check_for_launcher_and_restart()

		static func initialize(options: InitializeOptions) -> Result:
			return IEOS.platform_interface_initialize(options)

		static func get_desktop_crossplay_status_info() -> Dictionary:
			return IEOS.platform_interface_get_desktop_crossplay_status_info()

		static func set_application_status(status: ApplicationStatus) -> Result:
			return IEOS.platform_interface_set_application_status(status)

		static func get_application_status() -> ApplicationStatus:
			return IEOS.platform_interface_get_application_status()

		static func set_network_status(status: NetworkStatus) -> Result:
			return IEOS.platform_interface_set_network_status(status)

		static func get_network_status() -> NetworkStatus:
			return IEOS.platform_interface_get_network_status()

		static func release() -> void:
			IEOS.platform_interface_release()

		static func shutdown() -> Result:
			return IEOS.platform_interface_shutdown()

class Ecom:
	enum ItemType {Durable = 0, Consumable = 1, Other = 2}

	enum OwnershipStatus {NotOwned = 0, Owned = 1}

	class CheckoutOptions extends BaseClass:
		func _init():
			super._init("CheckoutOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entries: Array # Array[Dictionary] {offer_id: String}
		var override_catalog_namespace: String

	class CopyEntitlementByIdOptions extends BaseClass:
		func _init():
			super._init("CopyEntitlementByIdOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entitlement_id: String

	class CopyEntitlementByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyEntitlementByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entitlement_index: int

	class CopyEntitlementByNameAndIndexOptions extends BaseClass:
		func _init():
			super._init("CopyEntitlementByNameAndIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entitlement_name: String
		var index: int

	class CopyItemByIdOptions extends BaseClass:
		func _init():
			super._init("CopyItemByIdOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var item_id: String

	class CopyItemImageInfoByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyItemImageInfoByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var item_id: String
		var image_info_index: int

	class CopyItemReleaseByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyItemReleaseByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var item_id: String
		var release_index: int

	class CopyOfferByIdOptions extends BaseClass:
		func _init():
			super._init("CopyOfferByIdOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var offer_id: String

	class CopyOfferByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyOfferByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var offer_index: int

	class CopyOfferImageInfoByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyOfferImageInfoByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var offer_id: String
		var image_info_index: int

	class CopyOfferItemByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyOfferItemByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var offer_id: String
		var item_index: int

	class CopyTransactionByIdOptions extends BaseClass:
		func _init():
			super._init("CopyTransactionByIdOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var transaction_id: String

	class CopyTransactionByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyTransactionByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var transaction_index: int

	class GetEntitlementsByNameCountOptions extends BaseClass:
		func _init():
			super._init("GetEntitlementsByNameCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entitlement_name: String

	class GetEntitlementsCountOptions extends BaseClass:
		func _init():
			super._init("GetEntitlementsCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var item_id: String

	class GetItemImageInfoCountOptions extends BaseClass:
		func _init():
			super._init("GetItemImageInfoCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var item_id: String

	class GetItemReleaseCountOptions extends BaseClass:
		func _init():
			super._init("GetItemReleaseCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var item_id: String

	class GetOfferCountOptions extends BaseClass:
		func _init():
			super._init("GetOfferCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class GetOfferImageInfoCountOptions extends BaseClass:
		func _init():
			super._init("GetOfferImageInfoCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var offer_id: String

	class GetOfferItemCountOptions extends BaseClass:
		func _init():
			super._init("GetOfferItemCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var offer_id: String

	class GetTransactionCountOptions extends BaseClass:
		func _init():
			super._init("GetTransactionCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class QueryEntitlementsOptions extends BaseClass:
		func _init():
			super._init("QueryEntitlementsOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entitlement_names: Array # Array[String]
		var include_redeemed: bool

	class QueryOffersOptions extends BaseClass:
		func _init():
			super._init("QueryOffersOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var override_catalog_namespace: String

		var client_data = null

	class QueryOwnershipOptions extends BaseClass:
		func _init():
			super._init("QueryOwnershipOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var catalog_item_ids: Array # Array[String]
		var catalog_namespace: String

		var client_data = null

	class QueryOwnershipTokenOptions extends BaseClass:
		func _init():
			super._init("QueryOwnershipTokenOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var catalog_item_ids: Array # Array[String]
		var catalog_namespace = null # String

		var client_data = null

	class RedeemEntitlementsOptions extends BaseClass:
		func _init():
			super._init("RedeemEntitlementsOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var entitlement_ids: Array # Array[String]

		var client_data = null

	class GetLastRedeemedEntitlementsCountOptions extends BaseClass:
		func _init():
			super._init("GetLastRedeemedEntitlementsCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class CopyLastRedeemedEntitlementByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyLastRedeemedEntitlementByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var redeemed_entitlement_index: String

	class EcomInterface:
		static func checkout(options: CheckoutOptions) -> void:
			IEOS.ecom_interface_checkout(options)

		static func copy_entitlement_by_id(options: CopyEntitlementByIdOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_entitlement_by_id(options)

		static func copy_entitlement_by_index(options: CopyEntitlementByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_entitlement_by_index(options)

		static func copy_entitlement_by_name_and_index(options: CopyEntitlementByNameAndIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_entitlement_by_name_and_index(options)

		static func copy_item_by_id(options: CopyItemByIdOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_item_by_id(options)

		static func copy_item_image_info_by_index(options: CopyItemImageInfoByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_item_image_info_by_index(options)

		static func copy_item_release_by_index(options: CopyItemReleaseByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_item_release_by_index(options)

		static func copy_offer_by_id(options: CopyOfferByIdOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_offer_by_id(options)

		static func copy_offer_by_index(options: CopyOfferByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_offer_by_index(options)

		static func copy_offer_image_info_by_index(options: CopyOfferImageInfoByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_offer_image_info_by_index(options)

		static func copy_offer_item_by_index(options: CopyOfferItemByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_offer_item_by_index(options)

		static func copy_transaction_by_id(options: CopyTransactionByIdOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_transaction_by_id(options)

		static func copy_transaction_by_index(options: CopyTransactionByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_transaction_by_index(options)

		static func get_entitlements_by_name_count(options: GetEntitlementsByNameCountOptions) -> int:
			return IEOS.ecom_interface_get_entitlements_by_name_count(options)

		static func get_entitlements_count(options: GetEntitlementsCountOptions) -> int:
			return IEOS.ecom_interface_get_entitlements_count(options)

		static func get_item_image_info_count(options: GetItemImageInfoCountOptions) -> int:
			return IEOS.ecom_interface_get_item_image_info_count(options)

		static func get_item_release_count(options: GetItemReleaseCountOptions) -> int:
			return IEOS.ecom_interface_get_item_release_count(options)

		static func get_offer_count(options: GetOfferCountOptions) -> int:
			return IEOS.ecom_interface_get_offer_count(options)

		static func get_offer_image_info_count(options: GetOfferImageInfoCountOptions) -> int:
			return IEOS.ecom_interface_get_offer_image_info_count(options)

		static func get_offer_item_count(options: GetOfferItemCountOptions) -> int:
			return IEOS.ecom_interface_get_offer_item_count(options)

		static func get_transaction_count(options: GetTransactionCountOptions) -> int:
			return IEOS.ecom_interface_get_transaction_count(options)

		static func query_entitlements(options: QueryEntitlementsOptions) -> void:
			IEOS.ecom_interface_query_entitlements(options)

		static func query_offers(options: QueryOffersOptions) -> void:
			IEOS.ecom_interface_query_offers(options)

		static func query_ownership(options: QueryOwnershipOptions) -> void:
			IEOS.ecom_interface_query_ownership(options)

		static func query_ownership_token(options: QueryOwnershipTokenOptions) -> void:
			IEOS.ecom_interface_query_ownership_token(options)

		static func redeem_entitlements(options: RedeemEntitlementsOptions) -> void:
			IEOS.ecom_interface_redeem_entitlements(options)

		static func get_last_redeemed_entitlements_count(options: GetLastRedeemedEntitlementsCountOptions) -> int:
			return IEOS.ecom_interface_get_last_redeemed_entitlements_count(options)

		static func copy_last_redeemed_entitlement_by_index(options: CopyLastRedeemedEntitlementByIndexOptions) -> Dictionary:
			return IEOS.ecom_interface_copy_last_redeemed_entitlement_by_index(options)

class Friends:
	enum FriendsStatus {NotFriends = 0, InviteSent = 1, InviteReceived = 2, Friends = 3}

	class AcceptInviteOptions extends BaseClass:
		func _init():
			super._init("AcceptInviteOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class GetFriendAtIndexOptions extends BaseClass:
		func _init():
			super._init("GetFriendAtIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var index: int

	class GetFriendsCountOptions extends BaseClass:
		func _init():
			super._init("GetFriendsCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class GetStatusOptions extends BaseClass:
		func _init():
			super._init("GetStatusOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class QueryFriendsOptions extends BaseClass:
		func _init():
			super._init("QueryFriendsOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class RejectInviteOptions extends BaseClass:
		func _init():
			super._init("RejectInviteOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class SendInviteOptions extends BaseClass:
		func _init():
			super._init("SendInviteOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class FriendsInterface:
		static func accept_invite(options: AcceptInviteOptions) -> void:
			IEOS.friends_interface_accept_invite(options)

		static func get_friend_at_index(options: GetFriendAtIndexOptions) -> String:
			return IEOS.friends_interface_get_friend_at_index(options)

		static func get_friends_count(options: GetFriendsCountOptions) -> int:
			return IEOS.friends_interface_get_friends_count(options)

		static func get_status(options: GetStatusOptions) -> EOS.Friends.FriendsStatus:
			return IEOS.friends_interface_get_status(options)

		static func query_friends(options: QueryFriendsOptions) -> void:
			IEOS.friends_interface_query_friends(options)

		static func reject_invite(options: RejectInviteOptions) -> void:
			IEOS.friends_interface_reject_invite(options)

		static func send_invite(options: SendInviteOptions) -> void:
			IEOS.friends_interface_send_invite(options)

class KWS:
	enum KWSPermissionStatus {Granted = 0, Rejected = 1, Pending = 2}

	class CopyPermissionByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyPermissionByIndexOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var index: int

	class CreateUserOptions extends BaseClass:
		func _init():
			super._init("CreateUserOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		## Date of birth in ISO8601 form (YYYY-MM-DD)
		var date_of_birth: String
		var parent_email: String

	class GetPermissionByKeyOptions extends BaseClass:
		func _init():
			super._init("GetPermissionByKeyOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var key: String

	class GetPermissionsCountOptions extends BaseClass:
		func _init():
			super._init("GetPermissionsCountOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class QueryAgeGateOptions extends BaseClass:
		func _init():
			super._init("QueryAgeGateOptions")

	class QueryPermissionsOptions extends BaseClass:
		func _init():
			super._init("QueryPermissionsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class RequestPermissionsOptions extends BaseClass:
		func _init():
			super._init("RequestPermissionsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var permission_keys: Array # Array[String]

	class UpdateParentEmailOptions extends BaseClass:
		func _init():
			super._init("UpdateParentEmailOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var parent_email: String

	class KWSInterface:
		static func copy_permission_by_index(options: CopyPermissionByIndexOptions) -> Dictionary:
			return IEOS.kws_interface_copy_permission_by_index(options)

		static func create_user(options: CreateUserOptions) -> void:
			IEOS.kws_interface_create_user(options)

		static func get_permission_by_key(options: GetPermissionByKeyOptions) -> Dictionary:
			return IEOS.kws_interface_get_permission_by_key(options)

		static func get_permissions_count(options: GetPermissionsCountOptions) -> int:
			return IEOS.kws_interface_get_permissions_count(options)

		static func query_age_gate(options: QueryAgeGateOptions) -> void:
			IEOS.kws_interface_query_age_gate(options)

		static func query_permissions(options: QueryPermissionsOptions) -> void:
			IEOS.kws_interface_query_permissions(options)

		static func request_permissions(options: RequestPermissionsOptions) -> void:
			IEOS.kws_interface_request_permissions(options)

		static func update_parent_email(options: UpdateParentEmailOptions) -> void:
			IEOS.kws_interface_update_parent_email(options)

class Leaderboards:
	const LEADERBOARD_TIME_UNDEFINED = -1

	enum LeaderboardAggregation {Min = 0, Max = 1, Sum = 2, Latest = 3}

	class CopyLeaderboardDefinitionByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyLeaderboardDefinitionByIndexOptions")

		var leaderboard_index: int

	class CopyLeaderboardDefinitionByLeaderboardId extends BaseClass:
		func _init():
			super._init("CopyLeaderboardDefinitionByLeaderboardId")

		var leaderboard_id: String

	class CopyLeaderboardRecordByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyLeaderboardRecordByIndexOptions")

		var leaderboard_record_index: int

	class CopyLeaderboardRecordByUserIdOptions extends BaseClass:
		func _init():
			super._init("CopyLeaderboardRecordByUserIdOptions")

		var user_id: int

	class CopyLeaderboardUserScoreByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyLeaderboardUserScoreByIndexOptions")

		var leaderboard_user_score_index: int
		var stat_name: String

	class CopyLeaderboardUserScoreByUserIdOptions extends BaseClass:
		func _init():
			super._init("CopyLeaderboardUserScoreByUserIdOptions")

		var user_id: String
		var stat_name: String

	class GetLeaderboardDefinitionCountOptions extends BaseClass:
		func _init():
			super._init("GetLeaderboardDefinitionCountOptions")

	class GetLeaderboardRecordCountOptions extends BaseClass:
		func _init():
			super._init("GetLeaderboardRecordCountOptions")

	class GetLeaderboardUserScoreCountOptions extends BaseClass:
		func _init():
			super._init("GetLeaderboardUserScoreCountOptions")

		var stat_name: String

	class QueryLeaderboardDefinitionsOptions extends BaseClass:
		func _init():
			super._init("QueryLeaderboardDefinitionsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var start_time = LEADERBOARD_TIME_UNDEFINED
		var end_time = LEADERBOARD_TIME_UNDEFINED

		var client_data = null

	class QueryLeaderboardRanksOptions extends BaseClass:
		func _init():
			super._init("QueryLeaderboardRanksOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var leaderboard_id: String

		var client_data = null

	class QueryLeaderboardUserScoresOptions extends BaseClass:
		func _init():
			super._init("QueryLeaderboardUserScoresOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var user_ids: Array # Array[String]
		var stat_info: Array[Dictionary] # [{stat_name: String, aggregation: LeaderboardAggregation}]
		var start_time = null # String
		var end_time = null # String

		var client_data = null

	class LeaderboardsInterface:
		static func copy_leaderboard_definition_by_index(options: CopyLeaderboardDefinitionByIndexOptions) -> Dictionary:
			return IEOS.leaderboards_interface_copy_leaderboard_definition_by_index(options)

		static func copy_leaderboard_definition_by_leaderboard_id(options: CopyLeaderboardDefinitionByLeaderboardId) -> Dictionary:
			return IEOS.leaderboards_interface_copy_leaderboard_definition_by_leaderboard_id(options)

		static func copy_leaderboard_record_by_index(options: CopyLeaderboardRecordByIndexOptions) -> Dictionary:
			return IEOS.leaderboards_interface_copy_leaderboard_record_by_index(options)

		static func copy_leaderboard_record_by_user_id(options: CopyLeaderboardRecordByUserIdOptions) -> Dictionary:
			return IEOS.leaderboards_interface_copy_leaderboard_record_by_user_id(options)

		static func copy_leaderboard_user_score_by_index(options: CopyLeaderboardUserScoreByIndexOptions) -> Dictionary:
			return IEOS.leaderboards_interface_copy_leaderboard_user_score_by_index(options)

		static func copy_leaderboard_user_score_by_user_id(options: CopyLeaderboardUserScoreByUserIdOptions) -> Dictionary:
			return IEOS.leaderboards_interface_copy_leaderboard_user_score_by_user_id(options)

		static func get_leaderboard_definition_count(options: GetLeaderboardDefinitionCountOptions) -> int:
			return IEOS.leaderboards_interface_get_leaderboard_definition_count(options)

		static func get_leaderboard_record_count(options: GetLeaderboardRecordCountOptions) -> int:
			return IEOS.leaderboards_interface_get_leaderboard_record_count(options)

		static func get_leaderboard_user_score_count(options: GetLeaderboardUserScoreCountOptions) -> int:
			return IEOS.leaderboards_interface_get_leaderboard_user_score_count(options)

		static func query_leaderboard_definitions(options: QueryLeaderboardDefinitionsOptions) -> void:
			IEOS.leaderboards_interface_query_leaderboard_definitions(options)

		static func query_leaderboard_ranks(options: QueryLeaderboardRanksOptions) -> void:
			IEOS.leaderboards_interface_query_leaderboard_ranks(options)

		static func query_leaderboard_user_scores(options: QueryLeaderboardUserScoresOptions) -> void:
			IEOS.leaderboards_interface_query_leaderboard_user_scores(options)

class Lobby:
	enum LobbyAttributeVisibility {Public = 0, Private = 1}

	enum LobbyMemberStatus {
		Joined = 0,
		Left = 1,
		Disconnected = 2,
		Kicked = 3,
		Promoted = 4,
		Closed = 5
	}

	enum LobbyPermissionLevel {PublicAdvertised = 0, JoinViaPresence = 1, InviteOnly = 2}

	const SEARCH_BUCKET_ID = "bucket"
	const SEARCH_MINCURRENT_MEMBERS = "mincurrentmembers"
	const SEARCH_MINSLOTSAVAILABLE = "minslotsavailable"

	class CreateLobbyOptions extends BaseClass:
		func _init():
			super._init("CreateLobbyOptions")

		var allow_invites: bool
		var bucket_id: String
		var disable_host_migration: bool
		var enable_join_by_id: bool
		var enable_rtc_room: bool
		var lobby_id: String
		var local_user_id = EOSGRuntime.local_product_user_id
		var max_lobby_members: int
		var permission_level: LobbyPermissionLevel
		var presence_enabled: bool
		var rejoin_after_kick_requires_invite: bool

		## (Optional) Allows the local application to set local audio options for the RTC Room if it is enabled. Set this to a [Dictionary] to override the defaults.[br]
		## A [Dictionary] with keys: [br]
		## - flags: A bitwise-or union of [enum EOS.RTC.JoinRoomFlags],[br]
		## - use_manual_audio_input: [bool],[br]
		## - use_manual_audio_output: [bool],[br]
		## - local_audio_device_input_starts_muted: [bool]
		var local_rtc_options = null

		var client_data = null

	class DestroyLobbyOptions extends BaseClass:
		func _init():
			super._init("DestroyLobbyOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String

		var client_data = null

	class JoinLobbyOptions extends BaseClass:
		func _init():
			super._init("JoinLobbyOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_details: EOSGLobbyDetails
		var presence_enabled: bool
		## (Optional) Allows the local application to set local audio options for the RTC Room if it is enabled. Set this to a [Dictionary] to override the defaults.[br]
		## A [Dictionary] with keys: [br]
		## - flags: A bitwise-or union of [enum EOS.RTC.JoinRoomFlags],[br]
		## - use_manual_audio_input: [bool],[br]
		## - use_manual_audio_output: [bool],[br]
		## - local_audio_device_input_starts_muted: [bool]
		var local_rtc_options = null

		var client_data = null

	class JoinLobbyByIdOptions extends BaseClass:
		func _init():
			super._init("JoinLobbyByIdOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String
		var presence_enabled: bool
		## (Optional) Allows the local application to set local audio options for the RTC Room if it is enabled. Set this to a [Dictionary] to override the defaults.[br]
		## A [Dictionary] with keys: [br]
		## - flags: A bitwise-or union of [enum EOS.RTC.JoinRoomFlags],[br]
		## - use_manual_audio_input: [bool],[br]
		## - use_manual_audio_output: [bool],[br]
		## - local_audio_device_input_starts_muted: [bool]
		var local_rtc_options = null

		var client_data = null

	class LeaveLobbyOptions extends BaseClass:
		func _init():
			super._init("LeaveLobbyOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String

		var client_data = null

	class UpdateLobbyModificationOptions extends BaseClass:
		func _init():
			super._init("UpdateLobbyModificationOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String

		var client_data = null

	class UpdateLobbyOptions extends BaseClass:
		func _init():
			super._init("UpdateLobbyOptions")

		var lobby_modification: EOSGLobbyModification

		var client_data = null

	class PromoteMemberOptions extends BaseClass:
		func _init():
			super._init("PromoteMemberOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id: String
		var lobby_id: String

		var client_data = null

	class KickMemberOptions extends BaseClass:
		func _init():
			super._init("KickMemberOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id: String
		var lobby_id: String

		var client_data = null

	class HardMuteMemberOptions extends BaseClass:
		func _init():
			super._init("HardMuteMemberOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id: String
		var lobby_id: String
		var hard_mute: bool

		var client_data = null

	class SendInviteOptions extends BaseClass:
		func _init():
			super._init("SendInviteOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id: String
		var lobby_id: String

		var client_data = null

	class RejectInviteOptions extends BaseClass:
		func _init():
			super._init("RejectInviteOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var invite_id: String

		var client_data = null

	class QueryInvitesOptions extends BaseClass:
		func _init():
			super._init("QueryInvitesOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class GetInviteCountOptions extends BaseClass:
		func _init():
			super._init("GetInviteCountOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class GetInviteIdByIndexOptions extends BaseClass:
		func _init():
			super._init("GetInviteIdByIndexOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var index: int

	class CreateLobbySearchOptions extends BaseClass:
		func _init():
			super._init("CreateLobbySearchOptions")

		var max_results = 10

	class CopyLobbyDetailsByInviteIdOptions extends BaseClass:
		func _init():
			super._init("CopyLobbyDetailsByInviteIdOptions")

		var invite_id: String

	class CopyLobbyDetailsByUiEventIdOptions extends BaseClass:
		func _init():
			super._init("CopyLobbyDetailsByUiEventIdOptions")

		var ui_event_id: String

	class CopyLobbyDetailsOptions extends BaseClass:
		func _init():
			super._init("CopyLobbyDetailsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String

	class GetRtcRoomNameOptions extends BaseClass:
		func _init():
			super._init("GetRtcRoomNameOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String

	class IsRtcRoomConnectedOptions extends BaseClass:
		func _init():
			super._init("IsRtcRoomConnectedOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var lobby_id: String

	class LobbyInterface:
		static func create_lobby(options: CreateLobbyOptions) -> void:
			IEOS.lobby_interface_create_lobby(options)

		static func destroy_lobby(options: DestroyLobbyOptions) -> void:
			IEOS.lobby_interface_destroy_lobby(options)

		static func join_lobby(options: JoinLobbyOptions) -> void:
			IEOS.lobby_interface_join_lobby(options)

		static func join_lobby_by_id(options: JoinLobbyByIdOptions) -> void:
			IEOS.lobby_interface_join_lobby_by_id(options)

		static func leave_lobby(options: LeaveLobbyOptions) -> void:
			IEOS.lobby_interface_leave_lobby(options)

		static func update_lobby_modification(options: UpdateLobbyModificationOptions) -> Dictionary:
			return IEOS.lobby_interface_update_lobby_modification(options)

		static func update_lobby(options: UpdateLobbyOptions) -> void:
			IEOS.lobby_interface_update_lobby(options)

		static func promote_member(options: PromoteMemberOptions) -> void:
			IEOS.lobby_interface_promote_member(options)

		static func kick_member(options: KickMemberOptions) -> void:
			IEOS.lobby_interface_kick_member(options)

		static func hard_mute_member(options: HardMuteMemberOptions) -> void:
			IEOS.lobby_interface_hard_mute_member(options)

		static func send_invite(options: SendInviteOptions) -> void:
			IEOS.lobby_interface_send_invite(options)

		static func reject_invite(options: RejectInviteOptions) -> void:
			IEOS.lobby_interface_reject_invite(options)

		static func query_invites(options: QueryInvitesOptions) -> void:
			IEOS.lobby_interface_query_invites(options)

		static func get_invite_count(options: GetInviteCountOptions) -> int:
			return IEOS.lobby_interface_get_invite_count(options)

		static func get_invite_id_by_index(options: GetInviteIdByIndexOptions) -> Dictionary:
			return IEOS.lobby_interface_get_invite_id_by_index(options)

		static func create_lobby_search(options: CreateLobbySearchOptions) -> Dictionary:
			return IEOS.lobby_interface_create_lobby_search(options)

		static func copy_lobby_details_by_invite_id(options: CopyLobbyDetailsByInviteIdOptions) -> Dictionary:
			return IEOS.lobby_interface_copy_lobby_details_by_invite_id(options)

		static func copy_lobby_details_by_ui_event_id(options: CopyLobbyDetailsByUiEventIdOptions) -> Dictionary:
			return IEOS.lobby_interface_copy_lobby_details_by_ui_event_id(options)

		static func copy_lobby_details(options: CopyLobbyDetailsOptions) -> Dictionary:
			return IEOS.lobby_interface_copy_lobby_details(options)

		static func get_rtc_room_name(options: GetRtcRoomNameOptions) -> Dictionary:
			return IEOS.lobby_interface_get_rtc_room_name(options)

		static func is_rtc_room_connected(options: IsRtcRoomConnectedOptions) -> Dictionary:
			return IEOS.lobby_interface_is_rtc_room_connected(options)

class Metrics:
	enum MetricsAccountIdType {Epic = 0, External = 1}

	enum UserControllerType {Unknown = 0, MouseKeyboard = 1, GamepadControl = 2, TouchControl = 3}

	class BeginPlayerSessionOptions extends BaseClass:
		func _init():
			super._init("BeginPlayerSessionOptions")

		var account_id_type: MetricsAccountIdType
		var account_id: String
		var controller_type: UserControllerType
		var display_name: String
		var game_session_id = null # String
		var server_ip = null # String

	class EndPlayerSessionOptions extends BaseClass:
		func _init():
			super._init("EndPlayerSessionOptions")

		var account_id_type: MetricsAccountIdType
		var account_id: String

	class MetricsInterface:
		static func begin_player_session(options: BeginPlayerSessionOptions) -> Result:
			return IEOS.metrics_interface_begin_player_session(options)

		static func end_player_session(options: EndPlayerSessionOptions) -> Result:
			return IEOS.metrics_interface_end_player_session(options)

class Mods:
	enum ModEnumerationType {Installed = 0, AllAvailable}

	class CopyModInfoOptions extends BaseClass:
		func _init():
			super._init("CopyModInfoOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var type: ModEnumerationType

	class EnumerateModsOptions extends BaseClass:
		func _init():
			super._init("EnumerateModsOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var type: ModEnumerationType

	class InstallModOptions extends BaseClass:
		func _init():
			super._init("InstallModOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var mod: Dictionary

	class UninstallModOptions extends BaseClass:
		func _init():
			super._init("UninstallModOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var mod: Dictionary

	class UpdateModOptions extends BaseClass:
		func _init():
			super._init("UpdateModOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var mod: Dictionary

	class ModsInterface:
		static func copy_mod_info(options: CopyModInfoOptions) -> Dictionary:
			return IEOS.mods_interface_copy_mod_info(options)

		static func enumerate_mods(options: EnumerateModsOptions) -> void:
			IEOS.mods_interface_enumerate_mods(options)

		static func install_mod(options: InstallModOptions) -> void:
			IEOS.mods_interface_install_mod(options)

		static func uninstall_mod(options: UninstallModOptions) -> void:
			IEOS.mods_interface_uninstall_mod(options)

		static func update_mod(options: UpdateModOptions) -> void:
			IEOS.mods_interface_update_mod(options)

class P2P:
	enum ConnectionClosedReason {
		Unknown = 0,
		ClosedByLocalUser = 1,
		ClosedByPeer = 2,
		TimedOut = 3,
		TooManyConnections = 4,
		InvalidMessage = 5,
		InvalidData = 6,
		ConnectionFailed = 7,
		ConnectionClosed = 8,
		NegotiationFailed = 9,
		UnexpectedError = 10
	}

	enum ConnectionEstablishedType {NewConnection = 0, Reconnection = 1}

	enum NATType {Unknown = 0, Open = 1, Moderate = 2, Strict = 3}

	enum PacketReliability {UnreliableUnordered = 0, ReliableUnordered = 1, ReliableOrdered = 2}

	enum RelayControl {NoRelays = 0, AllowRelays = 1, ForceRelays = 2}

	enum NetworkType {NoConnection = 0, DirectConnection = 1, RelayedConnection = 2}

	enum Mode {None = 0, Server = 1, Client = 2, Mesh = 3}

	enum ConnectionStatus {Disconnected = 0, Connecting = 1, Connected = 2}

	class SetPortRangeOptions extends BaseClass:
		func _init():
			super._init("SetPortRangeOptions")

		var port: int
		var max_additional_ports_to_try: int

	class SetPacketQueueSizeOptions extends BaseClass:
		func _init():
			super._init("SetPacketQueueSizeOptions")

		var incoming_packet_queue_max_size_bytes: int
		var outgoing_packet_queue_max_size_bytes: int

	class P2PInterface:
		static func get_packet_queue_info() -> Dictionary:
			return IEOS.p2p_interface_get_packet_queue_info()

		static func get_port_range() -> Dictionary:
			return IEOS.p2p_interface_get_port_range()

		static func get_nat_type() -> NATType:
			return IEOS.p2p_interface_get_nat_type() as NATType

		static func get_relay_control() -> RelayControl:
			return IEOS.p2p_interface_get_relay_control() as RelayControl

		static func query_nat_type() -> void:
			IEOS.p2p_interface_query_nat_type()

		static func set_packet_queue_size(options: SetPacketQueueSizeOptions) -> void:
			IEOS.p2p_interface_set_packet_queue_size(options)

		static func set_port_range(options: SetPortRangeOptions) -> void:
			IEOS.p2p_interface_set_port_range(options)

		static func set_relay_control(relay_control: RelayControl) -> void:
			IEOS.p2p_interface_set_relay_control(relay_control)

class Presence:
	enum Status {Offline = 0, Online = 1, Away = 2, ExtendedAway = 3, DoNotDisturb = 4}

	class CopyPresenceOptions extends BaseClass:
		func _init():
			super._init("CopyPresenceOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class CreatePresenceModificationOptions extends BaseClass:
		func _init():
			super._init("CopyPresenceOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class PresenceModificationSetDataOptions extends BaseClass:
		func _init():
			super._init("PresenceModificationSetDataOptions")

		var records: Array[Dictionary] # {key: String, value: String}

	class PresenceModificationDeleteDataOptions extends BaseClass:
		func _init():
			super._init("PresenceModificationDeleteDataOptions")

		var records: Array # Array[String]

	class PresenceModificationSetJoinInfoOptions extends BaseClass:
		func _init():
			super._init("PresenceModificationSetJoinInfoOptions")

		var join_info: String

	class PresenceModificationSetRawRichTextOptions extends BaseClass:
		func _init():
			super._init("PresenceModificationSetRawRichTextOptions")

		var rich_text: String

	class PresenceModificationSetStatusOptions extends BaseClass:
		func _init():
			super._init("PresenceModificationSetStatusOptions")

		var status: int

	class GetJoinInfoOptions extends BaseClass:
		func _init():
			super._init("GetJoinInfoOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class HasPresenceOptions extends BaseClass:
		func _init():
			super._init("HasPresenceOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class QueryPresenceOptions extends BaseClass:
		func _init():
			super._init("QueryPresenceOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class SetPresenceOptions extends BaseClass:
		func _init():
			super._init("SetPresenceOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var presence_modification: RefCounted # PresenceModificationWrapper

		var client_data = null

	class PresenceInterface:
		static func copy_presence(options: CopyPresenceOptions) -> Dictionary:
			return IEOS.presence_interface_copy_presence(options)

		static func create_presence_modification(options: CreatePresenceModificationOptions) -> Dictionary:
			return IEOS.presence_interface_create_presence_modification(options)

		static func get_join_info(options: GetJoinInfoOptions) -> Dictionary:
			return IEOS.presence_interface_get_join_info(options)

		static func has_presence(options: HasPresenceOptions) -> bool:
			return IEOS.presence_interface_has_presence(options)

		static func query_presence(options: QueryPresenceOptions) -> void:
			IEOS.presence_interface_query_presence(options)

		static func set_presence(options: SetPresenceOptions) -> void:
			IEOS.presence_interface_set_presence(options)

class Reports:
	enum PlayerReportsCategory {
		Invalid = 0,
		Cheating = 1,
		Exploiting = 2,
		OffensiveProfile = 3,
		VerbalAbuse = 4,
		Scamming = 5,
		Spamming = 6,
		Other = 7
	}

	class SendPlayerBehaviorReportOptions extends BaseClass:
		func _init():
			super._init("SendPlayerBehaviorReportOptions")

		var category: PlayerReportsCategory
		var context: String
		var message: String
		var reported_user_id: String
		var reporter_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class ReportsInterface:
		static func send_player_behavior_report(options: SendPlayerBehaviorReportOptions) -> void:
			IEOS.reports_interface_send_player_behavior_report(options)

class ProgressionSnapshot:
	class BeginSnapshotOptions extends BaseClass:
		func _init():
			super._init("BeginSnapshotOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class AddProgressionOptions extends BaseClass:
		func _init():
			super._init("AddProgressionOptions")

		var key: String
		var snapshot_id: int
		var value: String

	class SubmitSnapshotOptions extends BaseClass:
		func _init():
			super._init("SubmitSnapshotOptions")

		var snapshot_id: int

		var client_data = null

	class DeleteSnapshotOptions extends BaseClass:
		func _init():
			super._init("DeleteSnapshotOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class EndSnapshotOptions extends BaseClass:
		func _init():
			super._init("EndSnapshotOptions")

		var snapshot_id: int

	class ProgressionSnapshotInterface:
		static func begin_snapshot(options: BeginSnapshotOptions) -> Dictionary:
			return IEOS.progression_snapshot_interface_begin_snapshot(options)

		static func add_progression(options: AddProgressionOptions) -> Result:
			return IEOS.progression_snapshot_interface_add_progression(options)

		static func submit_snapshot(options: SubmitSnapshotOptions) -> void:
			IEOS.progression_snapshot_interface_submit_snapshot(options)

		static func delete_snapshot(options: DeleteSnapshotOptions) -> void:
			IEOS.progression_snapshot_interface_delete_snapshot(options)

		static func end_snapshot(options: EndSnapshotOptions) -> void:
			IEOS.progression_snapshot_interface_end_snapshot(options)

class UI:
	const ModifierShift = 16
	const KeyTypeMask = (1 << ModifierShift) - 1
	const ModifierMask = ~KeyTypeMask
	const Shift = 1 << ModifierShift
	const Control_ = 2 << ModifierShift
	const Alt = 4 << ModifierShift
	const Meta = 8 << ModifierShift
	const ValidModifierMask = Shift|Control_|Alt|Meta

	enum NotificationLocation {TopLeft, TopRight, BottomLeft, BottomRight}

	enum KeyCombination {
		ModifierShift = 16,
		KeyTypeMask = (1 << ModifierShift) - 1,
		ModifierMask = ~KeyTypeMask,
		Shift = 1 << ModifierShift,
		Control_ = 2 << ModifierShift,
		Alt = 4 << ModifierShift,
		Meta = 8 << ModifierShift,
		ValidModifierMask = Shift|Control_|Alt|Meta,
		None = 0,
		Space,
		Backspace,
		Tab,
		Escape,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Left,
		Up,
		Right,
		Down,
		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		KeyA,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		NumpadAsterisk,
		NumpadPlus,
		NumpadMinus,
		NumpadPeriod,
		NumpadDivide,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		OemPlus,
		OemComma,
		OemMinus,
		OemPeriod,
		Oem1,
		Oem2,
		Oem3,
		Oem4,
		Oem5,
		Oem6,
		Oem7,
		Oem8,
		MaxKeyType
	}

	enum InputStateButtonFlags {
		None = 0,
		DPadLeft = (1 << 0),
		DPadRight = (1 << 1),
		DPadDown = (1 << 2),
		DPadUp = (1 << 3),
		FaceButtonLeft = (1 << 4),
		FaceButtonRight = (1 << 5),
		FaceButtonBottom = (1 << 6),
		FaceButtonTop = (1 << 7),
		LeftShoulder = (1 << 8),
		RightShoulder = (1 << 9),
		LeftTrigger = (1 << 10),
		RightTrigger = (1 << 11),
		SpecialLeft = (1 << 12),
		SpecialRight = (1 << 13),
		LeftThumbstick = (1 << 14),
		RightThumbstick = (1 << 15),
	}

	class AcknowledgeEventIdOptions extends BaseClass:
		func _init():
			super._init("AcknowledgeEventIdOptions")

		var ui_event_id: int
		var result: int

	class GetFriendsVisibleOptions extends BaseClass:
		func _init():
			super._init("GetFriendsVisibleOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class GetToggleFriendsKeyOptions extends BaseClass:
		func _init():
			super._init("GetToggleFriendsKeyOptions")

	class HideFriendsOptions extends BaseClass:
		func _init():
			super._init("HideFriendsOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class SetDisplayPreferenceOptions extends BaseClass:
		func _init():
			super._init("SetDisplayPreferenceOptions")

		var notification_location: NotificationLocation

	class SetToggleFriendsKeyOptions extends BaseClass:
		func _init():
			super._init("SetToggleFriendsKeyOptions")

		var key_combination: KeyCombination

	class ShowFriendsOptions extends BaseClass:
		func _init():
			super._init("ShowFriendsOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class GetFriendsExclusiveInputOptions extends BaseClass:
		func _init():
			super._init("GetFriendsExclusiveInputOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id

	class ShowBlockPlayerOptions extends BaseClass:
		func _init():
			super._init("ShowBlockPlayerOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id: String

	class ShowReportPlayerOptions extends BaseClass:
		func _init():
			super._init("ShowReportPlayerOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id: String

	class PauseSocialOverlayOptions extends BaseClass:
		func _init():
			super._init("PauseSocialOverlayOptions")

		var is_paused: bool

	class IsSocialOverlayPausedOptions extends BaseClass:
		func _init():
			super._init("IsSocialOverlayPausedOptions")

	class ReportInputStateOptions extends BaseClass:
		func _init():
			super._init("ReportInputStateOptions")

		var button_down_flags: InputStateButtonFlags
		var gamepad_index: int
		var left_stick_x: float
		var left_stick_y: float
		var left_trigger: float
		var mouse_pos_x: int
		var mouse_pos_y: int
		var right_stick_x: float
		var right_stick_y: float
		var right_trigger: float

	class PrePresentOptions extends BaseClass:
		func _init():
			super._init("PrePresentOptions")

	class ShowNativeProfileOptions extends BaseClass:
		func _init():
			super._init("ShowNativeProfileOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id: String

	class UIInterface:
		static func acknowledge_event_id(options: AcknowledgeEventIdOptions) -> Result:
			return IEOS.ui_interface_acknowledge_event_id(options)

		static func get_friends_visible(options: GetFriendsVisibleOptions) -> bool:
			return IEOS.ui_interface_get_friends_visible(options)

		static func get_notification_location_preference() -> EOS.UI.NotificationLocation:
			return IEOS.ui_interface_get_notification_location_preference()

		static func get_toggle_friends_key(options: GetToggleFriendsKeyOptions) -> EOS.UI.KeyCombination:
			return IEOS.ui_interface_get_toggle_friends_key(options)

		static func hide_friends(options: HideFriendsOptions) -> void:
			IEOS.ui_interface_hide_friends(options)

		static func is_valid_key_combination(key_combination: EOS.UI.KeyCombination) -> bool:
			return IEOS.ui_interface_is_valid_key_combination(key_combination)

		static func set_display_preference(options: SetDisplayPreferenceOptions) -> EOS.UI.NotificationLocation:
			return IEOS.ui_interface_set_display_preference(options)

		static func set_toggle_friends_key(options: SetToggleFriendsKeyOptions) -> Result:
			return IEOS.ui_interface_set_toggle_friends_key(options)

		static func show_friends(options: ShowFriendsOptions) -> void:
			IEOS.ui_interface_show_friends(options)

		static func get_friends_exclusive_input(options: GetFriendsExclusiveInputOptions) -> bool:
			return IEOS.ui_interface_get_friends_exclusive_input(options)

		static func show_block_player(options: ShowBlockPlayerOptions) -> void:
			IEOS.ui_interface_show_block_player(options)

		static func show_report_player(options: ShowReportPlayerOptions) -> void:
			IEOS.ui_interface_show_report_player(options)

		static func pause_social_overlay(options: PauseSocialOverlayOptions) -> Result:
			return IEOS.ui_interface_pause_social_overlay(options)

		static func is_social_overlay_paused(options: IsSocialOverlayPausedOptions) -> bool:
			return IEOS.ui_interface_is_social_overlay_paused(options)

		static func set_toggle_friends_button(options: SetToggleFriendsKeyOptions) -> Result:
			return IEOS.ui_interface_set_toggle_friends_button(options)

		static func report_input_state(options: ReportInputStateOptions) -> Result:
			return IEOS.ui_interface_report_input_state(options)

		static func pre_present(options: PrePresentOptions) -> Result:
			return IEOS.ui_interface_pre_present(options)

		static func show_native_profile(options: ShowNativeProfileOptions) -> void:
			IEOS.ui_interface_show_native_profile(options)

class UserInfo:
	class CopyExternalUserInfoByAccountIdOptions extends BaseClass:
		func _init():
			super._init("CopyExternalUserInfoByAccountIdOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id
		var account_id: String

	class CopyExternalUserInfoByAccountTypeOptions extends BaseClass:
		func _init():
			super._init("CopyExternalUserInfoByAccountTypeOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id
		var account_type: ExternalAccountType

	class CopyExternalUserInfoByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyExternalUserInfoByIndexOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id
		var index: int

	class CopyUserInfoOptions extends BaseClass:
		func _init():
			super._init("CopyUserInfoOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class GetExternalUserInfoCountOptions extends BaseClass:
		func _init():
			super._init("GetExternalUserInfoCountOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class QueryUserInfoOptions extends BaseClass:
		func _init():
			super._init("QueryUserInfoOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class QueryUserInfoByDisplayNameOptions extends BaseClass:
		func _init():
			super._init("QueryUserInfoByDisplayNameOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var display_name: String

		var client_data = null

	class QueryUserInfoByExternalAccountOptions extends BaseClass:
		func _init():
			super._init("QueryUserInfoByExternalAccountOptions")

		var account_type: ExternalAccountType
		var external_account_id: String
		var local_user_id = EOSGRuntime.local_epic_account_id

		var client_data = null

	class CopyBestDisplayNameOptions extends BaseClass:
		func _init():
			super._init("CopyBestDisplayNameOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id

	class CopyBestDisplayNameWithPlatformOptions extends BaseClass:
		func _init():
			super._init("CopyBestDisplayNameWithPlatformOptions")

		var local_user_id = EOSGRuntime.local_epic_account_id
		var target_user_id = EOSGRuntime.local_epic_account_id
		var target_platform_type: OnlinePlatformType

	class GetLocalPlatformTypeOptions extends BaseClass:
		func _init():
			super._init("GetLocalPlatformTypeOptions")

	class UserInfoInterface:
		static func copy_external_user_info_by_account_id(options: CopyExternalUserInfoByAccountIdOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_external_user_info_by_account_id(options)

		static func copy_external_user_info_by_account_type(options: CopyExternalUserInfoByAccountTypeOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_external_user_info_by_account_type(options)

		static func copy_external_user_info_by_index(options: CopyExternalUserInfoByIndexOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_external_user_info_by_index(options)

		static func copy_user_info(options: CopyUserInfoOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_user_info(options)

		static func get_external_user_info_count(options: GetExternalUserInfoCountOptions) -> int:
			return IEOS.user_info_interface_get_external_user_info_count(options)

		static func query_user_info(options: QueryUserInfoOptions) -> void:
			IEOS.user_info_interface_query_user_info(options)

		static func query_user_info_by_display_name(options: QueryUserInfoByDisplayNameOptions) -> void:
			IEOS.user_info_interface_query_user_info_by_display_name(options)

		static func query_user_info_by_external_account(options: QueryUserInfoByExternalAccountOptions) -> void:
			IEOS.user_info_interface_query_user_info_by_external_account(options)

		static func copy_best_display_name(options: CopyBestDisplayNameOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_best_display_name(options)

		static func copy_best_display_name_with_platform(options: CopyBestDisplayNameWithPlatformOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_best_display_name_with_platform(options)

		static func get_local_platform_type(options: GetLocalPlatformTypeOptions=GetLocalPlatformTypeOptions.new()) -> OnlinePlatformType:
			return IEOS.user_info_interface_get_local_platform_type(options)

class Logging:
	enum LogCategory {
		Core = 0,
		Auth = 1,
		Friends = 2,
		Presence = 3,
		UserInfo = 4,
		HttpSerialization = 5,
		Ecom = 6,
		P2P = 7,
		Sessions = 8,
		RateLimiter = 9,
		PlayerDataStorage = 10,
		Analytics = 11,
		Messaging = 12,
		Connect = 13,
		Overlay = 14,
		Achievements = 15,
		Stats = 16,
		Ui = 17,
		Lobby = 18,
		Leaderboards = 19,
		Keychain = 20,
		IntegratedPlatform = 21,
		TitleStorage = 22,
		Mods = 23,
		AntiCheat = 24,
		Reports = 25,
		Sanctions = 26,
		ProgressionSnapshots = 27,
		Kws = 28,
		Rtc = 29,
		RTCAdmin = 30,
		CustomInvites = 31,
		AllCategories = 0x7fffffff
	}

	enum LogLevel {
		Off = 0,
		Fatal = 100,
		Error = 200,
		Warning = 300,
		Info = 400,
		Verbose = 500,
		VeryVerbose = 600
	}

	class LogMessage extends BaseClass:
		func _init():
			super._init("LogMessage")

		var category: String
		var level: LogLevel
		var message: String

		static func from(msg_dict: Dictionary) -> LogMessage:
			var msg = LogMessage.new()
			msg.category = msg_dict.get("category")
			msg.level = msg_dict.get("level")
			msg.message = msg_dict.get("message")
			return msg

		func _to_string() -> String:
			return "%s | %s | %s" % [category, level, message]

	static func set_log_level(log_category: EOS.Logging.LogCategory, log_level: EOS.Logging.LogLevel) -> Result:
		return IEOS.logging_interface_set_log_level(log_category, log_level)

class TitleStorage:
	class QueryFileOptions extends BaseClass:
		func _init():
			super._init("QueryFileOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String

		var client_data = null

	class QueryFileListOptions extends BaseClass:
		func _init():
			super._init("QueryFileListOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var list_of_tags: Array # [String]

		var client_data = null

	class CopyFileMetadataByFilenameOptions extends BaseClass:
		func _init():
			super._init("CopyFileMetadataByFilenameOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String

	class GetFileMetadataCountOptions extends BaseClass:
		func _init():
			super._init("GetFileMetadataCountOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class CopyFileMetadataAtIndexOptions extends BaseClass:
		func _init():
			super._init("CopyFileMetadataAtIndexOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var index: int

	class DeleteCacheOptions extends BaseClass:
		func _init():
			super._init("DeleteCacheOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class ReadFileOptions extends BaseClass:
		func _init():
			super._init("ReadFileOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String
		var read_chunk_length_bytes = 4096

		var client_data = null

	class TitleStorageInterface:
		static func query_file(options: QueryFileOptions) -> void:
			IEOS.titlestorage_interface_query_file(options)

		static func query_file_list(options: QueryFileListOptions) -> void:
			IEOS.titlestorage_interface_query_file_list(options)

		static func copy_file_metadata_by_filename(options: CopyFileMetadataByFilenameOptions) -> Dictionary:
			return IEOS.titlestorage_interface_copy_file_metadata_by_filename(options)

		static func get_file_metadata_count(options: GetFileMetadataCountOptions) -> int:
			return IEOS.titlestorage_interface_get_file_metadata_count(options)

		static func copy_file_metadata_at_index(options) -> Dictionary:
			return IEOS.titlestorage_interface_copy_file_metadata_at_index(options)

		static func delete_cache(options: DeleteCacheOptions) -> Result:
			return IEOS.titlestorage_interface_delete_cache(options)

		static func read_file(options: ReadFileOptions) -> Variant:
			return IEOS.titlestorage_interface_read_file(options)

class PlayerDataStorage:
	class QueryFileOptions extends BaseClass:
		func _init():
			super._init("QueryFileOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String

		var client_data = null

	class QueryFileListOptions extends BaseClass:
		func _init():
			super._init("QueryFileListOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class CopyFileMetadataByFilenameOptions extends BaseClass:
		func _init():
			super._init("CopyFileMetadataByFilenameOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String

	class GetFileMetadataCountOptions extends BaseClass:
		func _init():
			super._init("GetFileMetadataCountOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class CopyFileMetadataAtIndexOptions extends BaseClass:
		func _init():
			super._init("CopyFileMetadataAtIndexOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var index: int

	class DuplicateFileOptions extends BaseClass:
		func _init():
			super._init("DuplicateFileOptions")

		var destination_filename: String
		var local_user_id = EOSGRuntime.local_product_user_id
		var source_filename: String

		var client_data = null

	class DeleteFileOptions extends BaseClass:
		func _init():
			super._init("DeleteFileOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String

		var client_data = null

	class DeleteCacheOptions extends BaseClass:
		func _init():
			super._init("DeleteCacheOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class ReadFileOptions extends BaseClass:
		func _init():
			super._init("ReadFileOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String
		var read_chunk_length_bytes = 4096

		var client_data = null

	class WriteFileOptions extends BaseClass:
		func _init():
			super._init("WriteFileOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var filename: String

		var data: PackedByteArray
		var chunk_length_bytes = 4096

		var client_data = null
		var written_buffer_length_bytes = 0

	class PlayerDataStorageInterface:
		static func query_file(options: QueryFileOptions) -> void:
			IEOS.playerdatastorage_interface_query_file(options)

		static func query_file_list(options: QueryFileListOptions) -> void:
			IEOS.playerdatastorage_interface_query_file_list(options)

		static func copy_file_metadata_by_filename(options: CopyFileMetadataByFilenameOptions) -> Dictionary:
			return IEOS.playerdatastorage_interface_copy_file_metadata_by_filename(options)

		static func get_file_metadata_count(options: GetFileMetadataCountOptions) -> Dictionary:
			return IEOS.playerdatastorage_interface_get_file_metadata_count(options)

		static func copy_file_metadata_at_index(options) -> Dictionary:
			return IEOS.playerdatastorage_interface_copy_file_metadata_at_index(options)

		static func duplicate_file(options: DuplicateFileOptions) -> void:
			IEOS.playerdatastorage_interface_duplicate_file(options)

		static func delete_file(options: DeleteFileOptions) -> void:
			IEOS.playerdatastorage_interface_delete_file(options)

		static func delete_cache(options: DeleteCacheOptions) -> Result:
			return IEOS.playerdatastorage_interface_delete_cache(options)

		static func read_file(options: ReadFileOptions) -> Variant:
			return IEOS.playerdatastorage_interface_read_file(options)

		static func write_file(options: WriteFileOptions) -> Variant:
			return IEOS.playerdatastorage_interface_write_file(options)

class Sanctions:
	enum AppealReason {
		Invalid = 0,
		IncorrectSanction = 1,
		CompromisedAccount = 2,
		UnfairPunishment = 3,
		AppealForForgiveness = 4,
	}

	class QueryActivePlayerSanctionsOptions extends BaseClass:
		func _init():
			super._init("QueryActivePlayerSanctionsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class GetPlayerSanctionCountOptions extends BaseClass:
		func _init():
			super._init("GetPlayerSanctionCountOptions")

		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class CopyPlayerSanctionByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyPlayerSanctionByIndexOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var sanction_index: int
		var target_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class CreatePlayerSanctionAppealOptions extends BaseClass:
		func _init():
			super._init("CreatePlayerSanctionAppealOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var reason: AppealReason
		var reference_id: String

		var client_data = null

	class SanctionsInterface:
		static func query_active_player_sanctions(options: QueryActivePlayerSanctionsOptions) -> void:
			IEOS.sanctions_interface_query_active_player_sanctions(options)

		static func get_player_sanction_count(options: GetPlayerSanctionCountOptions) -> int:
			return IEOS.sanctions_interface_get_player_sanction_count(options)

		static func copy_player_sanction_by_index(options: CopyPlayerSanctionByIndexOptions) -> Variant:
			return IEOS.sanctions_interface_copy_player_sanction_by_index(options)

		static func create_player_sanction_appeal(options: CreatePlayerSanctionAppealOptions) -> void:
			IEOS.sanctions_interface_create_player_sanction_appeal(options)

class Sessions:
	enum OnlineSessionState {
		NoSession,
		Creating,
		Pending,
		Starting,
		InProgress,
		Ending,
		Ended,
		Destroying
	}

	enum SessionAttributeAdvertisementType {
		DontAdvertise,
		Advertise
	}

	enum OnlineSessionPermissionLevel {
		PublicAdvertised,
		JoinViaPresence,
		InviteOnly
	}

	const SEARCH_BUCKET_ID = "bucket"
	const SEARCH_EMPTY_SERVERS_ONLY = "emptyonly"
	const SEARCH_NONEMPTY_SERVERS_ONLY = "nonemptyonly"
	const SEARCH_MINSLOTSAVAILABLE = "minslotsavailable"

	class CopyActiveSessionDetailsOptions extends BaseClass:
		func _init():
			super._init("CopyActiveSessionDetailsOptions")

		var session_name: String

	class CopySessionDetailsByInviteIdOptions extends BaseClass:
		func _init():
			super._init("CopySessionDetailsByInviteIdOptions")

		var invite_id: String

	class CopySessionDetailsByUiEventIdOptions extends BaseClass:
		func _init():
			super._init("CopySessionDetailsByUiEventIdOptions")

		var ui_event_id: String

	class CopySessionDetailsForPresenceOptions extends BaseClass:
		func _init():
			super._init("CopySessionDetailsForPresenceOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class CreateSessionModificationOptions extends BaseClass:
		func _init():
			super._init("CreateSessionModificationOptions")

		var allowed_platform_ids = [] # Array[int]
		var bucket_id: String
		var local_user_id = EOSGRuntime.local_product_user_id
		var max_players: int
		var presence_enabled: bool
		var sanctions_enabled: bool
		var session_id: String
		var session_name: String

	class CreateSessionSearchOptions extends BaseClass:
		func _init():
			super._init("CreateSessionSearchOptions")

		var max_search_results = 10

	class GetInviteIdByIndexOptions extends BaseClass:
		func _init():
			super._init("GetInviteIdByIndexOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var index: int

	class UpdateSessionModificationOptions extends BaseClass:
		func _init():
			super._init("UpdateSessionModificationOptions")

		var session_name: String

	class DumpSessionStateOptions extends BaseClass:
		func _init():
			super._init("DumpSessionStateOptions")

		var session_name: String

	class GetInviteCountOptions extends BaseClass:
		func _init():
			super._init("GetInviteCountOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

	class IsUserInSessionOptions extends BaseClass:
		func _init():
			super._init("IsUserInSessionOptions")

		var session_name: String
		var target_user_id = EOSGRuntime.local_product_user_id

	class DestroySessionOptions extends BaseClass:
		func _init():
			super._init("DestroySessionOptions")

		var session_name: String

		var client_data = null

	class EndSessionOptions extends BaseClass:
		func _init():
			super._init("EndSessionOptions")

		var session_name: String

		var client_data = null

	class JoinSessionOptions extends BaseClass:
		func _init():
			super._init("JoinSessionOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var presence_enabled: bool
		var session_details: EOSGSessionDetails
		var session_name: String

		var client_data = null

	class QueryInvitesOptions extends BaseClass:
		func _init():
			super._init("QueryInvitesOptions")

		var local_user_id = EOSGRuntime.local_product_user_id

		var client_data = null

	class RegisterPlayersOptions extends BaseClass:
		func _init():
			super._init("RegisterPlayersOptions")

		var session_name: String
		var players_to_register: Array # Array[String]

		var client_data = null

	class RejectInviteOptions extends BaseClass:
		func _init():
			super._init("RejectInviteOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var invite_id: String

		var client_data = null

	class SendInviteOptions extends BaseClass:
		func _init():
			super._init("SendInviteOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var session_name: String
		var target_user_id: String

		var client_data = null

	class StartSessionOptions extends BaseClass:
		func _init():
			super._init("StartSessionOptions")

		var session_name: String

		var client_data = null

	class UnregisterPlayersOptions extends BaseClass:
		func _init():
			super._init("UnregisterPlayersOptions")

		var session_name: String
		var players_to_unregister: Array # Array[String]

		var client_data = null

	class UpdateSessionOptions extends BaseClass:
		func _init():
			super._init("UpdateSessionOptions")

		var session_modification: EOSGSessionModification

		var client_data = null

	class SessionsInterface:
		static func copy_active_session_details(options: CopyActiveSessionDetailsOptions) -> Dictionary:
			return IEOS.sessions_interface_copy_active_session_details(options)

		static func copy_session_details_by_invite_id(options: CopySessionDetailsByInviteIdOptions) -> Dictionary:
			return IEOS.sessions_interface_copy_session_details_by_invite_id(options)

		static func copy_session_details_by_ui_event_id(options: CopySessionDetailsByUiEventIdOptions) -> Dictionary:
			return IEOS.sessions_interface_copy_session_details_by_ui_event_id(options)

		static func copy_session_details_for_presence(options: CopySessionDetailsForPresenceOptions) -> Dictionary:
			return IEOS.sessions_interface_copy_session_details_for_presence(options)

		static func create_session_modification(options: CreateSessionModificationOptions) -> Dictionary:
			return IEOS.sessions_interface_create_session_modification(options)

		static func create_session_search(options: CreateSessionSearchOptions) -> Dictionary:
			return IEOS.sessions_interface_create_session_search(options)

		static func get_invite_id_by_index(options: GetInviteIdByIndexOptions) -> Dictionary:
			return IEOS.sessions_interface_get_invite_id_by_index(options)

		static func update_session_modification(options: UpdateSessionModificationOptions) -> Dictionary:
			return IEOS.sessions_interface_update_session_modification(options)

		static func dump_session_state(options: DumpSessionStateOptions) -> int:
			return IEOS.sessions_interface_dump_session_state(options)

		static func get_invite_count(options: GetInviteCountOptions) -> int:
			return IEOS.sessions_interface_get_invite_count(options)

		static func is_user_in_session(options: IsUserInSessionOptions) -> int:
			return IEOS.sessions_interface_is_user_in_session(options)

		static func destroy_session(options: DestroySessionOptions) -> void:
			IEOS.sessions_interface_destroy_session(options)

		static func end_session(options: EndSessionOptions) -> void:
			IEOS.sessions_interface_end_session(options)

		static func join_session(options: JoinSessionOptions) -> void:
			IEOS.sessions_interface_join_session(options)

		static func query_invites(options: QueryInvitesOptions) -> void:
			IEOS.sessions_interface_query_invites(options)

		static func register_players(options: RegisterPlayersOptions) -> void:
			IEOS.sessions_interface_register_players(options)

		static func reject_invite(options: RejectInviteOptions) -> void:
			IEOS.sessions_interface_reject_invite(options)

		static func send_invite(options: SendInviteOptions) -> void:
			IEOS.sessions_interface_send_invite(options)

		static func start_session(options: StartSessionOptions) -> void:
			IEOS.sessions_interface_start_session(options)

		static func unregister_players(options: UnregisterPlayersOptions) -> void:
			IEOS.sessions_interface_unregister_players(options)

		static func update_session(options: UpdateSessionOptions) -> void:
			IEOS.sessions_interface_update_session(options)

class RTC:

	enum ParticipantStatus {
		Joined,
		Left,
	}

	class AddNotifyDisconnectedOptions extends BaseClass:
		func _init():
			super._init("AddNotifyDisconnectedOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class AddNotifyParticipantStatusChangedOptions extends BaseClass:
		func _init():
			super._init("AddNotifyParticipantStatusChangedOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class AddNotifyRoomStatisticsUpdatedOptions extends BaseClass:
		func _init():
			super._init("AddNotifyRoomStatisticsUpdatedOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class SetRoomSettingOptions extends BaseClass:
		func _init():
			super._init("SetRoomSettingOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var setting_key: String
		var setting_value: String

	class SetSettingOptions extends BaseClass:
		func _init():
			super._init("SetSettingOptions")

		var setting_key: String
		var setting_value: String

	class BlockParticipantOptions extends BaseClass:
		func _init():
			super._init("BlockParticipantOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var participant_id: String
		var blocked: bool

		var client_data = null

	class JoinRoomOptions extends BaseClass:
		func _init():
			super._init("JoinRoomOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var client_base_url: String
		var participant_token: String
		var participant_id: String
		var flags: int
		var manual_audio_input_enabled: bool
		var manual_audio_output_enabled: bool

		var client_data = null

	class LeaveRoomOptions extends BaseClass:
		func _init():
			super._init("LeaveRoomOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

		var client_data = null

	class RTCInterface:
		static func add_notify_disconnected(options: AddNotifyDisconnectedOptions) -> int:
			return IEOS.rtc_interface_add_notify_disconnected(options)

		static func add_notify_participant_status_changed(options: AddNotifyParticipantStatusChangedOptions) -> int:
			return IEOS.rtc_interface_add_notify_participant_status_changed(options)

		static func add_notify_room_statistics_updated(options: AddNotifyRoomStatisticsUpdatedOptions) -> int:
			return IEOS.rtc_interface_add_notify_room_statistics_updated(options)

		static func set_room_setting(options: SetRoomSettingOptions) -> int:
			return IEOS.rtc_interface_set_room_setting(options)

		static func set_setting(options: SetSettingOptions) -> int:
			return IEOS.rtc_interface_set_setting(options)

		static func block_participant(options: BlockParticipantOptions) -> void:
			IEOS.rtc_interface_block_participant(options)

		static func join_room(options: JoinRoomOptions) -> void:
			IEOS.rtc_interface_join_room(options)

		static func leave_room(options: LeaveRoomOptions) -> void:
			IEOS.rtc_interface_leave_room(options)

		static func remove_notify_disconnected(notification_id: int) -> void:
			IEOS.rtc_interface_remove_notify_disconnected(notification_id)

		static func remove_notify_participant_status_changed(notification_id: int) -> void:
			IEOS.rtc_interface_remove_notify_participant_status_changed(notification_id)

		static func remove_notify_room_statistics_updated(notification_id: int) -> void:
			IEOS.rtc_interface_remove_notify_room_statistics_updated(notification_id)

class RTCAudio:
	enum AudioStatus {
		## Audio unsupported by the source (no devices)
		Unsupported = 0,
		## Audio enabled
		Enabled = 1,
		## Audio disabled
		Disabled = 2,
		## Audio disabled by the administrator
		AdminDisabled = 3,
		## Audio channel is disabled temporarily for both sending and receiving
		NotListeningDisabled = 4
	}

	enum AudioInputStatus {
		## The device is not in used right now (e.g: you are alone in the room). In such cases, the hardware resources are not allocated.
		Idle = 0,
		## The device is being used and capturing audio
		Recording = 1,
		## The SDK is in a recording state, but actually capturing silence because the device is exclusively being used by the platform at the moment.
		## This only applies to certain platforms.
		RecordingSilent = 2,
		## The SDK is in a recording state, but actually capturing silence because the device is disconnected (e.g: the microphone is not plugged in).
		## This only applies to certain platforms.
		RecordingDisconnected = 3,
		## Something failed while trying to use the device
		Failed = 4
	}

	enum AudioOutputStatus {
		## The device is not in used right now (e.g: you are alone in the room). In such cases, the hardware resources are not allocated.
		Idle = 0,
		## Device is in use
		Playing = 1,
		## Something failed while trying to use the device
		Failed = 2
	}

	class CopyInputDeviceInformationByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyInputDeviceInformationByIndexOptions")

		var device_index: int

	class CopyOutputDeviceInformationByIndexOptions extends BaseClass:
		func _init():
			super._init("CopyOutputDeviceInformationByIndexOptions")

		var device_index: int

	class AddNotifyAudioBeforeRenderOptions extends BaseClass:
		func _init():
			super._init("AddNotifyAudioBeforeRenderOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var unmixed_audio: bool

	class AddNotifyAudioBeforeSendOptions extends BaseClass:
		func _init():
			super._init("AddNotifyAudioBeforeSendOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class AddNotifyAudioInputStateOptions extends BaseClass:
		func _init():
			super._init("AddNotifyAudioInputStateOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class AddNotifyAudioOutputStateOptions extends BaseClass:
		func _init():
			super._init("AddNotifyAudioOutputStateOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class AddNotifyParticipantUpdatedOptions extends BaseClass:
		func _init():
			super._init("AddNotifyParticipantUpdatedOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String

	class GetInputDevicesCountOptions extends BaseClass:
		func _init():
			super._init("GetInputDevicesCountOptions")

	class GetOutputDevicesCountOptions extends BaseClass:
		func _init():
			super._init("GetOutputDevicesCountOptions")

	class SendAudioOptions extends BaseClass:
		func _init():
			super._init("SendAudioOptions")

		# TODO: update once send_audio is implemented

	class QueryInputDevicesInformationOptions extends BaseClass:
		func _init():
			super._init("QueryInputDevicesInformationOptions")

			var client_data = null

	class QueryOutputDevicesInformationOptions extends BaseClass:
		func _init():
			super._init("QueryOutputDevicesInformationOptions")

		var client_data = null

	class RegisterPlatformUserOptions extends BaseClass:
		func _init():
			super._init("RegisterPlatformUserOptions")

		var platform_user_id: String

		var client_data = null

	class SetInputDeviceSettingsOptions extends BaseClass:
		func _init():
			super._init("SetInputDeviceSettingsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var real_device_id: String
		var platform_aec: bool

		var client_data = null

	class SetOutputDeviceSettingsOptions extends BaseClass:
		func _init():
			super._init("SetOutputDeviceSettingsOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var real_device_id: String

		var client_data = null

	class UnregisterPlatformUserOptions extends BaseClass:
		func _init():
			super._init("UnregisterPlatformUserOptions")

		var platform_user_id: String

		var client_data = null

	class UpdateParticipantVolumeOptions extends BaseClass:
		func _init():
			super._init("UpdateParticipantVolumeOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var participant_id: String
		var volume: float

		var client_data = null

	class UpdateReceivingOptions extends BaseClass:
		func _init():
			super._init("UpdateReceivingOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var participant_id: String
		var audio_enabled: bool

		var client_data = null

	class UpdateReceivingVolumeOptions extends BaseClass:
		func _init():
			super._init("UpdateReceivingVolumeOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var participant_id: String
		var volume: float

		var client_data = null

	class UpdateSendingOptions extends BaseClass:
		func _init():
			super._init("UpdateSendingOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var audio_status: AudioStatus

		var client_data = null

	class UpdateSendingVolumeOptions extends BaseClass:
		func _init():
			super._init("UpdateSendingVolumeOptions")

		var local_user_id = EOSGRuntime.local_product_user_id
		var room_name: String
		var volume: float

		var client_data = null

	class RTCAudioInterface:
		static func copy_input_device_information_by_index(options: CopyInputDeviceInformationByIndexOptions) -> Dictionary:
			return IEOS.rtc_audio_interface_copy_input_device_information_by_index(options)

		static func copy_output_device_information_by_index(options: CopyOutputDeviceInformationByIndexOptions) -> Dictionary:
			return IEOS.rtc_audio_interface_copy_output_device_information_by_index(options)

		static func add_notify_audio_before_render(options: AddNotifyAudioBeforeRenderOptions) -> int:
			return IEOS.rtc_audio_interface_add_notify_audio_before_render(options)

		static func add_notify_audio_before_send(options: AddNotifyAudioBeforeSendOptions) -> int:
			return IEOS.rtc_audio_interface_add_notify_audio_before_send(options)

		static func add_notify_audio_input_state(options: AddNotifyAudioInputStateOptions) -> int:
			return IEOS.rtc_audio_interface_add_notify_audio_input_state(options)

		static func add_notify_audio_output_state(options: AddNotifyAudioOutputStateOptions) -> int:
			return IEOS.rtc_audio_interface_add_notify_audio_output_state(options)

		static func add_notify_participant_updated(options: AddNotifyParticipantUpdatedOptions) -> int:
			return IEOS.rtc_audio_interface_add_notify_participant_updated(options)

		static func get_input_devices_count(options:=GetInputDevicesCountOptions.new()) -> int:
			return IEOS.rtc_audio_interface_get_input_devices_count(options)

		static func get_output_devices_count(options:=GetOutputDevicesCountOptions.new()) -> int:
			return IEOS.rtc_audio_interface_get_output_devices_count(options)

		static func send_audio(options: SendAudioOptions) -> int:
			return IEOS.rtc_audio_interface_send_audio(options)

		static func query_input_devices_information(options:=QueryInputDevicesInformationOptions.new()) -> void:
			IEOS.rtc_audio_interface_query_input_devices_information(options)

		static func query_output_devices_information(options:=QueryOutputDevicesInformationOptions.new()) -> void:
			IEOS.rtc_audio_interface_query_output_devices_information(options)

		static func register_platform_user(options: RegisterPlatformUserOptions) -> void:
			IEOS.rtc_audio_interface_register_platform_user(options)

		static func remove_notify_audio_before_render(notification_id: int) -> void:
			IEOS.rtc_audio_interface_remove_notify_audio_before_render(notification_id)

		static func remove_notify_audio_before_send(notification_id: int) -> void:
			IEOS.rtc_audio_interface_remove_notify_audio_before_send(notification_id)

		static func remove_notify_audio_input_state(notification_id: int) -> void:
			IEOS.rtc_audio_interface_remove_notify_audio_input_state(notification_id)

		static func remove_notify_audio_output_state(notification_id: int) -> void:
			IEOS.rtc_audio_interface_remove_notify_audio_output_state(notification_id)

		static func remove_notify_participant_updated(notification_id: int) -> void:
			IEOS.rtc_audio_interface_remove_notify_participant_updated(notification_id)

		static func set_input_device_settings(options: SetInputDeviceSettingsOptions) -> void:
			IEOS.rtc_audio_interface_set_input_device_settings(options)

		static func set_output_device_settings(options: SetOutputDeviceSettingsOptions) -> void:
			IEOS.rtc_audio_interface_set_output_device_settings(options)

		static func unregister_platform_user(options: UnregisterPlatformUserOptions) -> void:
			IEOS.rtc_audio_interface_unregister_platform_user(options)

		static func update_participant_volume(options: UpdateParticipantVolumeOptions) -> void:
			IEOS.rtc_audio_interface_update_participant_volume(options)

		static func update_receiving(options: UpdateReceivingOptions) -> void:
			IEOS.rtc_audio_interface_update_receiving(options)

		static func update_receiving_volume(options: UpdateReceivingVolumeOptions) -> void:
			IEOS.rtc_audio_interface_update_receiving_volume(options)

		static func update_sending(options: UpdateSendingOptions) -> void:
			IEOS.rtc_audio_interface_update_sending(options)

		static func update_sending_volume(options: UpdateSendingVolumeOptions) -> void:
			IEOS.rtc_audio_interface_update_sending_volume(options)

class AntiCheatServer:

	class BeginSessionOptions extends BaseClass:
		func _init():
			super._init("BeginSessionOptions")

		var register_timeout_seconds := 60
		var server_name: String
		var local_user_id: String
		var enable_gameplay_data: bool

	class EndSessionOptions extends BaseClass:
		func _init():
			super._init("EndSessionOptions")

	class RegisterClientOptions extends BaseClass:
		func _init():
			super._init("RegisterClientOptions")

		var client_handle: String
		var client_type: AntiCheatCommonClientType
		var client_platform: AntiCheatCommonClientPlatform
		var ip_address: String
		var user_id: String

	class UnregisterClientOptions extends BaseClass:
		func _init():
			super._init("UnregisterClientOptions")

		var client_handle: String

	class ReceiveMessageFromClientOptions extends BaseClass:
		func _init():
			super._init("ReceiveMessageFromClientOptions")

		var client_handle: String
		var data: PackedByteArray

	class SetClientDetailsOptions extends BaseClass:
		func _init():
			super._init("SetClientDetailsOptions")

		var client_handle: String
		var client_flags: AntiCheatCommonClientFlags
		var client_input: AntiCheatCommonClientInput

	class SetGameSessionIdOptions extends BaseClass:
		func _init():
			super._init("SetGameSessionIdOptions")

		var game_session_id: String

	class SetClientNetworkStateOptions extends BaseClass:
		func _init():
			super._init("SetClientNetworkStateOptions")

		var client_handle: String
		var is_network_active: bool

	class GetProtectMessageOutputLengthOptions extends BaseClass:
		func _init():
			super._init("GetProtectMessageOutputLengthOptions")

		var data_length_bytes: int

	class ProtectMessageOptions extends BaseClass:
		func _init():
			super._init("ProtectMessageOptions")

		var client_handle: String
		var data: PackedByteArray
		var out_buffer_size_bytes: int

	class UnprotectMessageOptions extends BaseClass:
		func _init():
			super._init("UnprotectMessageOptions")

		var client_handle: String
		var data: PackedByteArray
		var out_buffer_size_bytes: int

	class RegisterEventOptions extends BaseClass:
		func _init():
			super._init("RegisterEventOptions")

		var event_id: String
		var event_name: String
		var event_type: AntiCheatCommonEventType
		var param_defs: Array # {param_type: AntiCheatCommonEventParamType, param_name: String}

	class LogEventOptions extends BaseClass:
		func _init():
			super._init("LogEventOptions")

		var client_handle: String
		var event_id: String
		var params: Array # Supported types int, String, Vector3, float, Quaternion

	class LogGameRoundStartOptions extends BaseClass:
		func _init():
			super._init("LogGameRoundStartOptions")

		var session_identifier: String
		var level_name: String
		var mode_name: String
		var round_time_seconds: int

	class LogGameRoundEndOptions extends BaseClass:
		func _init():
			super._init("LogGameRoundEndOptions")

		var winning_team_id: String

	class LogPlayerSpawnOptions extends BaseClass:
		func _init():
			super._init("LogPlayerSpawnOptions")

		var spawned_player_handle: String
		var team_id: String
		var character_id: String

	class LogPlayerDespawnOptions extends BaseClass:
		func _init():
			super._init("LogPlayerDespawnOptions")

		var despawned_player_handle: String

	class LogPlayerReviveOptions extends BaseClass:
		func _init():
			super._init("LogPlayerReviveOptions")

		var revived_player_handle: String
		var reviver_player_handle: String

	class LogPlayerTickOptions extends BaseClass:
		func _init():
			super._init("LogPlayerTickOptions")

		var player_handle: String
		var player_position: Vector3
		var player_view_rotation: Quaternion
		var player_health: float
		var player_movement_state: AntiCheatCommonPlayerMovementState
		var player_view_position: Vector3
		var is_player_view_zoomed: bool

	class LogPlayerUseWeaponOptions extends BaseClass:
		func _init():
			super._init("LogPlayerUseWeaponOptions")

		var player_handle: String
		var player_position: Vector3
		var player_view_rotation: Quaternion
		var weapon_name: String
		var is_player_view_zoomed: bool
		var is_melee_attacK: bool

	class LogPlayerUseAbilityOptions extends BaseClass:
		func _init():
			super._init("LogPlayerUseAbilityOptions")

		var player_handle: String
		var ability_id: int
		var ability_duration_ms: int
		var ability_cooldown_ms: int

	class LogPlayerTakeDamageOptions extends BaseClass:
		func _init():
			super._init("LogPlayerTakeDamageOptions")

		var victim_player_handle: String
		var victim_player_position: Vector3
		var victim_player_view_rotation: Quaternion

		var attacker_player_handle: String
		var attacker_player_position: Vector3
		var attacker_player_view_rotation: Quaternion

		var damage_taken: float
		var health_remaining: float
		var damage_source: AntiCheatCommonPlayerTakeDamageSource
		var damage_type: AntiCheatCommonPlayerTakeDamageType
		var damage_result: AntiCheatCommonPlayerTakeDamageResult
		var tim_since_player_use_weapon_ms: int
		var damage_position: Vector3
		var attacked_player_view_position: Vector3
		var player_use_weapon_data: LogPlayerUseWeaponOptions

		var is_hitscan_attack: bool
		var has_line_of_sight: bool
		var is_critical_hit: bool

	class AntiCheatServerInterface:
		static func begin_session(options: BeginSessionOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_begin_session(options)

		static func end_session() -> EOS.Result:
			return IEOS.anticheat_server_interface_end_session(EndSessionOptions.new())

		static func register_client(options: RegisterClientOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_register_client(options)

		static func unregister_client(options: UnregisterClientOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_unregister_client(options)

		static func receive_message_from_client(options: ReceiveMessageFromClientOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_receive_message_from_client(options)

		static func set_client_details(options: SetClientDetailsOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_set_client_details(options)

		static func set_game_session_id(options: SetGameSessionIdOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_set_game_session_id(options)

		static func set_client_network_state(options: SetClientNetworkStateOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_set_client_network_state(options)

		static func get_protect_message_output_length(options: GetProtectMessageOutputLengthOptions) -> Dictionary:
			return IEOS.anticheat_server_interface_get_protect_message_output_length(options)

		static func protect_message(options: ProtectMessageOptions) -> Dictionary:
			return IEOS.anticheat_server_interface_protect_message(options)

		static func unprotect_message(options: UnprotectMessageOptions) -> Dictionary:
			return IEOS.anticheat_server_interface_unprotect_message(options)

		static func register_event(options: RegisterEventOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_register_event(options)

		static func log_event(options: LogEventOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_event(options)

		static func log_game_round_start(options: LogGameRoundStartOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_game_round_start(options)

		static func log_game_round_end(options: LogGameRoundEndOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_game_round_end(options)

		static func log_player_spawn(options: LogPlayerSpawnOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_spawn(options)

		static func log_player_despawn(options: LogPlayerDespawnOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_despawn(options)

		static func log_player_revive(options: LogPlayerReviveOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_revive(options)

		static func log_player_tick(options: LogPlayerTickOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_tick(options)

		static func log_player_use_weapon(options: LogPlayerUseWeaponOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_use_weapon(options)

		static func log_player_use_ability(options: LogPlayerUseAbilityOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_use_ability(options)

		static func log_player_take_damage(options: LogPlayerTakeDamageOptions) -> EOS.Result:
			return IEOS.anticheat_server_interface_log_player_take_damage(options)

class AntiCheatClient:

	class BeginSessionOptions extends BaseClass:
		func _init():
			super._init("BeginSessionOptions")

		var local_user_id: String
		var mode: AntiCheatClientMode

	class EndSessionOptions extends BaseClass:
		func _init():
			super._init("EndSessionOptions")

	class AddExternalIntegrityCatalogOptions extends BaseClass:
		func _init():
			super._init("AddExternalIntegrityCatalogOptions")

		var path_to_file_bin: String

	class ReceiveMessageFromServerOptions extends BaseClass:
		func _init():
			super._init("ReceiveMessageFromServerOptions")

		var data: PackedByteArray

	class GetProtectMessageOutputLengthOptions extends BaseClass:
		func _init():
			super._init("GetProtectMessageOutputLengthOptions")

		var data_length_bytes: int

	class ProtectMessageOptions extends BaseClass:
		func _init():
			super._init("ProtectMessageOptions")

		var data: PackedByteArray
		var out_buffer_size_bytes: int

	class UnprotectMessageOptions extends BaseClass:
		func _init():
			super._init("UnprotectMessageOptions")

		var data: PackedByteArray
		var out_buffer_size_bytes: int

	class RegisterPeerOptions extends BaseClass:
		func _init():
			super._init("RegisterPeerOptions")

		var peer_handle: String
		var client_type: AntiCheatCommonClientType
		var client_platform: AntiCheatCommonClientPlatform
		var authentication_timeout: int = 60
		var ip_address: String
		var peer_product_user_id: String

	class UnregisterPeerOptions extends BaseClass:
		func _init():
			super._init("UnregisterPeerOptions")

		var peer_handle: String

	class ReceiveMessageFromPeerOptions extends BaseClass:
		func _init():
			super._init("ReceiveMessageFromPeerOptions")

		var peer_handle: String
		var data: PackedByteArray

	class AntiCheatClientInterface:
		static func begin_session(options: BeginSessionOptions) -> EOS.Result:
			return IEOS.anticheat_client_interface_begin_session(options)

		static func end_session() -> EOS.Result:
			return IEOS.anticheat_client_interface_end_session(EndSessionOptions.new())

		static func add_external_integrity_catalog(options: AddExternalIntegrityCatalogOptions) -> EOS.Result:
			return IEOS.anticheat_client_interface_add_external_integrity_catalog(options)

		static func receive_message_from_server(options: ReceiveMessageFromServerOptions) -> EOS.Result:
			return IEOS.anticheat_client_interface_receive_message_from_server(options)

		static func get_protect_message_output_length(options: GetProtectMessageOutputLengthOptions) -> Dictionary:
			return IEOS.anticheat_client_interface_get_protect_message_output_length(options)

		static func protect_message(options: ProtectMessageOptions) -> Dictionary:
			return IEOS.anticheat_client_interface_protect_message(options)

		static func unprotect_message(options: UnprotectMessageOptions) -> Dictionary:
			return IEOS.anticheat_client_interface_unprotect_message(options)

		static func register_peer(options: RegisterPeerOptions) -> EOS.Result:
			return IEOS.anticheat_client_interface_register_peer(options)

		static func unregister_peer(options: UnregisterPeerOptions) -> EOS.Result:
			return IEOS.anticheat_client_interface_unregister_peer(options)

		static func receive_message_from_peer(options: ReceiveMessageFromPeerOptions) -> EOS.Result:
			return IEOS.anticheat_client_interface_receive_message_from_peer(options)

class Version:
	class VersionInterface:
		static func get_version() -> String:
			return IEOS.version_interface_get_version()

		static func get_constants() -> Dictionary:
			return IEOS.version_interface_get_constants()

enum Result {
	Success = 0,
	NoConnection = 1,
	InvalidCredentials = 2,
	InvalidUser = 3,
	InvalidAuth = 4,
	AccessDenied = 5,
	MissingPermissions = 6,
	TokenNotAccount = 7,
	TooManyRequests = 8,
	AlreadyPending = 9,
	InvalidParameters = 10,
	InvalidRequest = 11,
	UnrecognizedResponse = 12,
	IncompatibleVersion = 13,
	NotConfigured = 14,
	AlreadyConfigured = 15,
	NotImplemented = 16,
	Canceled = 17,
	NotFound = 18,
	OperationWillRetry = 19,
	NoChange = 20,
	VersionMismatch = 21,
	LimitExceeded = 22,
	Disabled = 23,
	DuplicateNotAllowed = 24,
	InvalidSandboxId = 26,
	TimedOut = 27,
	PartialResult = 28,
	MissingRole = 29,
	MissingFeature = 30,
	InvalidSandbox = 31,
	InvalidDeployment = 32,
	InvalidProduct = 33,
	InvalidProductUserID = 34,
	ServiceFailure = 35,
	CacheDirectoryMissing = 36,
	CacheDirectoryInvalid = 37,
	InvalidState = 38,
	RequestInProgress = 39,
	ApplicationSuspended = 40,
	NetworkDisconnected = 41,
	AuthAccountLocked = 1001,
	AuthAccountLockedForUpdate = 1002,
	AuthInvalidRefreshToken = 1003,
	AuthInvalidToken = 1004,
	AuthAuthenticationFailure = 1005,
	AuthInvalidPlatformToken = 1006,
	AuthWrongAccount = 1007,
	AuthWrongClient = 1008,
	AuthFullAccountRequired = 1009,
	AuthHeadlessAccountRequired = 1010,
	AuthPasswordResetRequired = 1011,
	AuthPasswordCannotBeReused = 1012,
	AuthExpired = 1013,
	AuthScopeConsentRequired = 1014,
	AuthApplicationNotFound = 1015,
	AuthScopeNotFound = 1016,
	AuthAccountFeatureRestricted = 1017,
	AuthAccountPortalLoadError = 1018,
	AuthCorrectiveActionRequired = 1019,
	AuthPinGrantCode = 1020,
	AuthPinGrantExpired = 1021,
	AuthPinGrantPending = 1022,
	AuthExternalAuthNotLinked = 1030,
	AuthExternalAuthRevoked = 1032,
	AuthExternalAuthInvalid = 1033,
	AuthExternalAuthRestricted = 1034,
	AuthExternalAuthCannotLogin = 1035,
	AuthExternalAuthExpired = 1036,
	AuthExternalAuthIsLastLoginType = 1037,
	AuthExchangeCodeNotFound = 1040,
	AuthOriginatingExchangeCodeSessionExpired = 1041,
	AuthAccountNotActive = 1050,
	AuthMFARequired = 1060,
	AuthParentalControls = 1070,
	AuthNoRealId = 1080,
	AuthUserInterfaceRequired = 1090,
	FriendsInviteAwaitingAcceptance = 2000,
	FriendsNoInvitation = 2001,
	FriendsAlreadyFriends = 2003,
	FriendsNotFriends = 2004,
	FriendsTargetUserTooManyInvites = 2005,
	FriendsLocalUserTooManyInvites = 2006,
	FriendsTargetUserFriendLimitExceeded = 2007,
	FriendsLocalUserFriendLimitExceeded = 2008,
	PresenceDataInvalid = 3000,
	PresenceDataLengthInvalid = 3001,
	PresenceDataKeyInvalid = 3002,
	PresenceDataKeyLengthInvalid = 3003,
	PresenceDataValueInvalid = 3004,
	PresenceDataValueLengthInvalid = 3005,
	PresenceRichTextInvalid = 3006,
	PresenceRichTextLengthInvalid = 3007,
	PresenceStatusInvalid = 3008,
	EcomEntitlementStale = 4000,
	EcomCatalogOfferStale = 4001,
	EcomCatalogItemStale = 4002,
	EcomCatalogOfferPriceInvalid = 4003,
	EcomCheckoutLoadError = 4004,
	EcomPurchaseProcessing = 4005,
	SessionsSessionInProgress = 5000,
	SessionsTooManyPlayers = 5001,
	SessionsNoPermission = 5002,
	SessionsSessionAlreadyExists = 5003,
	SessionsInvalidLock = 5004,
	SessionsInvalidSession = 5005,
	SessionsSandboxNotAllowed = 5006,
	SessionsInviteFailed = 5007,
	SessionsInviteNotFound = 5008,
	SessionsUpsertNotAllowed = 5009,
	SessionsAggregationFailed = 5010,
	SessionsHostAtCapacity = 5011,
	SessionsSandboxAtCapacity = 5012,
	SessionsSessionNotAnonymous = 5013,
	SessionsOutOfSync = 5014,
	SessionsTooManyInvites = 5015,
	SessionsPresenceSessionExists = 5016,
	SessionsDeploymentAtCapacity = 5017,
	SessionsNotAllowed = 5018,
	SessionsPlayerSanctioned = 5019,
	PlayerDataStorageFilenameInvalid = 6000,
	PlayerDataStorageFilenameLengthInvalid = 6001,
	PlayerDataStorageFilenameInvalidChars = 6002,
	PlayerDataStorageFileSizeTooLarge = 6003,
	PlayerDataStorageFileSizeInvalid = 6004,
	PlayerDataStorageFileHandleInvalid = 6005,
	PlayerDataStorageDataInvalid = 6006,
	PlayerDataStorageDataLengthInvalid = 6007,
	PlayerDataStorageStartIndexInvalid = 6008,
	PlayerDataStorageRequestInProgress = 6009,
	PlayerDataStorageUserThrottled = 6010,
	PlayerDataStorageEncryptionKeyNotSet = 6011,
	PlayerDataStorageUserErrorFromDataCallback = 6012,
	PlayerDataStorageFileHeaderHasNewerVersion = 6013,
	PlayerDataStorageFileCorrupted = 6014,
	ConnectExternalTokenValidationFailed = 7000,
	ConnectUserAlreadyExists = 7001,
	ConnectAuthExpired = 7002,
	ConnectInvalidToken = 7003,
	ConnectUnsupportedTokenType = 7004,
	ConnectLinkAccountFailed = 7005,
	ConnectExternalServiceUnavailable = 7006,
	ConnectExternalServiceConfigurationFailure = 7007,
	SocialOverlayLoadError = 8000,
	InconsistentVirtualMemoryFunctions = 8001,
	LobbyNotOwner = 9000,
	LobbyInvalidLock = 9001,
	LobbyLobbyAlreadyExists = 9002,
	LobbySessionInProgress = 9003,
	LobbyTooManyPlayers = 9004,
	LobbyNoPermission = 9005,
	LobbyInvalidSession = 9006,
	LobbySandboxNotAllowed = 9007,
	LobbyInviteFailed = 9008,
	LobbyInviteNotFound = 9009,
	LobbyUpsertNotAllowed = 9010,
	LobbyAggregationFailed = 9011,
	LobbyHostAtCapacity = 9012,
	LobbySandboxAtCapacity = 9013,
	LobbyTooManyInvites = 9014,
	LobbyDeploymentAtCapacity = 9015,
	LobbyNotAllowed = 9016,
	LobbyMemberUpdateOnly = 9017,
	LobbyPresenceLobbyExists = 9018,
	LobbyVoiceNotEnabled = 9019,
	LobbyPlatformNotAllowed = 9020,
	TitleStorageUserErrorFromDataCallback = 10000,
	TitleStorageEncryptionKeyNotSet = 10001,
	TitleStorageFileCorrupted = 10002,
	TitleStorageFileHeaderHasNewerVersion = 10003,
	ModsModSdkProcessIsAlreadyRunning = 11000,
	ModsModSdkCommandIsEmpty = 11001,
	ModsModSdkProcessCreationFailed = 11002,
	ModsCriticalError = 11003,
	ModsToolInternalError = 11004,
	ModsIPCFailure = 11005,
	ModsInvalidIPCResponse = 11006,
	ModsURILaunchFailure = 11007,
	ModsModIsNotInstalled = 11008,
	ModsUserDoesNotOwnTheGame = 11009,
	ModsOfferRequestByIdInvalidResult = 11010,
	ModsCouldNotFindOffer = 11011,
	ModsOfferRequestByIdFailure = 11012,
	ModsPurchaseFailure = 11013,
	ModsInvalidGameInstallInfo = 11014,
	ModsCannotGetManifestLocation = 11015,
	ModsUnsupportedOS = 11016,
	AntiCheatClientProtectionNotAvailable = 12000,
	AntiCheatInvalidMode = 12001,
	AntiCheatClientProductIdMismatch = 12002,
	AntiCheatClientSandboxIdMismatch = 12003,
	AntiCheatProtectMessageSessionKeyRequired = 12004,
	AntiCheatProtectMessageValidationFailed = 12005,
	AntiCheatProtectMessageInitializationFailed = 12006,
	AntiCheatPeerAlreadyRegistered = 12007,
	AntiCheatPeerNotFound = 12008,
	AntiCheatPeerNotProtected = 12009,
	AntiCheatClientDeploymentIdMismatch = 12010,
	AntiCheatDeviceIdAuthIsNotSupported = 12011,
	TooManyParticipants = 13000,
	RoomAlreadyExists = 13001,
	UserKicked = 13002,
	UserBanned = 13003,
	RoomWasLeft = 13004,
	ReconnectionTimegateExpired = 13005,
	ShutdownInvoked = 13006,
	UserIsInBlocklist = 13007,
	ProgressionSnapshotSnapshotIdUnavailable = 14000,
	ParentEmailMissing = 15000,
	UserGraduated = 15001,
	AndroidJavaVMNotStored = 17000,
	ReservedMustReferenceLocalVM = 17001,
	ReservedMustBeNull = 17002,
	PermissionRequiredPatchAvailable = 18000,
	PermissionRequiredSystemUpdate = 18001,
	PermissionAgeRestrictionFailure = 18002,
	PermissionAccountTypeFailure = 18003,
	PermissionChatRestriction = 18004,
	PermissionUGCRestriction = 18005,
	PermissionOnlinePlayRestricted = 18006,
	DesktopCrossplayApplicationNotBootstrapped = 19000,
	DesktopCrossplayServiceNotInstalled = 19001,
	DesktopCrossplayServiceStartFailed = 19002,
	DesktopCrossplayServiceNotRunning = 19003,
	CustomInvitesInviteFailed = 20000,
	UserInfoBestDisplayNameIndeterminate = 22000,
	OnNetworkRequestedDeprecatedCallbackNotSet = 23000,
	CacheStorage_SizeKBNotMultipleOf16 = 23001,
	CacheStorage_SizeKBBelowMinimumSize = 23002,
	CacheStorage_SizeKBExceedsMaximumSize = 23003,
	CacheStorage_IndexOutOfRangeRange = 23004,
	UnexpectedError = 0x7FFFFFFF
}

enum ExternalAccountType {
	Epic = 0,
	Steam = 1,
	Psn = 2,
	Xbl = 3,
	Discord = 4,
	Gog = 5,
	Nintendo = 6,
	Uplay = 7,
	Openid = 8,
	Apple = 9,
	Google = 10,
	Oculus = 11,
	Itchio = 12,
	Amazon = 13,
	Viveport = 14,
}

enum ExternalCredentialType {
	None = -1,
	Epic = 0,
	SteamAppTicket = 1,
	PsnIdToken = 2,
	XblXstsToken = 3,
	DiscordAccessToken = 4,
	GogSessionTicket = 5,
	NintendoIdToken = 6,
	NintendoNsaIdToken = 7,
	UplayAccessToken = 8,
	OpenidAccessToken = 9,
	DeviceidAccessToken = 10,
	AppleIdToken = 11,
	GoogleIdToken = 12,
	OculusUseridNonce = 13,
	ItchioJwt = 14,
	ItchioKey = 15,
	EpicIdToken = 16,
	AmazonAccessToken = 17,
	SteamSessionTicket = 18,
	ViveportUserToken = 19,
}

## This seems to be outdated
enum OnlinePlatformType {
	Unknown = 0,
	Epic = 100,
	Steam = 4000
}

# TODO: remove once OnlinePlatformType is documented in EOS docs
enum Undocumented_OnlinePlatformType {
	Unknown = 0,
	A = 1000,
	B = 2000,
	C = 3000,
	D = 4000,
}

enum LoginStatus {
	NotLoggedIn = 0,
	UsingLocalProfile = 1,
	LoggedIn = 2
}

enum AttributeType {
	Boolean = 0,
	Int64 = 1,
	Double = 2,
	String = 3
}

enum ComparisonOp {
	Equal = 0,
	NotEqual = 1,
	GreaterThan = 2,
	GreaterThanOrEqual = 3,
	LessThan = 4,
	LessThanOrEqual = 5,
	Distance = 6,
	AnyOf = 7,
	NotAnyOf = 8,
	OneOf = 9,
	NotOneOf = 10,
	Contains = 11
}

enum AntiCheatCommonClientType {
	ProtectedClient = 0,
	UnprotectedClient = 1,
	AIBot = 2
}

enum AntiCheatCommonClientPlatform {
	Unknown = 0,
	Windows = 1,
	Mac = 2,
	Linux = 3,
	Xbox = 4,
	PlayStation = 5,
	Nintendo = 6,
	iOS = 7,
	Android = 8,
}

enum AntiCheatCommonClientAction {
	Invalid = 0,
	RemovePlayer = 1,
}

enum AntiCheatCommonClientActionReason {
	Invalid = 0,
	InternalError = 1,
	InvalidMessage = 2,
	AuthenticationFailed = 3,
	NullClient = 4,
	HeartbeatTimeout = 5,
	ClientViolation = 6,
	BackendViolation = 7,
	TemporaryCooldown = 8,
	TemporaryBanned = 9,
	PermanentBanned = 10,
}

enum AntiCheatCommonClientAuthStatus {
	Invalid = 0,
	LocalAuthComplete = 1,
	RemoteAuthComplete = 2,
}

enum AntiCheatCommonClientFlags {
	None = 0,
	Admin = (1 << 0),
}

enum AntiCheatCommonClientInput {
	Unknown = 0,
	MouseKeyboard = 1,
	Gamepad = 2,
	TouchInput = 3,
}

enum AntiCheatCommonEventType {
	Invalid = 0,
	GameEvent = 1,
	PlayerEvent = 2,
}

enum AntiCheatCommonEventParamType {
	Invalid = 0,
	ClientHandle = 1,
	String = 2,
	UInt32 = 3,
	Int32 = 4,
	UInt64 = 5,
	Int64 = 6,
	Vector3f = 7,
	Quat = 8,
	Float = 9,
}

enum AntiCheatCommonPlayerMovementState {
	None = 0,
	Crouching = 1,
	Prone = 2,
	Mounted = 3,
	Swimming = 4,
	Falling = 5,
	Flying = 6,
	OnLadder = 7,
}

enum AntiCheatCommonPlayerTakeDamageSource {
	None = 0,
	Player = 1,
	NonPlayerCharacter = 2,
	World = 3,
}

enum AntiCheatCommonPlayerTakeDamageType {
	None = 0,
	PointDamage = 1,
	RadialDamage = 2,
	DamageOverTime = 3,
}

enum AntiCheatCommonPlayerTakeDamageResult {
	EOS_ACCPTDR_None = 0,
	EOS_ACCPTDR_Downed = 1,
	EOS_ACCPTDR_Eliminated = 2,
}

enum AntiCheatClientMode {
	Invalid = 0,
	ClientServer = 1,
	PeerToPeer = 2,
}

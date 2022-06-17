# Copyright 2022, Delano Lourenco
# https://github.com/3ddelano/epic-online-services-godot
# MIT License

# Wrapper for EOS C# SDK

extends Reference
class_name EOS


class Achievements:
	class CopyAchievementDefinitionV2ByAchievementIdOptions:
		extends BaseClass
		func _init().("CopyAchievementDefinitionV2ByAchievementIdOptions"): pass

		var achievement_id: String

	class CopyAchievementDefinitionV2ByIndexOptions:
		extends BaseClass
		func _init().("CopyAchievementDefinitionV2ByIndexOptions"): pass

		var achievement_index: int

	class CopyPlayerAchievementByAchievementIdOptions:
		extends BaseClass
		func _init().("CopyPlayerAchievementByAchievementIdOptions"): pass

		var target_user_id: String
		var achievement_id: String
		var local_user_id: String

	class CopyPlayerAchievementByIndexOptions:
		extends BaseClass
		func _init().("CopyPlayerAchievementByIndexOptions"): pass

		var target_user_id: String
		var achievement_index: int
		var local_user_id: String

	class GetAchievementDefinitionCountOptions:
		extends BaseClass
		func _init().("GetAchievementDefinitionCountOptions"): pass

	class QueryDefinitionsOptions:
		extends BaseClass
		func _init().("QueryDefinitionsOptions"): pass

		var local_user_id = ""

		var client_data = null

	class GetPlayerAchievementCountOptions:
		extends BaseClass
		func _init().("GetPlayerAchievementCountOptions"): pass

		var user_id: String

	class QueryPlayerAchievementsOptions:
		extends BaseClass
		func _init().("QueryPlayerAchievementsOptions"): pass

		var target_user_id: String
		var local_user_id: String

		var client_data = null

	class UnlockAchievementsOptions:
		extends BaseClass
		func _init().("UnlockAchievementsOptions"): pass

		var user_id: String
		var achievement_ids = []  # [String]
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
	class Credentials:
		extends BaseClass
		func _init().("Credentials"): pass

		var type: int  # ExternalCredentialType
		var token = null

	class UserLoginInfo:
		extends BaseClass
		func _init().("UserLoginInfo"): pass

		var display_name: String

	class LoginOptions:
		extends BaseClass
		func _init().("LoginOptions"): pass

		var credentials: Credentials
		var user_login_info = null  # UserLoginInfo

		var client_data = null

	class CopyIdTokenOptions:
		extends BaseClass
		func _init().("CopyIdTokenOptions"): pass

		var local_user_id: String

	class CopyProductUserExternalAccountByAccountIdOptions:
		extends BaseClass
		func _init().("CopyProductUserExternalAccountByAccountIdOptions"): pass

		var target_user_id: String
		var account_id: String

	class CopyProductUserExternalAccountByAccountTypeOptions:
		extends BaseClass
		func _init().("CopyProductUserExternalAccountByAccountTypeOptions"): pass

		var target_user_id: String
		var account_id_type: int  # ExternalAccountType

	class CopyProductUserExternalAccountByIndexOptions:
		extends BaseClass
		func _init().("CopyProductUserExternalAccountByIndexOptions"): pass

		var target_user_id: String
		var external_account_info_index: int

	class CopyProductUserInfoOptions:
		extends BaseClass
		func _init().("CopyProductUserInfoOptions"): pass

		var target_user_id: String

	class CreateDeviceIdOptions:
		extends BaseClass
		func _init().("CreateDeviceIdOptions"): pass

		var device_model: String

		var client_data = null

	class DeleteDeviceIdOptions:
		extends BaseClass
		func _init().("DeleteDeviceIdOptions"): pass

		var client_data = null

	class CreateUserOptions:
		extends BaseClass
		func _init().("CreateUserOptions"): pass

		var continuance_token: Reference # ContinuanceTokenWrapper

		var client_data = null

	class GetExternalAccountMappingsOptions:
		extends BaseClass
		func _init().("GetExternalAccountMappingsOptions"): pass

		var local_user_id: String
		var account_id_type: int # ExternalAccountType
		var target_external_user_id: String

	class GetProductUserExternalAccountCountOptions:
		extends BaseClass
		func _init().("GetProductUserExternalAccountCountOptions"): pass

		var target_user_id: String

	class GetProductUserIdMappingOptions:
		extends BaseClass
		func _init().("GetProductUserIdMappingOptions"): pass

		var local_user_id: String
		var account_id_type: int # ExternalAccountType
		var target_product_user_id: String

	class LinkAccountOptions:
		extends BaseClass
		func _init().("LinkAccountOptions"): pass

		var continuance_token = null # ContinuanceTokenWrapper
		var local_user_id = null # String

		var client_data = null

	class IdToken:
		extends BaseClass
		func _init().("IdToken"): pass

		var product_user_id: String
		var json_web_token: String

	class VerifyIdTokenOptions:
		extends BaseClass
		func _init().("VerifyIdTokenOptions"): pass

		var id_token: IdToken

		var client_data = null

	class TransferDeviceIdAccountOptions:
		extends BaseClass
		func _init().("TransferDeviceIdAccountOptions"): pass

		var primary_local_user_id: String
		var local_device_user_id: String
		var product_user_id_to_preserve: String

		var client_data = null

	class UnlinkAccountOptions:
		extends BaseClass
		func _init().("UnlinkAccountOptions"): pass

		var local_user_id: String

		var client_data = null

	class ConnectInterface:
		static func login(options: LoginOptions) -> void:
			IEOS.connect_interface_login(options)

		static func copy_id_token(options: CopyIdTokenOptions) -> Dictionary:
			return IEOS.connect_interface_copy_id_token(options)

		static func copy_product_user_external_account_by_account_id(
			options: CopyProductUserExternalAccountByAccountIdOptions
		) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_external_account_by_account_id(options)

		static func copy_product_user_external_account_by_account_type(
			options: CopyProductUserExternalAccountByAccountTypeOptions
		) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_external_account_by_account_type(
				options
			)

		static func copy_product_user_external_account_by_index(
			options: CopyProductUserExternalAccountByIndexOptions
		) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_external_account_by_index(options)

		static func copy_product_user_info(options: CopyProductUserInfoOptions) -> Dictionary:
			return IEOS.connect_interface_copy_product_user_info(options)

		static func create_device_id(options: CreateDeviceIdOptions) -> Dictionary:
			return IEOS.connect_interface_create_device_id(options)

		static func create_user(options: CreateUserOptions) -> Dictionary:
			return IEOS.connect_interface_create_user(options)

		static func delete_device_id(options: DeleteDeviceIdOptions) -> Dictionary:
			return IEOS.connect_interface_delete_device_id(options)

		static func get_external_account_mapping(options: GetExternalAccountMappingsOptions) -> String:
			return IEOS.connect_interface_get_external_account_mapping(options)

		static func get_logged_in_user_by_index(index: int) -> String:
			return IEOS.connect_interface_get_logged_in_user_by_index(index)

		static func get_logged_in_users_count() -> int:
			return IEOS.connect_interface_get_logged_in_users_count()

		static func get_login_status(local_user_id: String) -> int:
			return IEOS.connect_interface_get_login_status(local_user_id)

		static func get_product_user_external_account_count(options: GetProductUserExternalAccountCountOptions) -> int:
			return IEOS.connect_interface_get_product_user_external_account_count(options)

		static func get_product_user_id_mapping(options: GetProductUserIdMappingOptions):
			return IEOS.connect_interface_get_product_user_id_mapping(options)

		static func link_account(options: LinkAccountOptions):
			return IEOS.connect_interface_link_account(options)

		static func verify_id_token(options: VerifyIdTokenOptions):
			return IEOS.connect_interface_verify_id_token(options)

		static func transfer_device_id_account(options: TransferDeviceIdAccountOptions):
			return IEOS.connect_interface_transfer_device_id_account(options)

		static func unlink_account(options: UnlinkAccountOptions):
			return IEOS.connect_interface_unlink_account(options)


class Auth:
	enum ScopeFlags {
		NoFlags = 0x0,
		BasicProfile = 0x1,
		FriendsList = 0x2,
		Presence = 0x4,
		FriendsManagement = 0x8,
		Email = 0x10
	}

	enum AuthTokenType { Client = 0, User = 1 }

	enum LinkAccountFlags { NoFlags = 0x0, NintendoNsaId = 0x1 }

	enum LoginCredentialType {
		Password = 0,
		ExchangeCode = 1,
		PersistentAuth = 2,
		DeviceCode = 3,
		Developer = 4,
		RefreshToken = 5,
		AccountPortal = 6,
		ExternalAuth = 7
	}

	class LoginOptions:
		extends BaseClass
		func _init().("LoginOptions"): pass

		var type: int
		var credentials: Credentials
		var scope_flags: int

		var client_data = null

	class LogoutOptions:
		extends BaseClass
		func _init().("LogoutOptions"): pass

		var local_user_id: String

		var client_data = null

	class Credentials:
		extends BaseClass
		func _init().("Credentials"): pass

		var type: int  # LoginCredentialType
		var id = null  # String
		var token = null  # String
		var external_type = null  # ExternalCredentialType

	class CopyIdTokenOptions:
		extends BaseClass
		func _init().("CopyIdTokenOptions"): pass

		var account_id: String

	class CopyUserAuthTokenOptions:
		extends BaseClass
		func _init().("CopyUserAuthTokenOptions"): pass

	class DeletePersistentAuthOptions:
		extends BaseClass
		func _init().("DeletePersistentAuthOptions"): pass

		var refresh_token = null
		var client_data = null

	class LinkAccountOptions:
		extends BaseClass
		func _init().("LinkAccountOptions"): pass

		var link_account_flags: int # LinkAccountFlags
		var continuance_token: Reference # ContinuanceTokenWrapper
		var local_user_id: String

		var client_data = null

	class QueryIdTokenOptions:
		extends BaseClass
		func _init().("QueryIdTokenOptions"): pass

		var local_user_id: String
		var target_account_id: String

		var client_data = null

	class IdToken:
		extends BaseClass
		func _init().("IdToken"): pass

		var account_id: String
		var json_web_token: String

	class VerifyIdTokenOptions:
		extends BaseClass
		func _init().("VerifyIdTokenOptions"): pass

		var id_token: IdToken

		var client_data = null

	class Token:
		extends BaseClass
		func _init().("Token"): pass

		var app: String
		var client_id: String
		var account_id: String
		var access_token: String
		var expires_in: float
		var expires_at: String
		var auth_type: int
		var refresh_token: String
		var refresh_expires_in: float
		var refresh_expires_at: String

	class VerifyUserAuthOptions:
		extends BaseClass
		func _init().("VerifyUserAuthOptions"): pass

		var auth_token: Token

		var client_data = null

	class AuthInterface:
		static func login(options: LoginOptions) -> void:
			IEOS.auth_interface_login(options)

		static func logout(options: LogoutOptions) -> void:
			IEOS.auth_interface_logout(options)

		static func copy_id_token(options: CopyIdTokenOptions) -> Dictionary:
			return IEOS.auth_interface_copy_id_token(options)

		static func copy_user_auth_token(options: CopyUserAuthTokenOptions, local_user_id: String) -> Dictionary:
			var token_dict: Dictionary = IEOS.auth_interface_copy_user_auth_token(
				options, local_user_id
			)
			var token: Token = Token.new()
			token.app = token_dict.app
			token.client_id = token_dict.client_id
			token.account_id = token_dict.account_id
			token.access_token = token_dict.access_token
			token.expires_in = token_dict.expires_in
			token.expires_at = token_dict.expires_at
			token.auth_type = token_dict.auth_type
			token.refresh_token = token_dict.refresh_token
			token.refresh_expires_in = token_dict.refresh_expires_in
			token.refresh_expires_at = token_dict.refresh_expires_at

			return {user_auth_token = token, result_code = token_dict.result_code}

		static func delete_persistent_auth(options: DeletePersistentAuthOptions) -> void:
			IEOS.auth_interface_delete_persistent_auth(options)

		static func get_logged_in_account_by_index(index: int) -> String:
			return IEOS.auth_interface_get_logged_in_account_by_index(index)

		static func get_logged_in_accounts_count() -> int:
			return IEOS.auth_interface_get_logged_in_accounts_count()

		static func get_login_status(local_user_id: String) -> int:
			return IEOS.auth_interface_get_login_status(local_user_id)

		static func get_merged_account_by_index(local_user_id: String, index: int) -> String:
			return IEOS.auth_interface_get_merged_account_by_index(local_user_id, index)

		static func get_merged_accounts_count(local_user_id: String) -> int:
			return IEOS.auth_interface_get_merged_accounts_count(local_user_id)

		static func get_selected_account_id(local_user_id: String) -> Dictionary:
			return IEOS.auth_interface_get_selected_account_id(local_user_id)

		static func link_account(options: LinkAccountOptions):
			return IEOS.auth_interface_link_account(options)

		static func query_id_token(options: QueryIdTokenOptions) -> void:
			IEOS.auth_interface_query_id_token(options)

		static func verify_id_token(options: VerifyIdTokenOptions) -> void:
			IEOS.auth_interface_verify_id_token(options)

		static func verify_user_auth(options: VerifyUserAuthOptions) -> void:
			IEOS.auth_interface_verify_user_auth(options)


class CustomInvites:

	class SetCustomInviteOptions:
		extends BaseClass
		func _init().("SetCustomInviteOptions"): pass

		var local_user_id: String
		var payload: String

	class SendCustomInviteOptions:
		extends BaseClass
		func _init().("SendCustomInviteOptions"): pass

		var local_user_id: String
		var target_user_ids = [] # [String]
		var client_data = null

	class FinalizeInviteOptions:
		extends BaseClass
		func _init().("FinalizeInviteOptions"): pass

		var target_user_id: String
		var local_user_id: String
		var custom_invite_id: String
		var processing_result: int # Result

	class CustomInvitesInterface:
		static func set_custom_invite(options: SetCustomInviteOptions) -> int:
			return IEOS.custom_invites_interface_set_custom_invite(options)

		static func send_custom_invite(options: SendCustomInviteOptions) -> void:
			IEOS.custom_invites_interface_send_custom_invite(options)

		static func finalize_invite(options: FinalizeInviteOptions) -> int:
			return IEOS.custom_invites_interface_finalize_invite(options)


class Stats:

	class CopyStatByIndexOptions:
		extends BaseClass
		func _init().("CopyStatByIndexOptions"): pass

		var target_user_id: String
		var stat_index: int

	class CopyStatByNameOptions:
		extends BaseClass
		func _init().("CopyStatByNameOptions"): pass

		var target_user_id: String
		var name: String

	class GetStatsCountOptions:
		extends BaseClass
		func _init().("GetStatsCountOptions"): pass

		var target_user_id: String

	class IngestStatOptions:
		extends BaseClass
		func _init().("IngestStatOptions"): pass

		var local_user_id: String
		var target_user_id: String
		var stats: Array # IngestData: {stat_name: String, ingest_amount: int}

		var client_data = null

	class QueryStatsOptions:
		extends BaseClass
		func _init().("QueryStatsOptions"): pass

		var local_user_id: String
		var target_user_id: String
		var stat_names = null # [String]
		var start_time = null # String
		var end_time = null # String

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
		WindowsEnableOverlayOpengl = 0x00040
	}

	class InitializeOptions:
		extends BaseClass
		func _init().("InitializeOptions"): pass

		var product_name: String
		var product_version: String

	class CreateOptions:
		extends BaseClass
		func _init().("CreateOptions"): pass

		var product_id: String
		var sandbox_id: String
		var deployment_id: String
		var client_id: String
		var client_secret: String

		var is_server: bool
		var encryption_key: String
		var override_locale_code: String
		var flags: int
		var tick_budget_in_milliseconds: int
		# TODO: implement rtc_options in GDScript

#		var rtc_options: RTCOptions

#	class RTCOptions:
#		var platform_specific_options: RTCOptionsPlatformSpecificOptions
#
#	class RTCOptionsPlatformSpecificOptions:
#		extends Reference
#
#	class WindowsRTCOptionsPlatformSpecificOptions:
#		extends RTCOptionsPlatformSpecificOptions
#
#		var xaudio29_dll_path

	class PlatformInterface:
		static func initialize(options: InitializeOptions) -> int:
			return IEOS.platform_interface_initialize(options)

		static func create(options: CreateOptions) -> bool:
			return IEOS.platform_interface_create(options)

		static func check_for_launcher_and_restart() -> int:
			return IEOS.platform_interface_check_for_launcher_and_restart()

		static func get_active_country_code(user_id: String) -> Dictionary:
			return IEOS.platform_interface_get_active_country_code(user_id)

		static func get_override_country_code() -> Dictionary:
			return IEOS.platform_interface_get_override_country_code()

		static func set_override_country_code(country_code: String) -> Dictionary:
			return IEOS.platform_interface_set_override_country_code(country_code)

		static func get_active_locale_code(user_id: String) -> Dictionary:
			return IEOS.platform_interface_get_active_locale_code(user_id)

		static func get_override_locale_code() -> Dictionary:
			return IEOS.platform_interface_get_override_locale_code()

		static func set_override_locale_code(locale_code: String) -> Dictionary:
			return IEOS.platform_interface_set_override_locale_code(locale_code)


class Ecom:
	enum ItemType { Durable = 0, Consumable = 1, Other = 2 }

	enum OwnershipStatus { NotOwned = 0, Owned = 1 }


class Friends:
	enum FriendsStatus { NotFriends = 0, InviteSent = 1, InviteReceived = 2, Friends = 3 }

	class AcceptInviteOptions:
		extends BaseClass
		func _init().("AcceptInviteOptions"): pass

		var local_user_id: String
		var target_user_id: String

		var client_data = null

	class GetFriendAtIndexOptions:
		extends BaseClass
		func _init().("GetFriendAtIndexOptions"): pass

		var local_user_id: String
		var index: int

	class GetFriendsCountOptions:
		extends BaseClass
		func _init().("GetFriendsCountOptions"): pass

		var local_user_id: String

	class GetStatusOptions:
		extends BaseClass
		func _init().("GetStatusOptions"): pass

		var local_user_id: String
		var target_user_id: String

	class QueryFriendsOptions:
		extends BaseClass
		func _init().("QueryFriendsOptions"): pass

		var local_user_id: String

	class RejectInviteOptions:
		extends BaseClass
		func _init().("RejectInviteOptions"): pass

		var local_user_id: String
		var target_user_id: String

	class FriendsInterface:
		static func accept_invite(options: AcceptInviteOptions) -> void:
			IEOS.friends_interface_accept_invite(options)

		static func get_friend_at_index(options: GetFriendAtIndexOptions) -> String:
			return IEOS.friends_interface_get_friend_at_index(options)

		static func get_friends_count(options: GetFriendsCountOptions) -> int:
			return IEOS.friends_interface_get_friends_count(options)

		static func get_status(options: GetStatusOptions) -> int:
			return IEOS.friends_interface_get_status(options)

		static func query_friends(options: QueryFriendsOptions) -> void:
			IEOS.friends_interface_query_friends(options)

		static func reject_invite(options: RejectInviteOptions) -> void:
			IEOS.friends_interface_reject_invite(options)


class KWS:
	enum KWSPermissionStatus { Granted = 0, Rejected = 1, Pending = 2 }


class Leaderboards:
	enum LeaderboardAggregation { Min = 0, Max = 1, Sum = 2, Latest = 3 }

	class CopyLeaderboardDefinitionByIndexOptions:
		extends BaseClass
		func _init().("CopyLeaderboardDefinitionByIndexOptions"): pass

		var leaderboard_index: int

	class CopyLeaderboardDefinitionByLeaderboardId:
		extends BaseClass
		func _init().("CopyLeaderboardDefinitionByLeaderboardId"): pass

		var leaderboard_id: String

	class CopyLeaderboardRecordByIndexOptions:
		extends BaseClass
		func _init().("CopyLeaderboardRecordByIndexOptions"): pass

		var leaderboard_record_index: int

	class CopyLeaderboardRecordByUserIdOptions:
		extends BaseClass
		func _init().("CopyLeaderboardRecordByUserIdOptions"): pass

		var user_id: int

	class CopyLeaderboardUserScoreByIndexOptions:
		extends BaseClass
		func _init().("CopyLeaderboardUserScoreByIndexOptions"): pass

		var leaderboard_user_score_index: int
		var stat_name: String

	class CopyLeaderboardUserScoreByUserIdOptions:
		extends BaseClass
		func _init().("CopyLeaderboardUserScoreByUserIdOptions"): pass

		var user_id: String
		var stat_name: String

	class GetLeaderboardDefinitionCountOptions:
		extends BaseClass
		func _init().("GetLeaderboardDefinitionCountOptions"): pass

	class GetLeaderboardRecordCountOptions:
		extends BaseClass
		func _init().("GetLeaderboardRecordCountOptions"): pass

	class GetLeaderboardUserScoreCountOptions:
		extends BaseClass
		func _init().("GetLeaderboardUserScoreCountOptions"): pass

		var stat_name: String

	class QueryLeaderboardDefinitionsOptions:
		extends BaseClass
		func _init().("QueryLeaderboardDefinitionsOptions"): pass

		var local_user_id: String
		var start_time = null # String
		var end_time = null # String

		var client_data = null

	class QueryLeaderboardRanksOptions:
		extends BaseClass
		func _init().("QueryLeaderboardRanksOptions"): pass

		var local_user_id: String
		var leaderboard_id: String

		var client_data = null

	class QueryLeaderboardUserScoresOptions:
		extends BaseClass
		func _init().("QueryLeaderboardUserScoresOptions"): pass

		var local_user_id: String
		var user_ids = [] # [String]
		var stat_info = [] # [{stat_name: String, aggregation: LeaderboardAggregation}]
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
	enum LobbyAttributeVisibility { Public = 0, Private = 1 }

	enum LobbyMemberStatus {
		Joined = 0,
		Left = 1,
		Disconnected = 2,
		Kicked = 3,
		Promoted = 4,
		Closed = 5
	}

	enum LobbyPermissionLevel { Publicadvertised = 0, Joinviapresence = 1, Inviteonly = 2 }


class Metrics:
	enum MetricsAccountIdType { Epic = 0, External = 1 }

	enum UserControllerType { Unknown = 0, MouseKeyboard = 1, GamepadControl = 2, TouchControl = 3 }


class Mods:
	enum ModEnumerationType { Installed = 0, AllAvailable }


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

	enum ConnectionEstablishedType { NewConnection = 0, Reconnection = 1 }

	enum NATType { Unknown = 0, Open = 1, Moderate = 2, Strict = 3 }

	enum PacketReliability { UnreliableUnordered = 0, ReliableUnordered = 1, ReliableOrdered = 2 }

	enum RelayControl { NoRelays = 0, AllowRelays = 1, ForceRelays = 2 }


class PlayerDataStorage:
	enum ReadResult { ContinueReading = 1, FailRequest = 2, CancelRequest = 3 }

	enum WriteResult {
		ContinueWriting = 1,
		CompleteRequest = 2,
		FailRequest = 3,
		CancelRequest = 4
	}


class Presence:
	enum Status { Offline = 0, Online = 1, Away = 2, ExtendedAway = 3, DoNotDisturb = 4 }


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


class RTC:
	enum RTCParticipantStatus { Joined = 0, Left = 1 }


class Sessions:
	enum OnlineSessionPermissionLevel { PublicAdvertised = 0, JoinViaPresence = 1, InviteOnly = 2 }

	enum OnlineSessionState {
		NoSession = 0,
		Creating = 1,
		Pending = 2,
		Starting = 3,
		InProgress = 4,
		Ending = 5,
		Ended = 6,
		Destroying = 7
	}
	enum SessionAttributeAdvertisementType { DontAdvertise = 0, Advertise = 1 }


class TitleStorage:
	enum ReadResult { RrContinuereading = 1, RrFailrequest = 2, RrCancelrequest = 3 }


class UI:
	const ModifierShift = 16
	const KeyTypeMask = (1 << ModifierShift) - 1
	const ModifierMask = ~KeyTypeMask
	const Shift = 1 << ModifierShift
	const Control = 2 << ModifierShift
	const Alt = 4 << ModifierShift
	const Meta = 8 << ModifierShift
	const ValidModifierMask = Shift | Control | Alt | Meta

	enum KeyCombination {
		ModifierShift = 16,
		KeyTypeMask = (1 << ModifierShift) - 1,
		ModifierMask = ~KeyTypeMask,
		Shift = 1 << ModifierShift,
		Control = 2 << ModifierShift,
		Alt = 4 << ModifierShift,
		Meta = 8 << ModifierShift,
		ValidModifierMask = Shift | Control | Alt | Meta,
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

	enum NotificationLocation { TopLeft, TopRight, BottomLeft, BottomRight }

class UserInfo:

	class CopyExternalUserInfoByAccountIdOptions:
		extends BaseClass
		func _init().("CopyExternalUserInfoByAccountIdOptions"): pass

		var local_user_id: String
		var target_user_id: String
		var account_id: String

	class CopyExternalUserInfoByAccountTypeOptions:
		extends BaseClass
		func _init().("CopyExternalUserInfoByAccountTypeOptions"): pass

		var local_user_id: String
		var target_user_id: String
		var account_type: int # ExternalAccountType

	class CopyExternalUserInfoByIndexOptions:
		extends BaseClass
		func _init().("CopyExternalUserInfoByIndexOptions"): pass

		var local_user_id: String
		var target_user_id: String
		var index: int

	class CopyUserInfoOptions:
		extends BaseClass
		func _init().("CopyUserInfoOptions"): pass

		var local_user_id: String
		var target_user_id: String

	class GetExternalUserInfoCountOptions:
		extends BaseClass
		func _init().("GetExternalUserInfoCountOptions"): pass

		var local_user_id: String
		var target_user_id: String

	class QueryUserInfoOptions:
		extends BaseClass
		func _init().("QueryUserInfoOptions"): pass

		var local_user_id: String
		var target_user_id: String

		var client_data = null

	class QueryUserInfoByDisplayNameOptions:
		extends BaseClass
		func _init().("QueryUserInfoByDisplayNameOptions"): pass

		var local_user_id: String
		var display_name: String

		var client_data = null

	class QueryUserInfoByExternalAccountOptions:
		extends BaseClass
		func _init().("QueryUserInfoByExternalAccountOptions"): pass

		var local_user_id: String
		var external_account_id: String
		var account_type: int # ExternalAccountType

		var client_data = null

	class UserInfoInterface:
		static func copy_external_user_info_by_account_id(options: CopyExternalUserInfoByAccountIdOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_external_user_info_by_account_id(options)

		static func copy_external_user_info_by_account_type(options: CopyExternalUserInfoByAccountTypeOptions) -> Dictionary:
			return IEOS.user_info_interface_copy_external_user_info_by_account_ty(options)

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
		IdentityProvider = 21,
		TitleStorage = 22,
		Mods = 23,
		AntiCheat = 24,
		Reports = 25,
		Sanctions = 26,
		ProgressionSnapshots = 27,
		Kws = 28,
		Rtc = 29,
		RTCAdmin = 30,
		Inventory = 31,
		ReceiptValidator = 32,
		CustomInvites = 33,
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

	class LogMessage:
		extends BaseClass
		func _init().("LogMessage"): pass

		var category: String
		var level: int  # LogLevel enum
		var message: String

		static func from(msg_dict: Dictionary) -> LogMessage:
			var msg = LogMessage.new()
			msg.category = msg_dict.get("category")
			msg.level = msg_dict.get("level")
			msg.message = msg_dict.get("message")
			return msg

		func _to_string() -> String:
			return "%s | %s | %s" % [category, level, message]

	static func set_log_level(log_category: int, log_level: int) -> int:
		return IEOS.logging_interface_set_log_level(log_category, log_level)


static func get_instance() -> IEOS:
	return IEOS


static func print_result(result) -> String:
	if typeof(result) == TYPE_DICTIONARY:
		if result.has("result_code"):
			result = result["result_code"]
		else:
			printerr("EOS: print_result: Could not find result_code in ", result)
			return ""
	return Result.keys()[Result.values().find(result)]


#static class AccountFeatureRestrictedInfo:
#	var verificationURI
#static class AddNotifyLoginStatusChangedOptions:
#	pass

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
	MissingParametersDEPRECATED = 25,
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
	AuthPersistentAuthAccountNotActive = 1050,
	AuthMFARequired = 1060,
	AuthParentalControls = 1070,
	AuthNoRealId = 1080,
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
	ConnectLinkAccountFailedMissingNintendoIdAccountDEPRECATED = 7008,
	SocialOverlayLoadError = 8000,
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
	ProgressionSnapshotSnapshotIdUnavailable = 14000,
	ParentEmailMissing = 15000,
	UserGraduated = 15001,
	AndroidJavaVMNotStored = 17000,
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
	Amazon = 13
}

enum ExternalCredentialType {
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
	AmazonAccessToken = 17
}

enum LoginStatus { NotLoggedIn = 0, UsingLocalProfile = 1, LoggedIn = 2 }

enum RTCAudioInputStatus {
	Idle = 0,
	Recording = 1,
	RecordingSilent = 2,
	RecordingDisconnected = 3,
	Failed = 4
}

enum RTCAudioOutputStatus { Idle = 0, Playing = 1, Failed = 2 }

enum RTCAudioStatus {
	Unsupported = 0,
	Enabled = 1,
	Disabled = 2,
	AdminDisabled = 3,
	NotListeningDisabled = 4
}

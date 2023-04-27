# Copyright (c) 2023-present Delano Lourenco
# https://github.com/3ddelano/epic-online-services-godot/
# MIT License

extends RefCounted
class_name EOS

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

	class LoginOptions extends BaseClass:
		func _init():
			super._init("LoginOptions")

		var type: int
		var credentials: Credentials
		var scope_flags: int

		var client_data = null

	class LogoutOptions extends BaseClass:
		func _init():
			super._init("LogoutOptions")

		var local_user_id: String

		var client_data = null

	class Credentials extends BaseClass:
		func _init():
			super._init("Credentials")

		var type: int  # LoginCredentialType
		var id = null  # String
		var token = null  # String
		var external_type = null  # ExternalCredentialType

	class CopyIdTokenOptions extends BaseClass:
		func _init():
			super._init("CopyIdTokenOptions")

		var account_id: String

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

		var link_account_flags: int # LinkAccountFlags
		var continuance_token: RefCounted # ContinuanceTokenWrapper
		var local_user_id: String

		var client_data = null

	class QueryIdTokenOptions extends BaseClass:
		func _init():
			super._init("QueryIdTokenOptions")

		var local_user_id: String
		var target_account_id: String

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

		static func copy_user_auth_token(options: CopyUserAuthTokenOptions, local_user_id: String) -> Dictionary:
			var func_result: Dictionary = IEOS.auth_interface_copy_user_auth_token(
				options, local_user_id
			)
			var token: Token = Token.new()
			if func_result.token:
				var token_dict = func_result.token
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

			return {result_code = func_result.result_code, token = token}

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


class Platform extends RefCounted:
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

	class InitializeOptions extends BaseClass:
		func _init():
			super._init("InitializeOptions")

		var product_name: String
		var product_version: String

	class CreateOptions extends BaseClass:
		func _init():
			super._init("CreateOptions")

		var product_id: String
		var sandbox_id: String
		var deployment_id: String
		var client_id: String
		var client_secret: String
		var encryption_key: String

		var is_server: bool
		var override_locale_code: String
		var override_country_code: String
		var flags: int = -1
		var tick_budget_in_milliseconds: int
		# TODO: implement rtc_options in GDScript
		# var rtc_options: RTCOptions


	class PlatformInterface extends RefCounted:
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

		static func set_override_country_code(country_code: String) -> int:
			return IEOS.platform_interface_set_override_country_code(country_code)

		static func set_override_locale_code(locale_code: String) -> int:
			return IEOS.platform_interface_set_override_locale_code(locale_code)

		static func check_for_launcher_and_restart() -> int:
			return IEOS.platform_interface_check_for_launcher_and_restart()

		static func initialize(options: InitializeOptions) -> int:
			return IEOS.platform_interface_initialize(options)

		static func get_desktop_crossplay_status() -> Dictionary:
			return IEOS.platform_interface_get_desktop_crossplay_status()

		static func set_application_status(status: ApplicationStatus) -> int:
			return IEOS.platform_interface_set_application_status(status)

		static func get_application_status() -> int:
			return IEOS.platform_interface_get_application_status()

		static func set_network_status(status: ApplicationStatus) -> int:
			return IEOS.platform_interface_set_network_status(status)

		static func get_network_status() -> int:
			return IEOS.platform_interface_get_network_status()

		static func release() -> void:
			IEOS.platform_interface_release()

		static func shutdown() -> int:
			return IEOS.platform_interface_shutdown()


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
		return 1#IEOS.logging_interface_set_log_level(log_category, log_level)


class Version:
	class VersionInterface:
		static func get_version() -> String:
			return IEOS.version_interface_get_version()

		static func get_constants() -> Dictionary:
			return IEOS.version_interface_get_constants()


static func print_result(p_result: int) -> void:
	print_rich("[b]EOS_Result[/b]:%s[code](%s)[/code]" % [result_str(p_result), p_result])


static func result_str(p_result: int) -> String:
	var idx := Result.values().find(p_result)
	return Result.keys()[idx]


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
	LobbyVoiceNotEnabled = 9019,
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

enum LoginStatus {
	NotLoggedIn = 0,
	UsingLocalProfile = 1,
	LoggedIn = 2
}

enum RTCAudioInputStatus {
	Idle = 0,
	Recording = 1,
	RecordingSilent = 2,
	RecordingDisconnected = 3,
	Failed = 4
}

enum RTCAudioOutputStatus {
	Idle = 0,
	Playing = 1,
	Failed = 2
}

enum RTCAudioStatus {
	Unsupported = 0,
	Enabled = 1,
	Disabled = 2,
	AdminDisabled = 3,
	NotListeningDisabled = 4
}

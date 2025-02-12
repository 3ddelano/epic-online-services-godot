#include "ieos.h"
using namespace godot;

bool IEOS::platform_interface_create(Ref<RefCounted> p_options) {
    CharString productId = VARIANT_TO_CHARSTRING(p_options->get("product_id"));
    CharString sandboxId = VARIANT_TO_CHARSTRING(p_options->get("sandbox_id"));
    CharString deploymentId = VARIANT_TO_CHARSTRING(p_options->get("deployment_id"));
    CharString encryptionKey = VARIANT_TO_CHARSTRING(p_options->get("encryption_key"));
    int flags = p_options->get("flags");
    int tickBudgetInMilliseconds = p_options->get("tick_budget_in_milliseconds");
    Variant taskNetworkTimeoutSeconds = p_options->get("task_network_timeout_seconds");
    CharString clientId = VARIANT_TO_CHARSTRING(p_options->get("client_id"));
    CharString clientSecret = VARIANT_TO_CHARSTRING(p_options->get("client_secret"));
    CharString overrideCountryCode = VARIANT_TO_CHARSTRING(p_options->get("override_country_code"));
    CharString overrideLocaleCode = VARIANT_TO_CHARSTRING(p_options->get("override_locale_code"));
    CharString cache_directory = VARIANT_TO_CHARSTRING(p_options->get("cache_directory"));

    EOS_Platform_Options platformOptions;
    memset(&platformOptions, 0, sizeof(platformOptions));
    platformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
    platformOptions.ProductId = productId.get_data();
    platformOptions.SandboxId = sandboxId.get_data();

    if (clientId.length() != 0) {
        platformOptions.ClientCredentials.ClientId = clientId.get_data();
    }
    if (clientSecret.length() != 0) {
        platformOptions.ClientCredentials.ClientSecret = clientSecret.get_data();
    }
    platformOptions.bIsServer = VARIANT_TO_EOS_BOOL(p_options->get("is_server"));
    if (encryptionKey.length() != 0) {
        platformOptions.EncryptionKey = encryptionKey.get_data();
    }
    if (overrideCountryCode.length() != 0) {
        platformOptions.OverrideLocaleCode = overrideCountryCode.get_data();
    }
    if (overrideLocaleCode.length() != 0) {
        platformOptions.OverrideCountryCode = overrideLocaleCode.get_data();
    }
    if (cache_directory.length() != 0) {
        platformOptions.CacheDirectory = cache_directory.get_data();
    }
    platformOptions.DeploymentId = deploymentId.get_data();
    platformOptions.Flags = flags;
    platformOptions.TickBudgetInMilliseconds = tickBudgetInMilliseconds;

    EOS_Platform_RTCOptions rtcOptions;
    memset(&rtcOptions, 0, sizeof(rtcOptions));
    rtcOptions.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;
#ifdef _WIN32
    EOS_Windows_RTCOptions windowsRTCOptions;
    memset(&windowsRTCOptions, 0, sizeof(windowsRTCOptions));
    windowsRTCOptions.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
    CharString xAudio29DllPath;
    if (OS::get_singleton()->has_feature("editor")) {
        xAudio29DllPath = ProjectSettings::get_singleton()->globalize_path("res://addons/epic-online-services-godot/bin/windows/x64/xaudio2_9redist.dll").utf8();
        windowsRTCOptions.XAudio29DllPath = xAudio29DllPath.get_data();
    } else {
        xAudio29DllPath = OS::get_singleton()->get_executable_path().get_base_dir().path_join("xaudio2_9redist.dll").utf8();
        windowsRTCOptions.XAudio29DllPath = xAudio29DllPath.get_data();
    }
    rtcOptions.PlatformSpecificOptions = &windowsRTCOptions;
#else
    rtcOptions.PlatformSpecificOptions = nullptr;
#endif
    // Handle background mode
    Ref<RefCounted> p_rtc_options = p_options->get("rtc_options");
    if (p_rtc_options->get("background_mode").get_type() != Variant::NIL) {
        int backgroundMode = p_rtc_options->get("background_mode");
        rtcOptions.BackgroundMode = static_cast<EOS_ERTCBackgroundMode>(backgroundMode);
    }

    platformOptions.RTCOptions = &rtcOptions;

    double p_taskNetworkTimeoutSeconds = taskNetworkTimeoutSeconds;
    if (taskNetworkTimeoutSeconds.get_type() != Variant::NIL) {
        platformOptions.TaskNetworkTimeoutSeconds = &p_taskNetworkTimeoutSeconds;
    }

    if (!platformOptions.IntegratedPlatformOptionsContainerHandle) {
        EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions createIntegratedPlatformOptionsContainerOptions{};
        createIntegratedPlatformOptionsContainerOptions.ApiVersion = EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST;

        EOS_EResult createIntegratedPlatformOptionsContainerResult = EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer(&createIntegratedPlatformOptionsContainerOptions, &platformOptions.IntegratedPlatformOptionsContainerHandle);
        if (createIntegratedPlatformOptionsContainerResult != EOS_EResult::EOS_Success) {
            UtilityFunctions::printerr("\n[EOSG] Failed to create integrated platform options container:" + String(EOS_EResult_ToString(createIntegratedPlatformOptionsContainerResult)));
        }
    }

    s_platformInterface = EOS_Platform_Create(&platformOptions);

    if (platformOptions.IntegratedPlatformOptionsContainerHandle) {
        EOS_IntegratedPlatformOptionsContainer_Release(platformOptions.IntegratedPlatformOptionsContainerHandle);
    }

    if (s_platformInterface == nullptr) {
        isEOSValid = false;
        return false;
    } else {
        isEOSValid = true;
    }

    // -----
    // Achievements Interface
    // -----
    s_achievementsInterface = EOS_Platform_GetAchievementsInterface(s_platformInterface);
    if (s_achievementsInterface != nullptr) {
        EOS_Achievements_AddNotifyAchievementsUnlockedV2Options notifyAchievementsUnlockedV2Options;
        memset(&notifyAchievementsUnlockedV2Options, 0, sizeof(notifyAchievementsUnlockedV2Options));
        notifyAchievementsUnlockedV2Options.ApiVersion = EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKEDV2_API_LATEST;
        EOS_Achievements_AddNotifyAchievementsUnlockedV2(s_achievementsInterface, &notifyAchievementsUnlockedV2Options, nullptr, [](const EOS_Achievements_OnAchievementsUnlockedCallbackV2Info *data) {
            Dictionary ret;
            ret["user_id"] = eosg_product_user_id_to_string(data->UserId);
            ret["achievement_id"] = EOSG_GET_STRING(data->AchievementId);
            ret["unlock_time"] = data->UnlockTime;
            IEOS::get_singleton()->emit_signal("achievements_interface_achievements_unlocked_v2_callback", ret);
        });
    }

    // -----
    // Auth Interface
    // -----
    s_authInterface = EOS_Platform_GetAuthInterface(s_platformInterface);
    if (s_authInterface != nullptr) {
        EOS_Auth_AddNotifyLoginStatusChangedOptions notifyAuthLoginStatusChangedOptions;
        memset(&notifyAuthLoginStatusChangedOptions, 0, sizeof(notifyAuthLoginStatusChangedOptions));
        notifyAuthLoginStatusChangedOptions.ApiVersion = EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
        EOS_Auth_AddNotifyLoginStatusChanged(s_authInterface, &notifyAuthLoginStatusChangedOptions, nullptr, [](const EOS_Auth_LoginStatusChangedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["prev_status"] = static_cast<int>(data->PrevStatus);
            ret["current_status"] = static_cast<int>(data->CurrentStatus);
            IEOS::get_singleton()->emit_signal("auth_interface_login_status_changed", ret);
        });
    }

    // -----
    // Connect Interface
    // -----
    s_connectInterface = EOS_Platform_GetConnectInterface(s_platformInterface);
    if (s_connectInterface != nullptr) {
        EOS_Connect_AddNotifyAuthExpirationOptions notifyAuthExpirationOptions;
        memset(&notifyAuthExpirationOptions, 0, sizeof(notifyAuthExpirationOptions));
        notifyAuthExpirationOptions.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;
        EOS_Connect_AddNotifyAuthExpiration(s_connectInterface, &notifyAuthExpirationOptions, nullptr, [](const EOS_Connect_AuthExpirationCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            IEOS::get_singleton()->emit_signal("connect_interface_auth_expiration", ret);
        });
        EOS_Connect_AddNotifyLoginStatusChangedOptions notifyConnectLoginStatusChangedOptions;
        memset(&notifyConnectLoginStatusChangedOptions, 0, sizeof(notifyConnectLoginStatusChangedOptions));
        notifyConnectLoginStatusChangedOptions.ApiVersion = EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
        EOS_Connect_AddNotifyLoginStatusChanged(s_connectInterface, &notifyConnectLoginStatusChangedOptions, nullptr, [](const EOS_Connect_LoginStatusChangedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["previous_status"] = static_cast<int>(data->PreviousStatus);
            ret["current_status"] = static_cast<int>(data->CurrentStatus);
            IEOS::get_singleton()->emit_signal("connect_interface_login_status_changed", ret);
        });
    }

    // -----
    // Custom Invites Interface
    // -----
    s_customInvitesInterface = EOS_Platform_GetCustomInvitesInterface(s_platformInterface);
    if (s_customInvitesInterface != nullptr) {
        EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions notifyCustomInviteReceivedOptions;
        notifyCustomInviteReceivedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITERECEIVED_API_LATEST;
        EOS_CustomInvites_AddNotifyCustomInviteReceived(s_customInvitesInterface, &notifyCustomInviteReceivedOptions, nullptr, [](const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["custom_invite_id"] = EOSG_GET_STRING(data->CustomInviteId);
            ret["payload"] = EOSG_GET_STRING(data->Payload);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_custom_invite_received_callback", ret);
        });
        EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions notifyCustomInviteAcceptedOptions;
        notifyCustomInviteAcceptedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEACCEPTED_API_LATEST;
        EOS_CustomInvites_AddNotifyCustomInviteAccepted(s_customInvitesInterface, &notifyCustomInviteAcceptedOptions, nullptr, [](const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["custom_invite_id"] = EOSG_GET_STRING(data->CustomInviteId);
            ret["payload"] = EOSG_GET_STRING(data->Payload);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_custom_invite_accepted_callback", ret);
        });
        EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions notifyCustomInviteRejectedOptions;
        notifyCustomInviteRejectedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEREJECTED_API_LATEST;
        EOS_CustomInvites_AddNotifyCustomInviteRejected(s_customInvitesInterface, &notifyCustomInviteRejectedOptions, nullptr, [](const EOS_CustomInvites_CustomInviteRejectedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["custom_invite_id"] = EOSG_GET_STRING(data->CustomInviteId);
            ret["payload"] = EOSG_GET_STRING(data->Payload);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_custom_invite_rejected_callback", ret);
        });
        EOS_CustomInvites_AddNotifyRequestToJoinResponseReceivedOptions notifyCustomRequestToJoinResponseReceivedOptions;
        notifyCustomRequestToJoinResponseReceivedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRESPONSERECEIVED_API_LATEST;
        EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived(s_customInvitesInterface, &notifyCustomRequestToJoinResponseReceivedOptions, nullptr, [](const EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["from_user_id"] = eosg_product_user_id_to_string(data->FromUserId);
            ret["to_user_id"] = eosg_product_user_id_to_string(data->ToUserId);
            ret["response"] = static_cast<int>(data->Response);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_request_to_join_response_received_callback", ret);
        });
        EOS_CustomInvites_AddNotifyRequestToJoinReceivedOptions notifyCustomRequestToJoinReceivedOptions;
        notifyCustomRequestToJoinReceivedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRECEIVED_API_LATEST;
        EOS_CustomInvites_AddNotifyRequestToJoinReceived(s_customInvitesInterface, &notifyCustomRequestToJoinReceivedOptions, nullptr, [](const EOS_CustomInvites_RequestToJoinReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["from_user_id"] = eosg_product_user_id_to_string(data->FromUserId);
            ret["to_user_id"] = eosg_product_user_id_to_string(data->ToUserId);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_request_to_join_received_callback", ret);
        });
        EOS_CustomInvites_AddNotifyRequestToJoinAcceptedOptions notifyCustomRequestToJoinAcceptedOptions;
        notifyCustomRequestToJoinAcceptedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINACCEPTED_API_LATEST;
        EOS_CustomInvites_AddNotifyRequestToJoinAccepted(s_customInvitesInterface, &notifyCustomRequestToJoinAcceptedOptions, nullptr, [](const EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_request_to_join_accepted_callback", ret);
        });
        EOS_CustomInvites_AddNotifyRequestToJoinRejectedOptions notifyCustomRequestToJoinRejectedOptions;
        notifyCustomRequestToJoinRejectedOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINREJECTED_API_LATEST;
        EOS_CustomInvites_AddNotifyRequestToJoinRejected(s_customInvitesInterface, &notifyCustomRequestToJoinRejectedOptions, nullptr, [](const EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            IEOS::get_singleton()->emit_signal("custom_invites_interface_request_to_join_rejected_callback", ret);
        });
    }

    // -----
    // ECOM Interface
    // -----
    s_ecomInterface = EOS_Platform_GetEcomInterface(s_platformInterface);

    // -----
    // Friends Interface
    // -----
    s_friendsInterface = EOS_Platform_GetFriendsInterface(s_platformInterface);
    if (s_friendsInterface != nullptr) {
        EOS_Friends_AddNotifyFriendsUpdateOptions notifyFriendsUpdateOptions;
        notifyFriendsUpdateOptions.ApiVersion = EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST;
        EOS_Friends_AddNotifyFriendsUpdate(s_friendsInterface, &notifyFriendsUpdateOptions, nullptr, [](const EOS_Friends_OnFriendsUpdateInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
            ret["previous_status"] = static_cast<int>(data->PreviousStatus);
            ret["current_status"] = static_cast<int>(data->CurrentStatus);
            IEOS::get_singleton()->emit_signal("friends_interface_friends_update_callback", ret);
        });
        EOS_Friends_AddNotifyBlockedUsersUpdateOptions notifyBlockedUsersUpdateOptions;
        notifyBlockedUsersUpdateOptions.ApiVersion = EOS_FRIENDS_ADDNOTIFYBLOCKEDUSERSUPDATE_API_LATEST;
        EOS_Friends_AddNotifyBlockedUsersUpdate(s_friendsInterface, &notifyBlockedUsersUpdateOptions, nullptr, [](const EOS_Friends_OnBlockedUsersUpdateInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
            ret["blocked"] = EOSG_GET_BOOL(data->bBlocked);
            IEOS::get_singleton()->emit_signal("friends_interface_blocked_users_update_callback", ret);
        });
    }

    // -----
    // KWS Interface
    // -----
    s_kwsInterface = EOS_Platform_GetKWSInterface(s_platformInterface);
    if (s_kwsInterface != nullptr) {
        EOS_KWS_AddNotifyPermissionsUpdateReceivedOptions notifyPermissionsUpdateReceivedOptions;
        notifyPermissionsUpdateReceivedOptions.ApiVersion = EOS_KWS_ADDNOTIFYPERMISSIONSUPDATERECEIVED_API_LATEST;
        EOS_KWS_AddNotifyPermissionsUpdateReceived(s_kwsInterface, &notifyPermissionsUpdateReceivedOptions, nullptr, [](const EOS_KWS_PermissionsUpdateReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["kws_user_id"] = EOSG_GET_STRING(data->KWSUserId);
            ret["date_of_birth"] = EOSG_GET_STRING(data->DateOfBirth);
            ret["is_minor"] = EOSG_GET_BOOL(data->bIsMinor);
            ret["parent_email"] = EOSG_GET_STRING(data->ParentEmail);
            IEOS::get_singleton()->emit_signal("kws_interface_permissions_update_received_callback", ret);
        });
    }

    // -----
    // Leaderboards Interface
    // -----
    s_leaderboardsInterface = EOS_Platform_GetLeaderboardsInterface(s_platformInterface);

    // -----
    // Lobby Interface
    // -----
    s_lobbyInterface = EOS_Platform_GetLobbyInterface(s_platformInterface);
    if (s_lobbyInterface != nullptr) {
        EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions notifyLobbyUpdateReceivedOptions;
        notifyLobbyUpdateReceivedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST;
        EOS_Lobby_AddNotifyLobbyUpdateReceived(s_lobbyInterface, &notifyLobbyUpdateReceivedOptions, nullptr, [](const EOS_Lobby_LobbyUpdateReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            IEOS::get_singleton()->emit_signal("lobby_interface_lobby_update_received_callback", ret);
        });
        EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions notifyLobbyMemberUpdateReceivedOptions;
        notifyLobbyMemberUpdateReceivedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYMEMBERUPDATERECEIVED_API_LATEST;
        EOS_Lobby_AddNotifyLobbyMemberUpdateReceived(s_lobbyInterface, &notifyLobbyMemberUpdateReceivedOptions, nullptr, [](const EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            IEOS::get_singleton()->emit_signal("lobby_interface_lobby_member_update_received_callback", ret);
        });
        EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions notifyLobbyMemberStatusReceivedOptions;
        notifyLobbyMemberStatusReceivedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST;
        EOS_Lobby_AddNotifyLobbyMemberStatusReceived(s_lobbyInterface, &notifyLobbyMemberStatusReceivedOptions, nullptr, [](const EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["current_status"] = static_cast<int>(data->CurrentStatus);
            IEOS::get_singleton()->emit_signal("lobby_interface_lobby_member_status_received_callback", ret);
        });
        EOS_Lobby_AddNotifyLobbyInviteReceivedOptions notifyLobbyInviteReceivedOptions;
        notifyLobbyInviteReceivedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST;
        EOS_Lobby_AddNotifyLobbyInviteReceived(s_lobbyInterface, &notifyLobbyInviteReceivedOptions, nullptr, [](const EOS_Lobby_LobbyInviteReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
            IEOS::get_singleton()->emit_signal("lobby_interface_lobby_invite_received_callback", ret);
        });
        EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions notifyLobbyInviteAcceptedOptions;
        notifyLobbyInviteAcceptedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITEACCEPTED_API_LATEST;
        EOS_Lobby_AddNotifyLobbyInviteAccepted(s_lobbyInterface, &notifyLobbyInviteAcceptedOptions, nullptr, [](const EOS_Lobby_LobbyInviteAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            IEOS::get_singleton()->emit_signal("lobby_interface_lobby_invite_accepted_callback", ret);
        });
        EOS_Lobby_AddNotifyLobbyInviteRejectedOptions notifyLobbyInviteRejectedOptions;
        notifyLobbyInviteRejectedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITEREJECTED_API_LATEST;
        EOS_Lobby_AddNotifyLobbyInviteRejected(s_lobbyInterface, &notifyLobbyInviteRejectedOptions, nullptr, [](const EOS_Lobby_LobbyInviteRejectedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            IEOS::get_singleton()->emit_signal("lobby_interface_lobby_invite_rejected_callback", ret);
        });
        EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions notifyJoinLobbyAcceptedOptions;
        notifyJoinLobbyAcceptedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYJOINLOBBYACCEPTED_API_LATEST;
        EOS_Lobby_AddNotifyJoinLobbyAccepted(s_lobbyInterface, &notifyJoinLobbyAcceptedOptions, nullptr, [](const EOS_Lobby_JoinLobbyAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["ui_event_id"] = data->UiEventId;
            IEOS::get_singleton()->emit_signal("lobby_interface_join_lobby_accepted_callback", ret);
        });
        EOS_Lobby_AddNotifySendLobbyNativeInviteRequestedOptions notifySendLobbyNativeInviteRequestedOptions;
        notifySendLobbyNativeInviteRequestedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYSENDLOBBYNATIVEINVITEREQUESTED_API_LATEST;
        EOS_Lobby_AddNotifySendLobbyNativeInviteRequested(s_lobbyInterface, &notifySendLobbyNativeInviteRequestedOptions, nullptr, [](const EOS_Lobby_SendLobbyNativeInviteRequestedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["ui_event_id"] = data->UiEventId;
            ret["target_native_account_type"] = EOSG_GET_STRING(data->TargetNativeAccountType);
            ret["target_native_account_id"] = EOSG_GET_STRING(data->TargetUserNativeAccountId);
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            IEOS::get_singleton()->emit_signal("lobby_interface_send_lobby_native_invite_requested_callback", ret);
        });
        EOS_Lobby_AddNotifyRTCRoomConnectionChangedOptions notifyRTCRoomConnectionChangedOptions;
        notifyRTCRoomConnectionChangedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYRTCROOMCONNECTIONCHANGED_API_LATEST;
        EOS_Lobby_AddNotifyRTCRoomConnectionChanged(s_lobbyInterface, &notifyRTCRoomConnectionChangedOptions, nullptr, [](const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo *data) {
            Dictionary ret;
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["is_connected"] = EOSG_GET_BOOL(data->bIsConnected);
            ret["disconnect_reason"] = static_cast<int>(data->DisconnectReason);
            IEOS::get_singleton()->emit_signal("lobby_interface_rtc_room_connection_changed_callback", ret);
        });
        EOS_Lobby_AddNotifyLeaveLobbyRequestedOptions notifyLeaveLobbyRequestedOptions;
        notifyLeaveLobbyRequestedOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLEAVELOBBYREQUESTED_API_LATEST;
        EOS_Lobby_AddNotifyLeaveLobbyRequested(s_lobbyInterface, &notifyLeaveLobbyRequestedOptions, nullptr, [](const EOS_Lobby_LeaveLobbyRequestedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
            IEOS::get_singleton()->emit_signal("lobby_interface_leave_lobby_requested_callback", ret);
        });
    }

    // -----
    // Metrics Interface
    // -----
    s_metricsInterface = EOS_Platform_GetMetricsInterface(s_platformInterface);

    // -----
    // Mods Interface
    // -----
    s_modsInterface = EOS_Platform_GetModsInterface(s_platformInterface);

    // -----
    // P2P Interface
    // -----
    s_p2pInterface = EOS_Platform_GetP2PInterface(s_platformInterface);
    if (s_p2pInterface != nullptr) {
        EOS_P2P_AddNotifyIncomingPacketQueueFullOptions notifyIncomingPacketQueueFullOptions;
        notifyIncomingPacketQueueFullOptions.ApiVersion = EOS_P2P_ADDNOTIFYINCOMINGPACKETQUEUEFULL_API_LATEST;
        EOS_P2P_AddNotifyIncomingPacketQueueFull(s_p2pInterface, &notifyIncomingPacketQueueFullOptions, nullptr, [](const EOS_P2P_OnIncomingPacketQueueFullInfo *data) {
            Dictionary ret;
            ret["overflow_packet_channel"] = data->OverflowPacketChannel;
            ret["overflow_packet_local_user_id"] = eosg_product_user_id_to_string(data->OverflowPacketLocalUserId);
            ret["overflow_packet_size_bytes"] = data->OverflowPacketSizeBytes;
            ret["packet_queue_current_size_bytes"] = data->PacketQueueCurrentSizeBytes;
            ret["packet_queue_max_size_bytes"] = data->PacketQueueMaxSizeBytes;
            IEOS::get_singleton()->emit_signal("p2p_interface_incoming_packet_queue_full", ret);
        });
    }

    // -----
    // PlayerDataStorage Interface
    // -----
    s_playerDataStorageInterface = EOS_Platform_GetPlayerDataStorageInterface(s_platformInterface);

    // -----
    // Presence Interface
    // -----
    s_presenceInterface = EOS_Platform_GetPresenceInterface(s_platformInterface);
    if (s_presenceInterface != nullptr) {
        EOS_Presence_AddNotifyOnPresenceChangedOptions notifyOnPresenceChangedOptions;
        notifyOnPresenceChangedOptions.ApiVersion = EOS_PRESENCE_ADDNOTIFYONPRESENCECHANGED_API_LATEST;
        EOS_Presence_AddNotifyOnPresenceChanged(s_presenceInterface, &notifyOnPresenceChangedOptions, nullptr, [](const EOS_Presence_PresenceChangedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["presence_user_id"] = eosg_epic_account_id_to_string(data->PresenceUserId);
            IEOS::get_singleton()->emit_signal("presence_interface_presence_changed_callback", ret);
        });
        EOS_Presence_AddNotifyJoinGameAcceptedOptions notifyJoinGameAcceptedOptions;
        notifyJoinGameAcceptedOptions.ApiVersion = EOS_PRESENCE_ADDNOTIFYJOINGAMEACCEPTED_API_LATEST;
        EOS_Presence_AddNotifyJoinGameAccepted(s_presenceInterface, &notifyJoinGameAcceptedOptions, nullptr, [](const EOS_Presence_JoinGameAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
            ret["ui_event_id"] = data->UiEventId;
            ret["join_info"] = EOSG_GET_STRING(data->JoinInfo);
            IEOS::get_singleton()->emit_signal("presence_interface_join_game_accepted_callback", ret);
        });
    }

    // -----
    // ProgressionSnapshot Interface
    // -----
    s_progressionSnapshotInterface = EOS_Platform_GetProgressionSnapshotInterface(s_platformInterface);

    // -----
    // Reports Interface
    // -----
    s_reportsInterface = EOS_Platform_GetReportsInterface(s_platformInterface);

    // -----
    // Sanctions Interface
    // -----
    s_sanctionsInterface = EOS_Platform_GetSanctionsInterface(s_platformInterface);

    // -----
    // Sessions Interface
    // -----
    s_sessionsInterface = EOS_Platform_GetSessionsInterface(s_platformInterface);
    if (s_sessionsInterface != nullptr) {
        EOS_Sessions_AddNotifySessionInviteReceivedOptions notifySessionInviteReceivedOptions;
        notifySessionInviteReceivedOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITERECEIVED_API_LATEST;
        EOS_Sessions_AddNotifySessionInviteReceived(s_sessionsInterface, &notifySessionInviteReceivedOptions, nullptr, [](const EOS_Sessions_SessionInviteReceivedCallbackInfo *data) {
            Dictionary ret;
            ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            IEOS::get_singleton()->emit_signal("sessions_interface_session_invite_received_callback", ret);
        });
        EOS_Sessions_AddNotifySessionInviteAcceptedOptions notifySessionInviteAcceptedOptions;
        notifySessionInviteAcceptedOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITEACCEPTED_API_LATEST;
        EOS_Sessions_AddNotifySessionInviteAccepted(s_sessionsInterface, &notifySessionInviteAcceptedOptions, nullptr, [](const EOS_Sessions_SessionInviteAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["session_id"] = EOSG_GET_STRING(data->SessionId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            IEOS::get_singleton()->emit_signal("sessions_interface_session_invite_accepted_callback", ret);
        });
        EOS_Sessions_AddNotifySessionInviteRejectedOptions notifySessionInviteRejectedOptions;
        notifySessionInviteRejectedOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITEREJECTED_API_LATEST;
        EOS_Sessions_AddNotifySessionInviteRejected(s_sessionsInterface, &notifySessionInviteRejectedOptions, nullptr, [](const EOS_Sessions_SessionInviteRejectedCallbackInfo *data) {
            Dictionary ret;
            ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["session_id"] = EOSG_GET_STRING(data->SessionId);
            ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
            IEOS::get_singleton()->emit_signal("sessions_interface_session_invite_rejected_callback", ret);
        });
        EOS_Sessions_AddNotifyJoinSessionAcceptedOptions notifyJoinSessionAcceptedOptions;
        notifyJoinSessionAcceptedOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYJOINSESSIONACCEPTED_API_LATEST;
        EOS_Sessions_AddNotifyJoinSessionAccepted(s_sessionsInterface, &notifyJoinSessionAcceptedOptions, nullptr, [](const EOS_Sessions_JoinSessionAcceptedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["ui_event_id"] = data->UiEventId;
            IEOS::get_singleton()->emit_signal("sessions_interface_join_session_accepted_callback", ret);
        });
        EOS_Sessions_AddNotifyLeaveSessionRequestedOptions notifyLeaveSessionRequestedOptions;
        notifyLeaveSessionRequestedOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYLEAVESESSIONREQUESTED_API_LATEST;
        EOS_Sessions_AddNotifyLeaveSessionRequested(s_sessionsInterface, &notifyLeaveSessionRequestedOptions, nullptr, [](const EOS_Sessions_LeaveSessionRequestedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["session_name"] = EOSG_GET_STRING(data->SessionName);
            IEOS::get_singleton()->emit_signal("sessions_interface_leave_session_requested_callback", ret);
        });
        EOS_Sessions_AddNotifySendSessionNativeInviteRequestedOptions notifySendSessionNativeInviteRequestedOptions;
        notifySendSessionNativeInviteRequestedOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYSENDSESSIONNATIVEINVITEREQUESTED_API_LATEST;
        EOS_Sessions_AddNotifySendSessionNativeInviteRequested(s_sessionsInterface, &notifySendSessionNativeInviteRequestedOptions, nullptr, [](const EOS_Sessions_SendSessionNativeInviteRequestedCallbackInfo *data) {
            Dictionary ret;
            ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
            ret["session_id"] = EOSG_GET_STRING(data->SessionId);
            ret["target_native_account_type"] = EOSG_GET_STRING(data->TargetNativeAccountType);
            ret["target_user_native_account_id"] = EOSG_GET_STRING(data->TargetUserNativeAccountId);
            ret["ui_event_id"] = data->UiEventId;
            IEOS::get_singleton()->emit_signal("sessions_interface_send_session_native_invite_requested_callback", ret);
        });
    }

    // -----
    // Stats Interface
    // -----
    s_statsInterface = EOS_Platform_GetStatsInterface(s_platformInterface);

    // -----
    // RTC Interface
    // -----
    s_rtcInterface = EOS_Platform_GetRTCInterface(s_platformInterface);

    // -----
    // RTCAudio Interface
    // -----
    s_rtcAudioInterface = EOS_RTC_GetAudioInterface(s_rtcInterface);
    if (s_rtcAudioInterface != nullptr) {
        EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions notifyAudioDevicesChangedOptions;
        memset(&notifyAudioDevicesChangedOptions, 0, sizeof(notifyAudioDevicesChangedOptions));
        notifyAudioDevicesChangedOptions.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST;
        EOS_RTCAudio_AddNotifyAudioDevicesChanged(s_rtcAudioInterface, &notifyAudioDevicesChangedOptions, nullptr, [](const EOS_RTCAudio_AudioDevicesChangedCallbackInfo *data) {
            Dictionary ret;
            IEOS::get_singleton()->emit_signal("rtc_audio_audio_devices_changed", ret);
        });
    }

    // -----
    // TitleStorage Interface
    // -----
    s_titleStorageInterface = EOS_Platform_GetTitleStorageInterface(s_platformInterface);

    // -----
    // UI Interface
    // -----
    s_uiInterface = EOS_Platform_GetUIInterface(s_platformInterface);
    if (s_uiInterface != nullptr) {
        EOS_UI_AddNotifyDisplaySettingsUpdatedOptions notifyDisplaySettingsUpdatedOptions;
        notifyDisplaySettingsUpdatedOptions.ApiVersion = EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_LATEST;
        EOS_UI_AddNotifyDisplaySettingsUpdated(s_uiInterface, &notifyDisplaySettingsUpdatedOptions, nullptr, [](const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo *data) {
            Dictionary ret;
            ret["is_visible"] = EOSG_GET_BOOL(data->bIsVisible);
            ret["is_exclusive_input"] = EOSG_GET_BOOL(data->bIsExclusiveInput);
            IEOS::get_singleton()->emit_signal("ui_interface_display_settings_updated_callback", ret);
        });
        EOS_UI_AddNotifyMemoryMonitorOptions notifyMemoryMonitorOptions;
        notifyMemoryMonitorOptions.ApiVersion = EOS_UI_ADDNOTIFYMEMORYMONITOR_API_LATEST;
        EOS_UI_AddNotifyMemoryMonitor(s_uiInterface, &notifyMemoryMonitorOptions, nullptr, [](const EOS_UI_MemoryMonitorCallbackInfo *data) {
            Dictionary ret;
            IEOS::get_singleton()->emit_signal("ui_interface_memory_monitor_callback", ret);
        });
    }

    // -----
    // UserInfo Interface
    // -----
    s_userInfoInterface = EOS_Platform_GetUserInfoInterface(s_platformInterface);

    // -----
    // AntiCheatServer Interface
    // -----
    s_antiCheatServerInterface = EOS_Platform_GetAntiCheatServerInterface(s_platformInterface);
    if (s_antiCheatServerInterface != nullptr) {
        EOS_AntiCheatServer_AddNotifyMessageToClientOptions notifyMessageToClientOptions;
        notifyMessageToClientOptions.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYMESSAGETOCLIENT_API_LATEST;
        EOS_AntiCheatServer_AddNotifyMessageToClient(s_antiCheatServerInterface, &notifyMessageToClientOptions, nullptr, [](const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo *data) {
            PackedByteArray buffer;
            buffer.resize(data->MessageDataSizeBytes);
            std::memcpy(buffer.ptrw(), data->MessageData, data->MessageDataSizeBytes);

            String client_handle = IEOS::get_singleton()->_anticheat_player_handle_to_id(data->ClientHandle);

            Dictionary ret;
            ret["client_handle"] = client_handle;
            ret["message_data"] = buffer;
            IEOS::get_singleton()->emit_signal("anticheatserver_interface_message_to_client_callback", ret);
        });

        EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions notifyClientActionRequiredOptions;
        notifyClientActionRequiredOptions.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTACTIONREQUIRED_API_LATEST;
        EOS_AntiCheatServer_AddNotifyClientActionRequired(s_antiCheatServerInterface, &notifyClientActionRequiredOptions, nullptr, [](const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo *data) {
            Dictionary ret;
            String client_handle = IEOS::get_singleton()->_anticheat_player_handle_to_id(data->ClientHandle);
            ret["client_handle"] = client_handle;
            ret["client_action"] = static_cast<int>(data->ClientAction);
            ret["action_reason_code"] = static_cast<int>(data->ActionReasonCode);
            ret["action_reason_details_string"] = EOSG_GET_STRING(data->ActionReasonDetailsString);
            IEOS::get_singleton()->emit_signal("anticheatserver_interface_client_action_required_callback", ret);
        });

        EOS_AntiCheatServer_AddNotifyClientAuthStatusChangedOptions notifyClientAuthStatusChangedOptions;
        notifyClientAuthStatusChangedOptions.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTAUTHSTATUSCHANGED_API_LATEST;
        EOS_AntiCheatServer_AddNotifyClientAuthStatusChanged(s_antiCheatServerInterface, &notifyClientAuthStatusChangedOptions, nullptr, [](const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo *data) {
            Dictionary ret;
            String client_handle = IEOS::get_singleton()->_anticheat_player_handle_to_id(data->ClientHandle);
            ret["client_handle"] = client_handle;
            ret["client_auth_status"] = static_cast<int>(data->ClientAuthStatus);
            IEOS::get_singleton()->emit_signal("anticheatserver_interface_client_auth_status_changed_callback", ret);
        });
    }

    // -----
    // AntiCheatClient Interface
    // -----
    s_antiCheatClientInterface = EOS_Platform_GetAntiCheatClientInterface(s_platformInterface);
    if (s_antiCheatClientInterface != nullptr) {
        EOS_AntiCheatClient_AddNotifyMessageToServerOptions notifyMessageToServerOptions;
        notifyMessageToServerOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOSERVER_API_LATEST;
        EOS_AntiCheatClient_AddNotifyMessageToServer(s_antiCheatClientInterface, &notifyMessageToServerOptions, nullptr, [](const EOS_AntiCheatClient_OnMessageToServerCallbackInfo *data) {
            PackedByteArray buffer;
            buffer.resize(data->MessageDataSizeBytes);
            std::memcpy(buffer.ptrw(), data->MessageData, data->MessageDataSizeBytes);

            Dictionary ret;
            ret["message_data"] = buffer;
            IEOS::get_singleton()->emit_signal("anticheat_client_interface_message_to_server_callback", ret);
        });

        EOS_AntiCheatClient_AddNotifyMessageToPeerOptions notifyMessageToPeerOptions;
        notifyMessageToPeerOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOPEER_API_LATEST;
        EOS_AntiCheatClient_AddNotifyMessageToPeer(s_antiCheatClientInterface, &notifyMessageToPeerOptions, nullptr, [](const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo *data) {
            PackedByteArray buffer;
            buffer.resize(data->MessageDataSizeBytes);
            std::memcpy(buffer.ptrw(), data->MessageData, data->MessageDataSizeBytes);

            String client_handle = IEOS::get_singleton()->_anticheat_player_handle_to_id(data->ClientHandle);

            Dictionary ret;
            ret["client_handle"] = client_handle;
            ret["message_data"] = buffer;
            IEOS::get_singleton()->emit_signal("anticheat_client_interface_message_to_peer_callback", ret);
        });

        EOS_AntiCheatClient_AddNotifyPeerActionRequiredOptions notifyPeerActionRequiredOptions;
        notifyPeerActionRequiredOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYPEERACTIONREQUIRED_API_LATEST;
        EOS_AntiCheatClient_AddNotifyPeerActionRequired(s_antiCheatClientInterface, &notifyPeerActionRequiredOptions, nullptr, [](const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo *data) {
            String client_handle = IEOS::get_singleton()->_anticheat_player_handle_to_id(data->ClientHandle);
            Dictionary ret;
            ret["client_handle"] = client_handle;
            ret["client_action"] = static_cast<int>(data->ClientAction);
            ret["action_reason_code"] = static_cast<int>(data->ActionReasonCode);
            ret["action_reason_details_string"] = EOSG_GET_STRING(data->ActionReasonDetailsString);
        });

        EOS_AntiCheatClient_AddNotifyPeerAuthStatusChangedOptions notifyPeerAuthStatusChangedOptions;
        notifyPeerAuthStatusChangedOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYPEERAUTHSTATUSCHANGED_API_LATEST;
        EOS_AntiCheatClient_AddNotifyPeerAuthStatusChanged(s_antiCheatClientInterface, &notifyPeerAuthStatusChangedOptions, nullptr, [](const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo *data) {
            String client_handle = IEOS::get_singleton()->_anticheat_player_handle_to_id(data->ClientHandle);
            Dictionary ret;
            ret["client_handle"] = client_handle;
            ret["client_auth_status"] = static_cast<int>(data->ClientAuthStatus);
        });

        EOS_AntiCheatClient_AddNotifyClientIntegrityViolatedOptions notifyClientIntegrityViolatedOptions;
        notifyClientIntegrityViolatedOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYCLIENTINTEGRITYVIOLATED_API_LATEST;
        EOS_AntiCheatClient_AddNotifyClientIntegrityViolated(s_antiCheatClientInterface, &notifyClientIntegrityViolatedOptions, nullptr, [](const EOS_AntiCheatClient_OnClientIntegrityViolatedCallbackInfo *data) {
            Dictionary ret;
            ret["violation_type"] = static_cast<int>(data->ViolationType);
            ret["violation_message"] = EOSG_GET_STRING(data->ViolationMessage);
            IEOS::get_singleton()->emit_signal("anticheat_client_interface_client_integrity_violated_callback", ret);
        });
    }

    return true;
}

Dictionary IEOS::platform_interface_get_active_country_code(const String &p_local_user_id) {
    ERR_FAIL_NULL_V(s_platformInterface, {});
    CharString user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = EOS_EpicAccountId_FromString(user_id.get_data());
    char *OutBuffer = (char *)memalloc(EOS_COUNTRYCODE_MAX_LENGTH + 1);
    int OutBufferLength = 0;
    EOS_EResult res = EOS_Platform_GetActiveCountryCode(s_platformInterface, localUserId, OutBuffer, &OutBufferLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["country_code"] = "";
    if (res == EOS_EResult::EOS_Success) {
        ret["country_code"] = String(OutBuffer);
    }

    return ret;
}

Dictionary IEOS::platform_interface_get_active_locale_code(const String &p_user_id) {
    ERR_FAIL_NULL_V(s_platformInterface, {});
    CharString user_id = p_user_id.utf8();
    EOS_EpicAccountId LocalUserId = EOS_EpicAccountId_FromString(user_id.get_data());
    char *OutBuffer = (char *)memalloc(EOS_LOCALECODE_MAX_LENGTH + 1);
    int OutBufferLength = 0;
    EOS_EResult res = EOS_Platform_GetActiveLocaleCode(s_platformInterface, LocalUserId, OutBuffer, &OutBufferLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["locale_code"] = "";
    if (res == EOS_EResult::EOS_Success) {
        ret["locale_code"] = String(OutBuffer);
    }

    return ret;
}

Dictionary IEOS::platform_interface_get_override_country_code() {
    ERR_FAIL_NULL_V(s_platformInterface, {});
    char *OutBuffer = (char *)memalloc(EOS_COUNTRYCODE_MAX_LENGTH + 1);
    int32_t *OutBufferLength = (int32_t *)memalloc(sizeof(int32_t));
    EOS_EResult res = EOS_Platform_GetOverrideCountryCode(s_platformInterface, OutBuffer, OutBufferLength);

    Dictionary dict;
    dict["result_code"] = static_cast<int>(res);
    dict["country_code"] = "";
    if (res == EOS_EResult::EOS_Success) {
        dict["country_code"] = String(OutBuffer);
    }

    return dict;
}

Dictionary IEOS::platform_interface_get_override_locale_code() {
    ERR_FAIL_NULL_V(s_platformInterface, {});
    char *OutBuffer = (char *)memalloc(EOS_LOCALECODE_MAX_LENGTH + 1);
    int32_t *OutBufferLength = (int32_t *)memalloc(sizeof(int32_t));
    EOS_EResult res = EOS_Platform_GetOverrideLocaleCode(s_platformInterface, OutBuffer, OutBufferLength);

    Dictionary dict;
    dict["result_code"] = static_cast<int>(res);
    dict["locale_code"] = "";
    if (res == EOS_EResult::EOS_Success) {
        dict["locale_code"] = String(OutBuffer);
    }

    return dict;
}

int IEOS::platform_interface_set_override_country_code(const String &p_country_code) {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString country_code = p_country_code.utf8();
    return static_cast<int>(EOS_Platform_SetOverrideCountryCode(s_platformInterface, country_code.get_data()));
}

int IEOS::platform_interface_set_override_locale_code(const String &p_locale_code) {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString locale_code = p_locale_code.utf8();
    return static_cast<int>(EOS_Platform_SetOverrideLocaleCode(s_platformInterface, locale_code.get_data()));
}

int IEOS::platform_interface_check_for_launcher_and_restart() {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    return static_cast<int>(EOS_Platform_CheckForLauncherAndRestart(s_platformInterface));
}

int IEOS::platform_interface_initialize(Ref<RefCounted> p_options) {
    CharString productName = VARIANT_TO_CHARSTRING(p_options->get("product_name"));
    CharString productVersion = VARIANT_TO_CHARSTRING(p_options->get("product_version"));

    EOS_InitializeOptions initOptions;
    memset(&initOptions, 0, sizeof(initOptions));
    initOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
    initOptions.ProductName = productName.get_data();
    initOptions.ProductVersion = productVersion.get_data();
    initOptions.AllocateMemoryFunction = nullptr;
    initOptions.ReallocateMemoryFunction = nullptr;
    initOptions.ReleaseMemoryFunction = nullptr;
    initOptions.Reserved = nullptr;
    // TODO: Implement EOS_Android_InitializeOptions for Android
    initOptions.SystemInitializeOptions = nullptr;
    initOptions.OverrideThreadAffinity = nullptr;

    EOS_EResult res = EOS_Initialize(&initOptions);

    EOS_EResult setCallbackResult = EOS_Logging_SetCallback([](const EOS_LogMessage *logMessage) {
        Dictionary logMsg;
        logMsg["category"] = logMessage->Category;
        logMsg["message"] = logMessage->Message;
        logMsg["level"] = (int)logMessage->Level;
        IEOS::get_singleton()->emit_signal("logging_interface_callback", logMsg);
    });

    if (setCallbackResult != EOS_EResult::EOS_Success) {
        UtilityFunctions::print("[EOSG] Failed to set logging callback: " + String(EOS_EResult_ToString(setCallbackResult)));
    }

    return static_cast<int>(res);
}

Dictionary IEOS::platform_interface_get_desktop_crossplay_status_info() {
    ERR_FAIL_NULL_V(s_platformInterface, {});
    EOS_Platform_GetDesktopCrossplayStatusOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST;
    EOS_Platform_DesktopCrossplayStatusInfo outDesktopCrossplayStatusInfo;
    EOS_EResult result = EOS_Platform_GetDesktopCrossplayStatus(s_platformInterface, &options, &outDesktopCrossplayStatusInfo);

    Dictionary dict;
    dict["result_code"] = static_cast<int>(result);
    Dictionary desktopCrossplayStatus = Dictionary();
    if (result == EOS_EResult::EOS_Success) {
        desktopCrossplayStatus["status"] = static_cast<int>(outDesktopCrossplayStatusInfo.Status);
        desktopCrossplayStatus["service_init_result"] = static_cast<int>(outDesktopCrossplayStatusInfo.ServiceInitResult);
    }
    dict["desktop_crossplay_status"] = desktopCrossplayStatus;
    return dict;
}

int IEOS::platform_interface_set_application_status(int p_status) {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_EResult result = EOS_Platform_SetApplicationStatus(s_platformInterface, static_cast<EOS_EApplicationStatus>(p_status));
    return static_cast<int>(result);
}

int IEOS::platform_interface_get_application_status() {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_EApplicationStatus::EOS_AS_Foreground));
    EOS_EApplicationStatus status = EOS_Platform_GetApplicationStatus(s_platformInterface);
    return static_cast<int>(status);
}

int IEOS::platform_interface_set_network_status(int p_status) {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_EResult result = EOS_Platform_SetNetworkStatus(s_platformInterface, static_cast<EOS_ENetworkStatus>(p_status));
    return static_cast<int>(result);
}

int IEOS::platform_interface_get_network_status() {
    ERR_FAIL_NULL_V(s_platformInterface, static_cast<int>(EOS_ENetworkStatus::EOS_NS_Online));
    EOS_ENetworkStatus status = EOS_Platform_GetNetworkStatus(s_platformInterface);
    return static_cast<int>(status);
}

void IEOS::platform_interface_release() {
    isEOSValid = false;

    s_achievementsInterface = nullptr;
    s_authInterface = nullptr;
    s_connectInterface = nullptr;
    s_customInvitesInterface = nullptr;
    s_ecomInterface = nullptr;
    s_friendsInterface = nullptr;
    s_kwsInterface = nullptr;
    s_leaderboardsInterface = nullptr;
    s_lobbyInterface = nullptr;
    s_metricsInterface = nullptr;
    s_modsInterface = nullptr;
    s_p2pInterface = nullptr;
    s_playerDataStorageInterface = nullptr;
    s_presenceInterface = nullptr;
    s_progressionSnapshotInterface = nullptr;
    s_reportsInterface = nullptr;
    s_rtcInterface = nullptr;
    s_rtcAudioInterface = nullptr;
    s_sanctionsInterface = nullptr;
    s_sessionsInterface = nullptr;
    s_statsInterface = nullptr;
    s_titleStorageInterface = nullptr;
    s_uiInterface = nullptr;
    s_userInfoInterface = nullptr;
    s_antiCheatServerInterface = nullptr;
    s_antiCheatClientInterface = nullptr;

    if (s_platformInterface != nullptr) {
        EOS_Platform_Release(s_platformInterface);
        s_platformInterface = nullptr;
    }
}

int IEOS::platform_interface_shutdown() {
    isEOSValid = false;
    EOS_EResult res = EOS_Shutdown();
    return static_cast<int>(res);
}

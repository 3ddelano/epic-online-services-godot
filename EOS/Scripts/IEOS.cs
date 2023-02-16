// Copyright 2022, Delano Lourenco
// https://github.com/3ddelano/epic-online-services-godot
// MIT License

using Godot;
using Godot.Collections;
using System;
using System.Linq;

using Epic.OnlineServices;
using Epic.OnlineServices.Logging;
using Epic.OnlineServices.Platform;
using Epic.OnlineServices.Auth;
using Epic.OnlineServices.Connect;
using Epic.OnlineServices.Achievements;
using Epic.OnlineServices.CustomInvites;
using Epic.OnlineServices.Stats;
using Epic.OnlineServices.Leaderboards;
using Epic.OnlineServices.Friends;
using Epic.OnlineServices.UserInfo;
using Epic.OnlineServices.Ecom;
using Epic.OnlineServices.UI;
using Epic.OnlineServices.KWS;
using Epic.OnlineServices.Metrics;
using Epic.OnlineServices.Mods;
using Epic.OnlineServices.Version;
using Epic.OnlineServices.Reports;
using Epic.OnlineServices.ProgressionSnapshot;
using Epic.OnlineServices.Presence;

public class IEOS : Node
{
    // ------------------------
    // Signals
    // ------------------------
    [Signal]
    public delegate void logging_interface_callback(Dictionary log_message);
    [Signal]
    public delegate void auth_interface_login_callback(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_logout_callback(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_login_status_changed(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_delete_persistent_auth_callback(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_query_id_token_callback(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_verify_id_token_callback(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_verify_user_auth_callback(Dictionary callback_data);
    [Signal]
    public delegate void auth_interface_link_account_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_login_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_auth_expiration(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_login_status_changed(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_create_device_id_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_delete_device_id_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_create_user_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_link_account_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_verify_id_token_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_transfer_device_id_account_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_unlink_account_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_achievements_unlocked_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_query_definitions_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_query_player_achievements_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_unlock_achievements_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void custom_invites_interface_custom_invite_accepted_callback(Dictionary callback_data);
    [Signal]
    public delegate void custom_invites_interface_custom_invite_received_callback(Dictionary callback_data);
    [Signal]
    public delegate void custom_invites_interface_send_custom_invite_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void stats_interface_ingest_stat_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void stats_interface_query_stats_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void leaderboards_interface_query_leaderboard_definitions_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void leaderboards_interface_query_leaderboard_ranks_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void leaderboards_interface_query_leaderboard_user_scores_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void friends_interface_accept_invite_callback(Dictionary callback_data);
    [Signal]
    public delegate void friends_interface_friends_update_callback(Dictionary callback_data);
    [Signal]
    public delegate void friends_interface_query_friends_callback(Dictionary callback_data);
    [Signal]
    public delegate void friends_interface_reject_invite_callback(Dictionary callback_data);
    [Signal]
    public delegate void friends_interface_send_invite_callback(Dictionary callback_data);
    [Signal]
    public delegate void user_info_interface_query_user_info_callback(Dictionary callback_data);
    [Signal]
    public delegate void user_info_interface_query_user_info_by_display_name_callback(Dictionary callback_data);
    [Signal]
    public delegate void user_info_interface_query_user_info_by_external_account_callback(Dictionary callback_data);
    [Signal]
    public delegate void ecom_interface_checkout_callback(Dictionary callback_data);
    [Signal]
    public delegate void ecom_interface_query_entitlements_callback(Dictionary callback_data);
    [Signal]
    public delegate void ecom_interface_query_offers_callback(Dictionary callback_data);
    [Signal]
    public delegate void ecom_interface_query_ownership_callback(Dictionary callback_data);
    [Signal]
    public delegate void ecom_interface_query_ownership_token_callback(Dictionary callback_data);
    [Signal]
    public delegate void ecom_interface_redeem_entitlements_callback(Dictionary callback_data);
    [Signal]
    public delegate void ui_interface_display_settings_updated_callback(Dictionary callback_data);
    [Signal]
    public delegate void ui_interface_hide_friends_callback(Dictionary callback_data);
    [Signal]
    public delegate void ui_interface_show_friends_callback(Dictionary callback_data);
    [Signal]
    public delegate void kws_interface_permissions_update_received(Dictionary callback_data);
    [Signal]
    public delegate void kws_interface_create_user_callback(Dictionary callback_data);
    [Signal]
    public delegate void kws_interface_query_age_gate_callback(Dictionary callback_data);
    [Signal]
    public delegate void kws_interface_query_permissions_callback(Dictionary callback_data);
    [Signal]
    public delegate void kws_interface_request_permissions_callback(Dictionary callback_data);
    [Signal]
    public delegate void kws_interface_update_parent_email_callback(Dictionary callback_data);
    [Signal]
    public delegate void mods_interface_enumerate_mods_callback(Dictionary callback_data);
    [Signal]
    public delegate void reports_interface_report_callback(Dictionary callback_data);
    [Signal]
    public delegate void progression_snapshot_interface_submit_snapshot_callback(Dictionary callback_data);
    [Signal]
    public delegate void progression_snapshot_interface_delete_snapshot_callback(Dictionary callback_data);
    [Signal]
    public delegate void presence_interface_join_game_accepted(Dictionary callback_data);
    [Signal]
    public delegate void presence_interface_presence_changed(Dictionary callback_data);
    [Signal]
    public delegate void presence_interface_query_presence_callback(Dictionary callback_data);
    [Signal]
    public delegate void presence_interface_set_presence_callback(Dictionary callback_data);



    // ------------------------
    // Public Functions
    // ------------------------

    // -----
    // Logging Interface
    // -----
    public Result logging_interface_set_log_level(int category, int level)
    {
        return LoggingInterface.SetLogLevel((LogCategory)category, (LogLevel)level);
    }


    // -----
    // Platform Interface
    // -----
    public Result platform_interface_initialize(Reference p_options)
    {
        // Initialize the EOS SDK
        var initializeOptions = new InitializeOptions()
        {
            ProductName = new Utf8String((string)p_options.Get("product_name")),
            ProductVersion = new Utf8String((string)p_options.Get("product_version")),
        };

        Result res = PlatformInterface.Initialize(ref initializeOptions);

        LoggingInterface.SetCallback((ref LogMessage logMessage) =>
        {
            EmitSignal(nameof(logging_interface_callback), new Dictionary(){
                {"category", logMessage.Category.ToString()},
                {"message", logMessage.Message.ToString()},
                {"level", logMessage.Level.ToString()}
            });
        });

        return res;
    }

    public bool platform_interface_create(Reference p_create_options)
    {
#if EOS_PLATFORM_WINDOWS
        string XAudio29DllPath = ProjectSettings.GlobalizePath("res://EOS/Bin/x64/xaudio2_9redist.dll");
        var options = new WindowsOptions()
#else
        var options = new Options()
#endif
        {
            ProductId = (string)p_create_options.Get("product_id"),
            SandboxId = (string)p_create_options.Get("sandbox_id"),
            DeploymentId = (string)p_create_options.Get("deployment_id"),
            ClientCredentials = new ClientCredentials()
            {
                ClientId = (string)p_create_options.Get("client_id"),
                ClientSecret = (string)p_create_options.Get("client_secret"),
            },
            IsServer = (bool)p_create_options.Get("is_server"),
            EncryptionKey = (string)p_create_options.Get("encryption_key")
#if EOS_PLATFORM_WINDOWS
            ,
            RTCOptions = new WindowsRTCOptions()
            {
                PlatformSpecificOptions = new WindowsRTCOptionsPlatformSpecificOptions()
                {
                    XAudio29DllPath = XAudio29DllPath
                }
            },
#endif
        };
        if ((string)p_create_options.Get("override_locale_code") != "") options.OverrideCountryCode = (string)p_create_options.Get("override_locale_code");

        int p_flags = (int)p_create_options.Get("flags");
        options.Flags = (PlatformFlags)p_flags;

        int p_tick_budget_in_milliseconds = (int)p_create_options.Get("tick_budget_in_milliseconds");
        options.TickBudgetInMilliseconds = (uint)p_tick_budget_in_milliseconds;

        s_PlatformInterface = PlatformInterface.Create(ref options);
        if (s_PlatformInterface == null)
        {
            return false;
        }


        // Connect some interface callbacks
        s_ConnectInterface = s_PlatformInterface.GetConnectInterface();
        var connect_notify_auth_expiration_options = new AddNotifyAuthExpirationOptions();
        s_ConnectInterface.AddNotifyAuthExpiration(ref connect_notify_auth_expiration_options, null, (ref AuthExpirationCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()}
            };
            EmitSignal(nameof(connect_interface_auth_expiration), ret);
        });
        var connect_login_status_changed_options = new Epic.OnlineServices.Connect.AddNotifyLoginStatusChangedOptions();
        s_ConnectInterface.AddNotifyLoginStatusChanged(ref connect_login_status_changed_options, null, (ref Epic.OnlineServices.Connect.LoginStatusChangedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"previous_status", data.PreviousStatus},
                {"current_status", data.CurrentStatus},
            };
            EmitSignal(nameof(connect_interface_login_status_changed), ret);
        });


        s_AuthInterface = s_PlatformInterface.GetAuthInterface();
        var auth_login_status_changed_option = new Epic.OnlineServices.Auth.AddNotifyLoginStatusChangedOptions();
        s_AuthInterface.AddNotifyLoginStatusChanged(ref auth_login_status_changed_option, null, (ref Epic.OnlineServices.Auth.LoginStatusChangedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"prev_status", data.PrevStatus},
                {"current_status", data.CurrentStatus},
            };
            EmitSignal(nameof(auth_interface_login_status_changed), ret);
        });


        s_AchievementsInterface = s_PlatformInterface.GetAchievementsInterface();
        var achievements_achievements_unlocked_option = new AddNotifyAchievementsUnlockedV2Options();
        s_AchievementsInterface.AddNotifyAchievementsUnlockedV2(ref achievements_achievements_unlocked_option, null, (ref OnAchievementsUnlockedCallbackV2Info data) =>
        {
            Dictionary ret = new Dictionary(){
                {"user_id", data.UserId.ToString()},
                {"achievement_id", data.AchievementId.ToString()},
                {"unlock_time", data.UnlockTime?.ToString()},
            };
            EmitSignal(nameof(achievements_interface_achievements_unlocked_callback), ret);
        });


        s_CustomInvitesInterface = s_PlatformInterface.GetCustomInvitesInterface();
        var custom_invites_custom_invite_accepted_options = new AddNotifyCustomInviteAcceptedOptions();
        s_CustomInvitesInterface.AddNotifyCustomInviteAccepted(ref custom_invites_custom_invite_accepted_options, null, (ref OnCustomInviteAcceptedCallbackInfo data) =>
        {
            EmitSignal(nameof(custom_invites_interface_custom_invite_accepted_callback), _ConvertOnCustomInviteCallback2Dict(data));
        });
        var custom_invites_custom_invite_received_options = new AddNotifyCustomInviteReceivedOptions();
        s_CustomInvitesInterface.AddNotifyCustomInviteReceived(ref custom_invites_custom_invite_received_options, null, (ref OnCustomInviteReceivedCallbackInfo data) =>
        {
            EmitSignal(nameof(custom_invites_interface_custom_invite_received_callback), _ConvertOnCustomInviteCallback2Dict(data));
        });


        s_StatsInterface = s_PlatformInterface.GetStatsInterface();

        s_LeaderboardsInterface = s_PlatformInterface.GetLeaderboardsInterface();

        s_FriendsInterface = s_PlatformInterface.GetFriendsInterface();
        var friends_friends_update_options = new AddNotifyFriendsUpdateOptions();
        s_FriendsInterface.AddNotifyFriendsUpdate(ref friends_friends_update_options, null, (ref OnFriendsUpdateInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"target_user_id", data.TargetUserId.ToString()},
                {"previous_status", data.PreviousStatus},
                {"current_status", data.CurrentStatus},
            };
            EmitSignal(nameof(friends_interface_friends_update_callback), ret);
        });

        s_UserInfoInterface = s_PlatformInterface.GetUserInfoInterface();

        s_EcomInterface = s_PlatformInterface.GetEcomInterface();

        s_UIInterface = s_PlatformInterface.GetUIInterface();
        var ui_display_settings_updated_options = new AddNotifyDisplaySettingsUpdatedOptions();
        s_UIInterface.AddNotifyDisplaySettingsUpdated(ref ui_display_settings_updated_options, null, (ref OnDisplaySettingsUpdatedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"is_visible", data.IsVisible},
                {"is_exclusive_input", data.IsExclusiveInput},
            };
            EmitSignal(nameof(ui_interface_display_settings_updated_callback), ret);
        });


        s_KWSInterface = s_PlatformInterface.GetKWSInterface();
        var kws_permissions_update_received_options = new AddNotifyPermissionsUpdateReceivedOptions();
        s_KWSInterface.AddNotifyPermissionsUpdateReceived(ref kws_permissions_update_received_options, null, (ref PermissionsUpdateReceivedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
            };
            EmitSignal(nameof(kws_interface_permissions_update_received), ret);
        });

        s_MetricsInterface = s_PlatformInterface.GetMetricsInterface();

        s_ModsInterface = s_PlatformInterface.GetModsInterface();

        s_ReportsInterface = s_PlatformInterface.GetReportsInterface();

        s_ProgressionSnapshotInterface = s_PlatformInterface.GetProgressionSnapshotInterface();

        s_PresenceInterface = s_PlatformInterface.GetPresenceInterface();
        var presence_join_game_accepted_options = new AddNotifyJoinGameAcceptedOptions();
        s_PresenceInterface.AddNotifyJoinGameAccepted(ref presence_join_game_accepted_options, null, (ref JoinGameAcceptedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"join_info", data.JoinInfo.ToString()},
                {"local_user_id", data.LocalUserId.ToString()},
                {"target_user_id", data.TargetUserId.ToString()},
                {"ui_event_id", data.UiEventId},
            };
            EmitSignal(nameof(presence_interface_join_game_accepted));
        });
        var presence_presence_changed_options = new AddNotifyOnPresenceChangedOptions();
        s_PresenceInterface.AddNotifyOnPresenceChanged(ref presence_presence_changed_options, null, (ref PresenceChangedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"presence_user_id", data.PresenceUserId.ToString()},
            };
            EmitSignal(nameof(presence_interface_presence_changed));
        });

        return true; // platform created successfully
    }

    public Result platform_interface_check_for_launcher_and_restart()
    {
        return s_PlatformInterface.CheckForLauncherAndRestart();
    }

    public Dictionary platform_interface_get_active_country_code(string user_id)
    {
        Utf8String countryCode;
        Result res = s_PlatformInterface.GetActiveCountryCode(EpicAccountId.FromString(user_id), out countryCode);
        return new Dictionary(){
            {"result", res},
            {"country_code", countryCode?.ToString()}
        };
    }

    public Dictionary platform_interface_get_override_country_code()
    {
        Utf8String countryCode;
        Result res = s_PlatformInterface.GetOverrideCountryCode(out countryCode);
        return new Dictionary(){
            {"result", res},
            {"country_code", countryCode?.ToString()}
        };
    }

    public Result platform_interface_set_override_country_code(string country_code)
    {
        return s_PlatformInterface.SetOverrideCountryCode(country_code);
    }

    public Dictionary platform_interface_get_active_locale_code(string user_id)
    {
        Utf8String localeCode;
        Result res = s_PlatformInterface.GetActiveLocaleCode(EpicAccountId.FromString(user_id), out localeCode);
        return new Dictionary(){
            {"result", res},
            {"country_code", localeCode?.ToString()}
        };
    }

    public Dictionary platform_interface_get_override_locale_code()
    {
        Utf8String localeCode;
        Result res = s_PlatformInterface.GetOverrideLocaleCode(out localeCode);
        return new Dictionary(){
            {"result", res},
            {"country_code", localeCode?.ToString()}
        };
    }

    public Result platform_interface_set_override_locale_code(string locale_code)
    {
        return s_PlatformInterface.SetOverrideLocaleCode(locale_code);
    }

    public int platform_interface_get_network_status()
    {
        return (int)s_PlatformInterface.GetNetworkStatus();
    }

    public Result platform_interface_set_network_status(NetworkStatus newStatus)
    {
        return s_PlatformInterface.SetNetworkStatus(newStatus);
    }

    public void platform_interface_release()
    {
        if (s_PlatformInterface != null)
        {
            s_PlatformInterface.Release();
        }
    }

    public Result platform_interface_shutdown()
    {
        return PlatformInterface.Shutdown();
    }

    // -----
    // Auth Interface
    // -----
    public void auth_interface_login(Reference p_options)
    {
        var p_credentials = (Reference)p_options.Get("credentials");

        var credentials = new Epic.OnlineServices.Auth.Credentials()
        {
            Id = null,
            Token = null,
            Type = (LoginCredentialType)p_credentials.Get("type")
        };

        if (p_credentials.Get("id") != null) credentials.Id = (string)p_credentials.Get("id");
        if (p_credentials.Get("token") != null) credentials.Token = (string)p_credentials.Get("token");
        if (p_credentials.Get("external_type") != null) credentials.ExternalType = (ExternalCredentialType)p_credentials.Get("external_type");

        var options = new Epic.OnlineServices.Auth.LoginOptions()
        {
            Credentials = credentials,
            ScopeFlags = (AuthScopeFlags)p_options.Get("scope_flags"),
        };
        object client_data = _get_client_data(p_options);

        s_AuthInterface.Login(ref options, client_data, (ref Epic.OnlineServices.Auth.LoginCallbackInfo data) =>
        {

            Dictionary ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"selected_account_id", data.SelectedAccountId?.ToString()},
                {"continuance_token", null},
                {"pin_grant_info", null},
                {"account_feature_restricted_info", null},
            };
            if (data.ContinuanceToken != null)
            {
                ContinuanceTokenWrapper ctw = new ContinuanceTokenWrapper(data.ContinuanceToken);
                ret["continuance_token"] = ctw;
            }
            if (data.PinGrantInfo != null)
                ret["pin_grant_info"] = _ConvertPinGrantInfo2Dict(data.PinGrantInfo);
            if (data.AccountFeatureRestrictedInfo != null)
            {
                Dictionary account_feature_restricted_info = new Dictionary(){
                    {"verification_uri", data.AccountFeatureRestrictedInfo?.VerificationURI.ToString()},
                };
                ret["account_feature_restricted_info"] = account_feature_restricted_info;
            }
            if (data.ResultCode == Result.Success)
            {
                ret.Add("pending", false);
                ret.Add("success", true);
            }
            else if (Common.IsOperationComplete(data.ResultCode))
            {
                ret.Add("pending", false);
                ret.Add("success", false);
            }
            else
            {
                ret.Add("pending", true);
                ret.Add("success", false);
            }

            EmitSignal(nameof(auth_interface_login_callback), ret);
        });
    }

    public void auth_interface_logout(Reference p_options)
    {
        var options = new LogoutOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id"))
        };
        object client_data = _get_client_data(p_options);

        s_AuthInterface.Logout(ref options, client_data, (ref LogoutCallbackInfo data) =>
        {
            Dictionary ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };

            EmitSignal(nameof(auth_interface_logout_callback), ret);
        });
    }

    public Dictionary auth_interface_copy_id_token(Reference p_options)
    {
        var options = new Epic.OnlineServices.Auth.CopyIdTokenOptions()
        {
            AccountId = EpicAccountId.FromString((string)p_options.Get("account_id")),
        };

        Epic.OnlineServices.Auth.IdToken? outAuthIdToken;
        Result res = s_AuthInterface.CopyIdToken(ref options, out outAuthIdToken);
        var ret = new Dictionary(){
            {"result_code", res},
            {"id_token", null}
        };
        if (outAuthIdToken != null)
        {
            var auth_id_token_dict = new Dictionary(){
                {"account_id", outAuthIdToken?.AccountId?.ToString()},
                {"json_web_token", outAuthIdToken?.JsonWebToken?.ToString()}
            };
            ret["id_token"] = auth_id_token_dict;
        }
        return ret;
    }

    public Dictionary auth_interface_copy_user_auth_token(Reference p_options, string p_local_user_id)
    {
        var options = new CopyUserAuthTokenOptions() { };

        Token? outToken;
        Result res = s_AuthInterface.CopyUserAuthToken(ref options, EpicAccountId.FromString(p_local_user_id), out outToken);
        var ret = new Dictionary(){
            {"result_code", res},
            {"token", null}
        };
        if (outToken != null)
        {
            var token_dict = new Dictionary(){
                {"app", outToken?.App?.ToString()},
                {"client_id", outToken?.ClientId?.ToString()},
                {"account_id", outToken?.AccountId?.ToString()},
                {"access_token", outToken?.AccessToken?.ToString()},
                {"expires_in", outToken?.ExpiresIn},
                {"expires_at", outToken?.ExpiresAt?.ToString()},
                {"auth_type", outToken?.AuthType},
                {"refresh_token", outToken?.RefreshToken?.ToString()},
                {"refresh_expires_in", outToken?.RefreshExpiresIn},
                {"refresh_expires_at", outToken?.RefreshExpiresAt?.ToString()},
            };
            ret["token"] = token_dict;
        }
        return ret;
    }

    public void auth_interface_delete_persistent_auth(Reference p_options)
    {
        var options = new DeletePersistentAuthOptions()
        {
            RefreshToken = (string)p_options.Get("refresh_token")
        };
        object client_data = _get_client_data(p_options);

        s_AuthInterface.DeletePersistentAuth(ref options, client_data, (ref DeletePersistentAuthCallbackInfo data) =>
        {
            Dictionary ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(auth_interface_delete_persistent_auth_callback), ret);
        });
    }

    public string auth_interface_get_logged_in_account_by_index(int p_index)
    {
        var accountId = s_AuthInterface.GetLoggedInAccountByIndex(p_index);
        if (accountId == null || !accountId.IsValid())
            return "";
        return accountId.ToString();
    }

    public int auth_interface_get_logged_in_accounts_count()
    {
        return s_AuthInterface.GetLoggedInAccountsCount();
    }

    public int auth_interface_get_login_status(string p_local_user_id)
    {
        return (int)s_AuthInterface.GetLoginStatus(EpicAccountId.FromString(p_local_user_id));
    }

    public string auth_interface_get_merged_account_by_index(string p_local_user_id, int p_index)
    {
        var accountId = s_AuthInterface.GetMergedAccountByIndex(EpicAccountId.FromString(p_local_user_id), (uint)p_index);
        if (accountId == null || !accountId.IsValid())
            return "";
        return accountId.ToString();
    }

    public int auth_interface_get_merged_accounts_count(string p_local_user_id)
    {
        return (int)s_AuthInterface.GetMergedAccountsCount(EpicAccountId.FromString(p_local_user_id));
    }

    public Dictionary auth_interface_get_selected_account_id(string p_local_user_id)
    {
        EpicAccountId outSelectedAccountId;

        var res = s_AuthInterface.GetSelectedAccountId(EpicAccountId.FromString(p_local_user_id), out outSelectedAccountId);
        var ret = new Dictionary(){
            {"result_code", res},
            {"selected_account_id", outSelectedAccountId?.ToString()},
        };
        return ret;
    }

    public void auth_interface_query_id_token(Reference p_options)
    {
        var options = new QueryIdTokenOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetAccountId = EpicAccountId.FromString((string)p_options.Get("target_account_id")),
        };
        object client_data = _get_client_data(p_options);

        s_AuthInterface.QueryIdToken(ref options, client_data, (ref QueryIdTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_account_id", data.TargetAccountId?.ToString()}
            };
            EmitSignal(nameof(auth_interface_query_id_token_callback), ret);
        });
    }

    public void auth_interface_verify_id_token(Reference p_options)
    {
        var id_token = (Reference)p_options.Get("id_token");

        var options = new Epic.OnlineServices.Auth.VerifyIdTokenOptions()
        {
            IdToken = new Epic.OnlineServices.Auth.IdToken()
            {
                AccountId = EpicAccountId.FromString((string)id_token.Get("account_id")),
                JsonWebToken = (string)id_token.Get("json_web_token")
            }
        };
        object client_data = _get_client_data(p_options);

        s_AuthInterface.VerifyIdToken(ref options, client_data, (ref Epic.OnlineServices.Auth.VerifyIdTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"application_id", data.ApplicationId?.ToString()},
                {"client_id", data.ClientId?.ToString()},
                {"product_id", data.ProductId?.ToString()},
                {"deployment_id", data.DeploymentId?.ToString()},
            };
            EmitSignal(nameof(auth_interface_verify_id_token_callback), ret);
        });
    }

    public void auth_interface_link_account(Reference p_options)
    {
        var ctw = (ContinuanceTokenWrapper)p_options.Get("continuance_token");
        var options = new Epic.OnlineServices.Auth.LinkAccountOptions()
        {
            LinkAccountFlags = (LinkAccountFlags)p_options.Get("link_account_flags"),
            ContinuanceToken = ctw._internalToken
        };
        if (p_options.Get("local_user_id") != null)
            options.LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id"));
        object client_data = _get_client_data(p_options);

        s_AuthInterface.LinkAccount(ref options, client_data, (ref Epic.OnlineServices.Auth.LinkAccountCallbackInfo data) =>
        {
            Dictionary ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"selected_account_id", data.SelectedAccountId?.ToString()},
                {"pin_grant_info", null}
            };
            if (data.PinGrantInfo != null)
                ret["pin_grant_info"] = _ConvertPinGrantInfo2Dict(data.PinGrantInfo);

            EmitSignal(nameof(auth_interface_link_account_callback), ret);
        });
    }

    public void auth_interface_verify_user_auth(Reference p_options)
    {
        var p_auth_token = (Reference)p_options.Get("auth_token");
        var p_auth_type = (int)p_auth_token.Get("auth_type");
        var auth_token = new Token()
        {
            App = (string)p_auth_token.Get("app"),
            ClientId = (string)p_auth_token.Get("client_id"),
            AccountId = EpicAccountId.FromString((string)p_auth_token.Get("account_id")),
            AccessToken = (string)p_auth_token.Get("access_token"),
            ExpiresIn = Double.Parse(p_auth_token.Get("expires_in").ToString()),
            AuthType = (AuthTokenType)p_auth_type,
            RefreshToken = (string)p_auth_token.Get("refresh_token"),
            RefreshExpiresIn = Double.Parse(p_auth_token.Get("refresh_expires_in").ToString()),
            RefreshExpiresAt = (string)p_auth_token.Get("refresh_expires_at")
        };

        var options = new VerifyUserAuthOptions()
        {
            AuthToken = auth_token
        };
        object client_data = _get_client_data(p_options);

        s_AuthInterface.VerifyUserAuth(ref options, client_data, (ref VerifyUserAuthCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(auth_interface_verify_user_auth_callback), ret);
        });
    }


    // -----
    // Connect Interface
    // -----
    public void connect_interface_login(Reference p_options)
    {
        var p_credentials = (Reference)p_options.Get("credentials");

        var options = new Epic.OnlineServices.Connect.LoginOptions()
        {
            Credentials = new Epic.OnlineServices.Connect.Credentials()
            {
                Token = p_credentials.Get("token") != null ? (string)p_credentials.Get("token") : null,
                Type = (ExternalCredentialType)p_credentials.Get("type"),
            }
        };
        if (p_options.Get("user_login_info") != null)
        {
            Reference p_user_login_info = (Reference)p_options.Get("user_login_info");
            var userLoginInfo = new UserLoginInfo()
            {
                DisplayName = (string)p_user_login_info.Get("display_name"),
            };
            options.UserLoginInfo = userLoginInfo;
        }
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.Login(ref options, client_data, (ref Epic.OnlineServices.Connect.LoginCallbackInfo data) =>
        {

            Dictionary ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"continuance_token", null},
            };
            if (data.ContinuanceToken != null)
            {
                ContinuanceTokenWrapper ctw = new ContinuanceTokenWrapper(data.ContinuanceToken);
                ret["continuance_token"] = ctw;
            }
            if (data.ResultCode == Result.Success)
            {
                ret.Add("success", true);
                ret.Add("pending", false);
            }
            else if (Common.IsOperationComplete(data.ResultCode))
            {
                ret.Add("success", false);
                ret.Add("pending", false);
            }
            else
            {
                ret.Add("success", false);
                ret.Add("pending", true);
            }

            EmitSignal(nameof(connect_interface_login_callback), ret);
        });
    }

    public Dictionary connect_interface_copy_id_token(Reference p_options)
    {
        var options = new Epic.OnlineServices.Connect.CopyIdTokenOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };

        Epic.OnlineServices.Connect.IdToken? outConnectIdToken;
        Result res = s_ConnectInterface.CopyIdToken(ref options, out outConnectIdToken);
        var ret = new Dictionary(){
            {"result_code", res},
            {"id_token", null}
        };

        if (outConnectIdToken != null)
        {
            var connect_id_token_dict = new Dictionary(){
                {"product_user_id", outConnectIdToken?.ProductUserId?.ToString()},
                {"json_web_token", outConnectIdToken?.JsonWebToken?.ToString()}
            };
            ret["id_token"] = connect_id_token_dict;
        }

        return ret;
    }

    public Dictionary connect_interface_copy_product_user_external_account_by_account_id(Reference p_options)
    {
        var options = new CopyProductUserExternalAccountByAccountIdOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AccountId = (string)p_options.Get("accoun`t_id")
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo? outExternalAccountInfo;
        Result res = s_ConnectInterface.CopyProductUserExternalAccountByAccountId(ref options, out outExternalAccountInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_account_info", null}
        };
        if (outExternalAccountInfo != null)
            ret["external_account_info"] = _ConvertExternalAccountInfo2Dict(outExternalAccountInfo);
        return ret;
    }

    public Dictionary connect_interface_copy_product_user_external_account_by_account_type(Reference p_options)
    {
        var options = new CopyProductUserExternalAccountByAccountTypeOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AccountIdType = (ExternalAccountType)p_options.Get("account_id_type")
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo? outExternalAccountInfo;
        var res = s_ConnectInterface.CopyProductUserExternalAccountByAccountType(ref options, out outExternalAccountInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_account_info", null}
        };
        if (outExternalAccountInfo != null)
            ret["external_account_info"] = _ConvertExternalAccountInfo2Dict(outExternalAccountInfo);
        return ret;
    }

    public Dictionary connect_interface_copy_product_user_external_account_by_index(Reference p_options)
    {
        int p_external_account_info_index = (int)p_options.Get("external_account_info_index");
        var options = new CopyProductUserExternalAccountByIndexOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            ExternalAccountInfoIndex = (uint)p_external_account_info_index
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo? outExternalAccountInfo;
        var res = s_ConnectInterface.CopyProductUserExternalAccountByIndex(ref options, out outExternalAccountInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_account_info", null}
        };
        if (outExternalAccountInfo != null)
            ret["external_account_info"] = _ConvertExternalAccountInfo2Dict(outExternalAccountInfo);
        return ret;
    }

    public Dictionary connect_interface_copy_product_user_info(Reference p_options)
    {
        var options = new CopyProductUserInfoOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo? outExternalAccountInfo;
        var res = s_ConnectInterface.CopyProductUserInfo(ref options, out outExternalAccountInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_account_info", null}
        };
        if (outExternalAccountInfo != null)
            ret["external_account_info"] = _ConvertExternalAccountInfo2Dict(outExternalAccountInfo);
        return ret;
    }

    public void connect_interface_create_device_id(Reference p_options)
    {
        var options = new CreateDeviceIdOptions()
        {
            DeviceModel = (string)p_options.Get("device_model"),
        };
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.CreateDeviceId(ref options, client_data, (ref CreateDeviceIdCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(connect_interface_create_device_id_callback), ret);
        });
    }
    public void connect_interface_delete_device_id(Reference p_options)
    {
        DeleteDeviceIdOptions options = new DeleteDeviceIdOptions();
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.DeleteDeviceId(ref options, client_data, (ref DeleteDeviceIdCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
            };
            EmitSignal(nameof(connect_interface_delete_device_id_callback), ret);
        });
    }

    public void connect_interface_create_user(Reference p_options)
    {
        ContinuanceTokenWrapper ctw = (ContinuanceTokenWrapper)p_options.Get("continuance_token");
        var options = new Epic.OnlineServices.Connect.CreateUserOptions()
        {
            ContinuanceToken = ctw._internalToken
        };
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.CreateUser(ref options, client_data, (ref Epic.OnlineServices.Connect.CreateUserCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
            };
            EmitSignal(nameof(connect_interface_create_user_callback), ret);
        });
    }

    public string connect_interface_get_external_account_mapping(Reference p_options)
    {
        GetExternalAccountMappingsOptions options = new GetExternalAccountMappingsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            AccountIdType = (ExternalAccountType)p_options.Get("account_id_type"),
            TargetExternalUserId = (string)p_options.Get("target_external_user_id"),
        };
        ProductUserId product_user_id = s_ConnectInterface.GetExternalAccountMapping(ref options);
        if (product_user_id == null || !product_user_id.IsValid()) return "";
        return product_user_id.ToString();
    }

    public string connect_interface_get_logged_in_user_by_index(int p_index)
    {
        ProductUserId product_user_id = s_ConnectInterface.GetLoggedInUserByIndex(p_index);
        if (product_user_id == null || !product_user_id.IsValid()) return "";
        return product_user_id.ToString();
    }

    public int connect_interface_get_logged_in_users_count()
    {
        return s_ConnectInterface.GetLoggedInUsersCount();
    }

    public LoginStatus connect_interface_get_login_status(string p_local_user_id)
    {
        return s_ConnectInterface.GetLoginStatus(ProductUserId.FromString(p_local_user_id));
    }

    public int connect_interface_get_product_user_external_account_count(Reference p_options)
    {
        var options = new GetProductUserExternalAccountCountOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };
        uint count = s_ConnectInterface.GetProductUserExternalAccountCount(ref options);
        return (int)count;
    }

    public Dictionary connect_interface_get_product_user_id_mapping(Reference p_options)
    {
        var options = new GetProductUserIdMappingOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            AccountIdType = (ExternalAccountType)p_options.Get("account_id_type"),
            TargetProductUserId = ProductUserId.FromString((string)p_options.Get("target_product_user_id")),
        };
        Utf8String product_user_id = "";
        Result res = s_ConnectInterface.GetProductUserIdMapping(ref options, out product_user_id);

        Dictionary ret = new Dictionary(){
            {"result_code", res},
            {"product_user_id", product_user_id?.ToString()}
        };
        return ret;
    }

    public void connect_interface_link_account(Reference p_options)
    {
        var ctw = (ContinuanceTokenWrapper)p_options.Get("continuance_token");
        var options = new Epic.OnlineServices.Connect.LinkAccountOptions()
        {
            ContinuanceToken = ctw._internalToken
        };
        if (p_options.Get("local_user_id") != null)
            options.LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"));

        object client_data = _get_client_data(p_options);

        s_ConnectInterface.LinkAccount(ref options, client_data, (ref Epic.OnlineServices.Connect.LinkAccountCallbackInfo data) =>
        {
            Dictionary ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(connect_interface_link_account_callback), ret);
        });
    }

    public void connect_interface_verify_id_token(Reference p_options)
    {
        var id_token = (Reference)p_options.Get("id_token");
        var options = new Epic.OnlineServices.Connect.VerifyIdTokenOptions()
        {
            IdToken = new Epic.OnlineServices.Connect.IdToken()
            {
                ProductUserId = ProductUserId.FromString((string)id_token.Get("product_user_id")),
                JsonWebToken = (string)id_token.Get("json_web_token")
            }
        };
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.VerifyIdToken(ref options, client_data, (ref Epic.OnlineServices.Connect.VerifyIdTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"is_account_info_present", data.IsAccountInfoPresent},
                {"account_id_type", (int)data.AccountIdType},
                {"product_user_id", data.ProductUserId?.ToString()},
                {"account_id", data.AccountId?.ToString()},
                {"platform", data.Platform?.ToString()},
                {"device_type", data.DeviceType?.ToString()},
                {"client_id", data.ClientId?.ToString()},
                {"product_id", data.ProductId?.ToString()},
                {"sandbox_id", data.SandboxId?.ToString()},
                {"deployment_id", data.DeploymentId?.ToString()},
            };
            EmitSignal(nameof(connect_interface_verify_id_token_callback), ret);
        });

    }

    public void connect_interface_transfer_device_id_account(Reference p_options)
    {
        var options = new TransferDeviceIdAccountOptions()
        {
            PrimaryLocalUserId = ProductUserId.FromString((string)p_options.Get("primary_local_user_id")),
            LocalDeviceUserId = ProductUserId.FromString((string)p_options.Get("local_device_user_id")),
            ProductUserIdToPreserve = ProductUserId.FromString((string)p_options.Get("product_user_id_to_preserve")),
        };
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.TransferDeviceIdAccount(ref options, client_data, (ref TransferDeviceIdAccountCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
            };
            EmitSignal(nameof(connect_interface_transfer_device_id_account_callback), ret);
        });
    }

    public void connect_interface_unlink_account(Reference p_options)
    {
        var options = new UnlinkAccountOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_ConnectInterface.UnlinkAccount(ref options, client_data, (ref UnlinkAccountCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
            };
            EmitSignal(nameof(connect_interface_unlink_account_callback), ret);
        });
    }


    // -----
    // Achievements Interface
    // -----
    public Dictionary achievements_interface_copy_achievement_definition_v2_by_achievement_id(Reference p_options)
    {
        var options = new CopyAchievementDefinitionV2ByAchievementIdOptions()
        {
            AchievementId = (string)p_options.Get("achievement_id")
        };

        DefinitionV2? outDefinitionV2;
        Result res = s_AchievementsInterface.CopyAchievementDefinitionV2ByAchievementId(ref options, out outDefinitionV2);
        var ret = new Dictionary(){
            {"result_code", res},
            {"definition_v2", null}
        };
        if (outDefinitionV2 != null)
            ret["definition_v2"] = _ConvertAchievementDefinitionV22Dict(outDefinitionV2);
        return ret;
    }

    public Dictionary achievements_interface_copy_achievement_definition_v2_by_index(Reference p_options)
    {
        var p_achievement_index = (int)p_options.Get("achievement_index");
        var options = new CopyAchievementDefinitionV2ByIndexOptions()
        {
            AchievementIndex = (uint)p_achievement_index
        };

        DefinitionV2? outDefinitionV2;
        Result res = s_AchievementsInterface.CopyAchievementDefinitionV2ByIndex(ref options, out outDefinitionV2);
        var ret = new Dictionary(){
            {"result_code", res},
            {"definition_v2", null}
        };
        if (outDefinitionV2 != null)
            ret["definition_v2"] = _ConvertAchievementDefinitionV22Dict(outDefinitionV2);
        return ret;
    }

    public Dictionary achievements_interface_copy_player_achievement_by_achievement_id(Reference p_options)
    {
        var options = new CopyPlayerAchievementByAchievementIdOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AchievementId = (string)p_options.Get("achievement_id"),
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"))
        };
        PlayerAchievement? playerAchievement;
        Result res = s_AchievementsInterface.CopyPlayerAchievementByAchievementId(ref options, out playerAchievement);
        var ret = new Dictionary(){
            {"result_code", res},
            {"player_achievement", null}
        };
        if (playerAchievement != null)
            ret["player_achievement"] = _ConvertPlayerAchievement2Dict(playerAchievement);
        return ret;
    }

    public Dictionary achievements_interface_copy_player_achievement_by_index(Reference p_options)
    {
        int p_achievement_index = (int)p_options.Get("achievement_index");
        var options = new CopyPlayerAchievementByIndexOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AchievementIndex = (uint)p_achievement_index,
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"))
        };
        PlayerAchievement? playerAchievement;
        Result res = s_AchievementsInterface.CopyPlayerAchievementByIndex(ref options, out playerAchievement);
        var ret = new Dictionary(){
            {"result_code", res},
            {"player_achievement", null}
        };
        if (playerAchievement != null)
            ret["player_achievement"] = _ConvertPlayerAchievement2Dict(playerAchievement);
        return ret;
    }

    public int achievements_interface_get_achievement_definition_count(Reference p_options)
    {
        var options = new GetAchievementDefinitionCountOptions();
        return (int)s_AchievementsInterface.GetAchievementDefinitionCount(ref options);
    }

    public void achievements_interface_query_definitions(Reference p_options)
    {
        var options = new QueryDefinitionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"))
        };
        object client_data = _get_client_data(p_options);

        s_AchievementsInterface.QueryDefinitions(ref options, client_data, (ref OnQueryDefinitionsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(achievements_interface_query_definitions_complete_callback), ret);
        });
    }

    public int achievements_interface_get_player_achievement_count(Reference p_options)
    {
        var options = new GetPlayerAchievementCountOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id"))
        };
        return (int)s_AchievementsInterface.GetPlayerAchievementCount(ref options);
    }

    public void achievements_interface_query_player_achievements(Reference p_options)
    {
        var options = new QueryPlayerAchievementsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id"))
        };
        object client_data = _get_client_data(p_options);

        s_AchievementsInterface.QueryPlayerAchievements(ref options, client_data, (ref OnQueryPlayerAchievementsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(achievements_interface_query_player_achievements_complete_callback), ret);
        });
    }

    public void achievements_interface_unlock_achievements(Reference p_options)
    {
        var p_achievement_ids = ((System.Collections.IEnumerable)p_options.Get("achievement_ids")).Cast<string>().Select(x => new Utf8String(x)).ToArray();
        var options = new UnlockAchievementsOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id")),
            AchievementIds = p_achievement_ids
        };
        object client_data = _get_client_data(p_options);

        s_AchievementsInterface.UnlockAchievements(ref options, client_data, (ref OnUnlockAchievementsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"user_id", data.UserId?.ToString()},
                {"achievements_count", (int)data.AchievementsCount},
            };
            EmitSignal(nameof(achievements_interface_unlock_achievements_complete_callback), ret);
        });
    }


    // ------------------------
    // Custom Invites Interface
    // ------------------------
    public Result custom_invites_interface_finalize_invite(Reference p_options)
    {
        var options = new FinalizeInviteOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            CustomInviteId = (string)p_options.Get("custom_invite_id"),
            ProcessingResult = (Result)p_options.Get("processing_result"),
        };
        return s_CustomInvitesInterface.FinalizeInvite(ref options);
    }

    public void custom_invites_interface_send_custom_invite(Reference p_options)
    {
        var options = new SendCustomInviteOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            TargetUserIds = ((System.Collections.IEnumerable)p_options.Get("target_user_ids")).Cast<string>().Select(x => ProductUserId.FromString(x)).ToArray()
        };
        object client_data = _get_client_data(p_options);

        s_CustomInvitesInterface.SendCustomInvite(ref options, client_data, (ref SendCustomInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_user_ids", data.TargetUserIds?.Select(x => x.ToString()).ToArray()},
            };
            EmitSignal(nameof(custom_invites_interface_send_custom_invite_complete_callback), ret);
        });
    }

    public Result custom_invites_interface_set_custom_invite(Reference p_options)
    {
        var options = new SetCustomInviteOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            Payload = (string)p_options.Get("payload"),
        };

        return s_CustomInvitesInterface.SetCustomInvite(ref options);
    }


    // ------------------------
    // Stats Interface
    // ------------------------
    public Dictionary stats_interface_copy_stat_by_index(Reference p_options)
    {
        int p_stat_index = (int)p_options.Get("stat_index");
        var options = new CopyStatByIndexOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            StatIndex = (uint)p_stat_index
        };
        Stat? outStat;
        var res = s_StatsInterface.CopyStatByIndex(ref options, out outStat);
        var ret = new Dictionary()
        {
            {"result_code", res},
            {"stat", null}
        };
        if (outStat != null)
        {
            ret["stat"] = _ConvertStat2Dict(outStat);
        }
        return ret;
    }

    public Dictionary stats_interface_copy_stat_by_name(Reference p_options)
    {
        var options = new CopyStatByNameOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            Name = (string)p_options.Get("name")
        };
        Stat? outStat;
        var res = s_StatsInterface.CopyStatByName(ref options, out outStat);
        var ret = new Dictionary()
        {
            {"result_code", res},
            {"stat", null}
        };
        if (outStat != null)
        {
            ret["stat"] = _ConvertStat2Dict(outStat);
        }
        return ret;
    }

    public int stats_interface_get_stats_count(Reference p_options)
    {
        var options = new GetStatCountOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };
        uint count = s_StatsInterface.GetStatsCount(ref options);
        return (int)count;
    }

    public void stats_interface_ingest_stat(Reference p_options)
    {
        var p_stats = (Godot.Collections.Array)p_options.Get("stats");
        IngestData[] ingestData = new IngestData[p_stats.Count];
        for (int i = 0; i < p_stats.Count; i++)
        {
            var stat_dict = (Dictionary)p_stats[i];
            ingestData[i] = new IngestData()
            {
                StatName = (string)stat_dict["stat_name"],
                IngestAmount = (int)stat_dict["ingest_amount"]
            };
            i++;
        }
        var options = new IngestStatOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            Stats = ingestData,
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };

        object client_data = _get_client_data(p_options);

        s_StatsInterface.IngestStat(ref options, client_data, (ref IngestStatCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.LocalUserId != null) ret.Add("local_user_id", data.LocalUserId.ToString());
            if (data.TargetUserId != null) ret.Add("target_user_id", data.TargetUserId.ToString());
            EmitSignal(nameof(stats_interface_ingest_stat_complete_callback), ret);
        });
    }

    public void stats_interface_query_stats(Reference p_options)
    {
        var options = new QueryStatsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };
        if (p_options.Get("stat_names") != null) options.StatNames = ((System.Collections.IEnumerable)p_options.Get("stat_names"))
                                                                        .Cast<string>()
                                                                        .Select(x => new Utf8String(x))
                                                                        .ToArray();
        if (p_options.Get("start_time") != null) options.StartTime = DateTime.Parse((string)p_options.Get("start_time"));
        if (p_options.Get("end_time") != null) options.EndTime = DateTime.Parse((string)p_options.Get("end_time"));

        object client_data = _get_client_data(p_options);


        s_StatsInterface.QueryStats(ref options, client_data, (ref OnQueryStatsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.LocalUserId != null) ret.Add("local_user_id", data.LocalUserId.ToString());
            if (data.TargetUserId != null) ret.Add("target_user_id", data.TargetUserId.ToString());
            EmitSignal(nameof(stats_interface_query_stats_complete_callback), ret);
        });
    }


    // ------------------------
    // Leaderboards Interface
    // ------------------------
    public Dictionary leaderboards_interface_copy_leaderboard_definition_by_index(Reference p_options)
    {
        int p_leaderboard_index = (int)p_options.Get("leaderboard_index");
        var options = new CopyLeaderboardDefinitionByIndexOptions()
        {
            LeaderboardIndex = (uint)p_leaderboard_index
        };
        Epic.OnlineServices.Leaderboards.Definition? outLeaderboardDefinition;
        var res = s_LeaderboardsInterface.CopyLeaderboardDefinitionByIndex(ref options, out outLeaderboardDefinition);
        var ret = new Dictionary(){
            {"result_code", res},
            {"definition", null}
        };
        if (outLeaderboardDefinition != null)
            ret["definition"] = _ConvertLeaderboardDefinition2Dict(outLeaderboardDefinition);
        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_definition_by_leaderboard_id(Reference p_options)
    {
        var options = new CopyLeaderboardDefinitionByLeaderboardIdOptions()
        {
            LeaderboardId = (string)p_options.Get("leaderboard_id")
        };
        Epic.OnlineServices.Leaderboards.Definition? outLeaderboardDefinition;
        var res = s_LeaderboardsInterface.CopyLeaderboardDefinitionByLeaderboardId(ref options, out outLeaderboardDefinition);
        var ret = new Dictionary(){
            {"result_code", res},
            {"definition", null}
        };
        if (outLeaderboardDefinition != null)
            ret["definition"] = _ConvertLeaderboardDefinition2Dict(outLeaderboardDefinition);
        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_record_by_index(Reference p_options)
    {
        int p_leaderboard_record_index = (int)p_options.Get("leaderboard_record_index");
        var options = new CopyLeaderboardRecordByIndexOptions()
        {
            LeaderboardRecordIndex = (uint)p_leaderboard_record_index
        };

        LeaderboardRecord? outLeaderboardRecord;
        var res = s_LeaderboardsInterface.CopyLeaderboardRecordByIndex(ref options, out outLeaderboardRecord);
        var ret = new Dictionary(){
            {"result_code", res},
            {"leaderboard_record", null}
        };
        if (outLeaderboardRecord != null)
            ret["leaderboard_record"] = _ConvertLeaderboardRecord2Dict(outLeaderboardRecord);
        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_record_by_user_id(Reference p_options)
    {
        var options = new CopyLeaderboardRecordByUserIdOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id"))
        };

        LeaderboardRecord? outLeaderboardRecord;
        var res = s_LeaderboardsInterface.CopyLeaderboardRecordByUserId(ref options, out outLeaderboardRecord);
        var ret = new Dictionary(){
            {"result_code", res},
            {"leaderboard_record", null}
        };
        if (outLeaderboardRecord != null)
            ret["leaderboard_record"] = _ConvertLeaderboardRecord2Dict(outLeaderboardRecord);
        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_user_score_by_index(Reference p_options)
    {
        int p_user_score_index = (int)p_options.Get("leaderboard_user_score_index");
        var options = new CopyLeaderboardUserScoreByIndexOptions()
        {
            LeaderboardUserScoreIndex = (uint)p_user_score_index,
            StatName = (string)p_options.Get("stat_name")
        };
        LeaderboardUserScore? outLeaderboardUserScore;
        var res = s_LeaderboardsInterface.CopyLeaderboardUserScoreByIndex(ref options, out outLeaderboardUserScore);
        var ret = new Dictionary(){
            {"result_code", res},
            {"leaderboard_user_score", null}
        };
        if (outLeaderboardUserScore != null)
            ret["leaderboard_user_score"] = _ConvertLeaderboardUserScore2Dict(outLeaderboardUserScore);
        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_user_score_by_user_id(Reference p_options)
    {
        var options = new CopyLeaderboardUserScoreByUserIdOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id")),
            StatName = (string)p_options.Get("stat_name")
        };
        LeaderboardUserScore? outLeaderboardUserScore;
        var res = s_LeaderboardsInterface.CopyLeaderboardUserScoreByUserId(ref options, out outLeaderboardUserScore);
        var ret = new Dictionary(){
            {"result_code", res},
            {"leaderboard_user_score", null}
        };
        if (outLeaderboardUserScore != null)
            ret["leaderboard_user_score"] = _ConvertLeaderboardUserScore2Dict(outLeaderboardUserScore);
        return ret;
    }

    public int leaderboards_interface_get_leaderboard_definition_count(Reference p_options)
    {
        var options = new GetLeaderboardDefinitionCountOptions() { };
        return (int)s_LeaderboardsInterface.GetLeaderboardDefinitionCount(ref options);
    }

    public int leaderboards_interface_get_leaderboard_record_count(Reference p_options)
    {
        var options = new GetLeaderboardRecordCountOptions() { };
        return (int)s_LeaderboardsInterface.GetLeaderboardRecordCount(ref options);
    }

    public int leaderboards_interface_get_leaderboard_user_score_count(Reference p_options)
    {
        var options = new GetLeaderboardUserScoreCountOptions()
        {
            StatName = (string)p_options.Get("stat_name")
        };
        return (int)s_LeaderboardsInterface.GetLeaderboardUserScoreCount(ref options);
    }

    public void leaderboards_interface_query_leaderboard_definitions(Reference p_options)
    {
        var options = new QueryLeaderboardDefinitionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        if (p_options.Get("start_time") != null) options.StartTime = DateTime.Parse((string)p_options.Get("start_time"));
        if (p_options.Get("end_time") != null) options.EndTime = DateTime.Parse((string)p_options.Get("end_time"));
        object client_data = _get_client_data(p_options);

        s_LeaderboardsInterface.QueryLeaderboardDefinitions(ref options, client_data, (ref OnQueryLeaderboardDefinitionsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(leaderboards_interface_query_leaderboard_definitions_complete_callback), ret);
        });
    }

    public void leaderboards_interface_query_leaderboard_ranks(Reference p_options)
    {
        var options = new QueryLeaderboardRanksOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            LeaderboardId = (string)p_options.Get("leaderboard_id"),
        };
        object client_data = _get_client_data(p_options);

        s_LeaderboardsInterface.QueryLeaderboardRanks(ref options, client_data, (ref OnQueryLeaderboardRanksCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(leaderboards_interface_query_leaderboard_ranks_complete_callback), ret);
        });
    }

    public void leaderboards_interface_query_leaderboard_user_scores(Reference p_options)
    {
        var p_stat_info = (Godot.Collections.Array)p_options.Get("stat_info");
        UserScoresQueryStatInfo[] statInfo = new UserScoresQueryStatInfo[p_stat_info.Count];

        for (int i = 0; i < p_stat_info.Count; i++)
        {
            var p_stat_info_dict = (Dictionary)p_stat_info[i];
            statInfo[i] = new UserScoresQueryStatInfo()
            {
                StatName = (string)p_stat_info_dict["stat_name"],
                Aggregation = (LeaderboardAggregation)p_stat_info_dict["aggregation"],
            };
        }
        ProductUserId[] userIds = ((System.Collections.IEnumerable)p_options.Get("user_ids")).Cast<string>().Select(x => ProductUserId.FromString(new Utf8String(x))).ToArray();

        var options = new QueryLeaderboardUserScoresOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            UserIds = userIds,
            StatInfo = statInfo,
        };

        if (p_options.Get("start_time") != null) options.StartTime = DateTime.Parse((string)p_options.Get("start_time"));
        if (p_options.Get("end_time") != null) options.EndTime = DateTime.Parse((string)p_options.Get("end_time"));
        object client_data = _get_client_data(p_options);

        s_LeaderboardsInterface.QueryLeaderboardUserScores(ref options, client_data, (ref OnQueryLeaderboardUserScoresCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData}
            };
            EmitSignal(nameof(leaderboards_interface_query_leaderboard_user_scores_complete_callback), ret);
        });
    }


    // ------------------------
    // Friends Interface
    // ------------------------
    public void friends_interface_accept_invite(Reference p_options)
    {
        var options = new AcceptInviteOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_FriendsInterface.AcceptInvite(ref options, client_data, (ref AcceptInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"target_user_id", data.TargetUserId?.ToString()},
                {"local_user_id", data.LocalUserId?.ToString()},
            };
            EmitSignal(nameof(friends_interface_accept_invite_callback), ret);
        });
    }

    public string friends_interface_get_friend_at_index(Reference p_options)
    {
        var options = new GetFriendAtIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            Index = (int)p_options.Get("index")
        };
        EpicAccountId friendId = s_FriendsInterface.GetFriendAtIndex(ref options);
        if (friendId == null || !friendId.IsValid()) return "";
        return friendId.ToString();
    }

    public int friends_interface_get_friends_count(Reference p_options)
    {
        var options = new GetFriendsCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return s_FriendsInterface.GetFriendsCount(ref options);
    }

    public FriendsStatus friends_interface_get_status(Reference p_options)
    {
        var options = new GetStatusOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        return s_FriendsInterface.GetStatus(ref options);
    }

    public void friends_interface_query_friends(Reference p_options)
    {
        var options = new QueryFriendsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_FriendsInterface.QueryFriends(ref options, client_data, (ref QueryFriendsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(friends_interface_query_friends_callback), ret);
        });
    }

    public void friends_interface_reject_invite(Reference p_options)
    {
        var options = new RejectInviteOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_FriendsInterface.RejectInvite(ref options, client_data, (ref RejectInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_user_id", data.TargetUserId?.ToString()},
            };
            EmitSignal(nameof(friends_interface_reject_invite_callback), ret);
        });
    }

    public void friends_interface_send_invite(Reference p_options)
    {
        var options = new SendInviteOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_FriendsInterface.SendInvite(ref options, client_data, (ref SendInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_user_id", data.TargetUserId?.ToString()},
            };
            EmitSignal(nameof(friends_interface_send_invite_callback), ret);
        });
    }


    // ------------------------
    // User Info Interface
    // ------------------------
    public Dictionary user_info_interface_copy_external_user_info_by_account_id(Reference p_options)
    {
        var options = new CopyExternalUserInfoByAccountIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
            AccountId = (string)p_options.Get("account_id"),
        };
        ExternalUserInfo? outExternalUserInfo;
        Result res = s_UserInfoInterface.CopyExternalUserInfoByAccountId(ref options, out outExternalUserInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_user_info", null}
        };
        if (outExternalUserInfo != null)
            ret["external_user_info"] = _ConvertExternalUserInfo2Dict(outExternalUserInfo.Value);
        return ret;
    }

    public Dictionary user_info_interface_copy_external_user_info_by_account_type(Reference p_options)
    {
        var options = new CopyExternalUserInfoByAccountTypeOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
            AccountType = (ExternalAccountType)p_options.Get("account_type")
        };
        ExternalUserInfo? outExternalUserInfo;
        Result res = s_UserInfoInterface.CopyExternalUserInfoByAccountType(ref options, out outExternalUserInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_user_info", null}
        };
        if (outExternalUserInfo != null)
            ret["external_user_info"] = _ConvertExternalUserInfo2Dict(outExternalUserInfo.Value);
        return ret;
    }

    public Dictionary user_info_interface_copy_external_user_info_by_index(Reference p_options)
    {
        int p_index = (int)p_options.Get("index");
        var options = new CopyExternalUserInfoByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
            Index = (uint)p_index
        };
        ExternalUserInfo? outExternalUserInfo;
        Result res = s_UserInfoInterface.CopyExternalUserInfoByIndex(ref options, out outExternalUserInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"external_user_info", null}
        };
        if (outExternalUserInfo != null)
            ret["external_user_info"] = _ConvertExternalUserInfo2Dict(outExternalUserInfo.Value);
        return ret;
    }

    public Dictionary user_info_interface_copy_user_info(Reference p_options)
    {
        var options = new CopyUserInfoOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        UserInfoData? outUserInfoData;
        Result res = s_UserInfoInterface.CopyUserInfo(ref options, out outUserInfoData);
        var ret = new Dictionary(){
            {"result_code", res},
            {"user_info", null}
        };
        if (outUserInfoData != null)
        {
            ret["user_info"] = _ConvertUserInfoData2Dict(outUserInfoData);
        }
        return ret;
    }

    public int user_info_interface_get_external_user_info_count(Reference p_options)
    {
        var options = new GetExternalUserInfoCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        uint count = s_UserInfoInterface.GetExternalUserInfoCount(ref options);
        return (int)count;
    }

    public void user_info_interface_query_user_info(Reference p_options)
    {
        var options = new QueryUserInfoOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_UserInfoInterface.QueryUserInfo(ref options, client_data, (ref QueryUserInfoCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_user_id", data.TargetUserId?.ToString()},
            };
            EmitSignal(nameof(user_info_interface_query_user_info_callback), ret);
        });
    }

    public void user_info_interface_query_user_info_by_display_name(Reference p_options)
    {
        var options = new QueryUserInfoByDisplayNameOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            DisplayName = (string)p_options.Get("display_name"),
        };
        object client_data = _get_client_data(p_options);

        s_UserInfoInterface.QueryUserInfoByDisplayName(ref options, client_data, (ref QueryUserInfoByDisplayNameCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_user_id", data.TargetUserId?.ToString()},
                {"display_name", data.DisplayName != null ? String.Copy(data.DisplayName.ToString()) : null},
            };
            EmitSignal(nameof(user_info_interface_query_user_info_by_display_name_callback), ret);
        });
    }

    public void user_info_interface_query_user_info_by_external_account(Reference p_options)
    {
        var options = new QueryUserInfoByExternalAccountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ExternalAccountId = (string)p_options.Get("external_account_id"),
            AccountType = (ExternalAccountType)p_options.Get("account_type"),
        };
        object client_data = _get_client_data(p_options);

        s_UserInfoInterface.QueryUserInfoByExternalAccount(ref options, client_data, (ref QueryUserInfoByExternalAccountCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"target_user_id", data.TargetUserId?.ToString()},
                {"external_account_id", data.ExternalAccountId?.ToString()},
                {"account_type", data.AccountType},
            };
            EmitSignal(nameof(user_info_interface_query_user_info_by_external_account_callback), ret);
        });
    }


    // ------------------------
    // Ecom Interface
    // ------------------------
    public void ecom_interface_checkout(Reference p_options)
    {
        CheckoutEntry[] p_entries = ((System.Collections.IEnumerable)p_options.Get("entries")).Cast<Dictionary>().Select(
            x => new CheckoutEntry()
            {
                OfferId = (string)x["offer_id"]
            }
        ).ToArray();
        var options = new CheckoutOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            Entries = p_entries
        };
        if (p_options.Get("override_catalog_namespace") != null) options.OverrideCatalogNamespace = (string)p_options.Get("override_catalog_namespace");
        object client_data = _get_client_data(p_options);

        s_EcomInterface.Checkout(ref options, client_data, (ref CheckoutCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"transaction_id", data.TransactionId?.ToString()},
            };
            EmitSignal(nameof(ecom_interface_checkout_callback), ret);
        });
    }

    public Dictionary ecom_interface_copy_entitlement_by_id(Reference p_options)
    {
        var options = new CopyEntitlementByIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementId = (string)p_options.Get("entitlement_id"),
        };
        Entitlement? outEntitlement;
        Result res = s_EcomInterface.CopyEntitlementById(ref options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res},
            {"entitlement", null}
        };
        if (outEntitlement != null)
            ret["entitlement"] = _ConvertEntitlement2Dict(outEntitlement);
        return ret;
    }

    public Dictionary ecom_interface_copy_entitlement_by_index(Reference p_options)
    {
        int p_entitlement_index = (int)p_options.Get("entitlement_index");
        var options = new CopyEntitlementByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementIndex = (uint)p_entitlement_index
        };
        Entitlement? outEntitlement;
        Result res = s_EcomInterface.CopyEntitlementByIndex(ref options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res},
            {"entitlement", null}
        };
        if (outEntitlement != null)
            ret["entitlement"] = _ConvertEntitlement2Dict(outEntitlement);
        return ret;
    }

    public Dictionary ecom_interface_copy_entitlement_by_name_and_index(Reference p_options)
    {
        int p_index = (int)p_options.Get("index");
        var options = new CopyEntitlementByNameAndIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementName = (string)p_options.Get("entitlement_name"),
            Index = (uint)p_index
        };
        Entitlement? outEntitlement;
        Result res = s_EcomInterface.CopyEntitlementByNameAndIndex(ref options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res},
            {"entitlement", null}
        };
        if (outEntitlement != null)
            ret["entitlement"] = _ConvertEntitlement2Dict(outEntitlement);
        return ret;
    }

    public Dictionary ecom_interface_copy_item_by_id(Reference p_options)
    {
        var options = new CopyItemByIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id")
        };
        CatalogItem? outItem;
        Result res = s_EcomInterface.CopyItemById(ref options, out outItem);
        var ret = new Dictionary(){
            {"result_code", res},
            {"item", null}
        };
        if (outItem != null)
            ret["item"] = _ConvertCatalogItem2Dict(outItem);
        return ret;
    }

    public Dictionary ecom_interface_copy_item_image_info_by_index(Reference p_options)
    {
        int p_image_info_index = (int)p_options.Get("image_info_index");
        var options = new CopyItemImageInfoByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id"),
            ImageInfoIndex = (uint)p_image_info_index
        };
        KeyImageInfo? outImageInfo;
        Result res = s_EcomInterface.CopyItemImageInfoByIndex(ref options, out outImageInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"image_info", null}
        };
        if (outImageInfo != null)
            ret["image_info"] = _ConvertKeyImageInfo2Dict(outImageInfo);
        return ret;
    }

    public Dictionary ecom_interface_copy_item_release_by_index(Reference p_options)
    {
        int p_release_index = (int)p_options.Get("release_index");
        var options = new CopyItemReleaseByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id"),
            ReleaseIndex = (uint)p_release_index
        };
        CatalogRelease? outRelease;
        Result res = s_EcomInterface.CopyItemReleaseByIndex(ref options, out outRelease);
        var ret = new Dictionary(){
            {"result_code", res},
            {"release", null}
        };
        if (outRelease != null)
            ret["release"] = _ConvertCatalogRelease2Dict(outRelease);
        return ret;
    }

    public Dictionary ecom_interface_copy_offer_by_id(Reference p_options)
    {
        var options = new CopyOfferByIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
        };
        CatalogOffer? outOffer;
        Result res = s_EcomInterface.CopyOfferById(ref options, out outOffer);
        var ret = new Dictionary(){
            {"result_code", res},
            {"offer", null}
        };
        if (outOffer != null)
            ret["offer"] = _ConvertCatalogOffer2Dict(outOffer);
        return ret;
    }

    public Dictionary ecom_interface_copy_offer_by_index(Reference p_options)
    {
        int p_offer_index = (int)p_options.Get("offer_index");
        var options = new CopyOfferByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferIndex = (uint)p_offer_index
        };
        CatalogOffer? outOffer;
        Result res = s_EcomInterface.CopyOfferByIndex(ref options, out outOffer);
        var ret = new Dictionary(){
            {"result_code", res},
            {"offer", null}
        };
        if (outOffer != null)
            ret["offer"] = _ConvertCatalogOffer2Dict(outOffer);
        return ret;
    }

    public Dictionary ecom_interface_copy_offer_image_info_by_index(Reference p_options)
    {
        int p_image_info_index = (int)p_options.Get("image_info_index");
        var options = new CopyOfferImageInfoByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
            ImageInfoIndex = (uint)p_image_info_index
        };
        KeyImageInfo? outImageInfo;
        Result res = s_EcomInterface.CopyOfferImageInfoByIndex(ref options, out outImageInfo);
        var ret = new Dictionary(){
            {"result_code", res},
            {"image_info", null}
        };
        if (outImageInfo != null)
            ret["image_info"] = _ConvertKeyImageInfo2Dict(outImageInfo);
        return ret;
    }

    public Dictionary ecom_interface_copy_offer_item_by_index(Reference p_options)
    {
        int p_item_index = (int)p_options.Get("item_index");
        var options = new CopyOfferItemByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
            ItemIndex = (uint)p_item_index
        };
        CatalogItem? outItem;
        Result res = s_EcomInterface.CopyOfferItemByIndex(ref options, out outItem);
        var ret = new Dictionary(){
            {"result_code", res},
            {"item", null}
        };
        if (outItem != null)
            ret["item"] = _ConvertCatalogItem2Dict(outItem);
        return ret;
    }

    public Dictionary ecom_interface_copy_transaction_by_id(Reference p_options)
    {
        var options = new CopyTransactionByIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TransactionId = (string)p_options.Get("transaction_id"),
        };
        Transaction outTransaction;
        Result res = s_EcomInterface.CopyTransactionById(ref options, out outTransaction);
        var ret = new Dictionary(){
            {"result_code", res},
            {"transaction", null}
        };
        if (outTransaction != null)
        {
            var transaction_wrapper = new TransactionWrapper(outTransaction);
            ret["transaction"] = transaction_wrapper;
        }
        return ret;
    }

    public Dictionary ecom_interface_copy_transaction_by_index(Reference p_options)
    {
        int p_transaction_index = (int)p_options.Get("transaction_index");
        var options = new CopyTransactionByIndexOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TransactionIndex = (uint)p_transaction_index
        };
        Transaction outTransaction;
        Result res = s_EcomInterface.CopyTransactionByIndex(ref options, out outTransaction);
        var ret = new Dictionary(){
            {"result_code", res},
            {"transaction", null}
        };
        if (outTransaction != null)
        {
            var transaction_wrapper = new TransactionWrapper(outTransaction);
            ret["transaction"] = transaction_wrapper;
        }
        return ret;
    }

    public int ecom_interface_get_entitlements_by_name_count(Reference p_options)
    {
        var options = new GetEntitlementsByNameCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementName = (string)p_options.Get("entitlement_name")
        };
        return (int)s_EcomInterface.GetEntitlementsByNameCount(ref options);
    }

    public int ecom_interface_get_entitlements_count(Reference p_options)
    {
        var options = new GetEntitlementsCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return (int)s_EcomInterface.GetEntitlementsCount(ref options);
    }

    public int ecom_interface_get_item_image_info_count(Reference p_options)
    {
        var options = new GetItemImageInfoCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id"),
        };
        return (int)s_EcomInterface.GetItemImageInfoCount(ref options);
    }

    public int ecom_interface_get_item_release_count(Reference p_options)
    {
        var options = new GetItemReleaseCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id"),
        };
        return (int)s_EcomInterface.GetItemReleaseCount(ref options);
    }

    public int ecom_interface_get_offer_count(Reference p_options)
    {
        var options = new GetOfferCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return (int)s_EcomInterface.GetOfferCount(ref options);
    }

    public int ecom_interface_get_offer_image_info_count(Reference p_options)
    {
        var options = new GetOfferImageInfoCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
        };
        return (int)s_EcomInterface.GetOfferImageInfoCount(ref options);
    }

    public int ecom_interface_get_offer_item_count(Reference p_options)
    {
        var options = new GetOfferItemCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
        };
        return (int)s_EcomInterface.GetOfferItemCount(ref options);
    }

    public int ecom_interface_get_transaction_count(Reference p_options)
    {
        var options = new GetTransactionCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return (int)s_EcomInterface.GetTransactionCount(ref options);
    }

    public void ecom_interface_query_entitlements(Reference p_options)
    {
        var options = new QueryEntitlementsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementNames = ((System.Collections.IEnumerable)p_options.Get("entitlement_names")).Cast<string>().Select(x => new Utf8String(x)).ToArray(),
            IncludeRedeemed = (bool)p_options.Get("include_redeemed"),
        };
        object client_data = _get_client_data(p_options);


        s_EcomInterface.QueryEntitlements(ref options, client_data, (ref QueryEntitlementsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(ecom_interface_query_entitlements_callback), ret);
        });
    }

    public void ecom_interface_query_offers(Reference p_options)
    {
        var options = new QueryOffersOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        if (p_options.Get("override_catalog_namespace") != null) options.OverrideCatalogNamespace = (string)p_options.Get("override_catalog_namespace");
        object client_data = _get_client_data(p_options);

        s_EcomInterface.QueryOffers(ref options, client_data, (ref QueryOffersCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(ecom_interface_query_offers_callback), ret);
        });
    }

    public void ecom_interface_query_ownership(Reference p_options)
    {
        var options = new QueryOwnershipOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            CatalogItemIds = ((System.Collections.IEnumerable)p_options.Get("catalog_item_ids")).Cast<string>().Select(x => new Utf8String(x)).ToArray(),
        };
        if (p_options.Get("catalog_namespace") != null) options.CatalogNamespace = (string)p_options.Get("catalog_namespace");
        object client_data = _get_client_data(p_options);

        s_EcomInterface.QueryOwnership(ref options, client_data, (ref QueryOwnershipCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"item_ownership",
                    data.ItemOwnership?.Select(x => new Dictionary(){
                        {"id", x.Id?.ToString()},
                        {"ownership_status", x.OwnershipStatus},
                    }).ToArray()
                }
            };
            EmitSignal(nameof(ecom_interface_query_ownership_callback), ret);
        });
    }

    public void ecom_interface_query_ownership_token(Reference p_options)
    {
        var options = new QueryOwnershipTokenOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            CatalogItemIds = ((System.Collections.IEnumerable)p_options.Get("catalog_item_ids")).Cast<string>().Select(x => new Utf8String(x)).ToArray(),
        };
        if (p_options.Get("catalog_namespace") != null) options.CatalogNamespace = (string)p_options.Get("catalog_namespace");
        object client_data = _get_client_data(p_options);

        s_EcomInterface.QueryOwnershipToken(ref options, client_data, (ref QueryOwnershipTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"ownership_token", data.OwnershipToken?.ToString()},
            };
            EmitSignal(nameof(ecom_interface_query_ownership_token_callback), ret);
        });
    }

    public void ecom_interface_redeem_entitlements(Reference p_options)
    {
        var options = new RedeemEntitlementsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementIds = ((System.Collections.IEnumerable)p_options.Get("entitlement_ids")).Cast<string>().Select(x => new Utf8String(x)).ToArray(),
        };
        object client_data = _get_client_data(p_options);

        s_EcomInterface.RedeemEntitlements(ref options, client_data, (ref RedeemEntitlementsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
            };
            EmitSignal(nameof(ecom_interface_redeem_entitlements_callback), ret);
        });
    }


    // ------------------------
    // UI Interface
    // ------------------------
    public Result ui_interface_acknowledge_event_id(Reference p_options)
    {
        int p_event_id = (int)p_options.Get("ui_event_id");
        int p_result = (int)p_options.Get("result");
        var options = new AcknowledgeEventIdOptions()
        {
            UiEventId = (ulong)p_event_id,
            Result = (Result)p_result
        };
        return s_UIInterface.AcknowledgeEventId(ref options);
    }

    public bool ui_interface_get_friends_visible(Reference p_options)
    {
        var options = new GetFriendsVisibleOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return s_UIInterface.GetFriendsVisible(ref options);
    }

    public NotificationLocation ui_interface_get_notification_location_preference()
    {
        return s_UIInterface.GetNotificationLocationPreference();
    }

    public KeyCombination ui_interface_get_toggle_friends_key(Reference p_options)
    {
        var options = new GetToggleFriendsKeyOptions();
        return s_UIInterface.GetToggleFriendsKey(ref options);
    }

    public void ui_interface_hide_friends(Reference p_options)
    {
        var options = new HideFriendsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_UIInterface.HideFriends(ref options, client_data, (ref HideFriendsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(ui_interface_hide_friends_callback), ret);
        });
    }

    public bool ui_interface_is_valid_key_combination(int p_key_combination)
    {
        return s_UIInterface.IsValidKeyCombination(
            (KeyCombination)p_key_combination
        );
    }

    public Result ui_interface_set_display_preference(Reference p_options)
    {
        int p_notification_location = (int)p_options.Get("notification_location");
        var options = new SetDisplayPreferenceOptions()
        {
            NotificationLocation = (NotificationLocation)p_notification_location
        };
        return s_UIInterface.SetDisplayPreference(ref options);
    }

    public Result ui_interface_set_toggle_friends_key(Reference p_options)
    {
        var options = new SetToggleFriendsKeyOptions()
        {
            KeyCombination = (KeyCombination)p_options.Get("key_combination")
        };
        return s_UIInterface.SetToggleFriendsKey(ref options);
    }

    public void ui_interface_show_friends(Reference p_options)
    {
        var options = new ShowFriendsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_UIInterface.ShowFriends(ref options, client_data, (ref ShowFriendsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(ui_interface_show_friends_callback), ret);
        });
    }


    // ------------------------
    // KWS Interface
    // ------------------------
    public Dictionary kws_interface_copy_permission_by_index(Reference p_options)
    {
        int p_index = (int)p_options.Get("index");

        var options = new CopyPermissionByIndexOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            Index = (uint)p_index
        };
        PermissionStatus? outPermissionStatus;
        var res = s_KWSInterface.CopyPermissionByIndex(ref options, out outPermissionStatus);
        var ret = new Dictionary(){
            {"result_code", res},
            {"permission", null}
        };
        if (outPermissionStatus != null)
        {
            var permission_dict = new Dictionary(){
                {"name", outPermissionStatus?.Name?.ToString()},
                {"status", outPermissionStatus?.Status}
            };
            ret["permission"] = permission_dict;
        }
        return ret;
    }

    public void kws_interface_create_user(Reference p_options)
    {
        var options = new Epic.OnlineServices.KWS.CreateUserOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            DateOfBirth = (string)p_options.Get("date_of_birth"),
            ParentEmail = (string)p_options.Get("parent_email"),
        };
        object client_data = _get_client_data(p_options);

        s_KWSInterface.CreateUser(ref options, client_data, (ref Epic.OnlineServices.KWS.CreateUserCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"kws_user_id", data.KWSUserId?.ToString()},
                {"is_minor", data.IsMinor},
            };
            EmitSignal(nameof(kws_interface_create_user_callback), ret);
        });
    }

    public Dictionary kws_interface_get_permission_by_key(Reference p_options)
    {
        var options = new GetPermissionByKeyOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            Key = (string)p_options.Get("key")
        };
        KWSPermissionStatus outPermissionStatus;
        var res = s_KWSInterface.GetPermissionByKey(ref options, out outPermissionStatus);
        var ret = new Dictionary(){
            {"result_code", res},
            {"permission", outPermissionStatus}
        };
        return ret;
    }

    public int kws_interface_get_permissions_count(Reference p_options)
    {
        var options = new GetPermissionsCountOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        return s_KWSInterface.GetPermissionsCount(ref options);
    }

    public void kws_interface_query_age_gate(Reference p_options)
    {
        var options = new QueryAgeGateOptions() { };
        object client_data = _get_client_data(p_options);

        s_KWSInterface.QueryAgeGate(ref options, client_data, (ref QueryAgeGateCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"country_code", data.CountryCode?.ToString()},
                {"age_of_consent", (int)data.AgeOfConsent},
            };
            EmitSignal(nameof(kws_interface_query_age_gate_callback), ret);
        });
    }

    public void kws_interface_query_permissions(Reference p_options)
    {
        var options = new QueryPermissionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_KWSInterface.QueryPermissions(ref options, client_data, (ref QueryPermissionsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"kws_user_id", data.KWSUserId?.ToString()},
                {"date_of_birth", data.DateOfBirth?.ToString()},
                {"is_minor", data.IsMinor},
            };
            EmitSignal(nameof(kws_interface_query_permissions_callback), ret);
        });
    }

    public void kws_interface_request_permissions(Reference p_options)
    {
        var options = new RequestPermissionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            PermissionKeys = ((System.Collections.IEnumerable)p_options.Get("permission_keys")).Cast<string>().Select(x => new Utf8String(x)).ToArray(),
        };
        object client_data = _get_client_data(p_options);

        s_KWSInterface.RequestPermissions(ref options, client_data, (ref RequestPermissionsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()}
            };
            EmitSignal(nameof(kws_interface_request_permissions_callback), ret);
        });
    }

    public void kws_interface_update_parent_email(Reference p_options)
    {
        var options = new UpdateParentEmailOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            ParentEmail = (string)p_options.Get("parent_email"),
        };
        object client_data = _get_client_data(p_options);

        s_KWSInterface.UpdateParentEmail(ref options, client_data, (ref UpdateParentEmailCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
            };
            EmitSignal(nameof(kws_interface_update_parent_email_callback), ret);
        });
    }


    // ------------------------
    // Metrics Interface
    // ------------------------
    public Result metrics_interface_begin_player_session(Reference p_options)
    {
        int p_account_id_type = (int)p_options.Get("account_id_type");
        string p_account_id = (string)p_options.Get("account_id");

        MetricsAccountIdType account_id_type = (MetricsAccountIdType)p_account_id_type;
        BeginPlayerSessionOptionsAccountId account_id;
        if (account_id_type == MetricsAccountIdType.Epic)
        {
            account_id = new BeginPlayerSessionOptionsAccountId()
            {
                Epic = EpicAccountId.FromString(p_account_id)
            };
        }
        else
        {
            account_id = new BeginPlayerSessionOptionsAccountId()
            {
                External = p_account_id
            };
        }

        int p_controller_type = (int)p_options.Get("controller_type");
        var options = new BeginPlayerSessionOptions()
        {
            AccountId = account_id,
            DisplayName = (string)p_options.Get("display_name"),
            ControllerType = (UserControllerType)p_controller_type
        };
        if (p_options.Get("server_ip") != null) options.ServerIp = (string)p_options.Get("server_ip");
        else options.ServerIp = null;
        if (p_options.Get("game_session_id") != null) options.GameSessionId = (string)p_options.Get("game_session_id");

        return s_MetricsInterface.BeginPlayerSession(ref options);
    }
    public Result metrics_interface_end_player_session(Reference p_options)
    {
        int p_account_id_type = (int)p_options.Get("account_id_type");
        string p_account_id = (string)p_options.Get("account_id");

        MetricsAccountIdType account_id_type = (MetricsAccountIdType)p_account_id_type;
        EndPlayerSessionOptionsAccountId account_id;
        if (account_id_type == MetricsAccountIdType.Epic)
        {
            account_id = new EndPlayerSessionOptionsAccountId()
            {
                Epic = EpicAccountId.FromString(p_account_id)
            };
        }
        else
        {
            account_id = new EndPlayerSessionOptionsAccountId()
            {
                External = p_account_id
            };
        }

        var options = new EndPlayerSessionOptions()
        {
            AccountId = account_id
        };
        return s_MetricsInterface.EndPlayerSession(ref options);
    }

    // ------------------------
    // Version Interface
    // ------------------------
    public string version_interface_get_version()
    {
        return VersionInterface.GetVersion()?.ToString();
    }

    public Dictionary version_interface_get_constants()
    {
        return new Dictionary(){
            {"company_name", VersionInterface.CompanyName.ToString()},
            {"copyright_string", VersionInterface.CopyrightString.ToString()},
            {"product_identifier", VersionInterface.ProductIdentifier.ToString()},
            {"product_name", VersionInterface.ProductName.ToString()},
            {"major_version", VersionInterface.MajorVersion},
            {"minor_version", VersionInterface.MinorVersion},
            {"patch_version", VersionInterface.PatchVersion},
        };
    }


    // ------------------------
    // Mods Interface
    // ------------------------
    public Dictionary mods_interface_copy_mod_info(Reference p_options)
    {
        var options = new CopyModInfoOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            Type = (ModEnumerationType)p_options.Get("type"),
        };
        ModInfo? outMod;
        var res = s_ModsInterface.CopyModInfo(ref options, out outMod);
        var ret = new Dictionary(){
            {"result_code", res},
            {"mod", null}
        };
        if (outMod != null)
            ret["mod"] = new Dictionary(){
                {"mods", outMod?.Mods?.Select(x=> new Dictionary(){
                    {"namespace_id", x.NamespaceId?.ToString()},
                    {"item_id", x.ItemId?.ToString()},
                    {"artifact_id", x.ArtifactId?.ToString()},
                    {"title", x.Title?.ToString()},
                    {"version", x.Version?.ToString()},
                })},
                {"type", outMod?.Type},
            };
        return ret;
    }

    public void mods_interface_enumerate_mods(Reference p_options)
    {
        var options = new EnumerateModsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            Type = (ModEnumerationType)p_options.Get("type"),
        };
        object client_data = _get_client_data(p_options);
        s_ModsInterface.EnumerateMods(ref options, client_data, (ref EnumerateModsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
                {"local_user_id", data.LocalUserId?.ToString()},
                {"type", data.Type},
            };
            EmitSignal(nameof(mods_interface_enumerate_mods_callback), ret);
        });
    }


    // ------------------------
    // Reports Interface
    // ------------------------
    public void reports_interface_send_player_behavior_report(Reference p_options)
    {
        var options = new SendPlayerBehaviorReportOptions()
        {
            ReporterUserId = ProductUserId.FromString((string)p_options.Get("reporter_user_id")),
            ReportedUserId = ProductUserId.FromString((string)p_options.Get("reported_user_id")),
            Category = (PlayerReportsCategory)p_options.Get("category"),
            Message = new Utf8String((string)p_options.Get("message")),
            Context = new Utf8String((string)p_options.Get("context")),
        };
        object client_data = _get_client_data(p_options);

        s_ReportsInterface.SendPlayerBehaviorReport(ref options, client_data, (ref SendPlayerBehaviorReportCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
            };
            EmitSignal(nameof(reports_interface_report_callback), ret);
        });
    }


    // ------------------------
    // Progression Snapshot Interface
    // ------------------------
    public Dictionary progression_snapshot_interface_begin_snapshot(Reference p_options)
    {
        var options = new BeginSnapshotOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"))
        };
        uint outSnapshotId;
        Result res = s_ProgressionSnapshotInterface.BeginSnapshot(ref options, out outSnapshotId);
        return new Dictionary(){
            {"result_code", res},
            {"snapshot_id", outSnapshotId}
        };
    }

    public Result progression_snapshot_interface_add_progression(Reference p_options)
    {
        var p_snapshot_id = (int)p_options.Get("snapshot_id");
        var options = new AddProgressionOptions()
        {
            SnapshotId = (uint)p_snapshot_id,
            Key = new Utf8String((string)p_options.Get("key")),
            Value = new Utf8String((string)p_options.Get("value")),

        };
        return s_ProgressionSnapshotInterface.AddProgression(ref options);
    }

    public void progression_snapshot_interface_submit_snapshot(Reference p_options)
    {
        var p_snapshot_id = (int)p_options.Get("snapshot_id");
        var options = new SubmitSnapshotOptions()
        {
            SnapshotId = (uint)p_snapshot_id,

        };

        object client_data = _get_client_data(p_options);
        s_ProgressionSnapshotInterface.SubmitSnapshot(ref options, client_data, (ref SubmitSnapshotCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"snapshot_id", data.SnapshotId},
                {"client_data", data.ClientData}
            };

            // Clear internal memory resources allocated by the SDK
            var endSnapshotOptions = new EndSnapshotOptions()
            {
                SnapshotId = data.SnapshotId
            };
            var _res = s_ProgressionSnapshotInterface.EndSnapshot(ref endSnapshotOptions);
            EmitSignal(nameof(progression_snapshot_interface_submit_snapshot_callback), ret);
        });
    }

    public void progression_snapshot_interface_delete_snapshot(Reference p_options)
    {
        var options = new DeleteSnapshotOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"))
        };

        object client_data = _get_client_data(p_options);
        s_ProgressionSnapshotInterface.DeleteSnapshot(ref options, client_data, (ref DeleteSnapshotCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
            };
            if (data.LocalUserId != null)
            {
                ret["local_user_id"] = data.LocalUserId.ToString();
            }
            EmitSignal(nameof(progression_snapshot_interface_delete_snapshot_callback), ret);
        });
    }


    // ------------------------
    // Presence Interface
    // ------------------------
    public Dictionary presence_interface_copy_presence(Reference p_options)
    {
        Info? outPresence;
        var options = new CopyPresenceOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id"))
        };
        Result res = s_PresenceInterface.CopyPresence(ref options, out outPresence);
        if (res == Result.Success)
        {
            Godot.Collections.Array<Dictionary> records = new Godot.Collections.Array<Dictionary>();
            if (outPresence?.Records != null && outPresence?.Records.Length > 0)
            {
                foreach (var record in outPresence?.Records)
                {
                    records.Add(new Dictionary(){
                    {"key", record.Key.ToString()},
                    {"value", record.Value.ToString()}
                });
                    GD.Print(record.Key.ToString());
                    GD.Print(record.Value.ToString());
                }
                GD.Print(records);
            }
            var ret = new Dictionary(){
                {"result_code", res},
                {"status", (int)outPresence?.Status},
                {"user_id", outPresence?.UserId.ToString()},
                {"product_id", outPresence?.ProductId.ToString()},
                {"product_version", outPresence?.ProductVersion.ToString()},
                {"platform", outPresence?.Platform.ToString()},
                {"rich_text", outPresence?.RichText.ToString()},
                {"product_name", outPresence?.ProductName.ToString()},
                {"integrated_platform", outPresence?.IntegratedPlatform.ToString()},
                {"records", records}
            };
            return ret;
        }
        return new Dictionary() { { "result_code", res } };
    }

    public Dictionary presence_interface_create_presence_modification(Reference p_options)
    {
        PresenceModification outPresenceModification;
        var options = new CreatePresenceModificationOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        Result res = s_PresenceInterface.CreatePresenceModification(ref options, out outPresenceModification);

        if (res == Result.Success)
        {
            var presenceModificationWrapper = new PresenceModificationWrapper(outPresenceModification);

            var ret = new Dictionary(){
                {"result_code", res},
                {"presence_modification", presenceModificationWrapper},
            };
            return ret;
        }
        return new Dictionary() { { "result_code", res } };
    }

    public Dictionary presence_interface_get_join_info(Reference p_options)
    {
        var options = new GetJoinInfoOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id"))
        };
        Result res = s_PresenceInterface.GetJoinInfo(ref options, out var outJoinInfo);

        if (res == Result.Success)
        {
            var ret = new Dictionary(){
                {"result_code", res},
                {"join_info", outJoinInfo.ToString()}
            };
            return ret;
        }
        return new Dictionary() { { "result_code", res } };
    }

    public bool presence_interface_has_presence(Reference p_options)
    {
        var options = new HasPresenceOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id"))
        };
        return s_PresenceInterface.HasPresence(ref options);
    }

    public void presence_interface_query_presence(Reference p_options)
    {
        var options = new QueryPresenceOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        object client_data = _get_client_data(p_options);

        s_PresenceInterface.QueryPresence(ref options, client_data, (ref QueryPresenceCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
            };
            if (data.LocalUserId != null)
            {
                ret["local_user_id"] = data.LocalUserId.ToString();
            }
            if (data.TargetUserId != null)
            {
                ret["target_user_id"] = data.TargetUserId.ToString();
            }
            EmitSignal(nameof(presence_interface_query_presence_callback), ret);
        });
    }

    public void presence_interface_set_presence(Reference p_options)
    {
        var options = new SetPresenceOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            PresenceModificationHandle = ((PresenceModificationWrapper)p_options.Get("presence_modification"))._internalPresenceModification,
        };
        object client_data = _get_client_data(p_options);

        s_PresenceInterface.SetPresence(ref options, client_data, (ref SetPresenceCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"client_data", data.ClientData},
            };
            if (data.LocalUserId != null)
            {
                ret["local_user_id"] = data.LocalUserId.ToString();
            }
            EmitSignal(nameof(presence_interface_set_presence_callback), ret);
        });
    }


    // ------------------------
    // Internal Overrides
    // ------------------------
    public override void _Process(float delta)
    {

        if (s_PlatformInterface != null)
        {
            s_PlatformInterface.Tick();
        }
    }
    public override void _Notification(int what)
    {
        base._Notification(what);
        if (what == NotificationWmQuitRequest)
        {
            if (s_PlatformInterface != null)
            {
                s_PlatformInterface.Release();
                s_AuthInterface = null;
                s_ConnectInterface = null;
                s_AchievementsInterface = null;
                s_CustomInvitesInterface = null;
                s_StatsInterface = null;
                s_LeaderboardsInterface = null;
                s_FriendsInterface = null;
                s_UserInfoInterface = null;
                s_EcomInterface = null;
                s_UIInterface = null;
                s_MetricsInterface = null;
                s_ModsInterface = null;
                s_ReportsInterface = null;
                s_ProgressionSnapshotInterface = null;
                s_PlatformInterface = null;
                PlatformInterface.Shutdown();
            }
        }
    }



    // ------------------------
    // EOS to Dictionary Helpers
    // ------------------------
    private object _get_client_data(Reference p_options)
    {
        if (p_options.Get("client_data") != null) return p_options.Get("client_data");
        return null;
    }

    private Dictionary _ConvertOnCustomInviteCallback2Dict(dynamic data)
    {
        // data: OnCustomInviteAcceptedCallbackInfo |  OnCustomInviteReceivedCallbackInfo
        return new Dictionary(){
            {"target_user_id", data.TargetUserId.ToString()},
            {"local_user_id", data.LocalUserId.ToString()},
            {"client_data", data.ClientData},
            {"custom_invite_id", data.CustomInviteId?.ToString()},
            {"payload", data.Payload?.ToString()},
        };
    }

    private Dictionary _ConvertPinGrantInfo2Dict(PinGrantInfo? data)
    {
        return new Dictionary(){
            {"user_code", data?.UserCode.ToString()},
            {"verification_uri", data?.VerificationURI.ToString()},
            {"expires_in", data?.ExpiresIn},
            {"verification_uri_complete", data?.VerificationURIComplete.ToString()},
        };
    }

    private Dictionary _ConvertEntitlement2Dict(Entitlement? data)
    {
        return new Dictionary(){
            {"entitlement_name", data?.EntitlementName?.ToString()},
            {"entitlement_id", data?.EntitlementId?.ToString()},
            {"catalog_item_id", data?.CatalogItemId?.ToString()},
            {"server_index", data?.ServerIndex},
            {"redeemed", data?.Redeemed},
            {"end_timestamp", data?.EndTimestamp},
        };
    }

    private Dictionary _ConvertCatalogItem2Dict(CatalogItem? data)
    {
        return new Dictionary(){
            {"catalog_namespace", data?.CatalogNamespace?.ToString()},
            {"id", data?.Id?.ToString()},
            {"entitlement_name", data?.EntitlementName?.ToString()},
            {"title_text", data?.TitleText?.ToString()},
            {"description_text", data?.DescriptionText?.ToString()},
            {"long_description_text", data?.LongDescriptionText?.ToString()},
            {"developer_text", data?.DeveloperText?.ToString()},
            {"item_type", data?.ItemType},
            {"entitlement_end_timestamp", data?.EntitlementEndTimestamp},
        };
    }

    private Dictionary _ConvertCatalogRelease2Dict(CatalogRelease? data)
    {
        return new Dictionary(){
            {"release_note", data?.ReleaseNote?.ToString()},
            {"compatible_app_ids", data?.CompatibleAppIds?.Select(x => x.ToString())},
            {"compatible_platforms", data?.CompatiblePlatforms?.Select(x => x.ToString())},
        };
    }

    private Dictionary _ConvertCatalogOffer2Dict(CatalogOffer? data)
    {
        return new Dictionary(){
            {"server_index", data?.ServerIndex},
            {"catalog_namespace", data?.CatalogNamespace?.ToString()},
            {"id", data?.Id?.ToString()},
            {"title_text", data?.TitleText?.ToString()},
            {"description_text", data?.DescriptionText?.ToString()},
            {"long_description_text", data?.LongDescriptionText?.ToString()},
            {"currency_code", data?.CurrencyCode?.ToString()},
            {"price_result", data?.PriceResult},
            {"discount_percentage", (int)data?.DiscountPercentage},
            {"expiration_timestamp", data?.ExpirationTimestamp},
            {"purchased_limit", data?.PurchaseLimit},
            {"available_for_purchase", data?.AvailableForPurchase},
            {"original_price", data?.OriginalPrice64},
            {"current_price", data?.CurrentPrice64},
            {"decimal_point", data?.DecimalPoint},
        };
    }

    private Dictionary _ConvertKeyImageInfo2Dict(KeyImageInfo? data)
    {
        return new Dictionary(){
            {"type", data?.Type?.ToString()},
            {"url", data?.Url?.ToString()},
            {"width", (int)data?.Width},
            {"height", (int)data?.Height},
        };
    }

    public Dictionary _ConvertExternalAccountInfo2Dict(Epic.OnlineServices.Connect.ExternalAccountInfo? data)
    {
        return new Dictionary(){
            { "product_user_id", data?.ProductUserId?.ToString()},
            { "display_name", data?.DisplayName?.ToString()},
            { "account_id", data?.AccountId?.ToString()},
            { "account_id_type", data?.AccountIdType},
            { "last_login_time", data?.LastLoginTime?.ToString()},
        };
    }

    public Dictionary _ConvertAchievementDefinitionV22Dict(DefinitionV2? data)
    {
        Godot.Collections.Array<Dictionary> stats_thresholds = new Godot.Collections.Array<Dictionary>();
        if (data?.StatThresholds != null && data?.StatThresholds.Length > 0)
        {
            foreach (StatThresholds statThreshold in data?.StatThresholds)
            {
                Dictionary stat_thresholds_dict = new Dictionary(){
                    {"name", statThreshold.Name?.ToString()},
                    {"threshold", statThreshold.Threshold},
                };
                stats_thresholds.Add(stat_thresholds_dict);
            }
        }
        return new Dictionary(){
            {"achievement_id", data?.AchievementId.ToString()},
            {"unlocked_display_name", data?.UnlockedDisplayName.ToString()},
            {"unlocked_description", data?.UnlockedDescription.ToString()},
            {"locked_display_name", data?.LockedDisplayName.ToString()},
            {"locked_description", data?.LockedDescription.ToString()},
            {"flavor_text", data?.FlavorText.ToString()},
            {"unlocked_icon_url", data?.UnlockedIconURL.ToString()},
            {"locked_icon_url", data?.LockedIconURL.ToString()},
            {"is_hidden", data?.IsHidden},
            {"stat_thresholds", stats_thresholds},
        };
    }

    private Dictionary _ConvertPlayerAchievement2Dict(PlayerAchievement? data)
    {
        Godot.Collections.Array<Dictionary> stat_info = new Godot.Collections.Array<Dictionary>();
        if (data?.StatInfo != null && data?.StatInfo.Length > 0)
        {
            foreach (PlayerStatInfo statInfo in data?.StatInfo)
            {
                Dictionary player_stat_info = new Dictionary(){
                    {"name", statInfo.Name?.ToString()},
                    {"current_value", statInfo.CurrentValue},
                    {"threshold_value", statInfo.ThresholdValue},
                };
                stat_info.Add(player_stat_info);
            }
        }

        return new Dictionary(){
            {"achievement_id", data?.AchievementId?.ToString()},
            {"progress", data?.Progress},
            {"display_name", data?.DisplayName?.ToString()},
            {"description", data?.Description?.ToString()},
            {"icon_url", data?.IconURL?.ToString()},
            {"flavor_text", data?.FlavorText?.ToString()},
            {"unlock_time", data?.UnlockTime?.ToString()},
            {"stat_info", stat_info}
        };
    }

    private Dictionary _ConvertStat2Dict(Stat? data)
    {
        return new Dictionary(){
            {"name", data?.Name?.ToString()},
            {"start_time", data?.StartTime?.ToString()},
            {"end_time", data?.EndTime?.ToString()},
            {"value", data?.Value},
        };
    }

    private Dictionary _ConvertLeaderboardDefinition2Dict(Epic.OnlineServices.Leaderboards.Definition? data)
    {
        return new Dictionary(){
            {"leaderboard_id", data?.LeaderboardId?.ToString()},
            {"stat_name", data?.StatName?.ToString()},
            {"leaderboard_aggregation", data?.Aggregation},
            {"start_time", data?.StartTime?.ToString()},
            {"end_time", data?.EndTime?.ToString()},
        };
    }

    private Dictionary _ConvertLeaderboardRecord2Dict(Epic.OnlineServices.Leaderboards.LeaderboardRecord? data)
    {
        return new Dictionary(){
            {"user_id", data?.UserId?.ToString()},
                {"rank", (int)data?.Rank},
                {"score", data?.Score},
                {"user_display_name", data?.UserDisplayName?.ToString()},
        };
    }

    private Dictionary _ConvertLeaderboardUserScore2Dict(Epic.OnlineServices.Leaderboards.LeaderboardUserScore? data)
    {
        return new Dictionary(){
            {"user_id", data?.UserId?.ToString()},
            {"score", data?.Score},
        };
    }

    private Dictionary _ConvertExternalUserInfo2Dict(ExternalUserInfo? data)
    {
        return new Dictionary(){
            {"account_type", (int)data?.AccountType},
            {"account_id", data?.AccountId?.ToString()},
            {"display_name", data?.DisplayName?.ToString()},
        };
    }


    private Dictionary _ConvertUserInfoData2Dict(UserInfoData? data)
    {
        return new Dictionary(){
            {"user_id", data?.UserId?.ToString()},
            {"country", data?.Country?.ToString()},
            {"display_name", data?.DisplayName?.ToString()},
            {"preferred_language", data?.PreferredLanguage?.ToString()},
            {"nickname", data?.Nickname?.ToString()},
            {"display_name_sanitized", data?.DisplayNameSanitized?.ToString()},
        };
    }



    // ------------------------
    // Privates
    // ------------------------
    private static PlatformInterface s_PlatformInterface;
    private static AchievementsInterface s_AchievementsInterface;
    private static AuthInterface s_AuthInterface;
    private static ConnectInterface s_ConnectInterface;
    private static CustomInvitesInterface s_CustomInvitesInterface;
    private static StatsInterface s_StatsInterface;
    private static LeaderboardsInterface s_LeaderboardsInterface;
    private static FriendsInterface s_FriendsInterface;
    private static UserInfoInterface s_UserInfoInterface;
    private static EcomInterface s_EcomInterface;
    private static UIInterface s_UIInterface;
    private static KWSInterface s_KWSInterface;
    private static MetricsInterface s_MetricsInterface;
    private static ModsInterface s_ModsInterface;
    private static ReportsInterface s_ReportsInterface;
    private static ProgressionSnapshotInterface s_ProgressionSnapshotInterface;
    private static PresenceInterface s_PresenceInterface;
}

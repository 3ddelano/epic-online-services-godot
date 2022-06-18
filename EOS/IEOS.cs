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
            ProductName = (string)p_options.Get("product_name"),
            ProductVersion = (string)p_options.Get("product_version"),
        };

        var initializeResult = PlatformInterface.Initialize(initializeOptions);

        LoggingInterface.SetCallback((LogMessage logMessage) =>
        {
            EmitSignal(nameof(logging_interface_callback), new Dictionary<string, object>(){
                {"category", logMessage.Category},
                {"message", logMessage.Message},
                {"level", logMessage.Level}
            });
        });

        return initializeResult;
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
        if ((string)p_create_options.Get("override_locale_code") != "")
        {
            options.OverrideCountryCode = (string)p_create_options.Get("override_locale_code");
        }

        int flags = (int)p_create_options.Get("flags");
        options.Flags = (PlatformFlags)flags;

        int tickBudgetInMilliseconds = (int)p_create_options.Get("tick_budget_in_milliseconds");
        options.TickBudgetInMilliseconds = (uint)tickBudgetInMilliseconds;


        s_PlatformInterface = PlatformInterface.Create(options);
        if (s_PlatformInterface == null)
        {
            return false;
        }


        // Connect some interface callbacks
        s_ConnectInterface = s_PlatformInterface.GetConnectInterface();
        s_ConnectInterface.AddNotifyAuthExpiration(new AddNotifyAuthExpirationOptions(), null, (AuthExpirationCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()}
            };
            EmitSignal(nameof(connect_interface_auth_expiration), ret);
        });
        s_ConnectInterface.AddNotifyLoginStatusChanged(new Epic.OnlineServices.Connect.AddNotifyLoginStatusChangedOptions(), null, (Epic.OnlineServices.Connect.LoginStatusChangedCallbackInfo data) =>
        {
            Dictionary ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"previous_status", data.PreviousStatus},
                {"current_status", data.CurrentStatus},
                };
            EmitSignal(nameof(connect_interface_login_status_changed), ret);
        });

        s_AuthInterface = s_PlatformInterface.GetAuthInterface();
        s_AuthInterface.AddNotifyLoginStatusChanged(new Epic.OnlineServices.Auth.AddNotifyLoginStatusChangedOptions(), null, (Epic.OnlineServices.Auth.LoginStatusChangedCallbackInfo data) =>
            {
                Dictionary ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"prev_status", data.PrevStatus},
                {"current_status", data.CurrentStatus},
                };
                EmitSignal(nameof(auth_interface_login_status_changed), ret);
            });

        s_AchievementsInterface = s_PlatformInterface.GetAchievementsInterface();
        s_AchievementsInterface.AddNotifyAchievementsUnlockedV2(new AddNotifyAchievementsUnlockedV2Options(), null, (OnAchievementsUnlockedCallbackV2Info data) =>
            {
                Dictionary ret = new Dictionary(){
                {"user_id", data.UserId.ToString()},
                {"achievement_id", data.AchievementId},
                };
                if (data.UnlockTime.HasValue)
                {
                    ret.Add("unlock_time", data.UnlockTime.ToString());
                }
                else
                {
                    ret.Add("unlock_time", "");
                }
                EmitSignal(nameof(achievements_interface_achievements_unlocked_callback), ret);
            });

        s_CustomInvitesInterface = s_PlatformInterface.GetCustomInvitesInterface();
        s_CustomInvitesInterface.AddNotifyCustomInviteAccepted(new AddNotifyCustomInviteAcceptedOptions(), null, (OnCustomInviteAcceptedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"target_user_id", data.TargetUserId.ToString()},
                {"local_user_id", data.LocalUserId.ToString()},
            };

            if (data.CustomInviteId != null) ret.Add("custom_invite_id", data.CustomInviteId);
            if (data.Payload != null) ret.Add("payload", data.Payload);
            EmitSignal(nameof(custom_invites_interface_custom_invite_accepted_callback), ret);
        });
        s_CustomInvitesInterface.AddNotifyCustomInviteReceived(new AddNotifyCustomInviteReceivedOptions(), null, (OnCustomInviteReceivedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"target_user_id", data.TargetUserId.ToString()},
                {"local_user_id", data.LocalUserId.ToString()},
            };

            if (data.CustomInviteId != null) ret.Add("custom_invite_id", data.CustomInviteId);
            if (data.Payload != null) ret.Add("payload", data.Payload);
            EmitSignal(nameof(custom_invites_interface_custom_invite_received_callback), ret);
        });

        s_StatsInterface = s_PlatformInterface.GetStatsInterface();
        s_LeaderboardsInterface = s_PlatformInterface.GetLeaderboardsInterface();
        s_FriendsInterface = s_PlatformInterface.GetFriendsInterface();
        s_FriendsInterface.AddNotifyFriendsUpdate(new AddNotifyFriendsUpdateOptions(), null, (OnFriendsUpdateInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
                {"target_user_id", data.TargetUserId.ToString()},
                {"previous_status", (int) data.PreviousStatus},
                {"current_status", (int) data.CurrentStatus},
            };
            EmitSignal(nameof(friends_interface_friends_update_callback), ret);
        });

        s_UserInfoInterface = s_PlatformInterface.GetUserInfoInterface();

        s_EcomInterface = s_PlatformInterface.GetEcomInterface();

        s_UIInterface = s_PlatformInterface.GetUIInterface();
        s_UIInterface.AddNotifyDisplaySettingsUpdated(new AddNotifyDisplaySettingsUpdatedOptions(), null, (OnDisplaySettingsUpdatedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"is_visible", data.IsVisible},
                {"is_exclusive_input", data.IsExclusiveInput},
            };
            EmitSignal(nameof(ui_interface_display_settings_updated_callback), ret);
        });

        s_KWSInterface = s_PlatformInterface.GetKWSInterface();
        s_KWSInterface.AddNotifyPermissionsUpdateReceived(new AddNotifyPermissionsUpdateReceivedOptions(), null, (PermissionsUpdateReceivedCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"local_user_id", data.LocalUserId.ToString()},
            };
            EmitSignal(nameof(kws_interface_permissions_update_received), ret);
        });

        s_MetricsInterface = s_PlatformInterface.GetMetricsInterface();

        return true; // platform created successfully
    }
    public Result platform_interface_check_for_launcher_and_restart()
    {
        return s_PlatformInterface.CheckForLauncherAndRestart();
    }
    public Dictionary<string, object> platform_interface_get_active_country_code(string user_id)
    {
        string countryCode = "";
        Result res = s_PlatformInterface.GetActiveCountryCode(EpicAccountId.FromString(user_id), out countryCode);
        if (res == Result.Success)
        {
            return new Dictionary<string, object>(){
                {"country_code", countryCode},
                {"result_code", res}
            };
        }
        else return new Dictionary<string, object>(){
            {"result_code", res}
        };
    }
    public Dictionary<string, object> platform_interface_get_override_country_code()
    {
        string countryCode = "";
        Result res = s_PlatformInterface.GetOverrideCountryCode(out countryCode);
        if (res == Result.Success)
        {
            return new Dictionary<string, object>(){
                {"country_code", countryCode},
                {"result_code", res}
            };
        }
        else return new Dictionary<string, object>(){
            {"result_code", res}
        };
    }
    public Result platform_interface_set_override_country_code(string country_code)
    {
        return s_PlatformInterface.SetOverrideCountryCode(country_code);
    }
    public Dictionary<string, object> platform_interface_get_active_locale_code(string user_id)
    {
        string localeCode = "";
        Result res = s_PlatformInterface.GetActiveLocaleCode(EpicAccountId.FromString(user_id), out localeCode);
        if (res == Result.Success)
        {
            return new Dictionary<string, object>(){
                {"locale_code", localeCode},
                {"result_code", res}
            };
        }
        else return new Dictionary<string, object>(){
            {"result_code", res}
        };
    }
    public Dictionary<string, object> platform_interface_get_override_locale_code()
    {
        string localeCode = "";
        Result res = s_PlatformInterface.GetOverrideLocaleCode(out localeCode);
        if (res == Result.Success)
        {
            return new Dictionary<string, object>(){
                {"locale_code", localeCode},
                {"result_code", res}
            };
        }
        else return new Dictionary<string, object>(){
            {"result_code", res}
        };
    }
    public Result platform_interface_set_override_locale_code(string locale_code)
    {
        return s_PlatformInterface.SetOverrideLocaleCode(locale_code);
    }


    // -----
    // Auth Interface
    // -----
    public void auth_interface_login(Reference p_options)
    {
        var p_credentials = (Reference)p_options.Get("credentials");

        var credentials = new Epic.OnlineServices.Auth.Credentials()
        {
            Type = (LoginCredentialType)p_credentials.Get("type")
        };

        if (p_credentials.Get("id") != null)
        {
            credentials.Id = (string)p_credentials.Get("id");
        }
        if (p_credentials.Get("token") != null)
        {
            credentials.Token = (string)p_credentials.Get("token");
        }
        if (p_credentials.Get("external_type") != null)
        {
            credentials.ExternalType = (ExternalCredentialType)p_credentials.Get("external_type");
        }

        var loginOptions = new Epic.OnlineServices.Auth.LoginOptions()
        {
            Credentials = credentials,
            ScopeFlags = (AuthScopeFlags)p_options.Get("scope_flags"),
        };

        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_AuthInterface.Login(loginOptions, client_data, (Epic.OnlineServices.Auth.LoginCallbackInfo loginCallbackInfo) =>
        {

            Dictionary<string, object> ret = new Dictionary<string, object>(){
                {"result_code", loginCallbackInfo.ResultCode},
            };
            if (loginCallbackInfo.ClientData != null)
            {
                ret.Add("client_data", loginCallbackInfo.ClientData);
            }
            if (loginCallbackInfo.ContinuanceToken != null)
            {
                ContinuanceTokenWrapper ctw = new ContinuanceTokenWrapper(loginCallbackInfo.ContinuanceToken);
                ret.Add("continuance_token", ctw);
            }
            if (loginCallbackInfo.LocalUserId != null)
            {
                ret.Add("local_user_id", loginCallbackInfo.LocalUserId.ToString());
            }
            if (loginCallbackInfo.SelectedAccountId != null)
            {
                ret.Add("selected_account_id", loginCallbackInfo.SelectedAccountId.ToString());
            }
            if (loginCallbackInfo.PinGrantInfo != null)
            {
                Dictionary pin_grant_info = new Dictionary();
                pin_grant_info.Add("user_code", loginCallbackInfo.PinGrantInfo.UserCode);
                pin_grant_info.Add("verification_uri", loginCallbackInfo.PinGrantInfo.VerificationURI);
                pin_grant_info.Add("expires_in", loginCallbackInfo.PinGrantInfo.ExpiresIn);
                pin_grant_info.Add("verification_uri_complete", loginCallbackInfo.PinGrantInfo.VerificationURIComplete);
                ret.Add("pin_grant_info", pin_grant_info);
            }
            if (loginCallbackInfo.AccountFeatureRestrictedInfo != null)
            {
                Dictionary account_feature_restricted_info = new Dictionary();
                account_feature_restricted_info.Add("verification_uri", loginCallbackInfo.AccountFeatureRestrictedInfo.VerificationURI);
                ret.Add("account_feature_restricted_info", account_feature_restricted_info);
            }
            if (loginCallbackInfo.ResultCode == Result.Success)
            {
                ret.Add("pending", false);
                ret.Add("success", true);
            }
            else if (Common.IsOperationComplete(loginCallbackInfo.ResultCode))
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
        var logoutOptions = new LogoutOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id"))
        };

        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_AuthInterface.Logout(logoutOptions, client_data, (LogoutCallbackInfo logoutCallbackInfo) =>
        {
            Dictionary<string, object> ret = new Dictionary<string, object>(){
                {"result_code", logoutCallbackInfo.ResultCode},
            };
            if (logoutCallbackInfo.ClientData != null)
            {
                ret.Add("client_data", logoutCallbackInfo.ClientData);
            }
            if (logoutCallbackInfo.LocalUserId != null)
            {
                ret.Add("local_user_id", logoutCallbackInfo.LocalUserId.ToString());
            }

            EmitSignal(nameof(auth_interface_logout_callback), ret);
        });
    }
    public Dictionary auth_interface_copy_id_token(Reference p_options)
    {
        var options = new Epic.OnlineServices.Auth.CopyIdTokenOptions()
        {
            AccountId = EpicAccountId.FromString((string)p_options.Get("account_id")),
        };

        Epic.OnlineServices.Auth.IdToken token;
        Result res = s_AuthInterface.CopyIdToken(options, out token);
        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"account_id", token.AccountId.ToString()},
                {"json_web_token", token.JsonWebToken}
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public Dictionary auth_interface_copy_user_auth_token(Reference p_options, string p_local_user_id)
    {
        var options = new CopyUserAuthTokenOptions() { };

        Token token;
        Result res = s_AuthInterface.CopyUserAuthToken(options, EpicAccountId.FromString(p_local_user_id), out token);
        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"app", token.App},
                {"client_id", token.ClientId},
                {"account_id", token.AccountId.ToString()},
                {"access_token", token.AccessToken},
                {"expires_in", token.ExpiresIn},
                {"expires_at", token.ExpiresAt},
                {"auth_type", token.AuthType},
                {"refresh_token", token.RefreshToken},
                {"refresh_expires_in", token.RefreshExpiresIn},
                {"refresh_expires_at", token.RefreshExpiresAt},
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public void auth_interface_delete_persistent_auth(Reference p_options)
    {
        var options = new DeletePersistentAuthOptions()
        {
            RefreshToken = (string)p_options.Get("refresh_token")
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_AuthInterface.DeletePersistentAuth(options, client_data, (DeletePersistentAuthCallbackInfo data) =>
        {
            Dictionary<string, object> ret = new Dictionary<string, object>(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }

            EmitSignal(nameof(auth_interface_delete_persistent_auth_callback), ret);
        });
    }
    public string auth_interface_get_logged_in_account_by_index(int p_index)
    {
        var accountId = s_AuthInterface.GetLoggedInAccountByIndex(p_index);
        if (accountId == null) return "";
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
        if (accountId != null)
        {
            return accountId.ToString();
        }
        return "";
    }
    public int auth_interface_get_merged_accounts_count(string p_local_user_id)
    {
        return (int)s_AuthInterface.GetMergedAccountsCount(EpicAccountId.FromString(p_local_user_id));
    }
    public Dictionary auth_interface_get_selected_account_id(string p_local_user_id)
    {
        EpicAccountId selectedAccountId;

        var res = s_AuthInterface.GetSelectedAccountId(EpicAccountId.FromString(p_local_user_id), out selectedAccountId);
        var ret = new Dictionary(){
            {"result_code", res},
        };
        if (res == Result.Success)
        {
            ret.Add("selected_account_id", selectedAccountId.ToString());
        }
        return ret;
    }
    public void auth_interface_query_id_token(Reference p_options)
    {
        var options = new QueryIdTokenOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetAccountId = EpicAccountId.FromString((string)p_options.Get("target_account_id")),
        };

        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_AuthInterface.QueryIdToken(options, client_data, (QueryIdTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            if (data.LocalUserId != null)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
            if (data.TargetAccountId != null)
            {
                ret.Add("target_account_id", data.TargetAccountId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_AuthInterface.VerifyIdToken(options, client_data, (Epic.OnlineServices.Auth.VerifyIdTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            if (data.ApplicationId != null)
            {
                ret.Add("application_id", data.ApplicationId.ToString());
            }
            if (data.ClientId != null)
            {
                ret.Add("client_id", data.ClientId.ToString());
            }
            if (data.ProductId != null)
            {
                ret.Add("product_id", data.ProductId.ToString());
            }
            if (data.DeploymentId != null)
            {
                ret.Add("deployment_id", data.DeploymentId.ToString());
            }
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
        {
            options.LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id"));
        }
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_AuthInterface.LinkAccount(options, client_data, (Epic.OnlineServices.Auth.LinkAccountCallbackInfo data) =>
        {
            Dictionary<string, object> ret = new Dictionary<string, object>(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            if (data.LocalUserId != null)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
            if (data.SelectedAccountId != null)
            {
                ret.Add("selected_account_id", data.SelectedAccountId.ToString());
            }
            if (data.PinGrantInfo != null)
            {
                Dictionary pin_grant_info = new Dictionary();
                pin_grant_info.Add("user_code", data.PinGrantInfo.UserCode);
                pin_grant_info.Add("verification_uri", data.PinGrantInfo.VerificationURI);
                pin_grant_info.Add("expires_in", data.PinGrantInfo.ExpiresIn);
                pin_grant_info.Add("verification_uri_complete", data.PinGrantInfo.VerificationURIComplete);
                ret.Add("pin_grant_info", pin_grant_info);
            }

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
        // };

        var options = new VerifyUserAuthOptions()
        {
            AuthToken = auth_token
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_AuthInterface.VerifyUserAuth(options, client_data, (VerifyUserAuthCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
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

        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_ConnectInterface.Login(options, client_data, (Epic.OnlineServices.Connect.LoginCallbackInfo loginCallbackInfo) =>
        {

            Dictionary<string, object> ret = new Dictionary<string, object>(){
                {"result_code", loginCallbackInfo.ResultCode},
            };
            if (loginCallbackInfo.ClientData != null)
            {
                ret.Add("client_data", loginCallbackInfo.ClientData);
            }
            if (loginCallbackInfo.ContinuanceToken != null)
            {
                ContinuanceTokenWrapper ctw = new ContinuanceTokenWrapper(loginCallbackInfo.ContinuanceToken);
                ret.Add("continuance_token", ctw);
            }
            if (loginCallbackInfo.LocalUserId != null)
            {
                ret.Add("local_user_id", loginCallbackInfo.LocalUserId.ToString());
            }
            if (loginCallbackInfo.ResultCode == Result.Success)
            {
                ret.Add("success", true);
            }
            else if (Common.IsOperationComplete(loginCallbackInfo.ResultCode))
            {
                ret.Add("success", false);
            }
            else
            {
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

        Epic.OnlineServices.Connect.IdToken token;
        Result res = s_ConnectInterface.CopyIdToken(options, out token);
        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"product_user_id", token.ProductUserId.ToString()},
                {"json_web_token", token.JsonWebToken}
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public Dictionary connect_interface_copy_product_user_external_account_by_account_id(Reference p_options)
    {
        var options = new CopyProductUserExternalAccountByAccountIdOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AccountId = (string)p_options.Get("account_id")
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo externalAccountInfo;
        Result res = s_ConnectInterface.CopyProductUserExternalAccountByAccountId(options, out externalAccountInfo);

        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"product_user_id", externalAccountInfo.ProductUserId.ToString()},
                {"display_name", externalAccountInfo.DisplayName},
                {"account_id", externalAccountInfo.AccountId},
                {"account_id_type", externalAccountInfo.AccountIdType},
                {"last_login_time", externalAccountInfo.LastLoginTime.ToString()},
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public Dictionary connect_interface_copy_product_user_external_account_by_account_type(Reference p_options)
    {
        var options = new CopyProductUserExternalAccountByAccountTypeOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AccountIdType = (ExternalAccountType)p_options.Get("account_id_type")
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo externalAccountInfo;
        var res = s_ConnectInterface.CopyProductUserExternalAccountByAccountType(options, out externalAccountInfo);

        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"product_user_id", externalAccountInfo.ProductUserId.ToString()},
                {"display_name", externalAccountInfo.DisplayName},
                {"account_id", externalAccountInfo.AccountId},
                {"account_id_type", externalAccountInfo.AccountIdType},
                {"last_login_time", externalAccountInfo.LastLoginTime.ToString()},
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public Dictionary connect_interface_copy_product_user_external_account_by_index(Reference p_options)
    {
        int p_external_account_info_index = (int)p_options.Get("external_account_info_index");
        var options = new CopyProductUserExternalAccountByIndexOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            ExternalAccountInfoIndex = (uint)p_external_account_info_index
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo externalAccountInfo;
        var res = s_ConnectInterface.CopyProductUserExternalAccountByIndex(options, out externalAccountInfo);

        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"product_user_id", externalAccountInfo.ProductUserId.ToString()},
                {"display_name", externalAccountInfo.DisplayName},
                {"account_id", externalAccountInfo.AccountId},
                {"account_id_type", externalAccountInfo.AccountIdType},
                {"last_login_time", externalAccountInfo.LastLoginTime.ToString()},
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public Dictionary connect_interface_copy_product_user_info(Reference p_options)
    {
        var options = new CopyProductUserInfoOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };

        Epic.OnlineServices.Connect.ExternalAccountInfo externalAccountInfo;
        var res = s_ConnectInterface.CopyProductUserInfo(options, out externalAccountInfo);

        if (res == Result.Success)
        {
            Dictionary ret = new Dictionary(){
                {"result_code", res},
                {"product_user_id", externalAccountInfo.ProductUserId.ToString()},
                {"display_name", externalAccountInfo.DisplayName},
                {"account_id", externalAccountInfo.AccountId},
                {"account_id_type", externalAccountInfo.AccountIdType},
                {"last_login_time", externalAccountInfo.LastLoginTime.ToString()},
            };
            return ret;
        }
        else return new Dictionary(){
            {"result_code", res}
        };
    }
    public void connect_interface_create_device_id(Reference p_options)
    {
        var options = new CreateDeviceIdOptions()
        {
            DeviceModel = (string)p_options.Get("device_model"),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_ConnectInterface.CreateDeviceId(options, client_data, (CreateDeviceIdCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            EmitSignal(nameof(connect_interface_create_device_id_callback), ret);
        });
    }
    public void connect_interface_delete_device_id(Reference p_options)
    {
        DeleteDeviceIdOptions options = new DeleteDeviceIdOptions();
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_ConnectInterface.DeleteDeviceId(options, client_data, (DeleteDeviceIdCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_ConnectInterface.CreateUser(options, client_data, (Epic.OnlineServices.Connect.CreateUserCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
                {"local_user_id", data.LocalUserId},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
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
        var product_user_id = s_ConnectInterface.GetExternalAccountMapping(options);
        if (product_user_id == null || !product_user_id.IsValid()) return "";
        return product_user_id.ToString();
    }

    public string connect_interface_get_logged_in_user_by_index(int p_index)
    {
        var product_user_id = s_ConnectInterface.GetLoggedInUserByIndex(p_index);
        if (product_user_id == null || !product_user_id.IsValid()) return "";
        return product_user_id.ToString();
    }

    public int connect_interface_get_logged_in_users_count()
    {
        var count = s_ConnectInterface.GetLoggedInUsersCount();
        return count;
    }

    public int connect_interface_get_login_status(string p_local_user_id)
    {
        LoginStatus login_status = s_ConnectInterface.GetLoginStatus(ProductUserId.FromString(p_local_user_id));
        return (int)login_status;
    }

    public int connect_interface_get_product_user_external_account_count(Reference p_options)
    {
        var options = new GetProductUserExternalAccountCountOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };
        uint count = s_ConnectInterface.GetProductUserExternalAccountCount(options);
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
        string product_user_id = "";
        Result res = s_ConnectInterface.GetProductUserIdMapping(options, out product_user_id);

        Dictionary ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            ret.Add("product_user_id", product_user_id);
        }
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
        {
            options.LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"));
        }
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_ConnectInterface.LinkAccount(options, client_data, (Epic.OnlineServices.Connect.LinkAccountCallbackInfo data) =>
        {
            Dictionary<string, object> ret = new Dictionary<string, object>(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            if (data.LocalUserId != null)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_ConnectInterface.VerifyIdToken(options, client_data, (Epic.OnlineServices.Connect.VerifyIdTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ProductUserId != null) ret.Add("product_user_id", data.ProductUserId.ToString());
            ret.Add("is_account_info_present", data.IsAccountInfoPresent);
            ret.Add("account_id_type", (int)data.AccountIdType);
            if (data.AccountId != null) ret.Add("account_id", data.AccountId.ToString());
            if (data.Platform != null) ret.Add("platform", data.Platform);
            if (data.DeviceType != null) ret.Add("device_type", data.DeviceType);
            if (data.ClientId != null) ret.Add("client_id", data.ClientId);
            if (data.ProductId != null) ret.Add("product_id", data.ProductId);
            if (data.SandboxId != null) ret.Add("sandbox_id", data.SandboxId);
            if (data.DeploymentId != null) ret.Add("deployment_id", data.DeploymentId);
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
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_ConnectInterface.TransferDeviceIdAccount(options, client_data, (TransferDeviceIdAccountCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };

            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.LocalUserId != null) ret.Add("local_user_id", data.LocalUserId.ToString());

            EmitSignal(nameof(connect_interface_transfer_device_id_account_callback), ret);
        });
    }
    public void connect_interface_unlink_account(Reference p_options)
    {
        var options = new UnlinkAccountOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_ConnectInterface.UnlinkAccount(options, client_data, (UnlinkAccountCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.LocalUserId != null) ret.Add("local_user_id", data.LocalUserId.ToString());
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

        DefinitionV2 definitionV2;
        Result res = s_AchievementsInterface.CopyAchievementDefinitionV2ByAchievementId(options, out definitionV2);

        Dictionary definitionV2_dict = new Dictionary();
        if (res == Result.Success)
        {
            Godot.Collections.Array<Dictionary> stats_thresholds = new Godot.Collections.Array<Dictionary>();

            if (definitionV2.StatThresholds != null && definitionV2.StatThresholds.Length > 0)
            {
                foreach (StatThresholds statThreshold in definitionV2.StatThresholds)
                {
                    Dictionary stat_thresholds_dict = new Dictionary();
                    stat_thresholds_dict.Add("name", statThreshold.Name);
                    stat_thresholds_dict.Add("threshold", statThreshold.Threshold);
                    stats_thresholds.Add(stat_thresholds_dict);
                }
            }

            definitionV2_dict.Add("achievement_id", definitionV2.AchievementId);
            definitionV2_dict.Add("unlocked_display_name", definitionV2.UnlockedDisplayName);
            definitionV2_dict.Add("unlocked_description", definitionV2.UnlockedDescription);
            definitionV2_dict.Add("locked_display_name", definitionV2.LockedDisplayName);
            definitionV2_dict.Add("locked_description", definitionV2.LockedDescription);
            definitionV2_dict.Add("flavor_text", definitionV2.FlavorText);
            definitionV2_dict.Add("unlocked_icon_url", definitionV2.UnlockedIconURL);
            definitionV2_dict.Add("locked_icon_url", definitionV2.LockedIconURL);
            definitionV2_dict.Add("is_hidden", definitionV2.IsHidden);
            definitionV2_dict.Add("stat_thresholds", stats_thresholds);
        }

        Dictionary ret = new Dictionary(){
            {"result_code", res},
            {"definition_v2", definitionV2_dict}
        };
        return ret;
    }
    public Dictionary achievements_interface_copy_achievement_definition_v2_by_index(Reference p_options)
    {
        var p_achievement_index = (int)p_options.Get("achievement_index");
        var options = new CopyAchievementDefinitionV2ByIndexOptions()
        {
            AchievementIndex = (uint)p_achievement_index
        };


        DefinitionV2 definitionV2;
        Result res = s_AchievementsInterface.CopyAchievementDefinitionV2ByIndex(options, out definitionV2);

        Dictionary definitionV2_dict = new Dictionary();
        if (res == Result.Success)
        {
            Godot.Collections.Array<Dictionary> stats_thresholds = new Godot.Collections.Array<Dictionary>();
            if (definitionV2.StatThresholds != null && definitionV2.StatThresholds.Length > 0)
            {
                foreach (StatThresholds statThreshold in definitionV2.StatThresholds)
                {
                    Dictionary stat_thresholds_dict = new Dictionary();
                    stat_thresholds_dict.Add("name", statThreshold.Name);
                    stat_thresholds_dict.Add("threshold", statThreshold.Threshold);
                    stats_thresholds.Add(stat_thresholds_dict);
                }
            }

            definitionV2_dict.Add("achievement_id", definitionV2.AchievementId);
            definitionV2_dict.Add("unlocked_display_name", definitionV2.UnlockedDisplayName);
            definitionV2_dict.Add("unlocked_description", definitionV2.UnlockedDescription);
            definitionV2_dict.Add("locked_display_name", definitionV2.LockedDisplayName);
            definitionV2_dict.Add("locked_description", definitionV2.LockedDescription);
            definitionV2_dict.Add("flavor_text", definitionV2.FlavorText);
            definitionV2_dict.Add("unlocked_icon_url", definitionV2.UnlockedIconURL);
            definitionV2_dict.Add("locked_icon_url", definitionV2.LockedIconURL);
            definitionV2_dict.Add("is_hidden", definitionV2.IsHidden);
            definitionV2_dict.Add("stat_thresholds", stats_thresholds);
        }

        Dictionary ret = new Dictionary(){
            {"result_code", res},
            {"definition_v2", definitionV2_dict}
        };
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
        PlayerAchievement playerAchievement;
        Result res = s_AchievementsInterface.CopyPlayerAchievementByAchievementId(options, out playerAchievement);

        Dictionary player_achievement_dict = new Dictionary();
        if (res == Result.Success)
        {
            player_achievement_dict.Add("achievement_id", playerAchievement.AchievementId);
            player_achievement_dict.Add("progress", playerAchievement.Progress);

            if (playerAchievement.UnlockTime.HasValue)
            {
                player_achievement_dict.Add("unlock_time", playerAchievement.UnlockTime.ToString());
            }
            else
            {
                player_achievement_dict.Add("unlock_time", "");
            }

            Godot.Collections.Array<Dictionary> stat_info = new Godot.Collections.Array<Dictionary>();
            if (playerAchievement.StatInfo != null && playerAchievement.StatInfo.Length > 0)
            {
                foreach (PlayerStatInfo statInfo in playerAchievement.StatInfo)
                {
                    Dictionary player_stat_info = new Dictionary(){
                    {"name", statInfo.Name},
                    {"current_value", statInfo.CurrentValue},
                    {"threshold_value", statInfo.ThresholdValue},
                };
                    stat_info.Add(player_stat_info);
                }
            }

            player_achievement_dict.Add("stat_info", stat_info);

            player_achievement_dict.Add("display_name", playerAchievement.DisplayName);
            player_achievement_dict.Add("description", playerAchievement.Description);
            player_achievement_dict.Add("icon_url", playerAchievement.IconURL);
            player_achievement_dict.Add("flavor_text", playerAchievement.FlavorText);
        }

        Dictionary ret = new Dictionary(){
            {"result_code", res},
            {"player_achievement", player_achievement_dict}
        };
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
        PlayerAchievement playerAchievement;
        Result res = s_AchievementsInterface.CopyPlayerAchievementByIndex(options, out playerAchievement);

        Dictionary player_achievement_dict = new Dictionary();
        if (res == Result.Success)
        {
            player_achievement_dict.Add("achievement_id", playerAchievement.AchievementId);
            player_achievement_dict.Add("progress", playerAchievement.Progress);

            if (playerAchievement.UnlockTime.HasValue)
            {
                player_achievement_dict.Add("unlock_time", playerAchievement.UnlockTime.ToString());
            }
            else
            {
                player_achievement_dict.Add("unlock_time", "");
            }

            Godot.Collections.Array<Dictionary> stat_info = new Godot.Collections.Array<Dictionary>();
            if (playerAchievement.StatInfo != null && playerAchievement.StatInfo.Length > 0)
            {
                foreach (PlayerStatInfo statInfo in playerAchievement.StatInfo)
                {
                    Dictionary player_stat_info = new Dictionary(){
                        {"name", statInfo.Name},
                        {"current_value", statInfo.CurrentValue},
                        {"threshold_value", statInfo.ThresholdValue},
                    };
                    stat_info.Add(player_stat_info);
                }
            }
            player_achievement_dict.Add("stat_info", stat_info);

            player_achievement_dict.Add("display_name", playerAchievement.DisplayName);
            player_achievement_dict.Add("description", playerAchievement.Description);
            player_achievement_dict.Add("icon_url", playerAchievement.IconURL);
            player_achievement_dict.Add("flavor_text", playerAchievement.FlavorText);
        }

        Dictionary ret = new Dictionary(){
            {"result_code", res},
            {"player_achievement", player_achievement_dict}
        };
        return ret;
    }
    public int achievements_interface_get_achievement_definition_count(Reference p_options)
    {
        var options = new GetAchievementDefinitionCountOptions();
        return (int)s_AchievementsInterface.GetAchievementDefinitionCount(options);
    }
    public void achievements_interface_query_definitions(Reference p_options)
    {
        var options = new QueryDefinitionsOptions()
        {
            LocalUserId = null
        };
        if (p_options.Get("local_user_id") != null || (string)p_options.Get("local_user_id") != "")
        {
            options.LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id"));
        }
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_AchievementsInterface.QueryDefinitions(options, client_data, (OnQueryDefinitionsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            EmitSignal(nameof(achievements_interface_query_definitions_complete_callback), ret);
        });
    }
    public int achievements_interface_get_player_achievement_count(Reference p_options)
    {
        var options = new GetPlayerAchievementCountOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id"))
        };
        return (int)s_AchievementsInterface.GetPlayerAchievementCount(options);
    }
    public void achievements_interface_query_player_achievements(Reference p_options)
    {
        var options = new QueryPlayerAchievementsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id"))
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_AchievementsInterface.QueryPlayerAchievements(options, client_data, (OnQueryPlayerAchievementsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            EmitSignal(nameof(achievements_interface_query_player_achievements_complete_callback), ret);
        });
    }
    public void achievements_interface_unlock_achievements(Reference p_options)
    {
        string[] p_achievement_ids = ((System.Collections.IEnumerable)p_options.Get("achievement_ids")).Cast<string>().ToArray();
        var options = new UnlockAchievementsOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id")),
            AchievementIds = p_achievement_ids
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_AchievementsInterface.UnlockAchievements(options, client_data, (OnUnlockAchievementsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null)
            {
                ret.Add("client_data", data.ClientData);
            }
            if (data.ResultCode == Result.Success)
            {
                ret.Add("user_id", data.UserId.ToString());
                ret.Add("achievements_count", (int)data.AchievementsCount);
            }
            EmitSignal(nameof(achievements_interface_unlock_achievements_complete_callback), ret);
        });
    }


    // ------------------------
    // Custom Invites Interface
    // ------------------------
    public int custom_invites_interface_finalize_invite(Reference p_options)
    {
        var options = new FinalizeInviteOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            CustomInviteId = (string)p_options.Get("custom_invite_id"),
            ProcessingResult = (Result)p_options.Get("processing_result"),
        };
        Result res = s_CustomInvitesInterface.FinalizeInvite(options);
        return (int)res;
    }

    public void custom_invites_interface_send_custom_invite(Reference p_options)
    {
        var options = new SendCustomInviteOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            TargetUserIds = ((System.Collections.IEnumerable)p_options.Get("target_user_ids")).Cast<string>().Select(x => ProductUserId.FromString(x)).ToArray()
        };
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }

        s_CustomInvitesInterface.SendCustomInvite(options, client_data, (SendCustomInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.LocalUserId != null) ret.Add("local_user_id", data.LocalUserId.ToString());
            if (data.TargetUserIds != null) ret.Add("target_user_ids", data.TargetUserIds.Select(x => x.ToString()).ToArray());

            EmitSignal(nameof(custom_invites_interface_send_custom_invite_complete_callback), ret);
        });
    }

    public int custom_invites_interface_set_custom_invite(Reference p_options)
    {
        var options = new SetCustomInviteOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            Payload = (string)p_options.Get("payload"),
        };

        Result res = s_CustomInvitesInterface.SetCustomInvite(options);
        return (int)res;
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
        Stat outStat;
        var res = s_StatsInterface.CopyStatByIndex(options, out outStat);
        var ret = new Dictionary()
        {
            {"result_code", res},
        };
        if (outStat != null)
        {
            var stat_dict = new Dictionary(){
                {"name", outStat.Name},
                {"start_time", outStat.StartTime.ToString()},
                {"end_time", outStat.EndTime.ToString()},
                {"value", outStat.Value},
            };
            ret.Add("stat", stat_dict);
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
        Stat outStat;
        var res = s_StatsInterface.CopyStatByName(options, out outStat);
        var ret = new Dictionary()
        {
            {"result_code", res},
        };
        if (outStat != null)
        {
            var stat_dict = new Dictionary(){
                {"name", outStat.Name},
                {"start_time", outStat.StartTime.ToString()},
                {"end_time", outStat.EndTime.ToString()},
                {"value", outStat.Value},
            };
            ret.Add("stat", stat_dict);
        }
        return ret;
    }

    public int stats_interface_get_stats_count(Reference p_options)
    {
        var options = new GetStatCountOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
        };
        uint count = s_StatsInterface.GetStatsCount(options);
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
        object client_data = null;
        if (p_options.Get("client_data") != null)
        {
            client_data = p_options.Get("client_data");
        }
        s_StatsInterface.IngestStat(options, client_data, (IngestStatCompleteCallbackInfo data) =>
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
                                                                        .ToArray();
        if (p_options.Get("start_time") != null) options.StartTime = DateTime.Parse((string)p_options.Get("start_time"));
        if (p_options.Get("end_time") != null) options.EndTime = DateTime.Parse((string)p_options.Get("end_time"));

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_StatsInterface.QueryStats(options, client_data, (OnQueryStatsCompleteCallbackInfo data) =>
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
        Epic.OnlineServices.Leaderboards.Definition outLeaderboardDefinition;
        var res = s_LeaderboardsInterface.CopyLeaderboardDefinitionByIndex(options, out outLeaderboardDefinition);

        var ret = new Dictionary()
        {
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var definition_dict = new Dictionary(){
                {"leaderboard_id", outLeaderboardDefinition.LeaderboardId.ToString()},
                {"stat_name", outLeaderboardDefinition.StatName},
                {"leaderboard_aggregation", (int)outLeaderboardDefinition.Aggregation},
            };

            if (outLeaderboardDefinition.StartTime != null) definition_dict.Add("start_time", outLeaderboardDefinition.StartTime.ToString());
            if (outLeaderboardDefinition.EndTime != null) definition_dict.Add("end_time", outLeaderboardDefinition.EndTime.ToString());
            ret.Add("definition", definition_dict);
        }
        return ret;
    }
    public Dictionary leaderboards_interface_copy_leaderboard_definition_by_leaderboard_id(Reference p_options)
    {
        var options = new CopyLeaderboardDefinitionByLeaderboardIdOptions()
        {
            LeaderboardId = (string)p_options.Get("leaderboard_id")
        };
        Epic.OnlineServices.Leaderboards.Definition outLeaderboardDefinition;
        var res = s_LeaderboardsInterface.CopyLeaderboardDefinitionByLeaderboardId(options, out outLeaderboardDefinition);
        var ret = new Dictionary()
        {
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var definition_dict = new Dictionary(){
                {"leaderboard_id", outLeaderboardDefinition.LeaderboardId.ToString()},
                {"stat_name", outLeaderboardDefinition.StatName},
                {"leaderboard_aggregation", (int)outLeaderboardDefinition.Aggregation},
            };

            if (outLeaderboardDefinition.StartTime != null) definition_dict.Add("start_time", outLeaderboardDefinition.StartTime.ToString());
            if (outLeaderboardDefinition.EndTime != null) definition_dict.Add("end_time", outLeaderboardDefinition.EndTime.ToString());
            ret.Add("definition", definition_dict);
        }
        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_record_by_index(Reference p_options)
    {
        int p_leaderboard_record_index = (int)p_options.Get("leaderboard_record_index");
        var options = new CopyLeaderboardRecordByIndexOptions()
        {
            LeaderboardRecordIndex = (uint)p_leaderboard_record_index
        };

        LeaderboardRecord outLeaderboardRecord;
        var res = s_LeaderboardsInterface.CopyLeaderboardRecordByIndex(options, out outLeaderboardRecord);
        var ret = new Dictionary()
        {
            {"result_code", res}
        };

        if (res == Result.Success)
        {
            var leaderboard_record_dict = new Dictionary(){
                {"user_id", outLeaderboardRecord.UserId.ToString()},
                {"rank", (int)outLeaderboardRecord.Rank},
                {"score", outLeaderboardRecord.Score},
                {"user_display_name", outLeaderboardRecord.UserDisplayName},
            };
            ret.Add("leaderboard_record", leaderboard_record_dict);
        }

        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_record_by_user_id(Reference p_options)
    {
        var options = new CopyLeaderboardRecordByUserIdOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id"))
        };

        LeaderboardRecord outLeaderboardRecord;
        var res = s_LeaderboardsInterface.CopyLeaderboardRecordByUserId(options, out outLeaderboardRecord);
        var ret = new Dictionary()
        {
            {"result_code", res}
        };

        if (res == Result.Success)
        {
            var leaderboard_record_dict = new Dictionary(){
                {"user_id", outLeaderboardRecord.UserId.ToString()},
                {"rank", (int)outLeaderboardRecord.Rank},
                {"score", outLeaderboardRecord.Score},
                {"user_display_name", outLeaderboardRecord.UserDisplayName},
            };
            ret.Add("leaderboard_record", leaderboard_record_dict);
        }

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
        LeaderboardUserScore outLeaderboardUserScore;
        var res = s_LeaderboardsInterface.CopyLeaderboardUserScoreByIndex(options, out outLeaderboardUserScore);
        var ret = new Dictionary()
        {
            {"result_code", res}
        };

        if (res == Result.Success)
        {
            var leaderboard_user_score_dict = new Dictionary(){
                {"user_id", outLeaderboardUserScore.UserId.ToString()},
                {"score", outLeaderboardUserScore.Score},
            };
            ret.Add("leaderboard_user_score", leaderboard_user_score_dict);
        }

        return ret;
    }

    public Dictionary leaderboards_interface_copy_leaderboard_user_score_by_user_id(Reference p_options)
    {
        var options = new CopyLeaderboardUserScoreByUserIdOptions()
        {
            UserId = ProductUserId.FromString((string)p_options.Get("user_id")),
            StatName = (string)p_options.Get("stat_name")
        };
        LeaderboardUserScore outLeaderboardUserScore;
        var res = s_LeaderboardsInterface.CopyLeaderboardUserScoreByUserId(options, out outLeaderboardUserScore);
        var ret = new Dictionary()
        {
            {"result_code", res}
        };

        if (res == Result.Success)
        {
            var leaderboard_user_score_dict = new Dictionary(){
                {"user_id", outLeaderboardUserScore.UserId.ToString()},
                {"score", outLeaderboardUserScore.Score},
            };
            ret.Add("leaderboard_user_score", leaderboard_user_score_dict);
        }

        return ret;
    }

    public int leaderboards_interface_get_leaderboard_definition_count(Reference p_options)
    {
        var options = new GetLeaderboardDefinitionCountOptions()
        {
        };
        return (int)s_LeaderboardsInterface.GetLeaderboardDefinitionCount(options);
    }

    public int leaderboards_interface_get_leaderboard_record_count(Reference p_options)
    {
        var options = new GetLeaderboardRecordCountOptions()
        {
        };
        return (int)s_LeaderboardsInterface.GetLeaderboardRecordCount(options);
    }

    public int leaderboards_interface_get_leaderboard_user_score_count(Reference p_options)
    {
        var options = new GetLeaderboardUserScoreCountOptions()
        {
            StatName = (string)p_options.Get("stat_name")
        };
        return (int)s_LeaderboardsInterface.GetLeaderboardUserScoreCount(options);
    }

    public void leaderboards_interface_query_leaderboard_definitions(Reference p_options)
    {
        var options = new QueryLeaderboardDefinitionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        if (p_options.Get("start_time") != null) options.StartTime = DateTime.Parse((string)p_options.Get("start_time"));
        if (p_options.Get("end_time") != null) options.EndTime = DateTime.Parse((string)p_options.Get("end_time"));

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_LeaderboardsInterface.QueryLeaderboardDefinitions(options, client_data, (OnQueryLeaderboardDefinitionsCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
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

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_LeaderboardsInterface.QueryLeaderboardRanks(options, client_data, (OnQueryLeaderboardRanksCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
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
        ProductUserId[] userIds = ((System.Collections.IEnumerable)p_options.Get("user_ids")).Cast<string>().Select(x => ProductUserId.FromString(x)).ToArray();
        var options = new QueryLeaderboardUserScoresOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            UserIds = userIds,
            StatInfo = statInfo,
        };

        if (p_options.Get("start_time") != null) options.StartTime = DateTime.Parse((string)p_options.Get("start_time"));
        if (p_options.Get("end_time") != null) options.EndTime = DateTime.Parse((string)p_options.Get("end_time"));

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_LeaderboardsInterface.QueryLeaderboardUserScores(options, client_data, (OnQueryLeaderboardUserScoresCompleteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_FriendsInterface.AcceptInvite(options, client_data, (AcceptInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("target_user_id", data.TargetUserId.ToString());
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
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
        EpicAccountId friendId = s_FriendsInterface.GetFriendAtIndex(options);
        return friendId.ToString();
    }
    public int friends_interface_get_friends_count(Reference p_options)
    {
        var options = new GetFriendsCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return s_FriendsInterface.GetFriendsCount(options);
    }

    public int friends_interface_get_status(Reference p_options)
    {
        var options = new GetStatusOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        return (int)s_FriendsInterface.GetStatus(options);
    }

    public void friends_interface_query_friends(Reference p_options)
    {
        var options = new QueryFriendsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_FriendsInterface.QueryFriends(options, client_data, (QueryFriendsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_FriendsInterface.RejectInvite(options, client_data, (RejectInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("target_user_id", data.TargetUserId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_FriendsInterface.SendInvite(options, client_data, (SendInviteCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("target_user_id", data.TargetUserId.ToString());
            }
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
        ExternalUserInfo outExternalUserInfo;
        Result res = s_UserInfoInterface.CopyExternalUserInfoByAccountId(options, out outExternalUserInfo);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            Dictionary external_account_info_dict = new Dictionary(){
                {"account_type", (int)outExternalUserInfo.AccountType},
                {"account_id", outExternalUserInfo.AccountId},
                {"display_name", outExternalUserInfo.DisplayName},
            };
            ret.Add("external_user_info", external_account_info_dict);
        }
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
        ExternalUserInfo outExternalUserInfo;
        Result res = s_UserInfoInterface.CopyExternalUserInfoByAccountType(options, out outExternalUserInfo);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            Dictionary external_account_info_dict = new Dictionary(){
                {"account_type", (int)outExternalUserInfo.AccountType},
                {"account_id", outExternalUserInfo.AccountId},
                {"display_name", outExternalUserInfo.DisplayName},
            };
            ret.Add("external_user_info", external_account_info_dict);
        }
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
        ExternalUserInfo outExternalUserInfo;
        Result res = s_UserInfoInterface.CopyExternalUserInfoByIndex(options, out outExternalUserInfo);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            Dictionary external_account_info_dict = new Dictionary(){
                {"account_type", (int)outExternalUserInfo.AccountType},
                {"account_id", outExternalUserInfo.AccountId},
                {"display_name", outExternalUserInfo.DisplayName},
            };
            ret.Add("external_user_info", external_account_info_dict);
        }
        return ret;
    }

    public Dictionary user_info_interface_copy_user_info(Reference p_options)
    {
        var options = new CopyUserInfoOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        UserInfoData outUserInfoData;
        Result res = s_UserInfoInterface.CopyUserInfo(options, out outUserInfoData);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            Dictionary user_info_dict = new Dictionary(){
                {"user_id", outUserInfoData.UserId.ToString()},
            };
            if (outUserInfoData.Country != null) user_info_dict.Add("country", outUserInfoData.Country);
            else user_info_dict.Add("country", "");
            if (outUserInfoData.DisplayName != null) user_info_dict.Add("display_name", outUserInfoData.DisplayName);
            else user_info_dict.Add("display_name", "");
            if (outUserInfoData.PreferredLanguage != null) user_info_dict.Add("preferred_language", outUserInfoData.PreferredLanguage);
            else user_info_dict.Add("preferred_language", "");
            if (outUserInfoData.Nickname != null) user_info_dict.Add("nickname", outUserInfoData.Nickname);
            else user_info_dict.Add("nickname", "");
            ret.Add("user_info", user_info_dict);
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
        uint count = s_UserInfoInterface.GetExternalUserInfoCount(options);
        return (int)count;
    }

    public void user_info_interface_query_user_info(Reference p_options)
    {
        var options = new QueryUserInfoOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            TargetUserId = EpicAccountId.FromString((string)p_options.Get("target_user_id")),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_UserInfoInterface.QueryUserInfo(options, client_data, (QueryUserInfoCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("target_user_id", data.TargetUserId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_UserInfoInterface.QueryUserInfoByDisplayName(options, client_data, (QueryUserInfoByDisplayNameCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("target_user_id", data.TargetUserId.ToString());
                ret.Add("display_name", String.Copy(data.DisplayName));
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_UserInfoInterface.QueryUserInfoByExternalAccount(options, client_data, (QueryUserInfoByExternalAccountCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("target_user_id", data.TargetUserId.ToString());
                ret.Add("external_account_id", data.ExternalAccountId);
                ret.Add("account_type", (int)data.AccountType);
            }
            EmitSignal(nameof(user_info_interface_query_user_info_by_external_account_callback), ret);
        });
    }



    // ------------------------
    // Ecom Interface
    // ------------------------
    public void ecom_interface_checkout(Reference p_options)
    {
        CheckoutEntry[] p_entries = ((System.Collections.IEnumerable)p_options.Get("entries")).Cast<Dictionary>().Select(x => new CheckoutEntry()
        {
            OfferId = (string)x["offer_id"]
        }).ToArray();
        var options = new CheckoutOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            Entries = p_entries
        };
        if (p_options.Get("override_catalog_namespace") != null) options.OverrideCatalogNamespace = (string)p_options.Get("override_catalog_namespace");

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_EcomInterface.Checkout(options, client_data, (CheckoutCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode}
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("transaction_id", data.TransactionId.ToString());
            }
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
        Entitlement outEntitlement;
        Result res = s_EcomInterface.CopyEntitlementById(options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var entitlement_dict = new Dictionary(){
                {"entitlement_name", outEntitlement.EntitlementName},
                {"entitlement_id", outEntitlement.EntitlementId},
                {"catalog_item_id", outEntitlement.CatalogItemId},
                {"server_index", outEntitlement.ServerIndex},
                {"redeemed", outEntitlement.Redeemed},
                {"end_timestamp", (int) outEntitlement.EndTimestamp},
            };
            ret.Add("entitlement", entitlement_dict);
        }
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
        Entitlement outEntitlement;
        Result res = s_EcomInterface.CopyEntitlementByIndex(options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var entitlement_dict = new Dictionary(){
                {"entitlement_name", outEntitlement.EntitlementName},
                {"entitlement_id", outEntitlement.EntitlementId},
                {"catalog_item_id", outEntitlement.CatalogItemId},
                {"server_index", outEntitlement.ServerIndex},
                {"redeemed", outEntitlement.Redeemed},
                {"end_timestamp", (int) outEntitlement.EndTimestamp},
            };
            ret.Add("entitlement", entitlement_dict);
        }
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
        Entitlement outEntitlement;
        Result res = s_EcomInterface.CopyEntitlementByNameAndIndex(options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var entitlement_dict = new Dictionary(){
                {"entitlement_name", outEntitlement.EntitlementName},
                {"entitlement_id", outEntitlement.EntitlementId},
                {"catalog_item_id", outEntitlement.CatalogItemId},
                {"server_index", outEntitlement.ServerIndex},
                {"redeemed", outEntitlement.Redeemed},
                {"end_timestamp", (int) outEntitlement.EndTimestamp},
            };
            ret.Add("entitlement", entitlement_dict);
        }
        return ret;
    }

    public Dictionary ecom_interface_copy_item_by_id(Reference p_options)
    {
        var options = new CopyItemByIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id")
        };
        CatalogItem outItem;
        Result res = s_EcomInterface.CopyItemById(options, out outItem);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var item_dict = new Dictionary(){
                {"catalog_namespace", outItem.CatalogNamespace},
                {"id", outItem.Id},
                {"entitlement_name", outItem.EntitlementName},
                {"title_text", outItem.TitleText},
                {"description_text", outItem.DescriptionText},
                {"long_description_text", outItem.LongDescriptionText},
                {"developer_text", outItem.DeveloperText},
                {"item_type", (int)outItem.ItemType},
                {"entitlement_end_timestamp", (int)outItem.EntitlementEndTimestamp},
            };
            ret.Add("item", item_dict);
        }
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
        KeyImageInfo outImageInfo;
        Result res = s_EcomInterface.CopyItemImageInfoByIndex(options, out outImageInfo);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var image_info_dict = new Dictionary(){
                {"type", outImageInfo.Type},
                {"url", outImageInfo.Url},
                {"width", (int)outImageInfo.Width},
                {"height", (int)outImageInfo.Height},
            };
            ret.Add("image_info", image_info_dict);
        }
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
        CatalogRelease outImageInfo;
        Result res = s_EcomInterface.CopyItemReleaseByIndex(options, out outImageInfo);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var release_dict = new Dictionary(){
                {"release_note", outImageInfo.ReleaseNote},
                {"compatible_app_ids", outImageInfo.CompatibleAppIds},
                {"compatible_platforms", outImageInfo.CompatiblePlatforms},
            };
            ret.Add("release", release_dict);
        }
        return ret;
    }

    public Dictionary ecom_interface_copy_offer_by_id(Reference p_options)
    {
        var options = new CopyOfferByIdOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
        };
        CatalogOffer outOffer;
        Result res = s_EcomInterface.CopyOfferById(options, out outOffer);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var offer_dict = new Dictionary(){
                {"server_index", outOffer.ServerIndex},
                {"catalog_namespace", outOffer.CatalogNamespace},
                {"id", outOffer.Id},
                {"title_text", outOffer.TitleText},
                {"description_text", outOffer.DescriptionText},
                {"long_description_text", outOffer.LongDescriptionText},
                {"currency_code", outOffer.CurrencyCode},
                {"price_result", (int)outOffer.PriceResult},
                {"discount_percentage", outOffer.DiscountPercentage},
                {"expiration_timestamp", outOffer.ExpirationTimestamp},
                {"purchased_count", outOffer.PurchasedCount},
                {"purchased_limit", outOffer.PurchaseLimit},
                {"available_for_purchase", outOffer.AvailableForPurchase},
                {"original_price", outOffer.OriginalPrice64},
                {"current_price", outOffer.CurrentPrice64},
                {"decimal_point", outOffer.DecimalPoint},
            };
            ret.Add("offer", offer_dict);
        }
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
        CatalogOffer outOffer;
        Result res = s_EcomInterface.CopyOfferByIndex(options, out outOffer);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var offer_dict = new Dictionary(){
                {"server_index", outOffer.ServerIndex},
                {"catalog_namespace", outOffer.CatalogNamespace},
                {"id", outOffer.Id},
                {"title_text", outOffer.TitleText},
                {"description_text", outOffer.DescriptionText},
                {"long_description_text", outOffer.LongDescriptionText},
                {"currency_code", outOffer.CurrencyCode},
                {"price_result", (int)outOffer.PriceResult},
                {"discount_percentage", outOffer.DiscountPercentage},
                {"expiration_timestamp", outOffer.ExpirationTimestamp},
                {"purchased_count", outOffer.PurchasedCount},
                {"purchased_limit", outOffer.PurchaseLimit},
                {"available_for_purchase", outOffer.AvailableForPurchase},
                {"original_price", outOffer.OriginalPrice64},
                {"current_price", outOffer.CurrentPrice64},
                {"decimal_point", outOffer.DecimalPoint},
            };
            ret.Add("offer", offer_dict);
        }
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
        KeyImageInfo outImageInfo;
        Result res = s_EcomInterface.CopyOfferImageInfoByIndex(options, out outImageInfo);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var image_info_dict = new Dictionary(){
                {"type", outImageInfo.Type},
                {"url", outImageInfo.Url},
                {"width", (int)outImageInfo.Width},
                {"height", (int)outImageInfo.Height},
            };
            ret.Add("image_info", image_info_dict);
        }
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
        CatalogItem outItem;
        Result res = s_EcomInterface.CopyOfferItemByIndex(options, out outItem);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var item_dict = new Dictionary(){
                {"catalog_namespace", outItem.CatalogNamespace},
                {"id", outItem.Id},
                {"entitlement_name", outItem.EntitlementName},
                {"title_text", outItem.TitleText},
                {"description_text", outItem.DescriptionText},
                {"long_description_text", outItem.LongDescriptionText},
                {"developer_text", outItem.DeveloperText},
                {"item_type", (int)outItem.ItemType},
                {"entitlement_end_timestamp", (int)outItem.EntitlementEndTimestamp},
            };
            ret.Add("item", item_dict);
        }
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
        Result res = s_EcomInterface.CopyTransactionById(options, out outTransaction);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var transaction_wrapper = new TransactionWrapper(outTransaction);
            ret.Add("transaction", transaction_wrapper);
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
        Result res = s_EcomInterface.CopyTransactionByIndex(options, out outTransaction);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var transaction_wrapper = new TransactionWrapper(outTransaction);
            ret.Add("transaction", transaction_wrapper);
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
        return (int)s_EcomInterface.GetEntitlementsByNameCount(options);
    }

    public int ecom_interface_get_entitlements_count(Reference p_options)
    {
        var options = new GetEntitlementsCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return (int)s_EcomInterface.GetEntitlementsCount(options);
    }

    public int ecom_interface_get_item_image_info_count(Reference p_options)
    {
        var options = new GetItemImageInfoCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id"),
        };
        return (int)s_EcomInterface.GetItemImageInfoCount(options);
    }

    public int ecom_interface_get_item_release_count(Reference p_options)
    {
        var options = new GetItemReleaseCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            ItemId = (string)p_options.Get("item_id"),
        };
        return (int)s_EcomInterface.GetItemReleaseCount(options);
    }

    public int ecom_interface_get_offer_count(Reference p_options)
    {
        var options = new GetOfferCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return (int)s_EcomInterface.GetOfferCount(options);
    }

    public int ecom_interface_get_offer_image_info_count(Reference p_options)
    {
        var options = new GetOfferImageInfoCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
        };
        return (int)s_EcomInterface.GetOfferImageInfoCount(options);
    }

    public int ecom_interface_get_offer_item_count(Reference p_options)
    {
        var options = new GetOfferItemCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            OfferId = (string)p_options.Get("offer_id"),
        };
        return (int)s_EcomInterface.GetOfferItemCount(options);
    }

    public int ecom_interface_get_transaction_count(Reference p_options)
    {
        var options = new GetTransactionCountOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return (int)s_EcomInterface.GetTransactionCount(options);
    }

    public void ecom_interface_query_entitlements(Reference p_options)
    {
        var options = new QueryEntitlementsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementNames = ((System.Collections.IEnumerable)p_options.Get("entitlement_names")).Cast<string>().ToArray(),
            IncludeRedeemed = (bool)p_options.Get("include_redeemed"),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_EcomInterface.QueryEntitlements(options, client_data, (QueryEntitlementsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_EcomInterface.QueryOffers(options, client_data, (QueryOffersCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
            EmitSignal(nameof(ecom_interface_query_offers_callback), ret);
        });
    }

    public void ecom_interface_query_ownership(Reference p_options)
    {
        var options = new QueryOwnershipOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            CatalogItemIds = ((System.Collections.IEnumerable)p_options.Get("catalog_item_ids")).Cast<string>().ToArray(),
        };
        if (p_options.Get("catalog_namespace") != null) options.CatalogNamespace = (string)p_options.Get("catalog_namespace");

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_EcomInterface.QueryOwnership(options, client_data, (QueryOwnershipCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("item_ownership",
                    data.ItemOwnership.Select(x => new Dictionary(){
                        {"id", x.Id},
                        {"ownership_status", (int)x.OwnershipStatus},
                    }).ToArray()
                );
            }
            EmitSignal(nameof(ecom_interface_query_ownership_callback), ret);
        });
    }

    public void ecom_interface_query_ownership_token(Reference p_options)
    {
        var options = new QueryOwnershipTokenOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            CatalogItemIds = ((System.Collections.IEnumerable)p_options.Get("catalog_item_ids")).Cast<string>().ToArray(),
        };
        if (p_options.Get("catalog_namespace") != null) options.CatalogNamespace = (string)p_options.Get("catalog_namespace");

        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_EcomInterface.QueryOwnershipToken(options, client_data, (QueryOwnershipTokenCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("ownership_token", data.OwnershipToken);
            }
            EmitSignal(nameof(ecom_interface_query_ownership_token_callback), ret);
        });
    }

    public void ecom_interface_redeem_entitlements(Reference p_options)
    {
        var options = new RedeemEntitlementsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
            EntitlementIds = ((System.Collections.IEnumerable)p_options.Get("entitlement_ids")).Cast<string>().ToArray(),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_EcomInterface.RedeemEntitlements(options, client_data, (RedeemEntitlementsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
            EmitSignal(nameof(ecom_interface_redeem_entitlements_callback), ret);
        });
    }


    // ------------------------
    // UI Interface
    // ------------------------
    public int ui_interface_acknowledge_event_id(Reference p_options)
    {
        int p_event_id = (int)p_options.Get("ui_event_id");
        int p_result = (int)p_options.Get("result");
        var options = new AcknowledgeEventIdOptions()
        {
            UiEventId = (ulong)p_event_id,
            Result = (Result)p_result
        };
        return (int)s_UIInterface.AcknowledgeEventId(options);
    }

    public bool ui_interface_get_friends_visible(Reference p_options)
    {
        var options = new GetFriendsVisibleOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        return s_UIInterface.GetFriendsVisible(options);
    }

    public int ui_interface_get_notification_location_preference()
    {
        return (int)s_UIInterface.GetNotificationLocationPreference();
    }

    public int ui_interface_get_toggle_friends_key(Reference p_options)
    {
        return (int)s_UIInterface.GetToggleFriendsKey(new GetToggleFriendsKeyOptions());
    }

    public void ui_interface_hide_friends(Reference p_options)
    {
        var options = new HideFriendsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_UIInterface.HideFriends(options, client_data, (HideFriendsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
            EmitSignal(nameof(ui_interface_hide_friends_callback), ret);
        });
    }

    public bool ui_interface_is_valid_key_combination(int p_key_combination)
    {
        return s_UIInterface.IsValidKeyCombination(
            (KeyCombination)p_key_combination
        );
    }

    public int ui_interface_set_display_preference(Reference p_options)
    {
        int p_notification_location = (int)p_options.Get("notification_location");
        var options = new SetDisplayPreferenceOptions()
        {
            NotificationLocation = (NotificationLocation)p_notification_location
        };
        return (int)s_UIInterface.SetDisplayPreference(options);
    }

    public int ui_interface_set_toggle_friends_key(Reference p_options)
    {
        var options = new SetToggleFriendsKeyOptions()
        {
            KeyCombination = (KeyCombination)p_options.Get("key_combination")
        };
        return (int)s_UIInterface.SetToggleFriendsKey(options);
    }

    public void ui_interface_show_friends(Reference p_options)
    {
        var options = new ShowFriendsOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");
        s_UIInterface.ShowFriends(options, client_data, (ShowFriendsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
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
        PermissionStatus outPermission;
        var res = s_KWSInterface.CopyPermissionByIndex(options, out outPermission);
        var ret = new Dictionary(){
            {"result_code", res},
        };
        if (res == Result.Success)
        {
            var permission_dict = new Dictionary(){
                {"name", outPermission.Name},
                {"status", (int)outPermission.Status}
            };
            ret.Add("permission", permission_dict);
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_KWSInterface.CreateUser(options, client_data, (Epic.OnlineServices.KWS.CreateUserCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("kws_user_id", data.KWSUserId);
                ret.Add("is_minor", data.IsMinor);
            }
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
        KWSPermissionStatus outPermission;
        var res = s_KWSInterface.GetPermissionByKey(options, out outPermission);
        var ret = new Dictionary(){
            {"result_code", res},
        };
        if (res == Result.Success)
        {
            ret.Add("permission", (int)outPermission);
        }
        return ret;
    }

    public int kws_interface_get_permissions_count(Reference p_options)
    {
        var options = new GetPermissionsCountOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        return s_KWSInterface.GetPermissionsCount(options);
    }

    public void kws_interface_query_age_gate(Reference p_options)
    {
        var options = new QueryAgeGateOptions()
        {
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_KWSInterface.QueryAgeGate(options, client_data, (QueryAgeGateCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("country_code", data.CountryCode);
                ret.Add("age_of_consent", data.AgeOfConsent);
            }
            EmitSignal(nameof(kws_interface_query_age_gate_callback), ret);
        });
    }

    public void kws_interface_query_permissions(Reference p_options)
    {
        var options = new QueryPermissionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_KWSInterface.QueryPermissions(options, client_data, (QueryPermissionsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
                ret.Add("kws_user_id", data.KWSUserId);
                ret.Add("date_of_birth", data.DateOfBirth);
                ret.Add("is_minor", data.IsMinor);
            }
            EmitSignal(nameof(kws_interface_query_permissions_callback), ret);
        });
    }

    public void kws_interface_request_permissions(Reference p_options)
    {
        var options = new RequestPermissionsOptions()
        {
            LocalUserId = ProductUserId.FromString((string)p_options.Get("local_user_id")),
            PermissionKeys = ((System.Collections.IEnumerable)p_options.Get("permission_keys")).Cast<string>().ToArray(),
        };
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_KWSInterface.RequestPermissions(options, client_data, (RequestPermissionsCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
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
        object client_data = null;
        if (p_options.Get("client_data") != null) client_data = p_options.Get("client_data");

        s_KWSInterface.UpdateParentEmail(options, client_data, (UpdateParentEmailCallbackInfo data) =>
        {
            var ret = new Dictionary(){
                {"result_code", data.ResultCode},
            };
            if (data.ClientData != null) ret.Add("client_data", data.ClientData);
            if (data.ResultCode == Result.Success)
            {
                ret.Add("local_user_id", data.LocalUserId.ToString());
            }
            EmitSignal(nameof(kws_interface_update_parent_email_callback), ret);
        });
    }


    // ------------------------
    // Metrics Interface
    // ------------------------
    public int metrics_interface_begin_player_session(Reference p_options)
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

        return (int)s_MetricsInterface.BeginPlayerSession(options);
    }
    public int metrics_interface_end_player_session(Reference p_options)
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
        return (int)s_MetricsInterface.EndPlayerSession(options);
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
                s_PlatformInterface = null;
                PlatformInterface.Shutdown();
            }
        }
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
}

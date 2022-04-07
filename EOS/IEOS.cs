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
    public delegate void auth_interface_verify_user_auth_callback(Dictionary callback_data);
    // [Signal]
    // public delegate void auth_interface_link_account_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_login_callback(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_auth_expiration(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_login_status_changed(Dictionary callback_data);
    [Signal]
    public delegate void connect_interface_create_device_id_callback(Dictionary callback_data);

    // ------------------------
    // Public Functions
    // ------------------------

    // -----
    // Logging Interface
    // -----
    [Signal]
    public delegate void achievements_interface_achievements_unlocked_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_query_definitions_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_query_player_achievements_complete_callback(Dictionary callback_data);
    [Signal]
    public delegate void achievements_interface_unlock_achievements_complete_callback(Dictionary callback_data);

    public Result logging_interface_set_log_level(int category, int level)
    {
        return LoggingInterface.SetLogLevel((LogCategory)category, (LogLevel)level);
    }
    // -----
    // Platform Interface
    // -----
    public Result platform_interface_initialize(Reference p_initialize_options)
    {
        // Initialize the EOS SDK
        var initializeOptions = new InitializeOptions()
        {
            ProductName = (string)p_initialize_options.Get("product_name"),
            ProductVersion = (string)p_initialize_options.Get("product_version"),
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

        return true;
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
    public void auth_interface_login(Reference p_login_options)
    {
        var p_credentials = (Reference)p_login_options.Get("credentials");

        var loginOptions = new Epic.OnlineServices.Auth.LoginOptions()
        {
            Credentials = new Epic.OnlineServices.Auth.Credentials()
            {
                Type = (LoginCredentialType)p_credentials.Get("type"),
                Id = (string)p_credentials.Get("id"),
                Token = (string)p_credentials.Get("token"),
            },
            ScopeFlags = (AuthScopeFlags)p_login_options.Get("scope_flags"),
        };

        object client_data = null;
        if (p_login_options.Get("client_data") != null)
        {
            client_data = p_login_options.Get("client_data");
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
                ret.Add("continuance_token", loginCallbackInfo.ContinuanceToken.ToString());
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
    public void auth_interface_logout(Reference p_logout_options)
    {
        var logoutOptions = new LogoutOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_logout_options.Get("local_user_id"))
        };

        object client_data = null;
        if (p_logout_options.Get("client_data") != null)
        {
            client_data = p_logout_options.Get("client_data");
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
    public Dictionary auth_interface_copy_id_token(Reference p_copy_id_token_options)
    {
        var options = new Epic.OnlineServices.Auth.CopyIdTokenOptions()
        {
            AccountId = EpicAccountId.FromString((string)p_copy_id_token_options.Get("account_id")),
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
    public Dictionary auth_interface_copy_user_auth_token(Reference p_copy_user_auth_token_options, string p_local_user_id)
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
    public void auth_interface_delete_persistent_auth(Reference p_delete_persistent_auth_options)
    {
        var options = new DeletePersistentAuthOptions()
        {
            RefreshToken = (string)p_delete_persistent_auth_options.Get("refresh_token")
        };
        object client_data = null;
        if (p_delete_persistent_auth_options.Get("client_data") != null)
        {
            client_data = p_delete_persistent_auth_options.Get("client_data");
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
    public void auth_interface_query_id_token(Reference p_query_id_token_options)
    {
        var options = new QueryIdTokenOptions()
        {
            LocalUserId = EpicAccountId.FromString((string)p_query_id_token_options.Get("local_user_id")),
            TargetAccountId = EpicAccountId.FromString((string)p_query_id_token_options.Get("target_account_id")),
        };

        object client_data = null;
        if (p_query_id_token_options.Get("client_data") != null)
        {
            client_data = p_query_id_token_options.Get("client_data");
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
    public void auth_interface_verify_id_token(Reference p_verify_id_token_options)
    {
        var id_token = (Reference)p_verify_id_token_options.Get("id_token");

        var options = new Epic.OnlineServices.Auth.VerifyIdTokenOptions()
        {
            IdToken = new Epic.OnlineServices.Auth.IdToken()
            {
                AccountId = EpicAccountId.FromString((string)id_token.Get("account_id")),
                JsonWebToken = (string)id_token.Get("json_web_token")
            }
        };
        object client_data = null;
        if (p_verify_id_token_options.Get("client_data") != null)
        {
            client_data = p_verify_id_token_options.Get("client_data");
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
        });

    }
    // public void auth_interface_link_account(Reference p_link_account_options)
    // {

    //     object client_data = null;
    //     if (p_link_account_options.Get("client_data") != null)
    //     {
    //         client_data = p_link_account_options.Get("client_data");
    //     }


    //     var linkAccountOptions = new LinkAccountOptions()
    //     {
    //         LinkAccountFlags = (LinkAccountFlags)p_link_account_options.Get("link_account_flags"),
    //         // TODO: how to make ContinuanceToken?
    //     };
    //     if (p_link_account_options.Get("local_user_id") == null)
    //     {
    //         linkAccountOptions.LocalUserId = null;
    //     }
    //     else
    //     {
    //         linkAccountOptions.LocalUserId = EpicAccountId.FromString((string)p_link_account_options.Get("local_user_id"));
    //     }
    //     authInterface.LinkAccount(linkAccountOptions, client_data, (LinkAccountCallbackInfo data) =>
    //     {
    //         Dictionary<string, object> ret = new Dictionary<string, object>(){
    //             {"result_code", data.ResultCode},
    //         };
    //         if (data.ClientData != null)
    //         {
    //             ret.Add("client_data", data.ClientData);
    //         }
    //         if (data.LocalUserId != null)
    //         {
    //             ret.Add("local_user_id", data.LocalUserId.ToString());
    //         }
    //         if (data.SelectedAccountId != null)
    //         {
    //             ret.Add("selected_account_id", data.SelectedAccountId.ToString());
    //         }
    //         if (data.PinGrantInfo != null)
    //         {
    //             Dictionary pin_grant_info = new Dictionary();
    //             pin_grant_info.Add("user_code", data.PinGrantInfo.UserCode);
    //             pin_grant_info.Add("verification_uri", data.PinGrantInfo.VerificationURI);
    //             pin_grant_info.Add("expires_in", data.PinGrantInfo.ExpiresIn);
    //             pin_grant_info.Add("verification_uri_complete", data.PinGrantInfo.VerificationURIComplete);
    //             ret.Add("pin_grant_info", pin_grant_info);
    //         }

    //         EmitSignal(nameof(auth_interface_link_account_callback), ret);
    //     });
    // }
    public void auth_interface_verify_user_auth(Reference p_verify_user_auth_options)
    {
        var p_auth_token = (Reference)p_verify_user_auth_options.Get("auth_token");
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
        if (p_verify_user_auth_options.Get("client_data") != null)
        {
            client_data = p_verify_user_auth_options.Get("client_data");
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
    public void connect_interface_login(Reference p_login_options)
    {
        var p_credentials = (Reference)p_login_options.Get("credentials");

        var loginOptions = new Epic.OnlineServices.Connect.LoginOptions()
        {
            Credentials = new Epic.OnlineServices.Connect.Credentials()
            {
                Token = (string)p_credentials.Get("token"),
                Type = (ExternalCredentialType)p_credentials.Get("type"),
            }
        };
        if (p_login_options.Get("user_login_info") != null)
        {
            Reference p_user_login_info = (Reference)p_login_options.Get("user_login_info");
            var userLoginInfo = new UserLoginInfo()
            {
                DisplayName = (string)p_user_login_info.Get("display_name"),
            };
            loginOptions.UserLoginInfo = userLoginInfo;
        }

        object client_data = null;
        if (p_login_options.Get("client_data") != null)
        {
            client_data = p_login_options.Get("client_data");
        }

        s_ConnectInterface.Login(loginOptions, client_data, (Epic.OnlineServices.Connect.LoginCallbackInfo loginCallbackInfo) =>
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
                ret.Add("continuance_token", loginCallbackInfo.ContinuanceToken.ToString());
            }
            if (loginCallbackInfo.LocalUserId != null)
            {
                ret.Add("local_user_id", loginCallbackInfo.LocalUserId.ToString());
            }
            if (loginCallbackInfo.ResultCode == Result.Success)
            {
                ret.Add("sucess", true);
            }
            else if (Common.IsOperationComplete(loginCallbackInfo.ResultCode))
            {
                ret.Add("sucess", false);
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
        int p_external_account_id_type = (int)p_options.Get("account_id_type");
        var options = new CopyProductUserExternalAccountByAccountTypeOptions()
        {
            TargetUserId = ProductUserId.FromString((string)p_options.Get("target_user_id")),
            AccountIdType = (ExternalAccountType)p_external_account_id_type
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
        string[] p_achievement_ids = ((System.Collections.IEnumerable)p_options.Get("achievement_ids")).Cast<object>()
                                 .Select(x => x.ToString())
                                 .ToArray();

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
    // Internal Ovverides
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
                s_PlatformInterface = null;
                PlatformInterface.Shutdown();
            }
        }
    }


    // ------------------------
    // Privates
    // ------------------------
    private static PlatformInterface s_PlatformInterface;
    private static AuthInterface s_AuthInterface;
    private static ConnectInterface s_ConnectInterface;
    private static AchievementsInterface s_AchievementsInterface;
}

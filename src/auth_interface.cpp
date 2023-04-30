// #include "helpers/account_helpers.h"
#include "ieos.h"
using namespace godot;

void IEOS::auth_interface_login(Ref<RefCounted> p_options) {
    Ref<RefCounted> p_credentials = p_options->get("credentials");

    EOS_Auth_Credentials credentials;
    memset(&credentials, 0, sizeof(credentials));
    credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
    credentials.Type = static_cast<EOS_ELoginCredentialType>((int)p_credentials->get("type"));
    String p_id = p_credentials->get("id");
    CharString id = p_id.utf8();
    if (!p_id.is_empty()) {
        credentials.Id = id.get_data();
    }
    String p_token = p_credentials->get("token");
    CharString token = p_token.utf8();
    if (!p_token.is_empty()) {
        credentials.Token = token.get_data();
    }
    int p_external_type = p_credentials->get("external_type");
    if (p_external_type != -1) {
        credentials.ExternalType = static_cast<EOS_EExternalCredentialType>(p_external_type);
    }

    EOS_Auth_LoginOptions loginOptions;
    memset(&loginOptions, 0, sizeof(loginOptions));
    loginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
    loginOptions.Credentials = &credentials;
    loginOptions.ScopeFlags = static_cast<EOS_EAuthScopeFlags>((int)p_options->get("scope_flags"));

    Variant* client_data = new Variant(p_options->get("client_data"));
    EOS_Auth_Login(s_authInterface, &loginOptions, (void*)&client_data, [](const EOS_Auth_LoginCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = (int)data->ResultCode;
        Variant* client_data = reinterpret_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        ret["local_user_id"] = "";
        if (data->LocalUserId != nullptr) {
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        }
        ret["pin_grant_info"] = nullptr;
        if (data->PinGrantInfo != nullptr) {
            ret["pin_grant_info"] = eosg_auth_pin_grant_info_to_dict(data->PinGrantInfo);
        }
        ret["continuance_token"] = nullptr;
        if (data->ContinuanceToken != nullptr) {
            Ref<ContinuanceTokenEOSG> continuance_token = memnew(ContinuanceTokenEOSG());
            continuance_token->set_token(data->ContinuanceToken);
            ret["continuance_token"] = continuance_token;
        }
        ret["account_feature_restricted_info"] = nullptr;
        if (data->AccountFeatureRestrictedInfo != nullptr) {
            ret["account_feature_restricted_info"] = eosg_auth_account_feature_restricted_info_to_dict(data->AccountFeatureRestrictedInfo);
        }
        ret["selected_account_id"] = "";
        if (data->SelectedAccountId != nullptr) {
            ret["selected_account_id"] = eosg_epic_account_id_to_string(data->SelectedAccountId);
        }

        if (data->ResultCode == EOS_EResult::EOS_Success) {
            ret["pending"] = false;
            ret["success"] = true;
        } else if (EOS_EResult_IsOperationComplete(data->ResultCode)) {
            ret["pending"] = false;
            ret["success"] = false;
        } else {
            ret["pending"] = true;
            ret["success"] = false;
        }
        IEOS::get_singleton()->emit_signal("auth_interface_login_callback", ret);
        // delete client_data;
    });
    return;
}

void IEOS::auth_interface_logout(Ref<RefCounted> p_options) {
    EOS_Auth_LogoutOptions logoutOptions;
    memset(&logoutOptions, 0, sizeof(logoutOptions));
    Variant client_data = p_options->get("client_data");
    EOS_Auth_Logout(s_authInterface, &logoutOptions, (void*)client_data, [](const EOS_Auth_LogoutCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = (int)data->ResultCode;
        Variant client_data = *static_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        ret["local_user_id"] = "";
        if (data->LocalUserId != nullptr) {
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        }
        IEOS::get_singleton()->emit_signal("auth_interface_logout_callback", ret);
    });
    return;
}

Dictionary IEOS::auth_interface_copy_id_token(Ref<RefCounted> p_options) {
    EOS_Auth_CopyIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
    CharString account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));
    options.AccountId = eosg_string_to_epic_account_id(account_id.get_data());
    EOS_Auth_IdToken* outToken = nullptr;
    EOS_EResult res = EOS_Auth_CopyIdToken(s_authInterface, &options, &outToken);

    Dictionary ret;
    ret["result_code"] = (int)res;
    ret["id_token"] = nullptr;
    if (outToken != nullptr) {
        Dictionary idToken;
        idToken["account_id"] = eosg_epic_account_id_to_string(outToken->AccountId);
        idToken["json_web_token"] = String(outToken->JsonWebToken);
        EOS_Auth_IdToken_Release(outToken);
        ret["id_token"] = idToken;
    }

    return ret;
}

Dictionary IEOS::auth_interface_copy_user_auth_token(Ref<RefCounted> p_options, const String& p_local_user_id) {
    EOS_Auth_CopyUserAuthTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
    CharString local_user_id = p_local_user_id.utf8();
    EOS_Auth_Token* outToken = nullptr;
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EResult res = EOS_Auth_CopyUserAuthToken(s_authInterface, &options, localUserId, &outToken);

    Dictionary ret;
    ret["result_code"] = (int)res;
    ret["token"] = nullptr;
    if (outToken != nullptr) {
        Dictionary authToken;
        authToken["app"] = String(outToken->App);
        authToken["client_id"] = String(outToken->ClientId);
        authToken["account_id"] = eosg_epic_account_id_to_string(outToken->AccountId);
        authToken["access_token"] = String(outToken->AccessToken);
        authToken["expires_in"] = outToken->ExpiresIn;
        authToken["expires_at"] = String(outToken->ExpiresAt);
        authToken["auth_type"] = static_cast<int>(outToken->AuthType);
        authToken["refresh_token"] = String(outToken->RefreshToken);
        authToken["refresh_expires_in"] = outToken->RefreshExpiresIn;
        authToken["refresh_expires_at"] = String(outToken->RefreshExpiresAt);
        EOS_Auth_Token_Release(outToken);
        ret["token"] = authToken;
    }

    return ret;
}

void IEOS::auth_interface_delete_persistent_auth(Ref<RefCounted> p_options) {
    EOS_Auth_DeletePersistentAuthOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
    String p_refresh_token = p_options->get("refresh_token");
    CharString refresh_token = p_refresh_token.utf8();
    options.RefreshToken = refresh_token.get_data();

    Variant client_data = p_options->get("client_data");
    EOS_Auth_DeletePersistentAuth(s_authInterface, &options, (void*)client_data, [](const EOS_Auth_DeletePersistentAuthCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = (int)data->ResultCode;
        Variant client_data = *static_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        IEOS::get_singleton()->emit_signal("auth_interface_delete_persistent_auth_callback", ret);
        return;
    });
}

String IEOS::auth_interface_get_logged_in_account_by_index(int index) {
    EOS_EpicAccountId accountId = EOS_Auth_GetLoggedInAccountByIndex(s_authInterface, index);
    return eosg_epic_account_id_to_string(accountId);
}

int IEOS::auth_interface_get_logged_in_accounts_count() {
    return static_cast<int>(EOS_Auth_GetLoggedInAccountsCount(s_authInterface));
}

int IEOS::auth_interface_get_login_status(const String& p_local_user_id) {
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId accountId = eosg_string_to_epic_account_id(local_user_id.get_data());
    return static_cast<int>(EOS_Auth_GetLoginStatus(s_authInterface, accountId));
}

String IEOS::auth_interface_get_merged_account_by_index(const String& p_local_user_id, int index) {
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EpicAccountId accountId = EOS_Auth_GetMergedAccountByIndex(s_authInterface, localUserId, index);
    return eosg_epic_account_id_to_string(accountId);
}

int IEOS::auth_interface_get_merged_accounts_count(const String& p_local_user_id) {
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    return static_cast<int>(EOS_Auth_GetMergedAccountsCount(s_authInterface, localUserId));
}

Dictionary IEOS::auth_interface_get_selected_account_id(const String& p_local_user_id) {
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EpicAccountId outSelectedAccountId = nullptr;
    EOS_EResult res = EOS_Auth_GetSelectedAccountId(s_authInterface, localUserId, &outSelectedAccountId);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["selected_account_id"] = "";
    if (outSelectedAccountId != nullptr) {
        ret["selected_account_id"] = eosg_epic_account_id_to_string(outSelectedAccountId);
    }

    return ret;
}

void IEOS::auth_interface_query_id_token(Ref<RefCounted> p_options) {
    EOS_Auth_QueryIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_QUERYIDTOKEN_API_LATEST;

    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_account_id = VARIANT_TO_CHARSTRING(p_options->get("target_account_id"));
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EpicAccountId targetAccountId = eosg_string_to_epic_account_id(target_account_id.get_data());

    options.LocalUserId = localUserId;
    options.TargetAccountId = targetAccountId;

    Variant client_data = p_options->get("client_data");

    EOS_Auth_QueryIdToken(s_authInterface, &options, (void*)client_data, [](const EOS_Auth_QueryIdTokenCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Variant client_data = *static_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        if (data->ResultCode == EOS_EResult::EOS_Success) {
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["target_account_id"] = eosg_epic_account_id_to_string(data->TargetAccountId);
        }
        IEOS::get_singleton()->emit_signal("auth_interface_query_id_token_callback", ret);
        return;
    });

    return;
}

void IEOS::auth_interface_verify_id_token(Ref<RefCounted> p_options) {
    CharString p_account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));
    CharString p_json_web_token = VARIANT_TO_CHARSTRING(p_options->get("json_web_token"));

    EOS_Auth_IdToken idToken;
    memset(&idToken, 0, sizeof(idToken));
    idToken.ApiVersion = EOS_AUTH_IDTOKEN_API_LATEST;
    idToken.AccountId = eosg_string_to_epic_account_id(p_account_id.get_data());
    idToken.JsonWebToken = p_json_web_token.get_data();

    EOS_Auth_VerifyIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_VERIFYIDTOKEN_API_LATEST;
    options.IdToken = &idToken;

    Variant client_data = p_options->get("client_data");
    EOS_Auth_VerifyIdToken(s_authInterface, &options, (void*)client_data, [](const EOS_Auth_VerifyIdTokenCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Variant client_data = *static_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        if (data->ResultCode == EOS_EResult::EOS_Success) {
            ret["application_id"] = String(data->ApplicationId);
            ret["client_id"] = String(data->ClientId);
            ret["product_id"] = String(data->ProductId);
            ret["sandbox_id"] = String(data->SandboxId);
            ret["deployment_id"] = String(data->DeploymentId);
            ret["display_name"] = data->DisplayName;
            ret["is_external_account_info_present"] = data->bIsExternalAccountInfoPresent;
            ret["external_account_id_type"] = static_cast<int>(data->ExternalAccountIdType);
            ret["external_account_id"] = String(data->ExternalAccountId);
            ret["external_account_display_name"] = String(data->ExternalAccountDisplayName);
            ret["platform"] = String(data->Platform);
        }
        IEOS::get_singleton()->emit_signal("auth_interface_verify_id_token_callback", ret);
        return;
    });
    return;
}

void IEOS::auth_interface_link_account(Ref<RefCounted> p_options) {
    Object* continuance_token_obj = p_options->get("continuance_token");
    // Cast to ContinuanceTokenEOSG
    Ref<ContinuanceTokenEOSG> p_continuance_token = Object::cast_to<ContinuanceTokenEOSG>(continuance_token_obj);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Auth_LinkAccountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_LINKACCOUNT_API_LATEST;
    options.LinkAccountFlags = static_cast<EOS_ELinkAccountFlags>((int)p_options->get("link_account_flags"));
    options.ContinuanceToken = p_continuance_token->get_token();
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    Variant client_data = p_options->get("client_data");
    EOS_Auth_LinkAccount(s_authInterface, &options, (void*)client_data, [](const EOS_Auth_LinkAccountCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Variant client_data = *static_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        if (data->ResultCode == EOS_EResult::EOS_Success) {
            ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
            ret["pin_grant_info"] = eosg_auth_pin_grant_info_to_dict(data->PinGrantInfo);
            ret["selected_account_id"] = eosg_epic_account_id_to_string(data->SelectedAccountId);
        }
        IEOS::get_singleton()->emit_signal("auth_interface_link_account_callback", ret);
    });
    return;
}

void IEOS::auth_interface_verify_user_auth(Ref<RefCounted> p_options) {
    Ref<RefCounted> p_auth_token = Object::cast_to<RefCounted>(p_options->get("auth_token"));
    CharString p_app = VARIANT_TO_CHARSTRING(p_auth_token->get("app"));
    CharString p_client_id = VARIANT_TO_CHARSTRING(p_auth_token->get("client_id"));
    CharString p_access_token = VARIANT_TO_CHARSTRING(p_auth_token->get("access_token"));
    CharString p_expires_at = VARIANT_TO_CHARSTRING(p_auth_token->get("expires_at"));
    CharString p_refresh_token = VARIANT_TO_CHARSTRING(p_auth_token->get("refresh_token"));
    CharString p_refresh_expires_at = VARIANT_TO_CHARSTRING(p_auth_token->get("refresh_expires_at"));
    CharString p_account_id = VARIANT_TO_CHARSTRING(p_auth_token->get("account_id"));
    int p_auth_type = p_options->get("auth_type");
    EOS_EpicAccountId accountId = eosg_string_to_epic_account_id(p_account_id.get_data());

    EOS_Auth_Token authToken;
    memset(&authToken, 0, sizeof(authToken));
    authToken.ApiVersion = EOS_AUTH_TOKEN_API_LATEST;
    authToken.App = p_app.get_data();
    authToken.ClientId = p_client_id.get_data();
    authToken.AccessToken = p_access_token.get_data();
    authToken.ExpiresAt = p_expires_at.get_data();
    authToken.RefreshToken = p_refresh_token.get_data();
    authToken.RefreshExpiresAt = p_refresh_expires_at.get_data();
    authToken.AccountId = accountId;
    authToken.AuthType = static_cast<EOS_EAuthTokenType>(p_auth_type);
    authToken.RefreshExpiresIn = static_cast<double>(p_auth_token->get("refresh_expires_in"));
    authToken.ExpiresIn = static_cast<double>(p_auth_token->get("expires_in"));

    EOS_Auth_VerifyUserAuthOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_VERIFYUSERAUTH_API_LATEST;
    options.AuthToken = &authToken;
    Variant client_data = p_options->get("client_data");
    EOS_Auth_VerifyUserAuth(s_authInterface, &options, (void*)client_data, [](const EOS_Auth_VerifyUserAuthCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Variant client_data = *static_cast<Variant*>(data->ClientData);
        ret["client_data"] = client_data;
        IEOS::get_singleton()->emit_signal("auth_interface_verify_user_auth_callback", ret);
    });
    return;
}

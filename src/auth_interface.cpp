#include "ieos.h"
using namespace godot;

void IEOS::auth_interface_login(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
    Ref<RefCounted> p_credentials = p_options->get("credentials");

    EOS_Auth_Credentials credentials;
    memset(&credentials, 0, sizeof(credentials));
    credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
    credentials.Type = static_cast<EOS_ELoginCredentialType>((int)p_credentials->get("type"));
    CharString id = VARIANT_TO_CHARSTRING(p_credentials->get("id"));
    if (id.length() > 0) {
        credentials.Id = id.get_data();
    }
    CharString token = VARIANT_TO_CHARSTRING(p_credentials->get("token"));
    if (token.length() > 0) {
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
    loginOptions.ScopeFlags = static_cast<EOS_EAuthScopeFlags>(static_cast<int>(p_options->get("scope_flags")));
    loginOptions.LoginFlags = static_cast<uint64_t>(p_options->get("login_flags"));
    p_options->reference();

    EOS_Auth_Login(s_authInterface, &loginOptions, (void *)*p_options, [](const EOS_Auth_LoginCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["pin_grant_info"] = eosg_auth_pin_grant_info_to_dict(data->PinGrantInfo);
        ret["continuance_token"] = eosg_continuance_token_to_wrapper(data->ContinuanceToken);
        ret["selected_account_id"] = eosg_epic_account_id_to_string(data->SelectedAccountId);

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
    });
}

void IEOS::auth_interface_logout(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Auth_LogoutOptions logoutOptions;
    memset(&logoutOptions, 0, sizeof(logoutOptions));
    logoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
    logoutOptions.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    p_options->reference();

    EOS_Auth_Logout(s_authInterface, &logoutOptions, (void *)*p_options, [](const EOS_Auth_LogoutCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("auth_interface_logout_callback", ret);
    });
}

Dictionary IEOS::auth_interface_copy_id_token(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_authInterface, {});
    CharString account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));

    EOS_Auth_CopyIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
    options.AccountId = eosg_string_to_epic_account_id(account_id.get_data());

    EOS_Auth_IdToken *outToken = nullptr;
    EOS_EResult res = EOS_Auth_CopyIdToken(s_authInterface, &options, &outToken);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["id_token"] = eosg_auth_id_token_to_dict_and_release(outToken);
    return ret;
}

Dictionary IEOS::auth_interface_copy_user_auth_token(Ref<RefCounted> p_options, const String &p_local_user_id) {
    ERR_FAIL_NULL_V(s_authInterface, {});
    CharString local_user_id = p_local_user_id.utf8();

    EOS_Auth_CopyUserAuthTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

    EOS_Auth_Token *outToken = nullptr;
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EResult res = EOS_Auth_CopyUserAuthToken(s_authInterface, &options, localUserId, &outToken);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["token"] = eosg_auth_token_to_dict_and_release(outToken);
    return ret;
}

void IEOS::auth_interface_delete_persistent_auth(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
    CharString refresh_token = VARIANT_TO_CHARSTRING(p_options->get("refresh_token"));

    EOS_Auth_DeletePersistentAuthOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
    options.RefreshToken = nullptr;

    if (refresh_token.size() != 0) {
        options.RefreshToken = refresh_token.get_data();
    }
    p_options->reference();

    EOS_Auth_DeletePersistentAuth(s_authInterface, &options, (void *)*p_options, [](const EOS_Auth_DeletePersistentAuthCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("auth_interface_delete_persistent_auth_callback", ret);
    });
}

String IEOS::auth_interface_get_logged_in_account_by_index(int index) {
    ERR_FAIL_NULL_V(s_authInterface, "");
    EOS_EpicAccountId accountId = EOS_Auth_GetLoggedInAccountByIndex(s_authInterface, index);
    return eosg_epic_account_id_to_string(accountId);
}

int IEOS::auth_interface_get_logged_in_accounts_count() {
    ERR_FAIL_NULL_V(s_authInterface, 0);
    return static_cast<int>(EOS_Auth_GetLoggedInAccountsCount(s_authInterface));
}

int IEOS::auth_interface_get_login_status(const String &p_local_user_id) {
    ERR_FAIL_NULL_V(s_authInterface, static_cast<int>(EOS_ELoginStatus::EOS_LS_NotLoggedIn));
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId accountId = eosg_string_to_epic_account_id(local_user_id.get_data());
    return static_cast<int>(EOS_Auth_GetLoginStatus(s_authInterface, accountId));
}

String IEOS::auth_interface_get_merged_account_by_index(const String &p_local_user_id, int index) {
    ERR_FAIL_NULL_V(s_authInterface, "");
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EpicAccountId accountId = EOS_Auth_GetMergedAccountByIndex(s_authInterface, localUserId, index);
    return eosg_epic_account_id_to_string(accountId);
}

int IEOS::auth_interface_get_merged_accounts_count(const String &p_local_user_id) {
    ERR_FAIL_NULL_V(s_authInterface, 0);
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    return static_cast<int>(EOS_Auth_GetMergedAccountsCount(s_authInterface, localUserId));
}

Dictionary IEOS::auth_interface_get_selected_account_id(const String &p_local_user_id) {
    ERR_FAIL_NULL_V(s_authInterface, {});
    CharString local_user_id = p_local_user_id.utf8();
    EOS_EpicAccountId localUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    EOS_EpicAccountId outSelectedAccountId = nullptr;
    EOS_EResult res = EOS_Auth_GetSelectedAccountId(s_authInterface, localUserId, &outSelectedAccountId);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["selected_account_id"] = eosg_epic_account_id_to_string(outSelectedAccountId);
    return ret;
}

void IEOS::auth_interface_query_id_token(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
    CharString p_local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString p_target_account_id = VARIANT_TO_CHARSTRING(p_options->get("target_account_id"));

    EOS_Auth_QueryIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_QUERYIDTOKEN_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(p_local_user_id.get_data());
    options.TargetAccountId = eosg_string_to_epic_account_id(p_target_account_id.get_data());
    p_options->reference();

    EOS_Auth_QueryIdToken(s_authInterface, &options, (void *)*p_options, [](const EOS_Auth_QueryIdTokenCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_account_id"] = eosg_epic_account_id_to_string(data->TargetAccountId);
        IEOS::get_singleton()->emit_signal("auth_interface_query_id_token_callback", ret);
    });
}

void IEOS::auth_interface_verify_id_token(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
    Ref<RefCounted> p_id_token = p_options->get("id_token");
    CharString p_account_id = VARIANT_TO_CHARSTRING(p_id_token->get("account_id"));
    CharString p_json_web_token = VARIANT_TO_CHARSTRING(p_id_token->get("json_web_token"));

    EOS_Auth_IdToken idToken;
    memset(&idToken, 0, sizeof(idToken));
    idToken.ApiVersion = EOS_AUTH_IDTOKEN_API_LATEST;
    idToken.AccountId = eosg_string_to_epic_account_id(p_account_id.get_data());
    idToken.JsonWebToken = p_json_web_token.get_data();

    EOS_Auth_VerifyIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_VERIFYIDTOKEN_API_LATEST;
    options.IdToken = &idToken;
    p_options->reference();

    EOS_Auth_VerifyIdToken(s_authInterface, &options, (void *)*p_options, [](const EOS_Auth_VerifyIdTokenCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["application_id"] = EOSG_GET_STRING(data->ApplicationId);
        ret["client_id"] = EOSG_GET_STRING(data->ClientId);
        ret["product_id"] = EOSG_GET_STRING(data->ProductId);
        ret["sandbox_id"] = EOSG_GET_STRING(data->SandboxId);
        ret["deployment_id"] = EOSG_GET_STRING(data->DeploymentId);
        ret["display_name"] = EOSG_GET_STRING(data->DisplayName);
        ret["is_external_account_info_present"] = EOSG_GET_BOOL(data->bIsExternalAccountInfoPresent);
        ret["external_account_id_type"] = static_cast<int>(data->ExternalAccountIdType);
        ret["external_account_id"] = EOSG_GET_STRING(data->ExternalAccountId);
        ret["external_account_display_name"] = EOSG_GET_STRING(data->ExternalAccountDisplayName);
        ret["platform"] = EOSG_GET_STRING(data->Platform);
        IEOS::get_singleton()->emit_signal("auth_interface_verify_id_token_callback", ret);
    });
}

void IEOS::auth_interface_link_account(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
    Ref<EOSGContinuanceToken> p_continuance_token = Object::cast_to<EOSGContinuanceToken>(p_options->get("continuance_token"));
    ERR_FAIL_NULL_MSG(p_continuance_token, "Error linking account. LinkAccountOptions.continuance_token is null.");
    ERR_FAIL_NULL_MSG(p_continuance_token->get_internal(), "Error linking account. EOSGContinuanceToken is null.");

    CharString p_local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Auth_LinkAccountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_LINKACCOUNT_API_LATEST;
    options.LinkAccountFlags = static_cast<EOS_ELinkAccountFlags>((int)p_options->get("link_account_flags"));
    options.ContinuanceToken = p_continuance_token->get_internal();
    options.LocalUserId = eosg_string_to_epic_account_id(p_local_user_id.get_data());
    p_options->reference();

    EOS_Auth_LinkAccount(s_authInterface, &options, (void *)*p_options, [](const EOS_Auth_LinkAccountCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["pin_grant_info"] = eosg_auth_pin_grant_info_to_dict(data->PinGrantInfo);
        ret["selected_account_id"] = eosg_epic_account_id_to_string(data->SelectedAccountId);
        IEOS::get_singleton()->emit_signal("auth_interface_link_account_callback", ret);
    });
}

void IEOS::auth_interface_verify_user_auth(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_authInterface);
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
    p_options->reference();

    EOS_Auth_VerifyUserAuth(s_authInterface, &options, (void *)*p_options, [](const EOS_Auth_VerifyUserAuthCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("auth_interface_verify_user_auth_callback", ret);
    });
}

#pragma once
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/char_string.hpp"
#include "wrappers/continuance_token.h"
using namespace godot;

#define VARIANT_TO_CHARSTRING(str) ((String)str).utf8()
#define EOSG_GET_STRING(str) ((str == nullptr) ? "" : String(str))
#define EOSG_GET_BOOL(eosBool) ((eosBool == EOS_TRUE) ? true : false)

static char const* eosg_epic_account_id_to_string(EOS_EpicAccountId accountId) {
    if (accountId == nullptr) {
        return "";
    }

    static char tempBuffer[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
    int32_t tempBufferSize = sizeof(tempBuffer);
    EOS_EResult Result = EOS_EpicAccountId_ToString(accountId, tempBuffer, &tempBufferSize);

    if (Result == EOS_EResult::EOS_Success) {
        return tempBuffer;
    }

    UtilityFunctions::printerr("\nError: Got EOS Result: ", EOS_EResult_ToString(Result), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n ");

    return "";
}

static EOS_EpicAccountId eosg_string_to_epic_account_id(const char* p_account_id) {
    EOS_EpicAccountId accountId = EOS_EpicAccountId_FromString(p_account_id);
    return accountId;
}

static String eosg_product_user_id_to_string(EOS_ProductUserId localUserId) {
    if (localUserId == nullptr) {
        return String("");
    }

    static char tempBuffer[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
    int32_t tempBufferSize = sizeof(tempBuffer);
    EOS_EResult Result = EOS_ProductUserId_ToString(localUserId, tempBuffer, &tempBufferSize);

    if (Result == EOS_EResult::EOS_Success) {
        return String(tempBuffer);
    }

    UtilityFunctions::printerr("\nError: Got EOS Result: ", EOS_EResult_ToString(Result), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n ");

    return String("");
}

static EOS_ProductUserId eosg_string_to_product_user_id(const char* p_account_id) {
    EOS_ProductUserId accountId = EOS_ProductUserId_FromString(p_account_id);
    return accountId;
}

static Variant eosg_auth_pin_grant_info_to_dict(const EOS_Auth_PinGrantInfo* pinGrantInfo) {
    if (pinGrantInfo == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["user_code"] = EOSG_GET_STRING(pinGrantInfo->UserCode);
    ret["verification_uri"] = EOSG_GET_STRING(pinGrantInfo->VerificationURI);
    ret["expires_in"] = pinGrantInfo->ExpiresIn;
    ret["verification_uri_complete"] = EOSG_GET_STRING(pinGrantInfo->VerificationURIComplete);

    return ret;
}

static Variant eosg_auth_account_feature_restricted_info_to_dict(const EOS_Auth_AccountFeatureRestrictedInfo* accountFeaturesRestrictedInfo) {
    if (accountFeaturesRestrictedInfo == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["verification_uri"] = EOSG_GET_STRING(accountFeaturesRestrictedInfo->VerificationURI);
    return ret;
}

static Variant eosg_continuance_token_to_wrapper(EOS_ContinuanceToken continuanceToken) {
    if (continuanceToken == nullptr) {
        return Variant();
    }
    Ref<ContinuanceTokenEOSG> continuance_token = memnew(ContinuanceTokenEOSG());
    continuance_token->set_token(continuanceToken);
    return continuance_token;
}

static Variant eosg_auth_id_token_to_dict(EOS_Auth_IdToken* authIdToken) {
    if (authIdToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["account_id"] = eosg_epic_account_id_to_string(authIdToken->AccountId);
    ret["json_web_token"] = String(authIdToken->JsonWebToken);
    EOS_Auth_IdToken_Release(authIdToken);
    return ret;
}

static Variant eosg_connect_id_token_to_dict(EOS_Connect_IdToken* connectIdToken) {
    if (connectIdToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["product_user_id"] = eosg_product_user_id_to_string(connectIdToken->ProductUserId);
    ret["json_web_token"] = String(connectIdToken->JsonWebToken);
    EOS_Connect_IdToken_Release(connectIdToken);
    return ret;
}

static Variant eosg_auth_token_to_dict(EOS_Auth_Token* authToken) {
    if (authToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["app"] = EOSG_GET_STRING(authToken->App);
    ret["client_id"] = EOSG_GET_STRING(authToken->ClientId);
    ret["account_id"] = eosg_epic_account_id_to_string(authToken->AccountId);
    ret["access_token"] = EOSG_GET_STRING(authToken->AccessToken);
    ret["expires_in"] = authToken->ExpiresIn;
    ret["expires_at"] = EOSG_GET_STRING(authToken->ExpiresAt);
    ret["auth_type"] = static_cast<int>(authToken->AuthType);
    ret["refresh_token"] = EOSG_GET_STRING(authToken->RefreshToken);
    ret["refresh_expires_in"] = authToken->RefreshExpiresIn;
    ret["refresh_expires_at"] = EOSG_GET_STRING(authToken->RefreshExpiresAt);
    EOS_Auth_Token_Release(authToken);
    return ret;
}

static Variant eosg_connect_external_account_info_to_dict(EOS_Connect_ExternalAccountInfo* externalAccountInfo) {
    if (externalAccountInfo == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["product_user_id"] = eosg_product_user_id_to_string(externalAccountInfo->ProductUserId);
    ret["display_name"] = EOSG_GET_STRING(externalAccountInfo->DisplayName);
    ret["account_id"] = EOSG_GET_STRING(externalAccountInfo->AccountId);
    ret["account_id_type"] = static_cast<int>(externalAccountInfo->AccountIdType);
    ret["last_login_time"] = externalAccountInfo->LastLoginTime;
    return ret;
}
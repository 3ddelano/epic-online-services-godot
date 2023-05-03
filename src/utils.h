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

static char const* eosg_product_user_id_to_string(EOS_ProductUserId localUserId) {
    if (localUserId == nullptr) {
        return "";
    }

    static char tempBuffer[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
    int32_t tempBufferSize = sizeof(tempBuffer);
    EOS_EResult Result = EOS_ProductUserId_ToString(localUserId, tempBuffer, &tempBufferSize);

    if (Result == EOS_EResult::EOS_Success) {
        return tempBuffer;
    }

    UtilityFunctions::printerr("\nError: Got EOS Result: ", EOS_EResult_ToString(Result), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n ");

    return "";
}

static EOS_ProductUserId eosg_string_to_product_user_id(const char* p_account_id) {
    EOS_ProductUserId accountId = EOS_ProductUserId_FromString(p_account_id);
    return accountId;
}

static Dictionary
eosg_auth_pin_grant_info_to_dict(const EOS_Auth_PinGrantInfo* pinGrantInfo) {
    if (pinGrantInfo == nullptr) {
        return Dictionary();
    }
    Dictionary ret;
    ret["user_code"] = EOSG_GET_STRING(pinGrantInfo->UserCode);
    ret["verification_uri"] = EOSG_GET_STRING(pinGrantInfo->VerificationURI);
    ret["expires_in"] = pinGrantInfo->ExpiresIn;
    ret["verification_uri_complete"] = EOSG_GET_STRING(pinGrantInfo->VerificationURIComplete);

    return ret;
}

static Dictionary eosg_auth_account_feature_restricted_info_to_dict(const EOS_Auth_AccountFeatureRestrictedInfo* accountFeaturesRestrictedInfo) {
    if (accountFeaturesRestrictedInfo == nullptr) {
        return Dictionary();
    }
    Dictionary ret;
    ret["verification_uri"] = EOSG_GET_STRING(accountFeaturesRestrictedInfo->VerificationURI);
    return ret;
}

static Ref<ContinuanceTokenEOSG> eosg_continuance_token_to_wrapper(EOS_ContinuanceToken continuanceToken) {
    Ref<ContinuanceTokenEOSG> continuance_token = memnew(ContinuanceTokenEOSG());
    // if (continuanceToken == nullptr) {
    //     return continuance_token;
    // }
    continuance_token->set_token(continuanceToken);
    return continuance_token;
}

static Dictionary eosg_auth_id_token_to_dict(EOS_Auth_IdToken* authIdToken) {
    if (authIdToken == nullptr) {
        return Dictionary();
    }
    Dictionary token;
    token["account_id"] = eosg_epic_account_id_to_string(authIdToken->AccountId);
    token["json_web_token"] = String(authIdToken->JsonWebToken);
    EOS_Auth_IdToken_Release(authIdToken);
    return token;
}

static Dictionary eosg_auth_token_to_dict(EOS_Auth_Token* authToken) {
    if (authToken == nullptr) {
        return Dictionary();
    }
    Dictionary token;
    token["app"] = EOSG_GET_STRING(authToken->App);
    token["client_id"] = EOSG_GET_STRING(authToken->ClientId);
    token["account_id"] = eosg_epic_account_id_to_string(authToken->AccountId);
    token["access_token"] = EOSG_GET_STRING(authToken->AccessToken);
    token["expires_in"] = authToken->ExpiresIn;
    token["expires_at"] = EOSG_GET_STRING(authToken->ExpiresAt);
    token["auth_type"] = static_cast<int>(authToken->AuthType);
    token["refresh_token"] = EOSG_GET_STRING(authToken->RefreshToken);
    token["refresh_expires_in"] = authToken->RefreshExpiresIn;
    token["refresh_expires_at"] = EOSG_GET_STRING(authToken->RefreshExpiresAt);
    EOS_Auth_Token_Release(authToken);
    return token;
}
#pragma once
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/char_string.hpp"

using namespace godot;

#define VARIANT_TO_CHARSTRING(str) ((String)str).utf8()

static char const* eosg_epic_account_id_to_string(EOS_EpicAccountId accountId) {
    if (accountId == nullptr) {
        return "NULL";
    }

    static char tempBuffer[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
    int32_t tempBufferSize = sizeof(tempBuffer);
    EOS_EResult Result = EOS_EpicAccountId_ToString(accountId, tempBuffer, &tempBufferSize);

    if (Result == EOS_EResult::EOS_Success) {
        return tempBuffer;
    }

    UtilityFunctions::printerr("\nError: Got EOS Result: ", EOS_EResult_ToString(Result), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n ");

    return "ERROR";
}

static EOS_EpicAccountId eosg_string_to_epic_account_id(const char* p_account_id) {
    EOS_EpicAccountId accountId = EOS_EpicAccountId_FromString(p_account_id);
    return accountId;
}

static Dictionary
eosg_auth_pin_grant_info_to_dict(const EOS_Auth_PinGrantInfo* pinGrantInfo) {
    Dictionary ret;
    ret["user_code"] = String(pinGrantInfo->UserCode);
    ret["verification_uri"] = String(pinGrantInfo->VerificationURI);
    ret["expires_in"] = pinGrantInfo->ExpiresIn;
    ret["verification_uri_complete"] = String(pinGrantInfo->VerificationURIComplete);

    return ret;
}

static Dictionary eosg_auth_account_feature_restricted_info_to_dict(const EOS_Auth_AccountFeatureRestrictedInfo* accountFeaturesRestrictedInfo) {
    Dictionary ret;
    ret["verification_uri"] = String(accountFeaturesRestrictedInfo->VerificationURI);
    return ret;
}
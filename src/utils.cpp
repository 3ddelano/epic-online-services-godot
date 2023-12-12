#include "utils.h"

String eosg_epic_account_id_to_string(EOS_EpicAccountId accountId) {
    if (accountId == nullptr) {
        return String("");
    }

    char *tempBuffer = (char *)memalloc(EOS_EPICACCOUNTID_MAX_LENGTH + 1);
    int32_t tempBufferSize = EOS_EPICACCOUNTID_MAX_LENGTH + 1;
    EOS_EResult Result = EOS_EpicAccountId_ToString(accountId, tempBuffer, &tempBufferSize);

    if (Result == EOS_EResult::EOS_Success) {
        return String(tempBuffer);
    }

    if (IEOS::get_singleton()->isEOSValid) {
        UtilityFunctions::printerr("\nError: EOSG Utils: eosg_epic_account_id_to_string: Got EOS Result: ", EOS_EResult_ToString(Result), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n");
    }

    return "";
}

String eosg_product_user_id_to_string(EOS_ProductUserId localUserId) {
    if (localUserId == nullptr) {
        return String("");
    }

    char *tempBuffer = (char *)memalloc(EOS_PRODUCTUSERID_MAX_LENGTH + 1);
    int32_t tempBufferSize = EOS_PRODUCTUSERID_MAX_LENGTH + 1;
    EOS_EResult Result = EOS_ProductUserId_ToString(localUserId, tempBuffer, &tempBufferSize);

    if (Result == EOS_EResult::EOS_Success) {
        return String(tempBuffer);
    }

    if (IEOS::get_singleton()->isEOSValid) {
        UtilityFunctions::printerr("\nError: EOSG Utils: eosg_product_user_id_to_string: Got EOS Result: ", EOS_EResult_ToString(Result), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n");
    }

    return String("");
}
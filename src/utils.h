#pragma once
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/char_string.hpp"
#include "wrappers/continuance_token.h"
#include "wrappers/transaction.h"
using namespace godot;

#define VARIANT_TO_CHARSTRING(str) ((String)str).utf8()
#define VARIANT_TO_EOS_BOOL(var) \
    ((var.get_type() == Variant::Type::BOOL) ? ((var.operator bool()) ? EOS_TRUE : EOS_FALSE) : EOS_FALSE)
#define EOSG_GET_STRING(str) ((str == nullptr) ? String("") : String(str))
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

static Variant eosg_continuance_token_to_wrapper(EOS_ContinuanceToken p_continuance_token) {
    if (p_continuance_token == nullptr) {
        return Variant();
    }
    Ref<ContinuanceTokenEOSG> continuance_token = memnew(ContinuanceTokenEOSG());
    continuance_token->set_token(p_continuance_token);
    return continuance_token;
}

static Variant eosg_auth_id_token_to_dict(EOS_Auth_IdToken* authIdToken) {
    if (authIdToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["account_id"] = eosg_epic_account_id_to_string(authIdToken->AccountId);
    ret["json_web_token"] = EOSG_GET_STRING(authIdToken->JsonWebToken);
    EOS_Auth_IdToken_Release(authIdToken);
    return ret;
}

static Variant eosg_connect_id_token_to_dict(EOS_Connect_IdToken* connectIdToken) {
    if (connectIdToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["product_user_id"] = eosg_product_user_id_to_string(connectIdToken->ProductUserId);
    ret["json_web_token"] = EOSG_GET_STRING(connectIdToken->JsonWebToken);
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
    EOS_Connect_ExternalAccountInfo_Release(externalAccountInfo);
    return ret;
}

static Variant eosg_ecom_entitlement_to_dict(EOS_Ecom_Entitlement* entitlement) {
    if (entitlement == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["entitlement_name"] = EOSG_GET_STRING(entitlement->EntitlementName);
    ret["entitlement_id"] = EOSG_GET_STRING(entitlement->EntitlementId);
    ret["catalog_item_id"] = EOSG_GET_STRING(entitlement->CatalogItemId);
    ret["server_id"] = entitlement->ServerIndex;
    ret["redeemed"] = EOSG_GET_BOOL(entitlement->bRedeemed);
    ret["end_timestamp"] = entitlement->EndTimestamp;
    EOS_Ecom_Entitlement_Release(entitlement);
    return ret;
}

static Variant eosg_ecom_catalog_item_to_dict(EOS_Ecom_CatalogItem* item) {
    if (item == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["catalog_namespace"] = EOSG_GET_STRING(item->CatalogNamespace);
    ret["id"] = EOSG_GET_STRING(item->Id);
    ret["entitlement_name"] = EOSG_GET_STRING(item->EntitlementName);
    ret["title_text"] = EOSG_GET_STRING(item->TitleText);
    ret["description_text"] = EOSG_GET_STRING(item->DescriptionText);
    ret["long_description_text"] = EOSG_GET_STRING(item->LongDescriptionText);
    ret["technical_details_text"] = EOSG_GET_STRING(item->TechnicalDetailsText);
    ret["developer_text"] = EOSG_GET_STRING(item->DeveloperText);
    ret["item_type"] = static_cast<int>(item->ItemType);
    ret["entitlement_end_timestamp"] = item->EntitlementEndTimestamp;
    EOS_Ecom_CatalogItem_Release(item);
    return ret;
}

static Variant eosg_ecom_key_image_info_to_dict(EOS_Ecom_KeyImageInfo* keyInfo) {
    if (keyInfo == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["type"] = EOSG_GET_STRING(keyInfo->Type);
    ret["url"] = EOSG_GET_STRING(keyInfo->Url);
    ret["width"] = keyInfo->Width;
    ret["height"] = keyInfo->Height;
    EOS_Ecom_KeyImageInfo_Release(keyInfo);
    return ret;
}

static Variant eosg_ecom_catalog_release_to_dict(EOS_Ecom_CatalogRelease* release) {
    if (release == nullptr) {
        return Variant();
    }

    Dictionary ret;
    Array compatible_app_ids = Array();
    for (int i = 0; i < release->CompatibleAppIdCount; i++) {
        compatible_app_ids.append(EOSG_GET_STRING(release->CompatibleAppIds[i]));
    }
    ret["compatible_app_ids"] = compatible_app_ids;
    Array compatible_platforms = Array();
    for (int i = 0; i < release->CompatiblePlatformCount; i++) {
        compatible_platforms.append(EOSG_GET_STRING(release->CompatiblePlatforms[i]));
    }
    ret["compatible_platforms"] = compatible_platforms;
    ret["release_note"] = EOSG_GET_STRING(release->ReleaseNote);
    EOS_Ecom_CatalogRelease_Release(release);
    return ret;
}

static Variant eosg_ecom_catalog_offer_to_dict(EOS_Ecom_CatalogOffer* offer) {
    if (offer == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["server_index"] = offer->ServerIndex;
    ret["catalog_namespace"] = EOSG_GET_STRING(offer->CatalogNamespace);
    ret["id"] = EOSG_GET_STRING(offer->Id);
    ret["title_text"] = EOSG_GET_STRING(offer->TitleText);
    ret["description_text"] = EOSG_GET_STRING(offer->DescriptionText);
    ret["long_description_text"] = EOSG_GET_STRING(offer->LongDescriptionText);
    ret["currency_code"] = EOSG_GET_STRING(offer->CurrencyCode);
    ret["discount_percentage"] = offer->DiscountPercentage;
    ret["expiration_timestamp"] = offer->ExpirationTimestamp;
    ret["purchase_limit"] = offer->PurchaseLimit;
    ret["available_for_purchase"] = EOSG_GET_BOOL(offer->bAvailableForPurchase);
    ret["price_result"] = static_cast<int>(offer->PriceResult);
    ret["original_price"] = offer->OriginalPrice64;
    ret["current_price"] = offer->CurrentPrice64;
    ret["decimal_point"] = offer->DecimalPoint;
    ret["release_date_timstamp"] = offer->ReleaseDateTimestamp;
    ret["effective_date_timestamp"] = offer->EffectiveDateTimestamp;
    EOS_Ecom_CatalogOffer_Release(offer);
    return ret;
}

static Variant eosg_ecom_transaction_to_wrapper(EOS_Ecom_HTransaction p_transaction) {
    if (p_transaction == nullptr) {
        return Variant();
    }
    Ref<TransactionEOSG> transaction = memnew(TransactionEOSG());
    transaction->set_transaction(p_transaction);
    return transaction;
}

static Variant eosg_user_info_external_user_info_to_dict(EOS_UserInfo_ExternalUserInfo* externalUserInfo) {
    if (externalUserInfo == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["account_type"] = static_cast<int>(externalUserInfo->AccountType);
    ret["account_id"] = EOSG_GET_STRING(externalUserInfo->AccountId);
    ret["display_name"] = EOSG_GET_STRING(externalUserInfo->DisplayName);
    ret["display_name_sanitized"] = EOSG_GET_STRING(externalUserInfo->DisplayNameSanitized);
    EOS_UserInfo_ExternalUserInfo_Release(externalUserInfo);
    return ret;
}

static Variant eosg_user_info_user_info_to_dict(EOS_UserInfo* userInfo) {
    if (userInfo == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["user_id"] = eosg_epic_account_id_to_string(userInfo->UserId);
    ret["country"] = EOSG_GET_STRING(userInfo->Country);
    ret["display_name"] = EOSG_GET_STRING(userInfo->DisplayName);
    ret["display_name_sanitized"] = EOSG_GET_STRING(userInfo->DisplayNameSanitized);
    ret["preferred_language"] = EOSG_GET_STRING(userInfo->PreferredLanguage);
    ret["nickname"] = EOSG_GET_STRING(userInfo->Nickname);
    EOS_UserInfo_Release(userInfo);
    return ret;
}

static Variant eosg_mods_mod_info_to_dict(EOS_Mods_ModInfo* modsInfo) {
    if (modsInfo == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["mods_count"] = static_cast<int>(modsInfo->ModsCount);
    ret["type"] = static_cast<int>(modsInfo->Type);
    Array mods = Array();

    if (modsInfo->ModsCount > 0 && modsInfo->Mods != nullptr) {
        for (int i = 0; i < modsInfo->ModsCount; i++) {
            EOS_Mod_Identifier mod = modsInfo->Mods[i];
            Dictionary mod_dict;
            mod_dict["namespace_id"] = EOSG_GET_STRING(mod.NamespaceId);
            mod_dict["item_id"] = EOSG_GET_STRING(mod.ItemId);
            mod_dict["artifact_id"] = EOSG_GET_STRING(mod.ArtifactId);
            mod_dict["title"] = EOSG_GET_STRING(mod.Title);
            mod_dict["version"] = EOSG_GET_STRING(mod.Version);
            mods.append(mod_dict);
        }
    }
    ret["mods"] = mods;
    EOS_Mods_ModInfo_Release(modsInfo);
    return ret;
}

static EOS_Mod_Identifier eosg_dict_to_mods_mod_identifier(Dictionary p_mod) {
    CharString namespace_id = VARIANT_TO_CHARSTRING(p_mod["namespace_id"]);
    CharString item_id = VARIANT_TO_CHARSTRING(p_mod["item_id"]);
    CharString artifact_id = VARIANT_TO_CHARSTRING(p_mod["artifact_id"]);
    CharString title = VARIANT_TO_CHARSTRING(p_mod["title"]);
    CharString version = VARIANT_TO_CHARSTRING(p_mod["version"]);

    EOS_Mod_Identifier mod;
    memset(&mod, 0, sizeof(mod));
    mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;
    mod.NamespaceId = namespace_id.get_data();
    mod.ItemId = item_id.get_data();
    mod.ArtifactId = artifact_id.get_data();
    mod.Title = title.get_data();
    mod.Version = version.get_data();

    return mod;
}

static Variant eosg_mods_mod_identifier_to_dict(const EOS_Mod_Identifier* mod) {
    if (mod == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["namespace_id"] = EOSG_GET_STRING(mod->NamespaceId);
    ret["item_id"] = EOSG_GET_STRING(mod->ItemId);
    ret["artifact_id"] = EOSG_GET_STRING(mod->ArtifactId);
    ret["title"] = EOSG_GET_STRING(mod->Title);
    ret["version"] = EOSG_GET_STRING(mod->Version);
    return ret;
}
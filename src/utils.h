#pragma once
#include "eosg_active_session.h"
#include "eosg_continuance_token.h"
#include "eosg_lobby_details.h"
#include "eosg_lobby_modification.h"
#include "eosg_lobby_search.h"
#include "eosg_playerdatastorage_file_transfer_request.h"
#include "eosg_presence_modification.h"
#include "eosg_session_details.h"
#include "eosg_session_modification.h"
#include "eosg_session_search.h"
#include "eosg_titlestorage_file_transfer_request.h"
#include "eosg_transaction.h"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/templates/vector.hpp"
#include "godot_cpp/variant/char_string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "ieos.h"

using namespace godot;

#define VARIANT_TO_CHARSTRING(str) ((String)str).utf8()
#define VARIANT_TO_EOS_BOOL(var) \
    ((var.get_type() == Variant::BOOL) ? ((var.operator bool()) ? EOS_TRUE : EOS_FALSE) : EOS_FALSE)
#define EOSG_GET_STRING(str) ((str == nullptr) ? String("") : String(str))
#define EOSG_GET_BOOL(eosBool) ((eosBool == EOS_TRUE) ? true : false)

#ifdef _MSC_VER // Check if using Microsoft Visual Studio
#define STRNCPY_S(dest, destsz, src, count) strncpy_s(dest, destsz, src, count)
#else
#define STRNCPY_S(dest, destsz, src, count) strncpy(dest, src, count)
#endif

#define EOSG_EOS_HANDLE_TO_WRAPPER(m_eos_handle, m_wrapper) \
    if (m_eos_handle == nullptr) {                          \
        return Variant();                                   \
    }                                                       \
    Ref<m_wrapper> wrapper = memnew(m_wrapper());           \
    wrapper->set_internal(m_eos_handle);                    \
    return wrapper;

#define PERSISTENT_CHAR_ARRAY_CREATE(array_name, storage_vector, count) \
    Vector<CharString> storage_vector;                                  \
    storage_vector.resize(count);                                       \
    const char **array_name = (const char **)memalloc(sizeof(char *) * (count));

#define PERSISTENT_CHAR_ARRAY_SET(array_name, storage_vector, index, variant_value) \
    storage_vector.write[index] = VARIANT_TO_CHARSTRING(variant_value);             \
    array_name[index] = storage_vector[index].get_data();

String eosg_epic_account_id_to_string(EOS_EpicAccountId accountId);

static EOS_EpicAccountId eosg_string_to_epic_account_id(const char *p_account_id) {
    EOS_EpicAccountId accountId = EOS_EpicAccountId_FromString(p_account_id);
    return accountId;
}

String eosg_product_user_id_to_string(EOS_ProductUserId localUserId);

static EOS_ProductUserId eosg_string_to_product_user_id(const char *p_account_id) {
    EOS_ProductUserId accountId = EOS_ProductUserId_FromString(p_account_id);
    return accountId;
}

static Variant eosg_auth_pin_grant_info_to_dict(const EOS_Auth_PinGrantInfo *pinGrantInfo) {
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

static Variant eosg_continuance_token_to_wrapper(EOS_ContinuanceToken p_continuance_token) {
    EOSG_EOS_HANDLE_TO_WRAPPER(p_continuance_token, EOSGContinuanceToken);
}

static Variant eosg_auth_id_token_to_dict_and_release(EOS_Auth_IdToken *authIdToken) {
    if (authIdToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["account_id"] = eosg_epic_account_id_to_string(authIdToken->AccountId);
    ret["json_web_token"] = EOSG_GET_STRING(authIdToken->JsonWebToken);
    EOS_Auth_IdToken_Release(authIdToken);
    return ret;
}

static Variant eosg_connect_id_token_to_dict_and_release(EOS_Connect_IdToken *connectIdToken) {
    if (connectIdToken == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["product_user_id"] = eosg_product_user_id_to_string(connectIdToken->ProductUserId);
    ret["json_web_token"] = EOSG_GET_STRING(connectIdToken->JsonWebToken);
    EOS_Connect_IdToken_Release(connectIdToken);
    return ret;
}

static Variant eosg_auth_token_to_dict_and_release(EOS_Auth_Token *authToken) {
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

static Variant eosg_connect_external_account_info_to_dict_and_release(EOS_Connect_ExternalAccountInfo *externalAccountInfo) {
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

static Variant eosg_ecom_entitlement_to_dict_and_release(EOS_Ecom_Entitlement *entitlement) {
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

static Variant eosg_ecom_catalog_item_to_dict_and_release(EOS_Ecom_CatalogItem *item) {
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

static Variant eosg_ecom_key_image_info_to_dict_and_release(EOS_Ecom_KeyImageInfo *keyInfo) {
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

static Variant eosg_ecom_catalog_release_to_dict_and_release(EOS_Ecom_CatalogRelease *release) {
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

static Variant eosg_ecom_catalog_offer_to_dict_and_release(EOS_Ecom_CatalogOffer *offer) {
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
    EOSG_EOS_HANDLE_TO_WRAPPER(p_transaction, EOSGTransaction);
}

static Variant eosg_user_info_external_user_info_to_dict_and_release(EOS_UserInfo_ExternalUserInfo *externalUserInfo) {
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

static Variant eosg_user_info_user_info_to_dict_and_release(EOS_UserInfo *userInfo) {
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

static Variant eosg_user_info_best_display_name_to_dict_and_release(EOS_UserInfo_BestDisplayName *bestDisplayName) {
    if (bestDisplayName == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["user_id"] = eosg_epic_account_id_to_string(bestDisplayName->UserId);
    ret["display_name"] = EOSG_GET_STRING(bestDisplayName->DisplayName);
    ret["display_name_sanitized"] = EOSG_GET_STRING(bestDisplayName->DisplayNameSanitized);
    ret["nickname"] = EOSG_GET_STRING(bestDisplayName->Nickname);
    ret["platform_type"] = static_cast<int>(bestDisplayName->PlatformType);
    EOS_UserInfo_BestDisplayName_Release(bestDisplayName);
    return ret;
}

static Variant eosg_mods_mod_info_to_dict_and_release(EOS_Mods_ModInfo *modsInfo) {
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

static Variant eosg_mods_mod_identifier_to_dict(const EOS_Mod_Identifier *mod) {
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

static Variant eosg_presence_presence_info_to_dict_and_release(EOS_Presence_Info *presence) {
    if (presence == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["status"] = static_cast<int>(presence->Status);
    ret["user_id"] = eosg_epic_account_id_to_string(presence->UserId);
    ret["product_id"] = EOSG_GET_STRING(presence->ProductId);
    ret["product_version"] = EOSG_GET_STRING(presence->ProductVersion);
    ret["platform"] = EOSG_GET_STRING(presence->Platform);
    ret["rich_text"] = EOSG_GET_STRING(presence->RichText);
    ret["product_name"] = EOSG_GET_STRING(presence->ProductName);
    ret["integrated_platform"] = EOSG_GET_STRING(presence->IntegratedPlatform);

    Array records = Array();
    if (presence->RecordsCount > 0 && presence->Records != nullptr) {
        for (int i = 0; i < presence->RecordsCount; i++) {
            EOS_Presence_DataRecord record = presence->Records[i];
            Dictionary record_dict;
            record_dict["key"] = EOSG_GET_STRING(record.Key);
            record_dict["value"] = EOSG_GET_STRING(record.Value);
            records.append(record_dict);
        }
    }
    ret["records"] = records;
    EOS_Presence_Info_Release(presence);
    return ret;
}

static Variant eosg_presence_presence_modification_to_wrapper(EOS_HPresenceModification p_presence_modification) {
    EOSG_EOS_HANDLE_TO_WRAPPER(p_presence_modification, EOSGPresenceModification);
}

static Variant eosg_achievements_definition_to_dict_and_release(EOS_Achievements_DefinitionV2 *definition) {
    if (definition == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["achievement_id"] = EOSG_GET_STRING(definition->AchievementId);
    ret["unlocked_display_name"] = EOSG_GET_STRING(definition->UnlockedDisplayName);
    ret["unlocked_description"] = EOSG_GET_STRING(definition->UnlockedDescription);
    ret["locked_display_name"] = EOSG_GET_STRING(definition->LockedDisplayName);
    ret["locked_description"] = EOSG_GET_STRING(definition->LockedDescription);
    ret["flavor_text"] = EOSG_GET_STRING(definition->FlavorText);
    ret["unlocked_icon_url"] = EOSG_GET_STRING(definition->UnlockedIconURL);
    ret["locked_icon_url"] = EOSG_GET_STRING(definition->LockedIconURL);
    ret["is_hidden"] = EOSG_GET_BOOL(definition->bIsHidden);

    Array stat_thresholds = Array();
    for (int i = 0; i < definition->StatThresholdsCount; i++) {
        Dictionary stat_threshold_dict;
        stat_threshold_dict["name"] = EOSG_GET_STRING(definition->StatThresholds[i].Name);
        stat_threshold_dict["threshold"] = definition->StatThresholds[i].Threshold;
        stat_thresholds.append(stat_threshold_dict);
    }
    ret["stat_thresholds"] = stat_thresholds;
    EOS_Achievements_DefinitionV2_Release(definition);
    return ret;
}

static Variant eosg_achievements_player_achievement_to_dict_and_release(EOS_Achievements_PlayerAchievement *achievement) {
    if (achievement == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["achievement_id"] = EOSG_GET_STRING(achievement->AchievementId);
    ret["progress"] = achievement->Progress;
    ret["unlock_time"] = achievement->UnlockTime;
    ret["display_name"] = EOSG_GET_STRING(achievement->DisplayName);
    ret["description"] = EOSG_GET_STRING(achievement->Description);
    ret["icon_url"] = EOSG_GET_STRING(achievement->IconURL);
    ret["flavor_text"] = EOSG_GET_STRING(achievement->FlavorText);
    Array stat_infos = Array();
    for (int i = 0; i < achievement->StatInfoCount; i++) {
        Dictionary stat_info_dict;
        stat_info_dict["name"] = EOSG_GET_STRING(achievement->StatInfo[i].Name);
        stat_info_dict["current_value"] = achievement->StatInfo[i].CurrentValue;
        stat_info_dict["threshold_value"] = achievement->StatInfo[i].ThresholdValue;
        stat_infos.append(stat_info_dict);
    }
    ret["stat_infos"] = stat_infos;
    EOS_Achievements_PlayerAchievement_Release(achievement);
    return ret;
}

static Variant eosg_stats_stat_to_dict_and_release(EOS_Stats_Stat *stat) {
    if (stat == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["name"] = EOSG_GET_STRING(stat->Name);
    ret["start_time"] = stat->StartTime;
    ret["end_time"] = stat->EndTime;
    ret["value"] = stat->Value;
    EOS_Stats_Stat_Release(stat);
    return ret;
}

static Variant eosg_leaderboards_definition_to_dict_and_release(EOS_Leaderboards_Definition *definition) {
    if (definition == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["leaderboard_id"] = EOSG_GET_STRING(definition->LeaderboardId);
    ret["stat_name"] = EOSG_GET_STRING(definition->StatName);
    ret["aggregation"] = static_cast<int>(definition->Aggregation);
    ret["start_time"] = definition->StartTime;
    ret["end_time"] = definition->EndTime;
    EOS_Leaderboards_Definition_Release(definition);
    return ret;
}

static Variant eosg_leaderboards_leaderboard_record_to_dict_and_release(EOS_Leaderboards_LeaderboardRecord *record) {
    if (record == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["user_id"] = eosg_product_user_id_to_string(record->UserId);
    ret["rank"] = record->Rank;
    ret["score"] = record->Score;
    ret["user_display_name"] = EOSG_GET_STRING(record->UserDisplayName);
    EOS_Leaderboards_LeaderboardRecord_Release(record);
    return ret;
}

static Variant eosg_leaderboards_leaderboard_user_score_to_dict_and_release(EOS_Leaderboards_LeaderboardUserScore *userScore) {
    if (userScore == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["user_id"] = eosg_product_user_id_to_string(userScore->UserId);
    ret["score"] = userScore->Score;
    EOS_Leaderboards_LeaderboardUserScore_Release(userScore);
    return ret;
}

static Variant eosg_kws_permission_status_to_dict_and_release(EOS_KWS_PermissionStatus *permission) {
    if (permission == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["name"] = EOSG_GET_STRING(permission->Name);
    ret["status"] = static_cast<int>(permission->Status);
    EOS_KWS_PermissionStatus_Release(permission);
    return ret;
}

static Variant eosg_lobby_details_info_to_dict_and_release(EOS_LobbyDetails_Info *info) {
    if (info == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["lobby_id"] = EOSG_GET_STRING(info->LobbyId);
    ret["lobby_owner_user_id"] = eosg_product_user_id_to_string(info->LobbyOwnerUserId);
    ret["permission_level"] = static_cast<int>(info->PermissionLevel);
    ret["available_slots"] = info->AvailableSlots;
    ret["max_members"] = info->MaxMembers;
    ret["allow_invites"] = EOSG_GET_BOOL(info->bAllowInvites);
    ret["bucket_id"] = EOSG_GET_STRING(info->BucketId);
    ret["allow_host_migration"] = EOSG_GET_BOOL(info->bAllowHostMigration);
    ret["rtc_room_enabled"] = EOSG_GET_BOOL(info->bRTCRoomEnabled);
    ret["allow_join_by_id"] = EOSG_GET_BOOL(info->bAllowJoinById);
    ret["rejoin_after_kick_requires_invite"] = EOSG_GET_BOOL(info->bRejoinAfterKickRequiresInvite);
    ret["presence_enabled"] = EOSG_GET_BOOL(info->bPresenceEnabled);
    Array allowed_platform_ids = Array();
    for (int i = 0; i < info->AllowedPlatformIdsCount; i++) {
        allowed_platform_ids.append(static_cast<int>(info->AllowedPlatformIds[i]));
    }
    ret["allowed_platform_ids"] = allowed_platform_ids;
    EOS_LobbyDetails_Info_Release(info);
    return ret;
}

static Variant eosg_lobby_details_member_info_to_dict_and_release(EOS_LobbyDetails_MemberInfo *memberInfo) {
    if (memberInfo == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["user_id"] = eosg_product_user_id_to_string(memberInfo->UserId);
    ret["platform"] = static_cast<int>(memberInfo->Platform);
    ret["allows_crossplay"] = EOSG_GET_BOOL(memberInfo->bAllowsCrossplay);
    EOS_LobbyDetails_MemberInfo_Release(memberInfo);
    return ret;
}

static Variant eosg_lobby_attribute_data_to_dict(EOS_Lobby_AttributeData *attributeData) {
    if (attributeData == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["key"] = EOSG_GET_STRING(attributeData->Key);
    ret["value_type"] = static_cast<int>(attributeData->ValueType);

    switch (attributeData->ValueType) {
        case EOS_EAttributeType::EOS_AT_BOOLEAN:
            ret["value"] = attributeData->Value.AsBool;
            break;
        case EOS_EAttributeType::EOS_AT_INT64:
            ret["value"] = attributeData->Value.AsInt64;
            break;
        case EOS_EAttributeType::EOS_AT_DOUBLE:
            ret["value"] = attributeData->Value.AsDouble;
            break;
        case EOS_EAttributeType::EOS_AT_STRING:
            ret["value"] = EOSG_GET_STRING(attributeData->Value.AsUtf8);
            break;
        default:
            UtilityFunctions::printerr("\nError: EOSG Utils: eosg_lobby_attribute_data_to_dict: Unknown value type: ", static_cast<int>(attributeData->ValueType), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n");
            break;
    }

    return ret;
}

static Variant eosg_lobby_attribute_to_dict_and_release(EOS_Lobby_Attribute *attribute) {
    if (attribute == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["visibility"] = static_cast<int>(attribute->Visibility);
    ret["data"] = eosg_lobby_attribute_data_to_dict(attribute->Data);
    EOS_Lobby_Attribute_Release(attribute);
    return ret;
}

static Variant eosg_lobby_lobby_modification_to_wrapper(EOS_HLobbyModification lobbyModification) {
    EOSG_EOS_HANDLE_TO_WRAPPER(lobbyModification, EOSGLobbyModification);
}

static Variant eosg_lobby_lobby_search_to_wrapper(EOS_HLobbySearch lobbySearch) {
    EOSG_EOS_HANDLE_TO_WRAPPER(lobbySearch, EOSGLobbySearch);
}

static Variant eosg_lobby_lobby_details_to_wrapper(EOS_HLobbyDetails lobbyDetails) {
    EOSG_EOS_HANDLE_TO_WRAPPER(lobbyDetails, EOSGLobbyDetails);
}

static EOS_Lobby_LocalRTCOptions eosg_variant_to_lobby_local_rtc_options(Variant p_local_rtc_options) {
    Dictionary local_rtc_options = p_local_rtc_options;

    int flags = 0;
    if (local_rtc_options.has("flags")) {
        flags = local_rtc_options["flags"];
    }
    bool use_manual_audio_input = false;
    if (local_rtc_options.has("use_manual_audio_input")) {
        use_manual_audio_input = local_rtc_options["use_manual_audio_input"];
    }
    bool use_manual_audio_output = false;
    if (local_rtc_options.has("use_manual_audio_output")) {
        use_manual_audio_output = local_rtc_options["use_manual_audio_output"];
    }
    bool local_audio_device_input_starts_muted = false;
    if (local_rtc_options.has("local_audio_device_input_starts_muted")) {
        local_audio_device_input_starts_muted = local_rtc_options["local_audio_device_input_starts_muted"];
    }

    EOS_Lobby_LocalRTCOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_LOCALRTCOPTIONS_API_LATEST;
    options.Flags = flags;
    options.bUseManualAudioInput = use_manual_audio_input ? EOS_TRUE : EOS_FALSE;
    options.bUseManualAudioOutput = use_manual_audio_output ? EOS_TRUE : EOS_FALSE;
    options.bLocalAudioDeviceInputStartsMuted = local_audio_device_input_starts_muted ? EOS_TRUE : EOS_FALSE;

    return options;
}

static Variant eosg_playerdatastorage_file_metadata_to_dict_and_release(EOS_PlayerDataStorage_FileMetadata *metadata) {
    if (metadata == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["file_size_bytes"] = metadata->FileSizeBytes;
    ret["md5_hash"] = EOSG_GET_STRING(metadata->MD5Hash);
    ret["filename"] = EOSG_GET_STRING(metadata->Filename);
    ret["last_modified_time"] = metadata->LastModifiedTime;
    ret["unencrypted_data_size_bytes"] = metadata->UnencryptedDataSizeBytes;
    EOS_PlayerDataStorage_FileMetadata_Release(metadata);
    return ret;
}

static Variant eosg_playerdatastorage_file_tranfer_request_to_wrapper(EOS_HPlayerDataStorageFileTransferRequest request) {
    EOSG_EOS_HANDLE_TO_WRAPPER(request, EOSGPlayerDataStorageFileTransferRequest);
}

static Variant eosg_titlestorage_file_metadata_to_dict_and_release(EOS_TitleStorage_FileMetadata *metadata) {
    if (metadata == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["file_size_bytes"] = metadata->FileSizeBytes;
    ret["md5_hash"] = EOSG_GET_STRING(metadata->MD5Hash);
    ret["filename"] = EOSG_GET_STRING(metadata->Filename);
    ret["unencrypted_data_size_bytes"] = metadata->UnencryptedDataSizeBytes;
    EOS_TitleStorage_FileMetadata_Release(metadata);
    return ret;
}

static Variant eosg_titlestorage_file_tranfer_request_to_wrapper(EOS_HTitleStorageFileTransferRequest request) {
    EOSG_EOS_HANDLE_TO_WRAPPER(request, EOSGTitleStorageFileTransferRequest);
}

static Variant eosg_sanctions_player_sanction_to_dict_and_release(EOS_Sanctions_PlayerSanction *playerSanction) {
    if (playerSanction == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["time_placed"] = playerSanction->TimePlaced;
    ret["action"] = EOSG_GET_STRING(playerSanction->Action);
    ret["time_expires"] = playerSanction->TimeExpires;
    ret["reference_id"] = EOSG_GET_STRING(playerSanction->ReferenceId);
    EOS_Sanctions_PlayerSanction_Release(playerSanction);
    return ret;
}

static Variant eosg_sessions_session_details_settings_to_dict(const EOS_SessionDetails_Settings *settings) {
    if (settings == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["bucket_id"] = EOSG_GET_STRING(settings->BucketId);
    ret["num_public_connections"] = static_cast<int>(settings->NumPublicConnections);
    ret["allow_join_in_progress"] = EOSG_GET_BOOL(settings->bAllowJoinInProgress);
    ret["permission_level"] = static_cast<int>(settings->PermissionLevel);
    ret["invites_allowed"] = EOSG_GET_BOOL(settings->bInvitesAllowed);
    ret["sanctions_enabled"] = EOSG_GET_BOOL(settings->bSanctionsEnabled);

    Array allowed_platform_ids_array = Array();
    for (int i = 0; i < settings->AllowedPlatformIdsCount; i++) {
        allowed_platform_ids_array.append(static_cast<int>(settings->AllowedPlatformIds[i]));
    }
    ret["allowed_platform_ids"] = allowed_platform_ids_array;
    return ret;
}

static Variant eosg_sessions_session_details_info_to_dict(const EOS_SessionDetails_Info *sessionDetails) {
    if (sessionDetails == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["session_id"] = EOSG_GET_STRING(sessionDetails->SessionId);
    ret["host_address"] = EOSG_GET_STRING(sessionDetails->HostAddress);
    ret["num_open_public_connections"] = static_cast<int>(sessionDetails->NumOpenPublicConnections);
    ret["owner_user_id"] = eosg_product_user_id_to_string(sessionDetails->OwnerUserId);
    ret["owner_server_client_id"] = EOSG_GET_STRING(sessionDetails->OwnerServerClientId);
    ret["settings"] = eosg_sessions_session_details_settings_to_dict(sessionDetails->Settings);
    return ret;
}

static Variant eosg_sessions_session_details_info_to_dict_and_release(EOS_SessionDetails_Info *sessionDetails) {
    Variant ret = eosg_sessions_session_details_info_to_dict(sessionDetails);
    EOS_SessionDetails_Info_Release(sessionDetails);
    return ret;
}

static Variant eosg_sessions_active_session_info_to_dict_and_release(EOS_ActiveSession_Info *info) {
    if (info == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["session_name"] = EOSG_GET_STRING(info->SessionName);
    ret["local_user_id"] = eosg_product_user_id_to_string(info->LocalUserId);
    ret["state"] = static_cast<int>(info->State);
    ret["session_details"] = eosg_sessions_session_details_info_to_dict(info->SessionDetails);
    EOS_ActiveSession_Info_Release(info);
    return ret;
}

static Variant eosg_sessions_attribute_data_to_dict(EOS_Sessions_AttributeData *attributeData) {
    if (attributeData == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["key"] = EOSG_GET_STRING(attributeData->Key);
    ret["value_type"] = static_cast<int>(attributeData->ValueType);

    switch (attributeData->ValueType) {
        case EOS_EAttributeType::EOS_AT_INT64:
            ret["value"] = attributeData->Value.AsInt64;
            break;
        case EOS_EAttributeType::EOS_AT_DOUBLE:
            ret["value"] = attributeData->Value.AsDouble;
            break;
        case EOS_EAttributeType::EOS_AT_STRING:
            ret["value"] = EOSG_GET_STRING(attributeData->Value.AsUtf8);
            break;
        default:
            UtilityFunctions::printerr("\nError: EOSG Utils: eosg_sessions_attribute_data_to_dict: Unknown value type: ", static_cast<int>(attributeData->ValueType), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n");
            break;
    }

    return ret;
}

static Variant eosg_sessions_session_details_attribute_to_dict_and_release(EOS_SessionDetails_Attribute *attribute) {
    if (attribute == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["data"] = eosg_sessions_attribute_data_to_dict(attribute->Data);
    ret["advertisement_type"] = static_cast<int>(attribute->AdvertisementType);
    EOS_SessionDetails_Attribute_Release(attribute);
    return ret;
}

static Variant eosg_sessions_active_session_to_wrapper(EOS_HActiveSession p_active_session) {
    EOSG_EOS_HANDLE_TO_WRAPPER(p_active_session, EOSGActiveSession);
}

static Variant eosg_sessions_session_details_to_wrapper(EOS_HSessionDetails p_session_details) {
    EOSG_EOS_HANDLE_TO_WRAPPER(p_session_details, EOSGSessionDetails);
}

static Variant eosg_sessions_session_modification_to_wrapper(EOS_HSessionModification p_session_modification) {
    EOSG_EOS_HANDLE_TO_WRAPPER(p_session_modification, EOSGSessionModification);
}

static Variant eosg_sessions_session_search_to_wrapper(EOS_HSessionSearch p_session_search) {
    EOSG_EOS_HANDLE_TO_WRAPPER(p_session_search, EOSGSessionSearch);
}

static Variant eosg_rtc_audio_input_device_information_to_dict_and_release(EOS_RTCAudio_InputDeviceInformation *info) {
    if (info == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["default_device"] = EOSG_GET_BOOL(info->bDefaultDevice);
    ret["device_id"] = EOSG_GET_STRING(info->DeviceId);
    ret["device_name"] = EOSG_GET_STRING(info->DeviceName);
    EOS_RTCAudio_InputDeviceInformation_Release(info);
    return ret;
}

static Variant eosg_rtc_audio_output_device_information_to_dict_and_release(EOS_RTCAudio_OutputDeviceInformation *info) {
    if (info == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["default_device"] = EOSG_GET_BOOL(info->bDefaultDevice);
    ret["device_id"] = EOSG_GET_STRING(info->DeviceId);
    ret["device_name"] = EOSG_GET_STRING(info->DeviceName);
    EOS_RTCAudio_OutputDeviceInformation_Release(info);
    return ret;
}

static Variant eosg_rtc_audio_audio_buffer_to_dict(EOS_RTCAudio_AudioBuffer *audio_buffer) {
    if (audio_buffer == nullptr) {
        return Variant();
    }

    Dictionary ret;
    ret["frames_count"] = static_cast<int>(audio_buffer->FramesCount);
    Array frames = Array();
    for (int i = 0; i < audio_buffer->FramesCount; i++) {
        frames.append(audio_buffer->Frames[i]);
    }
    ret["frames"] = frames;
    ret["sample_rate"] = audio_buffer->SampleRate;
    ret["channels"] = audio_buffer->Channels;
    return ret;
}

static EOS_AntiCheatCommon_LogPlayerUseWeaponData eosg_refcounted_to_anticheat_log_player_use_weapon_data(Ref<RefCounted> p_data) {
    Variant p_player_handle = p_data->get("player_handle");
    Vector3 p_player_position = p_data->get("player_position");
    Quaternion p_player_view_rotation = p_data->get("player_view_rotation");
    CharString p_weapon_name = VARIANT_TO_CHARSTRING(p_data->get("weapon_name"));

    EOS_AntiCheatCommon_LogPlayerUseWeaponData use_weapon_data;
    memset(&use_weapon_data, 0, sizeof(use_weapon_data));
    use_weapon_data.PlayerHandle = (void *)p_player_handle;
    use_weapon_data.PlayerPosition->x = p_player_position.x;
    use_weapon_data.PlayerPosition->y = p_player_position.y;
    use_weapon_data.PlayerPosition->z = p_player_position.z;
    use_weapon_data.PlayerViewRotation->x = p_player_view_rotation.x;
    use_weapon_data.PlayerViewRotation->y = p_player_view_rotation.y;
    use_weapon_data.PlayerViewRotation->z = p_player_view_rotation.z;
    use_weapon_data.PlayerViewRotation->w = p_player_view_rotation.w;
    use_weapon_data.bIsPlayerViewZoomed = VARIANT_TO_EOS_BOOL(p_data->get("is_player_view_zoomed"));
    use_weapon_data.bIsMeleeAttack = VARIANT_TO_EOS_BOOL(p_data->get("is_melee_attack"));
    use_weapon_data.WeaponName = p_weapon_name.get_data();

    return use_weapon_data;
}
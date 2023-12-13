#pragma once
#include "continuance_token.h"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/char_string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "ieos.h"
#include "lobby_details.h"
#include "lobby_modification.h"
#include "lobby_search.h"
#include "presence_modification.h"
#include "transaction.h"

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
    if (p_continuance_token == nullptr) {
        return Variant();
    }
    Ref<ContinuanceTokenEOSG> continuance_token = memnew(ContinuanceTokenEOSG());
    continuance_token->set_internal(p_continuance_token);
    return continuance_token;
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
    if (p_transaction == nullptr) {
        return Variant();
    }
    Ref<TransactionEOSG> transaction = memnew(TransactionEOSG());
    transaction->set_internal(p_transaction);
    return transaction;
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
    if (p_presence_modification == nullptr) {
        return Variant();
    }
    Ref<PresenceModificationEOSG> presence_modification = memnew(PresenceModificationEOSG());
    presence_modification->set_internal(p_presence_modification);
    return presence_modification;
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

static Variant eos_lobby_details_info_to_dict_and_release(EOS_LobbyDetails_Info *info) {
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
    EOS_LobbyDetails_Info_Release(info);
    return ret;
}

static Variant eos_lobby_details_member_info_to_dict_and_release(EOS_LobbyDetails_MemberInfo *memberInfo) {
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

static Variant eos_lobby_attribute_data_to_dict(EOS_Lobby_AttributeData *attributeData) {
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
            UtilityFunctions::printerr("\nError: EOSG Utils: eos_lobby_attribute_data_to_dict: Unknown value type: ", static_cast<int>(attributeData->ValueType), "\n\tat: ", __func__, " (", __FILE__, ":", __LINE__, ") ", "\n");
            break;
    }

    return ret;
}

static Variant eos_lobby_attribute_to_dict_and_release(EOS_Lobby_Attribute *attribute) {
    if (attribute == nullptr) {
        return Variant();
    }
    Dictionary ret;
    ret["visibility"] = static_cast<int>(attribute->Visibility);
    ret["data"] = eos_lobby_attribute_data_to_dict(attribute->Data);
    EOS_Lobby_Attribute_Release(attribute);
    return ret;
}

static Variant eosg_lobby_lobby_modification_to_wrapper(EOS_HLobbyModification lobbyModification) {
    if (lobbyModification == nullptr) {
        return Variant();
    }

    Ref<LobbyModificationEOSG> lobby_modification = memnew(LobbyModificationEOSG());
    lobby_modification->set_internal(lobbyModification);
    return lobby_modification;
}

static Variant eosg_lobby_lobby_search_to_wrapper(EOS_HLobbySearch lobbySearch) {
    if (lobbySearch == nullptr) {
        return Variant();
    }

    Ref<LobbySearchEOSG> lobby_search = memnew(LobbySearchEOSG());
    lobby_search->set_internal(lobbySearch);
    return lobby_search;
}

static Variant eosg_lobby_lobby_details_to_wrapper(EOS_HLobbyDetails lobbyDetails) {
    if (lobbyDetails == nullptr) {
        return Variant();
    }

    Ref<LobbyDetailsEOSG> lobby_details = memnew(LobbyDetailsEOSG());
    lobby_details->set_internal(lobbyDetails);
    return lobby_details;
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
    options.ApiVersion = EOS_LOBBY_LOCALRTCOPTIONS_API_LATEST;
    options.Flags = flags;
    options.bUseManualAudioInput = use_manual_audio_input;
    options.bUseManualAudioOutput = use_manual_audio_output;
    options.bLocalAudioDeviceInputStartsMuted = local_audio_device_input_starts_muted;

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
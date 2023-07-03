#include "ieos.h"

using namespace godot;

Dictionary IEOS::achievements_interface_copy_achievement_definition_v2_by_achievement_id(Ref<RefCounted> p_options) {
    CharString achievement_id = VARIANT_TO_CHARSTRING(p_options->get("achievement_id"));

    EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYACHIEVEMENTID_API_LATEST;
    options.AchievementId = achievement_id.get_data();

    EOS_Achievements_DefinitionV2* outDefinition = nullptr;
    EOS_EResult res = EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId(s_achievementsInterface, &options, &outDefinition);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["definition_v2"] = eosg_achievements_definition_to_dict(outDefinition);
    return ret;
}

Dictionary IEOS::achievements_interface_copy_achievement_definition_v2_by_index(Ref<RefCounted> p_options) {
    EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYINDEX_API_LATEST;
    options.AchievementIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("achievement_index")));

    EOS_Achievements_DefinitionV2* outDefinition = nullptr;
    EOS_EResult res = EOS_Achievements_CopyAchievementDefinitionV2ByIndex(s_achievementsInterface, &options, &outDefinition);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["definition_v2"] = eosg_achievements_definition_to_dict(outDefinition);
    return ret;
}

Dictionary IEOS::achievements_interface_copy_player_achievement_by_achievement_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString achievement_id = VARIANT_TO_CHARSTRING(p_options->get("achievement_id"));

    EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYACHIEVEMENTID_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.AchievementId = achievement_id.get_data();

    EOS_Achievements_PlayerAchievement* outAchievement = nullptr;
    EOS_EResult res = EOS_Achievements_CopyPlayerAchievementByAchievementId(s_achievementsInterface, &options, &outAchievement);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["player_achievement"] = eosg_achievements_player_achievement_to_dict(outAchievement);
    return ret;
}

Dictionary IEOS::achievements_interface_copy_player_achievement_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Achievements_CopyPlayerAchievementByIndexOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.AchievementIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("achievement_index")));

    EOS_Achievements_PlayerAchievement* outAchievement = nullptr;
    EOS_EResult res = EOS_Achievements_CopyPlayerAchievementByIndex(s_achievementsInterface, &options, &outAchievement);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["player_achievement"] = eosg_achievements_player_achievement_to_dict(outAchievement);
    return ret;
}

int IEOS::achievements_interface_get_achievement_definition_count(Ref<RefCounted> p_options) {
    EOS_Achievements_GetAchievementDefinitionCountOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST;

    return static_cast<int>(EOS_Achievements_GetAchievementDefinitionCount(s_achievementsInterface, &options));
}

void IEOS::achievements_interface_query_definitions(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Achievements_QueryDefinitionsOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_Achievements_QueryDefinitions(s_achievementsInterface, &options, (void*)*p_options, [](const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("achievements_interface_query_definitions_callback", ret);
    });
    return;
}

int IEOS::achievements_interface_get_player_achievement_count(Ref<RefCounted> p_options) {
    CharString user_id = VARIANT_TO_CHARSTRING(p_options->get("user_id"));

    EOS_Achievements_GetPlayerAchievementCountOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST;
    options.UserId = eosg_string_to_product_user_id(user_id.get_data());

    return static_cast<int>(EOS_Achievements_GetPlayerAchievementCount(s_achievementsInterface, &options));
}

void IEOS::achievements_interface_query_player_achievements(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Achievements_QueryPlayerAchievementsOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    p_options->reference();

    EOS_Achievements_QueryPlayerAchievements(s_achievementsInterface, &options, (void*)*p_options, [](const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["user_id"] = eosg_product_user_id_to_string(data->UserId);
        IEOS::get_singleton()->emit_signal("achievements_interface_query_player_achievements_callback", ret);
    });
    return;
}

void IEOS::achievements_interface_unlock_achievements(Ref<RefCounted> p_options) {
    CharString user_id = VARIANT_TO_CHARSTRING(p_options->get("user_id"));
    Array p_achievement_ids = p_options->get("achievement_ids");
    int achievement_ids_count = p_achievement_ids.size();

    const char** achievement_ids = (const char**)memalloc(sizeof(char*) * achievement_ids_count);
    for (int i = 0; i < achievement_ids_count; i++) {
        CharString achievement_id = VARIANT_TO_CHARSTRING(p_achievement_ids[i]);
        achievement_ids[i] = achievement_id.get_data();
    }

    EOS_Achievements_UnlockAchievementsOptions options;
    options.ApiVersion = EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST;
    options.UserId = eosg_string_to_product_user_id(user_id.get_data());
    options.AchievementIds = achievement_ids;
    options.AchievementsCount = achievement_ids_count;
    p_options->reference();

    EOS_Achievements_UnlockAchievements(s_achievementsInterface, &options, (void*)*p_options, [](const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["user_id"] = eosg_product_user_id_to_string(data->UserId);
        ret["achievements_count"] = static_cast<int>(data->AchievementsCount);
        IEOS::get_singleton()->emit_signal("achievements_interface_unlock_achievements_callback", ret);
    });
    return;
}
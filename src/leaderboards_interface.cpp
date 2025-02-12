#include "ieos.h"
using namespace std;

Dictionary IEOS::leaderboards_interface_copy_leaderboard_definition_by_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, {});
    EOS_Leaderboards_CopyLeaderboardDefinitionByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDDEFINITIONBYINDEX_API_LATEST;
    options.LeaderboardIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("leaderboard_index")));

    EOS_Leaderboards_Definition *outLeaderboardDefinition = nullptr;
    EOS_EResult res = EOS_Leaderboards_CopyLeaderboardDefinitionByIndex(s_leaderboardsInterface, &options, &outLeaderboardDefinition);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["definition"] = eosg_leaderboards_definition_to_dict_and_release(outLeaderboardDefinition);
    return ret;
}

Dictionary IEOS::leaderboards_interface_copy_leaderboard_definition_by_leaderboard_id(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, {});
    CharString leaderboard_id = VARIANT_TO_CHARSTRING(p_options->get("leaderboard_id"));
    EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDDEFINITIONBYLEADERBOARDID_API_LATEST;
    options.LeaderboardId = leaderboard_id.get_data();

    EOS_Leaderboards_Definition *outLeaderboardDefinition = nullptr;
    EOS_EResult res = EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId(s_leaderboardsInterface, &options, &outLeaderboardDefinition);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["definition"] = eosg_leaderboards_definition_to_dict_and_release(outLeaderboardDefinition);
    return ret;
}

Dictionary IEOS::leaderboards_interface_copy_leaderboard_record_by_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, {});
    EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDRECORDBYINDEX_API_LATEST;
    options.LeaderboardRecordIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("leaderboard_record_index")));

    EOS_Leaderboards_LeaderboardRecord *outLeaderboardRecord = nullptr;
    EOS_EResult res = EOS_Leaderboards_CopyLeaderboardRecordByIndex(s_leaderboardsInterface, &options, &outLeaderboardRecord);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["record"] = eosg_leaderboards_leaderboard_record_to_dict_and_release(outLeaderboardRecord);
    return ret;
}

Dictionary IEOS::leaderboards_interface_copy_leaderboard_record_by_user_id(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, {});
    CharString user_id = VARIANT_TO_CHARSTRING(p_options->get("user_id"));

    EOS_Leaderboards_CopyLeaderboardRecordByUserIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDRECORDBYUSERID_API_LATEST;
    options.UserId = eosg_string_to_product_user_id(user_id.get_data());

    EOS_Leaderboards_LeaderboardRecord *outLeaderboardRecord = nullptr;
    EOS_EResult res = EOS_Leaderboards_CopyLeaderboardRecordByUserId(s_leaderboardsInterface, &options, &outLeaderboardRecord);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["record"] = eosg_leaderboards_leaderboard_record_to_dict_and_release(outLeaderboardRecord);
    return ret;
}

Dictionary IEOS::leaderboards_interface_copy_leaderboard_user_score_by_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, {});
    CharString stat_name = VARIANT_TO_CHARSTRING(p_options->get("stat_name"));

    EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYINDEX_API_LATEST;
    options.LeaderboardUserScoreIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("leaderboard_user_score_index")));
    options.StatName = stat_name.get_data();

    EOS_Leaderboards_LeaderboardUserScore *outLeaderboardUserScore = nullptr;
    EOS_EResult res = EOS_Leaderboards_CopyLeaderboardUserScoreByIndex(s_leaderboardsInterface, &options, &outLeaderboardUserScore);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["user_score"] = eosg_leaderboards_leaderboard_user_score_to_dict_and_release(outLeaderboardUserScore);
    return ret;
}

Dictionary IEOS::leaderboards_interface_copy_leaderboard_user_score_by_user_id(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, {});
    CharString user_id = VARIANT_TO_CHARSTRING(p_options->get("user_id"));
    CharString stat_name = VARIANT_TO_CHARSTRING(p_options->get("stat_name"));

    EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYUSERID_API_LATEST;
    options.UserId = eosg_string_to_product_user_id(user_id.get_data());
    options.StatName = stat_name.get_data();

    EOS_Leaderboards_LeaderboardUserScore *outLeaderboardUserScore = nullptr;
    EOS_EResult res = EOS_Leaderboards_CopyLeaderboardUserScoreByUserId(s_leaderboardsInterface, &options, &outLeaderboardUserScore);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["user_score"] = eosg_leaderboards_leaderboard_user_score_to_dict_and_release(outLeaderboardUserScore);
    return ret;
}

int IEOS::leaderboards_interface_get_leaderboard_definition_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, 0);
    EOS_Leaderboards_GetLeaderboardDefinitionCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_GETLEADERBOARDDEFINITIONCOUNT_API_LATEST;

    return static_cast<int>(EOS_Leaderboards_GetLeaderboardDefinitionCount(s_leaderboardsInterface, &options));
}

int IEOS::leaderboards_interface_get_leaderboard_record_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, 0);
    EOS_Leaderboards_GetLeaderboardRecordCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_GETLEADERBOARDRECORDCOUNT_API_LATEST;

    return static_cast<int>(EOS_Leaderboards_GetLeaderboardRecordCount(s_leaderboardsInterface, &options));
}

int IEOS::leaderboards_interface_get_leaderboard_user_score_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_leaderboardsInterface, 0);
    CharString stat_name = VARIANT_TO_CHARSTRING(p_options->get("stat_name"));

    EOS_Leaderboards_GetLeaderboardUserScoreCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_GETLEADERBOARDUSERSCORECOUNT_API_LATEST;
    options.StatName = stat_name.get_data();

    return static_cast<int>(EOS_Leaderboards_GetLeaderboardUserScoreCount(s_leaderboardsInterface, &options));
}

void IEOS::leaderboards_interface_query_leaderboard_definitions(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_leaderboardsInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Leaderboards_QueryLeaderboardDefinitionsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDDEFINITIONS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.StartTime = static_cast<int64_t>(p_options->get("start_time"));
    options.EndTime = static_cast<int64_t>(p_options->get("end_time"));
    p_options->reference();

    EOS_Leaderboards_QueryLeaderboardDefinitions(s_leaderboardsInterface, &options, (void *)*p_options, [](const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("leaderboards_interface_query_leaderboard_definitions_callback", ret);
    });
}

void IEOS::leaderboards_interface_query_leaderboard_ranks(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_leaderboardsInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString leaderboard_id = VARIANT_TO_CHARSTRING(p_options->get("leaderboard_id"));

    EOS_Leaderboards_QueryLeaderboardRanksOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDRANKS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LeaderboardId = leaderboard_id.get_data();
    p_options->reference();

    EOS_Leaderboards_QueryLeaderboardRanks(s_leaderboardsInterface, &options, (void *)*p_options, [](const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["leaderboard_id"] = EOSG_GET_STRING(data->LeaderboardId);
        IEOS::get_singleton()->emit_signal("leaderboards_interface_query_leaderboard_ranks_callback", ret);
    });
}

void IEOS::leaderboards_interface_query_leaderboard_user_scores(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_leaderboardsInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    Array p_user_ids = p_options->get("user_ids");
    int p_user_ids_size = p_user_ids.size();

    EOS_ProductUserId *user_ids = (EOS_ProductUserId *)memalloc(sizeof(EOS_ProductUserId) * p_user_ids_size);
	PERSISTENT_CHAR_ARRAY_CREATE(user_ids_cstr, user_ids_cstr_charstrings, p_user_ids_size)
    for (int i = 0; i < p_user_ids_size; i++) {
		PERSISTENT_CHAR_ARRAY_SET(user_ids_cstr, user_ids_cstr_charstrings, i, p_user_ids[i]);
        user_ids[i] = eosg_string_to_product_user_id(user_ids_cstr[i]);
    }

    Array p_stat_info = p_options->get("stat_info");
    int p_stat_info_size = p_stat_info.size();

    EOS_Leaderboards_UserScoresQueryStatInfo *statInfo = (EOS_Leaderboards_UserScoresQueryStatInfo *)memalloc(sizeof(EOS_Leaderboards_UserScoresQueryStatInfo) * p_stat_info_size);
	PERSISTENT_CHAR_ARRAY_CREATE(stat_name_cstr, stat_name_cstr_charstrings, p_stat_info_size)
    for (int i = 0; i < p_stat_info_size; i++) {
		PERSISTENT_CHAR_ARRAY_SET(stat_name_cstr, stat_name_cstr_charstrings, i, p_stat_info[i].get("stat_name"));
        int aggregation = static_cast<int>(p_stat_info[i].get("aggregation"));
        statInfo[i].ApiVersion = EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST;
        statInfo[i].StatName = stat_name_cstr[i];
        statInfo[i].Aggregation = static_cast<EOS_ELeaderboardAggregation>(aggregation);
    }

    EOS_Leaderboards_QueryLeaderboardUserScoresOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDUSERSCORES_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.UserIds = user_ids;
    options.UserIdsCount = p_user_ids_size;
    options.StatInfo = statInfo;
    options.StatInfoCount = p_stat_info_size;
    options.StartTime = static_cast<int64_t>(p_options->get("start_time"));
    options.EndTime = static_cast<int64_t>(p_options->get("end_time"));
    p_options->reference();

    EOS_Leaderboards_QueryLeaderboardUserScores(s_leaderboardsInterface, &options, (void *)*p_options, [](const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("leaderboards_interface_query_leaderboard_user_scores_callback", ret);
    });
}

#include "ieos.h"
using namespace std;

Dictionary IEOS::stats_interface_copy_stat_by_index(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Stats_CopyStatByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_STATS_COPYSTATBYINDEX_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.StatIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("stat_index")));

    EOS_Stats_Stat *outStat = nullptr;
    EOS_EResult res = EOS_Stats_CopyStatByIndex(s_statsInterface, &options, &outStat);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["stat"] = eosg_stats_stat_to_dict_and_release(outStat);
    return ret;
}

Dictionary IEOS::stats_interface_copy_stat_by_name(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString name = VARIANT_TO_CHARSTRING(p_options->get("name"));

    EOS_Stats_CopyStatByNameOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_STATS_COPYSTATBYNAME_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.Name = name.get_data();

    EOS_Stats_Stat *outStat = nullptr;
    EOS_EResult res = EOS_Stats_CopyStatByName(s_statsInterface, &options, &outStat);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["stat"] = eosg_stats_stat_to_dict_and_release(outStat);
    return ret;
}

int IEOS::stats_interface_get_stats_count(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Stats_GetStatCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_STATS_GETSTATSCOUNT_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());

    return static_cast<int>(EOS_Stats_GetStatsCount(s_statsInterface, &options));
}

void IEOS::stats_interface_ingest_stat(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    Array p_stats = p_options->get("stats");
    int stats_count = p_stats.size();

    EOS_Stats_IngestData *stats = (EOS_Stats_IngestData *)memalloc(sizeof(EOS_Stats_IngestData) * stats_count);
    for (int i = 0; i < stats_count; i++) {
        Dictionary p_stat = p_stats[i];
        CharString stat_name = VARIANT_TO_CHARSTRING(p_stat["stat_name"]);
        stats[i].ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;
        stats[i].StatName = stat_name.get_data();
        stats[i].IngestAmount = static_cast<int32_t>(static_cast<int>(p_stat["ingest_amount"]));
    }

    EOS_Stats_IngestStatOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_STATS_INGESTSTAT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.StatsCount = stats_count;
    options.Stats = stats;
    p_options->reference();

    EOS_Stats_IngestStat(s_statsInterface, &options, (void *)*p_options, [](const EOS_Stats_IngestStatCompleteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("stats_interface_ingest_stat_callback", ret);
    });
}

void IEOS::stats_interface_query_stats(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    Array p_stat_names = p_options->get("stat_names");
    int stat_names_count = p_stat_names.size();

    const char **stat_names = (const char **)memalloc(sizeof(const char *) * stat_names_count);
    for (int i = 0; i < stat_names_count; i++) {
        CharString stat_name = VARIANT_TO_CHARSTRING(p_stat_names[i]);
        stat_names[i] = stat_name.get_data();
    }

    EOS_Stats_QueryStatsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_STATS_QUERYSTATS_API_LATEST;
    if (local_user_id.length() > 0) {
        options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    }
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    if (stat_names_count > 0) {
        options.StatNamesCount = stat_names_count;
        options.StatNames = stat_names;
    }
    options.StartTime = static_cast<int64_t>(p_options->get("start_time"));
    options.EndTime = static_cast<int64_t>(p_options->get("end_time"));
    p_options->reference();

    EOS_Stats_QueryStats(s_statsInterface, &options, (void *)*p_options, [](const EOS_Stats_OnQueryStatsCompleteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("stats_interface_query_stats_callback", ret);
    });
}

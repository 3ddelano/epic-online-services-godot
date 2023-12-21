#include "eosg_active_session.h"
#include "utils.h"

using namespace godot;

void EOSGActiveSession::_bind_methods() {
    ClassDB::bind_method(D_METHOD("copy_info"), &EOSGActiveSession::copy_info);
    ClassDB::bind_method(D_METHOD("get_registered_player_count"), &EOSGActiveSession::get_registered_player_count);
    ClassDB::bind_method(D_METHOD("get_registered_player_by_index", "player_index"), &EOSGActiveSession::get_registered_player_by_index);
}

Dictionary EOSGActiveSession::copy_info() {
    EOS_ActiveSession_CopyInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ACTIVESESSION_COPYINFO_API_LATEST;

    EOS_ActiveSession_Info *outInfo = nullptr;
    EOS_EResult res = EOS_ActiveSession_CopyInfo(m_internal, &options, &outInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["info"] = eosg_sessions_active_session_info_to_dict_and_release(outInfo);
    return ret;
}

int EOSGActiveSession::get_registered_player_count() {
    EOS_ActiveSession_GetRegisteredPlayerCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ACTIVESESSION_GETREGISTEREDPLAYERCOUNT_API_LATEST;

    return static_cast<int>(EOS_ActiveSession_GetRegisteredPlayerCount(m_internal, &options));
}

String EOSGActiveSession::get_registered_player_by_index(int p_player_index) {
    EOS_ActiveSession_GetRegisteredPlayerByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ACTIVESESSION_GETREGISTEREDPLAYERBYINDEX_API_LATEST;
    options.PlayerIndex = static_cast<uint32_t>(p_player_index);

    EOS_ProductUserId userId = EOS_ActiveSession_GetRegisteredPlayerByIndex(m_internal, &options);
    return eosg_product_user_id_to_string(userId);
}
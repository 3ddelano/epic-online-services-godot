#include "lobby_search.h"

#include "utils.h"

using namespace godot;

void LobbySearchEOSG::_bind_methods() {
    ClassDB::bind_method(D_METHOD("find", "local_user_id"), &LobbySearchEOSG::find);
    ClassDB::bind_method(D_METHOD("set_lobby_id", "lobby_id"), &LobbySearchEOSG::set_lobby_id);
    ClassDB::bind_method(D_METHOD("set_target_user_id", "target_user_id"), &LobbySearchEOSG::set_target_user_id);
    ClassDB::bind_method(D_METHOD("set_parameter", "key", "value", "comparison_op"), &LobbySearchEOSG::set_parameter);
    ClassDB::bind_method(D_METHOD("remove_parameter", "key", "comparison_op"), &LobbySearchEOSG::remove_parameter);
    ClassDB::bind_method(D_METHOD("set_max_results", "max_results"), &LobbySearchEOSG::set_max_results);
    ClassDB::bind_method(D_METHOD("get_search_result_count"), &LobbySearchEOSG::get_search_result_count);
    ClassDB::bind_method(D_METHOD("copy_search_result_by_index", "index"), &LobbySearchEOSG::copy_search_result_by_index);
}

void LobbySearchEOSG::find(const String& p_local_user_id) {
    CharString local_user_id = p_local_user_id.utf8();
    EOS_LobbySearch_FindOptions options;
    memset(&options, 0, sizeof(EOS_LobbySearch_FindOptions));
    options.ApiVersion = EOS_LOBBYSEARCH_FIND_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    // TODO: Handle passing clientData
    EOS_LobbySearch_Find(m_internal, &options, nullptr, [](const EOS_LobbySearch_FindCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        ret["client_data"] = Variant();
        IEOS::get_singleton()->emit_signal("lobby_search_find_callback", ret);
    });
    return;
}

int LobbySearchEOSG::set_lobby_id(const String& p_lobby_id) {
    CharString lobby_id = p_lobby_id.utf8();
    EOS_LobbySearch_SetLobbyIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_SETLOBBYID_API_LATEST;
    options.LobbyId = lobby_id.get_data();

    return static_cast<int>(EOS_LobbySearch_SetLobbyId(m_internal, &options));
}

int LobbySearchEOSG::set_target_user_id(const String& p_target_user_id) {
    CharString target_user_id = p_target_user_id.utf8();

    EOS_LobbySearch_SetTargetUserIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_SETTARGETUSERID_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());

    return static_cast<int>(EOS_LobbySearch_SetTargetUserId(m_internal, &options));
}

int LobbySearchEOSG::set_parameter(const String& p_key, Variant p_value, int p_comparison_op) {
    CharString key = p_key.utf8();

    EOS_Lobby_AttributeData attributeData;
    memset(&attributeData, 0, sizeof(attributeData));
    attributeData.ApiVersion = EOS_LOBBY_ATTRIBUTEDATA_API_LATEST;
    attributeData.Key = key.get_data();

    if (p_value.get_type() == Variant::Type::BOOL) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_BOOLEAN;
        attributeData.Value.AsBool = p_value;
    } else if (p_value.get_type() == Variant::Type::INT) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_INT64;
        attributeData.Value.AsInt64 = p_value;
    } else if (p_value.get_type() == Variant::Type::FLOAT) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_DOUBLE;
        attributeData.Value.AsDouble = p_value;
    } else if (p_value.get_type() == Variant::Type::STRING) {
        CharString value = VARIANT_TO_CHARSTRING(p_value);
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_STRING;
        attributeData.Value.AsUtf8 = value.get_data();
    }

    EOS_LobbySearch_SetParameterOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
    options.ComparisonOp = static_cast<EOS_EComparisonOp>(p_comparison_op);
    options.Parameter = &attributeData;

    return static_cast<int>(EOS_LobbySearch_SetParameter(m_internal, &options));
}

int LobbySearchEOSG::remove_parameter(const String& p_key, int p_comparison_op) {
    CharString key = p_key.utf8();

    EOS_LobbySearch_RemoveParameterOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_REMOVEPARAMETER_API_LATEST;
    options.Key = key.get_data();
    options.ComparisonOp = static_cast<EOS_EComparisonOp>(p_comparison_op);

    return static_cast<int>(EOS_LobbySearch_RemoveParameter(m_internal, &options));
}

int LobbySearchEOSG::set_max_results(int p_max_results) {
    EOS_LobbySearch_SetMaxResultsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_SETMAXRESULTS_API_LATEST;
    options.MaxResults = static_cast<uint32_t>(p_max_results);

    return static_cast<int>(EOS_LobbySearch_SetMaxResults(m_internal, &options));
}

int LobbySearchEOSG::get_search_result_count() {
    EOS_LobbySearch_GetSearchResultCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;

    return static_cast<int>(EOS_LobbySearch_GetSearchResultCount(m_internal, &options));
}

Dictionary LobbySearchEOSG::copy_search_result_by_index(int p_index) {
    EOS_LobbySearch_CopySearchResultByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
    options.LobbyIndex = static_cast<uint32_t>(p_index);

    EOS_HLobbyDetails outLobbyDetails = nullptr;
    EOS_EResult res = EOS_LobbySearch_CopySearchResultByIndex(m_internal, &options, &outLobbyDetails);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_details"] = eosg_lobby_lobby_details_to_wrapper(outLobbyDetails);
    return ret;
}

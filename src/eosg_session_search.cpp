#include "eosg_session_search.h"
#include "utils.h"

using namespace godot;

void EOSGSessionSearch::_bind_methods() {
    ClassDB::bind_method(D_METHOD("copy_search_result_by_index", "index"), &EOSGSessionSearch::copy_search_result_by_index);
    ClassDB::bind_method(D_METHOD("get_search_result_count"), &EOSGSessionSearch::get_search_result_count);
    ClassDB::bind_method(D_METHOD("remove_parameter", "key", "comparison_op"), &EOSGSessionSearch::remove_parameter);
    ClassDB::bind_method(D_METHOD("set_max_results", "max_results"), &EOSGSessionSearch::set_max_results);
    ClassDB::bind_method(D_METHOD("set_parameter", "key", "value", "comparison_op"), &EOSGSessionSearch::set_parameter);
    ClassDB::bind_method(D_METHOD("set_session_id", "session_id"), &EOSGSessionSearch::set_session_id);
    ClassDB::bind_method(D_METHOD("set_target_user_id", "target_user_id"), &EOSGSessionSearch::set_target_user_id);
    ClassDB::bind_method(D_METHOD("find", "local_user_id"), &EOSGSessionSearch::find);
}

Dictionary EOSGSessionSearch::copy_search_result_by_index(int p_index) {
	ERR_FAIL_NULL_V(m_internal, {});
    EOS_SessionSearch_CopySearchResultByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
    options.SessionIndex = static_cast<uint32_t>(p_index);

    EOS_HSessionDetails outInfo = nullptr;
    EOS_EResult res = EOS_SessionSearch_CopySearchResultByIndex(m_internal, &options, &outInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_details"] = eosg_sessions_session_details_to_wrapper(outInfo);
    return ret;
}

int EOSGSessionSearch::get_search_result_count() {
	ERR_FAIL_NULL_V(m_internal, 0);
    EOS_SessionSearch_GetSearchResultCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;

    int ret = EOS_SessionSearch_GetSearchResultCount(m_internal, &options);
    return ret;
}

int EOSGSessionSearch::remove_parameter(const String &p_key, int p_comparison_op) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString key = p_key.utf8();

    EOS_SessionSearch_RemoveParameterOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_REMOVEPARAMETER_API_LATEST;
    options.ComparisonOp = static_cast<EOS_EComparisonOp>(p_comparison_op);
    options.Key = key.get_data();

    EOS_EResult ret = EOS_SessionSearch_RemoveParameter(m_internal, &options);
    return static_cast<int>(ret);
}

int EOSGSessionSearch::set_max_results(int p_max_results) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_SessionSearch_SetMaxResultsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_SETMAXSEARCHRESULTS_API_LATEST;
    options.MaxSearchResults = static_cast<uint32_t>(p_max_results);

    EOS_EResult ret = EOS_SessionSearch_SetMaxResults(m_internal, &options);
    return static_cast<int>(ret);
}

int EOSGSessionSearch::set_parameter(const String &p_key, Variant p_value, int p_comparison_op) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString key = p_key.utf8();

    EOS_Sessions_AttributeData attributeData;
    memset(&attributeData, 0, sizeof(attributeData));
    attributeData.ApiVersion = EOS_SESSIONS_ATTRIBUTEDATA_API_LATEST;
    attributeData.Key = key.get_data();
    CharString value;

    if (p_value.get_type() == Variant::Type::BOOL) {
        attributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;
        attributeData.Value.AsBool = p_value;
    } else if (p_value.get_type() == Variant::Type::INT) {
        attributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;
        attributeData.Value.AsInt64 = p_value;
    } else if (p_value.get_type() == Variant::Type::FLOAT) {
        attributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;
        attributeData.Value.AsDouble = p_value;
    } else if (p_value.get_type() == Variant::Type::STRING) {
        value = VARIANT_TO_CHARSTRING(p_value);
        attributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;
        attributeData.Value.AsUtf8 = value.get_data();
    }

    EOS_SessionSearch_SetParameterOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_SETPARAMETER_API_LATEST;
    options.ComparisonOp = static_cast<EOS_EOnlineComparisonOp>(p_comparison_op);
    options.Parameter = &attributeData;

    return static_cast<int>(EOS_SessionSearch_SetParameter(m_internal, &options));
}

int EOSGSessionSearch::set_session_id(const String &p_session_id) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString session_id = p_session_id.utf8();

    EOS_SessionSearch_SetSessionIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_SETSESSIONID_API_LATEST;
    options.SessionId = session_id.get_data();

    EOS_EResult ret = EOS_SessionSearch_SetSessionId(m_internal, &options);
    return static_cast<int>(ret);
}

int EOSGSessionSearch::set_target_user_id(const String &p_target_user_id) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString target_user_id = p_target_user_id.utf8();

    EOS_SessionSearch_SetTargetUserIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_SETTARGETUSERID_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());

    EOS_EResult ret = EOS_SessionSearch_SetTargetUserId(m_internal, &options);
    return static_cast<int>(ret);
}

void EOSGSessionSearch::find(const String &p_local_user_id) {
	ERR_FAIL_NULL(m_internal);
    CharString local_user_id = p_local_user_id.utf8();

    EOS_SessionSearch_FindOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONSEARCH_FIND_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    // TODO: Handle passing ClientData if needed
    EOS_SessionSearch_Find(m_internal, &options, nullptr, [](const EOS_SessionSearch_FindCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        ret["client_data"] = Variant();
        IEOS::get_singleton()->emit_signal("session_search_find_callback", ret);
    });
}
#include "eosg_session_details.h"
#include "utils.h"
using namespace godot;

void EOSGSessionDetails::_bind_methods() {
    ClassDB::bind_method(D_METHOD("copy_info"), &EOSGSessionDetails::copy_info);
    ClassDB::bind_method(D_METHOD("copy_session_attribute_by_index", "attribute_index"), &EOSGSessionDetails::copy_session_attribute_by_index);
    ClassDB::bind_method(D_METHOD("copy_session_attribute_by_key", "attribute_key"), &EOSGSessionDetails::copy_session_attribute_by_key);
    ClassDB::bind_method(D_METHOD("get_session_attribute_count"), &EOSGSessionDetails::get_session_attribute_count);
}

Dictionary EOSGSessionDetails::copy_info() {
    EOS_SessionDetails_CopyInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONDETAILS_COPYINFO_API_LATEST;

    EOS_SessionDetails_Info *outInfo = nullptr;
    EOS_EResult res = EOS_SessionDetails_CopyInfo(m_internal, &options, &outInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["info"] = eosg_sessions_session_details_info_to_dict_and_release(outInfo);
    return ret;
}

Dictionary EOSGSessionDetails::copy_session_attribute_by_index(int p_attribute_index) {
    EOS_SessionDetails_CopySessionAttributeByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYINDEX_API_LATEST;
    options.AttrIndex = static_cast<uint32_t>(p_attribute_index);

    EOS_SessionDetails_Attribute *outAttr = nullptr;
    EOS_EResult res = EOS_SessionDetails_CopySessionAttributeByIndex(m_internal, &options, &outAttr);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["attribute"] = eosg_sessions_session_details_attribute_to_dict_and_release(outAttr);
    return ret;
}

Dictionary EOSGSessionDetails::copy_session_attribute_by_key(const String &p_attribute_key) {
    CharString attribute_key = p_attribute_key.utf8();

    EOS_SessionDetails_CopySessionAttributeByKeyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYKEY_API_LATEST;
    options.AttrKey = attribute_key.get_data();

    EOS_SessionDetails_Attribute *outAttr = nullptr;
    EOS_EResult res = EOS_SessionDetails_CopySessionAttributeByKey(m_internal, &options, &outAttr);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["attribute"] = eosg_sessions_session_details_attribute_to_dict_and_release(outAttr);
    return ret;
}

int EOSGSessionDetails::get_session_attribute_count() {
    EOS_SessionDetails_GetSessionAttributeCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONDETAILS_GETSESSIONATTRIBUTECOUNT_API_LATEST;

    return static_cast<int>(EOS_SessionDetails_GetSessionAttributeCount(m_internal, &options));
}
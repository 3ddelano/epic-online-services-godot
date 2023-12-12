#include "lobby_details.h"

#include "utils.h"

using namespace godot;

void LobbyDetailsEOSG::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_lobby_owner"), &LobbyDetailsEOSG::get_lobby_owner);
    ClassDB::bind_method(D_METHOD("copy_info"), &LobbyDetailsEOSG::copy_info);
    ClassDB::bind_method(D_METHOD("get_attribute_count"), &LobbyDetailsEOSG::get_attribute_count);
    ClassDB::bind_method(D_METHOD("copy_attribute_by_index", "index"), &LobbyDetailsEOSG::copy_attribute_by_index);
    ClassDB::bind_method(D_METHOD("copy_attribute_by_key", "key"), &LobbyDetailsEOSG::copy_attribute_by_key);
    ClassDB::bind_method(D_METHOD("get_member_count"), &LobbyDetailsEOSG::get_member_count);
    ClassDB::bind_method(D_METHOD("get_member_by_index", "index"), &LobbyDetailsEOSG::get_member_by_index);
    ClassDB::bind_method(D_METHOD("get_member_attribute_count", "target_user_id"), &LobbyDetailsEOSG::get_member_attribute_count);
    ClassDB::bind_method(D_METHOD("copy_member_attribute_by_index", "target_user_id", "index"), &LobbyDetailsEOSG::copy_member_attribute_by_index);
    ClassDB::bind_method(D_METHOD("copy_member_attribute_by_key", "target_user_id", "key"), &LobbyDetailsEOSG::copy_member_attribute_by_key);
}

String LobbyDetailsEOSG::get_lobby_owner() {
    EOS_LobbyDetails_GetLobbyOwnerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_GETLOBBYOWNER_API_LATEST;
    return eosg_product_user_id_to_string(EOS_LobbyDetails_GetLobbyOwner(m_internal, &options));
}

Dictionary LobbyDetailsEOSG::copy_info() {
    EOS_LobbyDetails_CopyInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_COPYINFO_API_LATEST;

    EOS_LobbyDetails_Info *outLobbyDetails = nullptr;
    EOS_EResult res = EOS_LobbyDetails_CopyInfo(m_internal, &options, &outLobbyDetails);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_details"] = eos_lobby_details_info_to_dict(outLobbyDetails);
    return ret;
}

int LobbyDetailsEOSG::get_attribute_count() {
    EOS_LobbyDetails_GetAttributeCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST;
    return static_cast<int>(EOS_LobbyDetails_GetAttributeCount(m_internal, &options));
}

Dictionary LobbyDetailsEOSG::copy_attribute_by_index(int p_index) {
    EOS_LobbyDetails_CopyAttributeByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST;
    options.AttrIndex = static_cast<uint32_t>(p_index);

    EOS_Lobby_Attribute *outAttr = nullptr;
    EOS_EResult res = EOS_LobbyDetails_CopyAttributeByIndex(m_internal, &options, &outAttr);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["attribute"] = eos_lobby_attribute_to_dict(outAttr);
    return ret;
}

Dictionary LobbyDetailsEOSG::copy_attribute_by_key(const String &p_key) {
    CharString key = p_key.utf8();

    EOS_LobbyDetails_CopyAttributeByKeyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_COPYATTRIBUTEBYKEY_API_LATEST;
    options.AttrKey = key.get_data();

    EOS_Lobby_Attribute *outAttr = nullptr;
    EOS_EResult res = EOS_LobbyDetails_CopyAttributeByKey(m_internal, &options, &outAttr);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["attribute"] = eos_lobby_attribute_to_dict(outAttr);
    return ret;
}

int LobbyDetailsEOSG::get_member_count() {
    EOS_LobbyDetails_GetMemberCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST;
    return static_cast<int>(EOS_LobbyDetails_GetMemberCount(m_internal, &options));
}

String LobbyDetailsEOSG::get_member_by_index(int p_index) {
    EOS_LobbyDetails_GetMemberByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST;
    options.MemberIndex = static_cast<uint32_t>(p_index);
    return eosg_product_user_id_to_string(EOS_LobbyDetails_GetMemberByIndex(m_internal, &options));
}

int LobbyDetailsEOSG::get_member_attribute_count(const String &p_target_user_id) {
    CharString targetUserId = p_target_user_id.utf8();

    EOS_LobbyDetails_GetMemberAttributeCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERATTRIBUTECOUNT_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(targetUserId.get_data());

    return static_cast<int>(EOS_LobbyDetails_GetMemberAttributeCount(m_internal, &options));
}

Dictionary LobbyDetailsEOSG::copy_member_attribute_by_index(const String &p_target_user_id, int p_index) {
    CharString targetUserId = p_target_user_id.utf8();

    EOS_LobbyDetails_CopyMemberAttributeByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYINDEX_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(targetUserId.get_data());
    options.AttrIndex = static_cast<uint32_t>(p_index);

    EOS_Lobby_Attribute *outAttr = nullptr;
    EOS_EResult res = EOS_LobbyDetails_CopyMemberAttributeByIndex(m_internal, &options, &outAttr);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["attribute"] = eos_lobby_attribute_to_dict(outAttr);
    return ret;
}

Dictionary LobbyDetailsEOSG::copy_member_attribute_by_key(const String &p_target_user_id, const String &p_key) {
    CharString targetUserId = p_target_user_id.utf8();
    CharString key = p_key.utf8();

    EOS_LobbyDetails_CopyMemberAttributeByKeyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYKEY_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(targetUserId.get_data());
    options.AttrKey = key.get_data();

    EOS_Lobby_Attribute *outAttr = nullptr;
    EOS_EResult res = EOS_LobbyDetails_CopyMemberAttributeByKey(m_internal, &options, &outAttr);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["attribute"] = eos_lobby_attribute_to_dict(outAttr);
    return ret;
}

Dictionary LobbyDetailsEOSG::copy_member_info(const String &p_target_user_id) {
    CharString targetUserId = p_target_user_id.utf8();

    EOS_LobbyDetails_CopyMemberInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERINFO_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(targetUserId.get_data());

    EOS_LobbyDetails_MemberInfo *outMemberInfo = nullptr;
    EOS_EResult res = EOS_LobbyDetails_CopyMemberInfo(m_internal, &options, &outMemberInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_details"] = eos_lobby_details_member_info_to_dict(outMemberInfo);
    return ret;
}
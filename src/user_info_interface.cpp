#include "ieos.h"
using namespace std;

Dictionary IEOS::user_info_interface_copy_external_user_info_by_account_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));

    EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_COPYEXTERNALUSERINFOBYACCOUNTID_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    options.AccountId = account_id.get_data();

    EOS_UserInfo_ExternalUserInfo* outExternalUserInfo = nullptr;
    EOS_EResult res = EOS_UserInfo_CopyExternalUserInfoByAccountId(s_userInfoInterface, &options, &outExternalUserInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_user_info"] = eosg_user_info_external_user_info_to_dict(outExternalUserInfo);
    return ret;
}

Dictionary IEOS::user_info_interface_copy_external_user_info_by_account_type(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_COPYEXTERNALUSERINFOBYACCOUNTTYPE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    options.AccountType = static_cast<EOS_EExternalAccountType>(static_cast<int>(p_options->get("account_type")));

    EOS_UserInfo_ExternalUserInfo* outExternalUserInfo = nullptr;
    EOS_EResult res = EOS_UserInfo_CopyExternalUserInfoByAccountType(s_userInfoInterface, &options, &outExternalUserInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_user_info"] = eosg_user_info_external_user_info_to_dict(outExternalUserInfo);
    return ret;
}

Dictionary IEOS::user_info_interface_copy_external_user_info_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UserInfo_CopyExternalUserInfoByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_COPYEXTERNALUSERINFOBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    options.Index = static_cast<uint32_t>(p_options->get("index"));

    EOS_UserInfo_ExternalUserInfo* outExternalUserInfo = nullptr;
    EOS_EResult res = EOS_UserInfo_CopyExternalUserInfoByIndex(s_userInfoInterface, &options, &outExternalUserInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_user_info"] = eosg_user_info_external_user_info_to_dict(outExternalUserInfo);
    return ret;
}

Dictionary IEOS::user_info_interface_copy_user_info(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UserInfo_CopyUserInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());

    EOS_UserInfo* outUserInfo = nullptr;
    EOS_EResult res = EOS_UserInfo_CopyUserInfo(s_userInfoInterface, &options, &outUserInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["user_info"] = eosg_user_info_user_info_to_dict(outUserInfo);
    return ret;
}

int IEOS::user_info_interface_get_external_user_info_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UserInfo_GetExternalUserInfoCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_GETEXTERNALUSERINFOCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());

    return static_cast<int>(EOS_UserInfo_GetExternalUserInfoCount(s_userInfoInterface, &options));
}

void IEOS::user_info_interface_query_user_info(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UserInfo_QueryUserInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_UserInfo_QueryUserInfo(s_userInfoInterface, &options, (void*)*p_options, [](const EOS_UserInfo_QueryUserInfoCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("user_info_interface_query_user_info_callback", ret);
    });
    return;
}

void IEOS::user_info_interface_query_user_info_by_display_name(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString display_name = VARIANT_TO_CHARSTRING(p_options->get("display_name"));

    EOS_UserInfo_QueryUserInfoByDisplayNameOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_QUERYUSERINFOBYDISPLAYNAME_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.DisplayName = display_name.get_data();
    p_options->reference();

    EOS_UserInfo_QueryUserInfoByDisplayName(s_userInfoInterface, &options, (void*)*p_options, [](const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        ret["display_name"] = EOSG_GET_STRING(data->DisplayName);
        IEOS::get_singleton()->emit_signal("user_info_interface_query_user_info_by_display_name_callback", ret);
    });
    return;
}

void IEOS::user_info_interface_query_user_info_by_external_account(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString external_account_id = VARIANT_TO_CHARSTRING(p_options->get("external_account_id"));

    EOS_UserInfo_QueryUserInfoByExternalAccountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_USERINFO_QUERYUSERINFOBYEXTERNALACCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.ExternalAccountId = external_account_id.get_data();
    options.AccountType = static_cast<EOS_EExternalAccountType>(static_cast<int>(p_options->get("account_type")));
    p_options->reference();

    EOS_UserInfo_QueryUserInfoByExternalAccount(s_userInfoInterface, &options, (void*)*p_options, [](const EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        ret["external_account_id"] = EOSG_GET_STRING(data->ExternalAccountId);
        ret["account_type"] = static_cast<int>(data->AccountType);
        IEOS::get_singleton()->emit_signal("user_info_interface_query_user_info_by_external_account_callback", ret);
    });
    return;
}

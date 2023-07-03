#include "ieos.h"
using namespace godot;

void IEOS::connect_interface_login(Ref<RefCounted> p_options) {
    Ref<RefCounted> p_credentials = p_options->get("credentials");
    Ref<RefCounted> p_user_login_info = p_options->get("user_login_info");
    CharString token = VARIANT_TO_CHARSTRING(p_credentials->get("token"));
    String p_display_name;
    if (p_user_login_info != nullptr) {
        p_display_name = p_user_login_info->get("display_name");
    }
    CharString display_name = p_display_name.utf8();

    EOS_Connect_Credentials credentials;
    memset(&credentials, 0, sizeof(EOS_Connect_Credentials));
    credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
    credentials.Type = static_cast<EOS_EExternalCredentialType>((int)p_credentials->get("type"));
    credentials.Token = token.get_data();

    EOS_Connect_UserLoginInfo userLoginInfo;
    memset(&userLoginInfo, 0, sizeof(EOS_Connect_LoginOptions));
    if (!p_display_name.is_empty()) {
        userLoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
        userLoginInfo.DisplayName = display_name.get_data();
    }

    EOS_Connect_LoginOptions options;
    memset(&options, 0, sizeof(EOS_Connect_LoginOptions));
    options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
    options.Credentials = &credentials;
    if (!p_display_name.is_empty()) {
        options.UserLoginInfo = &userLoginInfo;
    }
    p_options->reference();

    EOS_Connect_Login(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_LoginCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["continuance_token"] = eosg_continuance_token_to_wrapper(data->ContinuanceToken);
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);

        if (data->ResultCode == EOS_EResult::EOS_Success) {
            ret["success"] = true;
            ret["pending"] = false;
        } else if (EOS_EResult_IsOperationComplete(data->ResultCode)) {
            ret["success"] = false;
            ret["pending"] = true;
        } else {
            ret["success"] = false;
            ret["pending"] = false;
        }

        IEOS::get_singleton()->emit_signal("connect_interface_login_callback", ret);
    });
    return;
}

Dictionary IEOS::connect_interface_copy_id_token(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Connect_CopyIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    EOS_Connect_IdToken* outToken = nullptr;
    EOS_EResult res = EOS_Connect_CopyIdToken(s_connectInterface, &options, &outToken);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["id_token"] = eosg_connect_id_token_to_dict(outToken);
    return ret;
}

Dictionary IEOS::connect_interface_copy_product_user_external_account_by_account_id(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));

    EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTID_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.AccountId = account_id.get_data();

    EOS_Connect_ExternalAccountInfo* outExternalAccountInfo = nullptr;

    EOS_EResult res = EOS_Connect_CopyProductUserExternalAccountByAccountId(s_connectInterface, &options, &outExternalAccountInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_account_info"] = eosg_connect_external_account_info_to_dict(outExternalAccountInfo);
    return ret;
}

Dictionary IEOS::connect_interface_copy_product_user_external_account_by_account_type(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTTYPE_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.AccountIdType = static_cast<EOS_EExternalAccountType>(static_cast<int>(p_options->get("account_id_type")));

    EOS_Connect_ExternalAccountInfo* outExternalAccountInfo = nullptr;
    EOS_EResult res = EOS_Connect_CopyProductUserExternalAccountByAccountType(s_connectInterface, &options, &outExternalAccountInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_account_info"] = eosg_connect_external_account_info_to_dict(outExternalAccountInfo);
    return ret;
}

Dictionary IEOS::connect_interface_copy_product_user_external_account_by_index(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Connect_CopyProductUserExternalAccountByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.ExternalAccountInfoIndex = static_cast<uint32_t>(static_cast<int>(p_options->get("external_account_info_index")));

    EOS_Connect_ExternalAccountInfo* outExternalAccountInfo = nullptr;
    EOS_EResult res = EOS_Connect_CopyProductUserExternalAccountByIndex(s_connectInterface, &options, &outExternalAccountInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_account_info"] = eosg_connect_external_account_info_to_dict(outExternalAccountInfo);
    return ret;
}

Dictionary IEOS::connect_interface_copy_product_user_info(Ref<RefCounted> p_options) {
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Connect_CopyProductUserInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_COPYPRODUCTUSERINFO_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());

    EOS_Connect_ExternalAccountInfo* outExternalAccountInfo = nullptr;
    EOS_EResult res = EOS_Connect_CopyProductUserInfo(s_connectInterface, &options, &outExternalAccountInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["external_account_info"] = eosg_connect_external_account_info_to_dict(outExternalAccountInfo);
    return ret;
}

void IEOS::connect_interface_create_device_id(Ref<RefCounted> p_options) {
    CharString p_device_model = VARIANT_TO_CHARSTRING(p_options->get("device_model"));

    EOS_Connect_CreateDeviceIdOptions options;
    memset(&options, 0, sizeof(EOS_Connect_CreateDeviceIdOptions));
    options.ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;
    options.DeviceModel = p_device_model.get_data();
    p_options->reference();

    EOS_Connect_CreateDeviceId(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_CreateDeviceIdCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("connect_interface_create_device_id_callback", ret);
    });
    return;
}

void IEOS::connect_interface_delete_device_id(Ref<RefCounted> p_options) {
    EOS_Connect_DeleteDeviceIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_DELETEDEVICEID_API_LATEST;

    p_options->reference();
    EOS_Connect_DeleteDeviceId(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_DeleteDeviceIdCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("connect_interface_delete_device_id_callback", ret);
    });
    return;
}

void IEOS::connect_interface_create_user(Ref<RefCounted> p_options) {
    Ref<ContinuanceTokenEOSG> p_continuance_token = Object::cast_to<ContinuanceTokenEOSG>(p_options->get("continuance_token"));

    EOS_Connect_CreateUserOptions options;
    memset(&options, 0, sizeof(EOS_Connect_CreateUserOptions));
    options.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
    options.ContinuanceToken = p_continuance_token->get_internal();
    p_options->reference();

    EOS_Connect_CreateUser(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_CreateUserCallbackInfo* data) {
        Dictionary ret;
        ret["reuslt_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);

        IEOS::get_singleton()->emit_signal("connect_interface_create_user_callback", ret);
    });
    return;
}

String IEOS::connect_interface_get_external_account_mapping(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_external_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_external_user_id"));

    EOS_Connect_GetExternalAccountMappingsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_GETEXTERNALACCOUNTMAPPINGS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.AccountIdType = static_cast<EOS_EExternalAccountType>(static_cast<int>(p_options->get("account_id_type")));
    options.TargetExternalUserId = target_external_user_id.get_data();

    return eosg_product_user_id_to_string(EOS_Connect_GetExternalAccountMapping(s_connectInterface, &options));
}

String IEOS::connect_interface_get_logged_in_user_by_index(int p_index) {
    return eosg_product_user_id_to_string(EOS_Connect_GetLoggedInUserByIndex(s_connectInterface, p_index));
}

int IEOS::connect_interface_get_logged_in_users_count() {
    return static_cast<int>(EOS_Connect_GetLoggedInUsersCount(s_connectInterface));
}

int IEOS::connect_interface_get_login_status(const String& p_local_user_id) {
    CharString local_user_id = p_local_user_id.utf8();
    EOS_ProductUserId localUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    return static_cast<int>(EOS_Connect_GetLoginStatus(s_connectInterface, localUserId));
}

int IEOS::connect_interface_get_product_user_external_account_count(Ref<RefCounted> p_options) {
    CharString p_target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Connect_GetProductUserExternalAccountCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(p_target_user_id.get_data());

    return static_cast<int>(EOS_Connect_GetProductUserExternalAccountCount(s_connectInterface, &options));
}

Dictionary IEOS::connect_interface_get_product_user_id_mapping(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_product_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_product_user_id"));

    EOS_Connect_GetProductUserIdMappingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.AccountIdType = static_cast<EOS_EExternalAccountType>(static_cast<int>(p_options->get("account_id_type")));
    options.TargetProductUserId = eosg_string_to_product_user_id(target_product_user_id.get_data());

    char* outAccountId = (char*)memalloc(EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH + 1);
    int outAccountIdLength = 0;
    EOS_EResult res = EOS_Connect_GetProductUserIdMapping(s_connectInterface, &options, outAccountId, &outAccountIdLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["account_id"] = EOSG_GET_STRING(outAccountId);
    return ret;
}

void IEOS::connect_interface_query_product_user_id_mapping(Ref<RefCounted> p_options) {
    CharString p_local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    TypedArray<String> p_product_user_ids = p_options->get("product_user_ids");

    // Convert the p_product_user_ids to array of EOS_ProductUserId*
    EOS_ProductUserId* product_user_ids = nullptr;
    if (p_product_user_ids.size() > 0) {
        product_user_ids = (EOS_ProductUserId*)memalloc(sizeof(EOS_ProductUserId) * p_product_user_ids.size());
        for (int i = 0; i < p_product_user_ids.size(); i++) {
            String product_user_id = p_product_user_ids[i];
            CharString product_user_id_cstr = product_user_id.utf8();
            product_user_ids[i] = eosg_string_to_product_user_id(product_user_id_cstr.get_data());
        }
    }

    EOS_Connect_QueryProductUserIdMappingsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(p_local_user_id.get_data());
    options.ProductUserIds = product_user_ids;
    options.ProductUserIdCount = p_product_user_ids.size();
    p_options->reference();

    EOS_Connect_QueryProductUserIdMappings(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("connect_interface_query_product_user_id_mappings_callback", ret);
    });
    return;
}

void IEOS::connect_interface_link_account(Ref<RefCounted> p_options) {
    Ref<ContinuanceTokenEOSG> p_continuance_token = Object::cast_to<ContinuanceTokenEOSG>(p_options->get("continuance_token"));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Connect_LinkAccountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_LINKACCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.ContinuanceToken = p_continuance_token->get_internal();
    p_options->reference();

    EOS_Connect_LinkAccount(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_LinkAccountCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("connect_interface_link_account_callback", ret);
    });
    return;
}

void IEOS::connect_interface_verify_id_token(Ref<RefCounted> p_options) {
    Ref<RefCounted> p_id_token = p_options->get("id_token");
    CharString p_product_user_id = VARIANT_TO_CHARSTRING(p_id_token->get("product_user_id"));
    CharString p_json_web_token = VARIANT_TO_CHARSTRING(p_id_token->get("json_web_token"));

    EOS_Connect_IdToken idToken;
    memset(&idToken, 0, sizeof(idToken));
    idToken.ApiVersion = EOS_CONNECT_IDTOKEN_API_LATEST;
    idToken.ProductUserId = eosg_string_to_product_user_id(p_product_user_id.get_data());
    idToken.JsonWebToken = p_json_web_token.get_data();

    EOS_Connect_VerifyIdTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_VERIFYIDTOKEN_API_LATEST;
    options.IdToken = &idToken;

    EOS_Connect_VerifyIdToken(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_VerifyIdTokenCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["product_user_id"] = eosg_product_user_id_to_string(data->ProductUserId);
        ret["is_account_info_present"] = EOSG_GET_BOOL(data->bIsAccountInfoPresent);
        ret["account_id_type"] = static_cast<int>(data->AccountIdType);
        ret["account_id"] = String(data->AccountId);
        IEOS::get_singleton()->emit_signal("connect_interface_verify_id_token_callback", ret);
    });
    return;
}

void IEOS::connect_interface_transfer_device_id_account(Ref<RefCounted> p_options) {
    String p_primary_local_user_id = p_options->get("primary_local_user_id");
    CharString primary_local_user_id = VARIANT_TO_CHARSTRING(p_options->get("primary_local_user_id"));
    CharString local_device_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_device_user_id"));
    String p_product_user_id_to_preserve = p_options->get("product_user_id_to_preserve");
    CharString product_user_id_to_preserve = VARIANT_TO_CHARSTRING(p_options->get("product_user_id_to_preserve"));

    EOS_ProductUserId primaryLocalUserId = eosg_string_to_product_user_id(primary_local_user_id.get_data());
    EOS_ProductUserId localDeviceUserId = eosg_string_to_product_user_id(local_device_user_id.get_data());

    EOS_Connect_TransferDeviceIdAccountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_TRANSFERDEVICEIDACCOUNT_API_LATEST;
    options.PrimaryLocalUserId = primaryLocalUserId;
    options.LocalDeviceUserId = localDeviceUserId;
    if (p_product_user_id_to_preserve == p_primary_local_user_id) {
        options.ProductUserIdToPreserve = primaryLocalUserId;
    } else {
        options.PrimaryLocalUserId = localDeviceUserId;
    }
    p_options->reference();

    EOS_Connect_TransferDeviceIdAccount(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_TransferDeviceIdAccountCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("connect_interface_transfer_device_id_account_callback", ret);
    });
    return;
}

void IEOS::connect_interface_unlink_account(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Connect_UnlinkAccountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CONNECT_UNLINKACCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_Connect_UnlinkAccount(s_connectInterface, &options, (void*)*p_options, [](const EOS_Connect_UnlinkAccountCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("connect_interface_unlink_account_callback", ret);
    });
    return;
}

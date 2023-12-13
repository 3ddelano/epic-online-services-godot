#include "ieos.h"
using namespace godot;

Dictionary IEOS::kws_interface_copy_permission_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_KWS_CopyPermissionByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_COPYPERMISSIONBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Index = static_cast<uint32_t>(static_cast<int>(p_options->get("index")));

    EOS_KWS_PermissionStatus *outPermission = nullptr;
    EOS_EResult res = EOS_KWS_CopyPermissionByIndex(s_kwsInterface, &options, &outPermission);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["permission"] = eosg_kws_permission_status_to_dict_and_release(outPermission);
    return ret;
}

void IEOS::kws_interface_create_user(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString parent_email = VARIANT_TO_CHARSTRING(p_options->get("parent_email"));
    CharString date_of_birth = VARIANT_TO_CHARSTRING(p_options->get("date_of_birth"));

    EOS_KWS_CreateUserOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_CREATEUSER_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.ParentEmail = parent_email.get_data();
    options.DateOfBirth = date_of_birth.get_data();
    p_options->reference();

    EOS_KWS_CreateUser(s_kwsInterface, &options, (void *)*p_options, [](const EOS_KWS_CreateUserCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["kws_user_id"] = EOSG_GET_STRING(data->KWSUserId);
        ret["is_minor"] = EOSG_GET_BOOL(data->bIsMinor);
        IEOS::get_singleton()->emit_signal("kws_interface_create_user_callback", ret);
    });
}

Dictionary IEOS::kws_interface_get_permission_by_key(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString key = VARIANT_TO_CHARSTRING(p_options->get("key"));

    EOS_KWS_GetPermissionByKeyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_GETPERMISSIONBYKEY_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Key = key.get_data();

    EOS_EKWSPermissionStatus *outPermission;
    EOS_EResult res = EOS_KWS_GetPermissionByKey(s_kwsInterface, &options, outPermission);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["permission"] = static_cast<int>(*outPermission);
    return ret;
}

int IEOS::kws_interface_get_permissions_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_KWS_GetPermissionsCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_GETPERMISSIONSCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    return static_cast<int>(EOS_KWS_GetPermissionsCount(s_kwsInterface, &options));
}

void IEOS::kws_interface_query_age_gate(Ref<RefCounted> p_options) {
    EOS_KWS_QueryAgeGateOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_QUERYAGEGATE_API_LATEST;
    p_options->reference();

    EOS_KWS_QueryAgeGate(s_kwsInterface, &options, (void *)*p_options, [](const EOS_KWS_QueryAgeGateCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["country_code"] = EOSG_GET_STRING(data->CountryCode);
        ret["age_of_consent"] = data->AgeOfConsent;
        IEOS::get_singleton()->emit_signal("kws_interface_query_age_gate_callback", ret);
    });
}

void IEOS::kws_interface_query_permissions(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_KWS_QueryPermissionsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_QUERYPERMISSIONS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_KWS_QueryPermissions(s_kwsInterface, &options, (void *)*p_options, [](const EOS_KWS_QueryPermissionsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["kws_user_id"] = EOSG_GET_STRING(data->KWSUserId);
        ret["is_minor"] = EOSG_GET_BOOL(data->bIsMinor);
        ret["parent_email"] = EOSG_GET_STRING(data->ParentEmail);
        IEOS::get_singleton()->emit_signal("kws_interface_query_permissions_callback", ret);
    });
}

void IEOS::kws_interface_request_permissions(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    Array p_permission_keys = p_options->get("permission_keys");
    int p_permission_keys_size = p_permission_keys.size();

    const char **permissionsKeys = (const char **)memalloc(sizeof(char *) * p_permission_keys_size);
    for (int i = 0; i < p_permission_keys_size; i++) {
        CharString permission_key = VARIANT_TO_CHARSTRING(p_permission_keys[i]);
        permissionsKeys[i] = permission_key.get_data();
    }

    EOS_KWS_RequestPermissionsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_REQUESTPERMISSIONS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.PermissionKeys = permissionsKeys;
    options.PermissionKeyCount = p_permission_keys_size;
    p_options->reference();

    EOS_KWS_RequestPermissions(s_kwsInterface, &options, (void *)*p_options, [](const EOS_KWS_RequestPermissionsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("kws_interface_request_permissions_callback", ret);
    });
}

void IEOS::kws_interface_update_parent_email(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString parent_email = VARIANT_TO_CHARSTRING(p_options->get("parent_email"));

    EOS_KWS_UpdateParentEmailOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_KWS_UPDATEPARENTEMAIL_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.ParentEmail = parent_email.get_data();
    p_options->reference();

    EOS_KWS_UpdateParentEmail(s_kwsInterface, &options, (void *)*p_options, [](const EOS_KWS_UpdateParentEmailCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("kws_interface_update_parent_email_callback", ret);
    });
}

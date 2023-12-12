#include "ieos.h"
using namespace std;

Dictionary IEOS::presence_interface_copy_presence(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Presence_CopyPresenceOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_COPYPRESENCE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());

    EOS_Presence_Info* outPresenceInfo = nullptr;
    EOS_EResult res = EOS_Presence_CopyPresence(s_presenceInterface, &options, &outPresenceInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["presence_info"] = eosg_presence_presence_info_to_dict_and_release(outPresenceInfo);
    return ret;
}

Dictionary IEOS::presence_interface_create_presence_modification(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Presence_CreatePresenceModificationOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    EOS_HPresenceModification outPresenceModification = nullptr;
    EOS_EResult res = EOS_Presence_CreatePresenceModification(s_presenceInterface, &options, &outPresenceModification);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["presence_modification"] = eosg_presence_presence_modification_to_wrapper(outPresenceModification);
    return ret;
}

Dictionary IEOS::presence_interface_get_join_info(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Presence_GetJoinInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_GETJOININFO_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());

    char* outBuffer = (char*)memalloc(EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH + 1);
    int outBufferLength = EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH + 1;

    EOS_EResult res = EOS_Presence_GetJoinInfo(s_presenceInterface, &options, outBuffer, &outBufferLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["join_info"] = outBuffer;
    return ret;
}

bool IEOS::presence_interface_has_presence(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Presence_HasPresenceOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_HASPRESENCE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());

    return EOSG_GET_BOOL(
        EOS_Presence_HasPresence(s_presenceInterface, &options));
}

void IEOS::presence_interface_query_presence(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Presence_QueryPresenceOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_QUERYPRESENCE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_Presence_QueryPresence(s_presenceInterface, &options, (void*)*p_options, [](const EOS_Presence_QueryPresenceCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("presence_interface_query_presence_callback", ret);
    });
    return;
}

void IEOS::presence_interface_set_presence(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    Ref<PresenceModificationEOSG> presence_modification = Object::cast_to<PresenceModificationEOSG>(p_options->get("presence_modification"));

    EOS_Presence_SetPresenceOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_SETPRESENCE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.PresenceModificationHandle = presence_modification->get_internal();
    p_options->reference();

    EOS_Presence_SetPresence(s_presenceInterface, &options, (void*)*p_options, [](const EOS_Presence_SetPresenceCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("presence_interface_set_presence_callback", ret);
    });
    return;
}

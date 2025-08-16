#include "ieos.h"
using namespace std;

int IEOS::custom_invites_interface_finalize_invite(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_customInvitesInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString custom_invite_id = VARIANT_TO_CHARSTRING(p_options->get("custom_invite_id"));

    EOS_CustomInvites_FinalizeInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CUSTOMINVITES_FINALIZEINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.CustomInviteId = custom_invite_id.get_data();
    options.ProcessingResult = static_cast<EOS_EResult>(static_cast<int>(p_options->get("processing_result")));

    return static_cast<int>(EOS_CustomInvites_FinalizeInvite(s_customInvitesInterface, &options));
}

void IEOS::custom_invites_interface_send_custom_invite(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_customInvitesInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    Array p_target_user_ids = p_options->get("target_user_ids");
    int target_user_ids_count = p_target_user_ids.size();

    EOS_ProductUserId *targetUserIds = (EOS_ProductUserId *)memalloc(sizeof(EOS_ProductUserId) * target_user_ids_count);
	PERSISTENT_CHAR_ARRAY_CREATE(target_user_ids, target_user_ids_charstrings, target_user_ids_count)
    for (int i = 0; i < target_user_ids_count; i++) {
        PERSISTENT_CHAR_ARRAY_SET(target_user_ids, target_user_ids_charstrings, i, p_target_user_ids[i]);
        targetUserIds[i] = eosg_string_to_product_user_id(target_user_ids[i]);
    }

    EOS_CustomInvites_SendCustomInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CUSTOMINVITES_SENDCUSTOMINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserIds = targetUserIds;
    options.TargetUserIdsCount = target_user_ids_count;
    p_options->reference();

    EOS_CustomInvites_SendCustomInvite(s_customInvitesInterface, &options, (void *)*p_options, [](const EOS_CustomInvites_SendCustomInviteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);

        Array target_user_ids = Array();
        for (int i = 0; i < data->TargetUserIdsCount; i++) {
            target_user_ids.append(eosg_product_user_id_to_string(data->TargetUserIds[i]));
        }
        ret["target_user_ids"] = target_user_ids;
        IEOS::get_singleton()->emit_signal("custom_invites_interface_send_custom_invite_callback", ret);
    });
}

int IEOS::custom_invites_interface_set_custom_invite(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_customInvitesInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString payload = VARIANT_TO_CHARSTRING(p_options->get("payload"));

    EOS_CustomInvites_SetCustomInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Payload = payload.get_data();

    return static_cast<int>(EOS_CustomInvites_SetCustomInvite(s_customInvitesInterface, &options));
}

void IEOS::custom_invites_interface_send_request_to_join(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_customInvitesInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_CustomInvites_SendRequestToJoinOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CUSTOMINVITES_SENDREQUESTTOJOIN_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    p_options->reference();

    EOS_CustomInvites_SendRequestToJoin(s_customInvitesInterface, &options, (void *)*p_options, [](const EOS_CustomInvites_SendRequestToJoinCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");

        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);

        IEOS::get_singleton()->emit_signal("custom_invites_interface_send_request_to_join_callback", ret);
    });
}

// TODO: impl EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested
// TODO: impl EOS_CustomInvites_RemoveNotifySendCustomNativeInviteRequested

void IEOS::custom_invites_interface_accept_request_to_join(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_customInvitesInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_CustomInvites_AcceptRequestToJoinOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CUSTOMINVITES_ACCEPTREQUESTTOJOIN_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    p_options->reference();

    EOS_CustomInvites_AcceptRequestToJoin(s_customInvitesInterface, &options, (void *)*p_options, [](const EOS_CustomInvites_AcceptRequestToJoinCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");

        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);

        IEOS::get_singleton()->emit_signal("custom_invites_interface_accept_request_to_join_callback", ret);
    });
}

void IEOS::custom_invites_interface_reject_request_to_join(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_customInvitesInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_CustomInvites_RejectRequestToJoinOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_CUSTOMINVITES_REJECTREQUESTTOJOIN_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    p_options->reference();

    EOS_CustomInvites_RejectRequestToJoin(s_customInvitesInterface, &options, (void *)*p_options, [](const EOS_CustomInvites_RejectRequestToJoinCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");

        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);

        IEOS::get_singleton()->emit_signal("custom_invites_interface_reject_request_to_join_callback", ret);
    });
}
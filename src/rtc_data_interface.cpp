#include "ieos.h"

int IEOS::rtc_data_interface_add_notify_data_received(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcDataInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTCData_AddNotifyDataReceivedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCDATA_ADDNOTIFYDATARECEIVED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    return static_cast<int>(EOS_RTCData_AddNotifyDataReceived(s_rtcDataInterface, &options, (void *)*p_options, [](const EOS_RTCData_DataReceivedCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["data_length_bytes"] = data->DataLengthBytes;
        PackedByteArray data_arr;
        data_arr.resize(data->DataLengthBytes);
        memcpy(data_arr.ptrw(), data->Data, data->DataLengthBytes);
        ret["data"] = data_arr;
        IEOS::get_singleton()->emit_signal("rtc_data_data_received", ret);
    }));
}

int IEOS::rtc_data_interface_add_notify_participant_updated(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcDataInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTCData_AddNotifyParticipantUpdatedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCDATA_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    return static_cast<int>(EOS_RTCData_AddNotifyParticipantUpdated(s_rtcDataInterface, &options, (void *)*p_options, [](const EOS_RTCData_ParticipantUpdatedCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["data_status"] = static_cast<int>(data->DataStatus);
        IEOS::get_singleton()->emit_signal("rtc_data_participant_updated", ret);
    }));
}

int IEOS::rtc_data_interface_send_data(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcDataInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    PackedByteArray p_data = p_options->get("data");

    EOS_RTCData_SendDataOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCDATA_SENDDATA_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.Data = p_data.ptr();
    options.DataLengthBytes = p_data.size();

    return static_cast<int>(EOS_RTCData_SendData(s_rtcDataInterface, &options));
}

void IEOS::rtc_data_interface_remove_notify_data_received(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcDataInterface);
    EOS_RTCData_RemoveNotifyDataReceived(s_rtcDataInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_data_interface_remove_notify_participant_updated(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcDataInterface);
    EOS_RTCData_RemoveNotifyParticipantUpdated(s_rtcDataInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_data_interface_update_receiving(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcDataInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    CharString participant_id = VARIANT_TO_CHARSTRING(p_options->get("participant_id"));
    bool data_enabled = p_options->get("data_enabled");

    EOS_RTCData_UpdateReceivingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCDATA_UPDATERECEIVING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.ParticipantId = eosg_string_to_product_user_id(participant_id.get_data());
    options.bDataEnabled = data_enabled ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_RTCData_UpdateReceiving(s_rtcDataInterface, &options, (void *)*p_options, [](const EOS_RTCData_UpdateReceivingCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["data_enabled"] = EOSG_GET_BOOL(data->bDataEnabled);
        IEOS::get_singleton()->emit_signal("rtc_data_interface_update_receiving_callback", ret);
    });
}

void IEOS::rtc_data_interface_update_sending(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcDataInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    bool data_enabled = p_options->get("data_enabled");

    EOS_RTCData_UpdateSendingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCDATA_UPDATESENDING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.bDataEnabled = data_enabled ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_RTCData_UpdateSending(s_rtcDataInterface, &options, (void *)*p_options, [](const EOS_RTCData_UpdateSendingCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["data_enabled"] = EOSG_GET_BOOL(data->bDataEnabled);
        IEOS::get_singleton()->emit_signal("rtc_data_interface_update_sending_callback", ret);
    });
}
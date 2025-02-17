#include "ieos.h"

void IEOS::rtc_interface_join_room(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_rtcInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    CharString client_base_url = VARIANT_TO_CHARSTRING(p_options->get("client_base_url"));
    CharString participant_token = VARIANT_TO_CHARSTRING(p_options->get("participant_token"));
    CharString participant_id = VARIANT_TO_CHARSTRING(p_options->get("participant_id"));
    int flags = p_options->get("flags");
    bool manual_audio_input_enabled = p_options->get("manual_audio_input_enabled");
    bool manual_audio_output_enabled = p_options->get("manual_audio_output_enabled");

    EOS_RTC_JoinRoomOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_JOINROOM_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.ClientBaseUrl = client_base_url.get_data();
    options.ParticipantToken = participant_token.get_data();
    options.ParticipantId = eosg_string_to_product_user_id(participant_id.get_data());
    options.Flags = static_cast<uint32_t>(flags);
    options.bManualAudioInputEnabled = manual_audio_input_enabled ? EOS_TRUE : EOS_FALSE;
    options.bManualAudioOutputEnabled = manual_audio_output_enabled ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_RTC_JoinRoom(s_rtcInterface, &options, (void *)*p_options, [](const EOS_RTC_JoinRoomCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        Array room_options = Array();
        if (data->RoomOptionsCount > 0) {
            for (int i = 0; i < data->RoomOptionsCount; i++) {
                Dictionary room_option;
                room_option["key"] = EOSG_GET_STRING(data->RoomOptions[i].Key);
                room_option["value"] = EOSG_GET_STRING(data->RoomOptions[i].Value);
                room_options.append(room_option);
            }
        }
        ret["room_options"] = room_options;
        IEOS::get_singleton()->emit_signal("rtc_interface_join_room_callback", ret);
    });
}

void IEOS::rtc_interface_leave_room(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_rtcInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTC_LeaveRoomOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_LEAVEROOM_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    p_options->reference();

    EOS_RTC_LeaveRoom(s_rtcInterface, &options, (void *)*p_options, [](const EOS_RTC_LeaveRoomCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        IEOS::get_singleton()->emit_signal("rtc_interface_leave_room_callback", ret);
    });
}

void IEOS::rtc_interface_block_participant(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_rtcInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    CharString participant_id = VARIANT_TO_CHARSTRING(p_options->get("participant_id"));
    bool blocked = p_options->get("blocked");

    EOS_RTC_BlockParticipantOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_BLOCKPARTICIPANT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.ParticipantId = eosg_string_to_product_user_id(participant_id.get_data());
    options.bBlocked = blocked ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_RTC_BlockParticipant(s_rtcInterface, &options, (void *)*p_options, [](const EOS_RTC_BlockParticipantCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["blocked"] = EOSG_GET_BOOL(data->bBlocked);
        IEOS::get_singleton()->emit_signal("rtc_interface_block_participant_callback", ret);
    });
}

int IEOS::rtc_interface_set_setting(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_rtcInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString setting_name = VARIANT_TO_CHARSTRING(p_options->get("setting_name"));
    CharString setting_value = VARIANT_TO_CHARSTRING(p_options->get("setting_value"));

    EOS_RTC_SetSettingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_SETSETTING_API_LATEST;
    options.SettingName = setting_name.get_data();
    options.SettingValue = setting_value.get_data();

    return static_cast<int>(EOS_RTC_SetSetting(s_rtcInterface, &options));
}

int IEOS::rtc_interface_set_room_setting(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_rtcInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString lcoal_user_id = VARIANT_TO_CHARSTRING(p_options->get("lcoal_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    CharString setting_name = VARIANT_TO_CHARSTRING(p_options->get("setting_name"));
    CharString setting_value = VARIANT_TO_CHARSTRING(p_options->get("setting_value"));

    EOS_RTC_SetRoomSettingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_SETROOMSETTING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(lcoal_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.SettingName = setting_name.get_data();
    options.SettingValue = setting_value.get_data();

    return static_cast<int>(EOS_RTC_SetRoomSetting(s_rtcInterface, &options));
}

int IEOS::rtc_interface_add_notify_disconnected(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_rtcInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTC_AddNotifyDisconnectedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_ADDNOTIFYDISCONNECTED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    return static_cast<int>(EOS_RTC_AddNotifyDisconnected(s_rtcInterface, &options, (void *)*p_options, [](const EOS_RTC_DisconnectedCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        IEOS::get_singleton()->emit_signal("rtc_interface_disconnected", ret);
    }));
}

int IEOS::rtc_interface_add_notify_participant_status_changed(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_rtcInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTC_AddNotifyParticipantStatusChangedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_ADDNOTIFYPARTICIPANTSTATUSCHANGED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    return static_cast<int>(EOS_RTC_AddNotifyParticipantStatusChanged(s_rtcInterface, &options, (void *)*p_options, [](const EOS_RTC_ParticipantStatusChangedCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["participant_status"] = static_cast<int>(data->ParticipantStatus);
        ret["participant_in_blocklist"] = EOSG_GET_BOOL(data->bParticipantInBlocklist);

        Array participant_metadata = Array();
        if (data->ParticipantMetadataCount > 0) {
            for (int i = 0; i < data->ParticipantMetadataCount; i++) {
                Dictionary metadata;
                metadata["key"] = EOSG_GET_STRING(data->ParticipantMetadata[i].Key);
                metadata["value"] = EOSG_GET_STRING(data->ParticipantMetadata[i].Value);
                participant_metadata.append(metadata);
            }
        }
        ret["participant_metadata"] = participant_metadata;
        IEOS::get_singleton()->emit_signal("rtc_interface_participant_status_changed", ret);
    }));
}

int IEOS::rtc_interface_add_notify_room_statistics_updated(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL_V(s_rtcInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTC_ADDNOTIFYROOMSTATISTICSUPDATED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
	
    return static_cast<int>(EOS_RTC_AddNotifyRoomStatisticsUpdated(s_rtcInterface, &options, (void *)*p_options, [](const EOS_RTC_RoomStatisticsUpdatedInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["statistic"] = EOSG_GET_STRING(data->Statistic);
        IEOS::get_singleton()->emit_signal("rtc_interface_room_statistics_updated", ret);
    }));
}

void IEOS::rtc_interface_remove_notify_disconnected(int p_notification_id) {
	ERR_FAIL_NULL(s_rtcInterface);
    EOS_RTC_RemoveNotifyDisconnected(s_rtcInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_interface_remove_notify_participant_status_changed(int p_notification_id) {
	ERR_FAIL_NULL(s_rtcInterface);
    EOS_RTC_RemoveNotifyParticipantStatusChanged(s_rtcInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_interface_remove_notify_room_statistics_updated(int p_notification_id) {
	ERR_FAIL_NULL(s_rtcInterface);
    EOS_RTC_RemoveNotifyRoomStatisticsUpdated(s_rtcInterface, static_cast<EOS_NotificationId>(p_notification_id));
}
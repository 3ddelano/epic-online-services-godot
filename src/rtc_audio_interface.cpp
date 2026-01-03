#include "godot_cpp/variant/packed_int32_array.hpp"
#include "ieos.h"

Dictionary IEOS::rtc_audio_interface_copy_input_device_information_by_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, {});
    int device_index = p_options->get("device_index");

    EOS_RTCAudio_CopyInputDeviceInformationByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_COPYINPUTDEVICEINFORMATIONBYINDEX_API_LATEST;
    options.DeviceIndex = static_cast<uint32_t>(device_index);

    EOS_RTCAudio_InputDeviceInformation *outInputDeviceInfo;
    EOS_EResult res = EOS_RTCAudio_CopyInputDeviceInformationByIndex(s_rtcAudioInterface, &options, &outInputDeviceInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["input_device_information"] = eosg_rtc_audio_input_device_information_to_dict_and_release(outInputDeviceInfo);
    return ret;
}

Dictionary IEOS::rtc_audio_interface_copy_output_device_information_by_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, {});
    int device_index = p_options->get("device_index");

    EOS_RTCAudio_CopyOutputDeviceInformationByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_COPYOUTPUTDEVICEINFORMATIONBYINDEX_API_LATEST;
    options.DeviceIndex = static_cast<uint32_t>(device_index);

    EOS_RTCAudio_OutputDeviceInformation *outOutputDeviceInfo;
    EOS_EResult res = EOS_RTCAudio_CopyOutputDeviceInformationByIndex(s_rtcAudioInterface, &options, &outOutputDeviceInfo);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["output_device_information"] = eosg_rtc_audio_output_device_information_to_dict_and_release(outOutputDeviceInfo);
    return ret;
}

int IEOS::rtc_audio_interface_get_input_devices_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, 0);
    EOS_RTCAudio_GetInputDevicesCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_GETINPUTDEVICESCOUNT_API_LATEST;

    return static_cast<int>(EOS_RTCAudio_GetInputDevicesCount(s_rtcAudioInterface, &options));
}

int IEOS::rtc_audio_interface_get_output_devices_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, 0);
    EOS_RTCAudio_GetOutputDevicesCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_GETOUTPUTDEVICESCOUNT_API_LATEST;

    return static_cast<int>(EOS_RTCAudio_GetOutputDevicesCount(s_rtcAudioInterface, &options));
}

int IEOS::rtc_audio_interface_send_audio(const Ref<RefCounted> &p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, static_cast<int>(EOS_EResult::EOS_InvalidParameters));

    const CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    const CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    // Extract buffer data from options
    const int sample_rate = p_options->get("sample_rate");
    const int channels = p_options->get("channels");
    PackedInt32Array frames_array = p_options->get("frames");
    const int frames_count = frames_array.size();

    // Allocate and copy frame data
    int16_t *frames_data = nullptr;
    if (frames_count > 0) {
        frames_data = (int16_t *)memalloc(sizeof(int16_t) * frames_count);
        for (int i = 0; i < frames_count; i++) {
            frames_data[i] = static_cast<int16_t>(static_cast<int>(frames_array[i]));
        }
    }

    // Setup audio buffer
    EOS_RTCAudio_AudioBuffer audio_buffer;
    memset(&audio_buffer, 0, sizeof(audio_buffer));
    audio_buffer.ApiVersion = EOS_RTCAUDIO_AUDIOBUFFER_API_LATEST;
    audio_buffer.Frames = frames_data;
    audio_buffer.FramesCount = static_cast<uint32_t>(frames_count);
    audio_buffer.SampleRate = static_cast<uint32_t>(sample_rate);
    audio_buffer.Channels = static_cast<uint32_t>(channels);

    // Setup send audio options
    EOS_RTCAudio_SendAudioOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_SENDAUDIO_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.Buffer = &audio_buffer;

    EOS_EResult result = EOS_RTCAudio_SendAudio(s_rtcAudioInterface, &options);

    // Free allocated memory
    if (frames_data != nullptr) {
        memfree(frames_data);
    }

    return static_cast<int>(result);
}

void IEOS::rtc_audio_interface_query_input_devices_information(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_QueryInputDevicesInformationOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_QUERYINPUTDEVICESINFORMATION_API_LATEST;
    p_options->reference();

    EOS_RTCAudio_QueryInputDevicesInformation(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_OnQueryInputDevicesInformationCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_query_input_devices_information_callback", ret);
    });
}

void IEOS::rtc_audio_interface_query_output_devices_information(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_QueryOutputDevicesInformationOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_QUERYOUTPUTDEVICESINFORMATION_API_LATEST;
    p_options->reference();

    EOS_RTCAudio_QueryOutputDevicesInformation(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_OnQueryOutputDevicesInformationCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_query_output_devices_information_callback", ret);
    });
}

void IEOS::rtc_audio_interface_register_platform_user(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString platform_user_id = VARIANT_TO_CHARSTRING(p_options->get("platform_user_id"));

    EOS_RTCAudio_RegisterPlatformUserOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_REGISTERPLATFORMUSER_API_LATEST;
    options.PlatformUserId = platform_user_id.get_data();
    p_options->reference();

    EOS_RTCAudio_RegisterPlatformUser(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_OnRegisterPlatformUserCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["platform_user_id"] = EOSG_GET_STRING(data->PlatformUserId);
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_register_platform_user_callback", ret);
    });
}

void IEOS::rtc_audio_interface_set_input_device_settings(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString real_device_id = VARIANT_TO_CHARSTRING(p_options->get("real_device_id"));
    bool platform_aec = p_options->get("platform_aec");

    EOS_RTCAudio_SetInputDeviceSettingsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_SETINPUTDEVICESETTINGS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RealDeviceId = real_device_id.get_data();
    options.bPlatformAEC = platform_aec ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_RTCAudio_SetInputDeviceSettings(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_OnSetInputDeviceSettingsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["real_device_id"] = EOSG_GET_STRING(data->RealDeviceId);
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_set_input_device_settings_callback", ret);
    });
}

void IEOS::rtc_audio_interface_set_output_device_settings(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString real_device_id = VARIANT_TO_CHARSTRING(p_options->get("real_device_id"));

    EOS_RTCAudio_SetOutputDeviceSettingsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_SETOUTPUTDEVICESETTINGS_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RealDeviceId = real_device_id.get_data();
    p_options->reference();

    EOS_RTCAudio_SetOutputDeviceSettings(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_OnSetOutputDeviceSettingsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["real_device_id"] = EOSG_GET_STRING(data->RealDeviceId);
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_set_output_device_settings_callback", ret);
    });
}

void IEOS::rtc_audio_interface_unregister_platform_user(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString platform_user_id = VARIANT_TO_CHARSTRING(p_options->get("platform_user_id"));

    EOS_RTCAudio_UnregisterPlatformUserOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_UNREGISTERPLATFORMUSER_API_LATEST;
    options.PlatformUserId = platform_user_id.get_data();
    p_options->reference();

    EOS_RTCAudio_UnregisterPlatformUser(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_OnUnregisterPlatformUserCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["platform_user_id"] = EOSG_GET_STRING(data->PlatformUserId);
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_unregister_platform_user_callback", ret);
    });
}

void IEOS::rtc_audio_interface_update_participant_volume(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    CharString participant_id = VARIANT_TO_CHARSTRING(p_options->get("participant_id"));
    float volume = p_options->get("volume");

    EOS_RTCAudio_UpdateParticipantVolumeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_UPDATEPARTICIPANTVOLUME_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.ParticipantId = eosg_string_to_product_user_id(participant_id.get_data());
    options.Volume = volume;
    p_options->reference();

    EOS_RTCAudio_UpdateParticipantVolume(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["volume"] = data->Volume;
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_update_participant_volume_callback", ret);
    });
}

void IEOS::rtc_audio_interface_update_receiving_volume(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    float volume = p_options->get("volume");

    EOS_RTCAudio_UpdateReceivingVolumeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_UPDATERECEIVINGVOLUME_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.Volume = volume;
    p_options->reference();

    EOS_RTCAudio_UpdateReceivingVolume(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["volume"] = data->Volume;
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_update_receiving_volume_callback", ret);
    });
}

void IEOS::rtc_audio_interface_update_receiving(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    CharString participant_id = VARIANT_TO_CHARSTRING(p_options->get("participant_id"));
    bool audio_enabled = p_options->get("audio_enabled");

    EOS_RTCAudio_UpdateReceivingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_UPDATERECEIVING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.ParticipantId = eosg_string_to_product_user_id(participant_id.get_data());
    options.bAudioEnabled = audio_enabled ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_RTCAudio_UpdateReceiving(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_UpdateReceivingCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["audio_enabled"] = data->bAudioEnabled == EOS_TRUE;
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_update_receiving_callback", ret);
    });
}

void IEOS::rtc_audio_interface_update_sending_volume(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    float volume = p_options->get("volume");

    EOS_RTCAudio_UpdateSendingVolumeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_UPDATESENDINGVOLUME_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.Volume = volume;
    p_options->reference();

    EOS_RTCAudio_UpdateSendingVolume(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_UpdateSendingVolumeCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["volume"] = data->Volume;
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_update_sending_volume_callback", ret);
    });
}

void IEOS::rtc_audio_interface_update_sending(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    int audio_status = p_options->get("audio_status");

    EOS_RTCAudio_UpdateSendingOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_UPDATESENDING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.AudioStatus = static_cast<EOS_ERTCAudioStatus>(audio_status);
    p_options->reference();

    EOS_RTCAudio_UpdateSending(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_UpdateSendingCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        client_data->unreference();
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["audio_status"] = static_cast<int>(data->AudioStatus);
        IEOS::get_singleton()->emit_signal("rtc_audio_interface_update_sending_callback", ret);
    });
}

int IEOS::rtc_audio_interface_add_notify_audio_before_render(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));
    bool unmixed_audio = p_options->get("unmixed_audio");

    EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();
    options.bUnmixedAudio = unmixed_audio ? EOS_TRUE : EOS_FALSE;

    EOS_NotificationId notification_id = EOS_RTCAudio_AddNotifyAudioBeforeRender(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_AudioBeforeRenderCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["buffer"] = eosg_rtc_audio_audio_buffer_to_dict(data->Buffer);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        IEOS::get_singleton()->emit_signal("rtc_audio_audio_before_render", ret);
    });

    return static_cast<int>(notification_id);
}

int IEOS::rtc_audio_interface_add_notify_audio_before_send(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTCAudio_AddNotifyAudioBeforeSendOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    EOS_NotificationId notification_id = EOS_RTCAudio_AddNotifyAudioBeforeSend(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_AudioBeforeSendCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["buffer"] = eosg_rtc_audio_audio_buffer_to_dict(data->Buffer);
        IEOS::get_singleton()->emit_signal("rtc_audio_audio_before_send", ret);
    });
    return static_cast<int>(notification_id);
}

int IEOS::rtc_audio_interface_add_notify_audio_input_state(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTCAudio_AddNotifyAudioInputStateOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    EOS_NotificationId notification_id = EOS_RTCAudio_AddNotifyAudioInputState(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_AudioInputStateCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["status"] = static_cast<int>(data->Status);
        IEOS::get_singleton()->emit_signal("rtc_audio_audio_input_state", ret);
    });
    return static_cast<int>(notification_id);
}

int IEOS::rtc_audio_interface_add_notify_audio_output_state(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTCAudio_AddNotifyAudioOutputStateOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    EOS_NotificationId notification_id = EOS_RTCAudio_AddNotifyAudioOutputState(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_AudioOutputStateCallbackInfo *data) {
        Dictionary ret;
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["status"] = static_cast<int>(data->Status);
        IEOS::get_singleton()->emit_signal("rtc_audio_audio_output_state", ret);
    });
    return static_cast<int>(notification_id);
}

int IEOS::rtc_audio_interface_add_notify_participant_updated(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_rtcAudioInterface, static_cast<int>(EOS_INVALID_NOTIFICATIONID));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString room_name = VARIANT_TO_CHARSTRING(p_options->get("room_name"));

    EOS_RTCAudio_AddNotifyParticipantUpdatedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.RoomName = room_name.get_data();

    EOS_NotificationId notification_id = EOS_RTCAudio_AddNotifyParticipantUpdated(s_rtcAudioInterface, &options, (void *)*p_options, [](const EOS_RTCAudio_ParticipantUpdatedCallbackInfo *data) {
        Dictionary ret;
        ret["room_name"] = EOSG_GET_STRING(data->RoomName);
        ret["participant_id"] = eosg_product_user_id_to_string(data->ParticipantId);
        ret["speaking"] = EOSG_GET_BOOL(data->bSpeaking);
        ret["audio_status"] = static_cast<int>(data->AudioStatus);
        IEOS::get_singleton()->emit_signal("rtc_audio_participant_updated", ret);
    });
    return static_cast<int>(notification_id);
}

void IEOS::rtc_audio_interface_remove_notify_audio_before_render(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_RemoveNotifyAudioBeforeRender(s_rtcAudioInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_audio_interface_remove_notify_audio_before_send(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_RemoveNotifyAudioBeforeSend(s_rtcAudioInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_audio_interface_remove_notify_audio_input_state(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_RemoveNotifyAudioInputState(s_rtcAudioInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_audio_interface_remove_notify_audio_output_state(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_RemoveNotifyAudioOutputState(s_rtcAudioInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

void IEOS::rtc_audio_interface_remove_notify_participant_updated(int p_notification_id) {
    ERR_FAIL_NULL(s_rtcAudioInterface);
    EOS_RTCAudio_RemoveNotifyParticipantUpdated(s_rtcAudioInterface, static_cast<EOS_NotificationId>(p_notification_id));
}

#include "ieos.h"
using namespace godot;

int IEOS::ui_interface_acknowledge_event_id(Ref<RefCounted> p_options) {
    EOS_UI_AcknowledgeEventIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_ACKNOWLEDGEEVENTID_API_LATEST;
    options.UiEventId = static_cast<EOS_UI_EventId>(p_options->get("ui_event_id"));
    options.Result = static_cast<EOS_EResult>(static_cast<int>(p_options->get("result")));

    return static_cast<int>(EOS_UI_AcknowledgeEventId(s_uiInterface, &options));
}

bool IEOS::ui_interface_get_friends_visible(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_UI_GetFriendsVisibleOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_GETFRIENDSVISIBLE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return EOSG_GET_BOOL(EOS_UI_GetFriendsVisible(s_uiInterface, &options));
}

int IEOS::ui_interface_get_notification_location_preference() {
    return static_cast<int>(EOS_UI_GetNotificationLocationPreference(s_uiInterface));
}

int IEOS::ui_interface_get_toggle_friends_key(Ref<RefCounted> p_options) {
    EOS_UI_GetToggleFriendsKeyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_GETTOGGLEFRIENDSKEY_API_LATEST;

    return static_cast<int>(EOS_UI_GetToggleFriendsKey(s_uiInterface, &options));
}

void IEOS::ui_interface_hide_friends(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_UI_HideFriendsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_HIDEFRIENDS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    p_options->reference();

    EOS_UI_HideFriends(s_uiInterface, &options, (void *)*p_options, [](const EOS_UI_HideFriendsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("ui_interface_hide_friends_callback", ret);
    });
}

bool IEOS::ui_interface_is_valid_key_combination(int key_combination) {
    return EOSG_GET_BOOL(EOS_UI_IsValidKeyCombination(s_uiInterface, static_cast<EOS_UI_EKeyCombination>(key_combination)));
}

int IEOS::ui_interface_set_display_preference(Ref<RefCounted> p_options) {
    EOS_UI_SetDisplayPreferenceOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SETDISPLAYPREFERENCE_API_LATEST;
    options.NotificationLocation = static_cast<EOS_UI_ENotificationLocation>(static_cast<int>(p_options->get("notification_location")));

    return static_cast<int>(EOS_UI_SetDisplayPreference(s_uiInterface, &options));
}

int IEOS::ui_interface_set_toggle_friends_key(Ref<RefCounted> p_options) {
    EOS_UI_SetToggleFriendsKeyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SETTOGGLEFRIENDSKEY_API_LATEST;
    options.KeyCombination = static_cast<EOS_UI_EKeyCombination>(static_cast<int>(p_options->get("key_combination")));

    return static_cast<int>(EOS_UI_SetToggleFriendsKey(s_uiInterface, &options));
}

void IEOS::ui_interface_show_friends(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_UI_ShowFriendsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SHOWFRIENDS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    p_options->reference();

    EOS_UI_ShowFriends(s_uiInterface, &options, (void *)*p_options, [](const EOS_UI_ShowFriendsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("ui_interface_show_friends_callback", ret);
    });
}

bool IEOS::ui_interface_get_friends_exclusive_input(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_UI_GetFriendsExclusiveInputOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_GETFRIENDSEXCLUSIVEINPUT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return EOSG_GET_BOOL(EOS_UI_GetFriendsExclusiveInput(s_uiInterface, &options));
}

void IEOS::ui_interface_show_block_player(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UI_ShowBlockPlayerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SHOWBLOCKPLAYER_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_UI_ShowBlockPlayer(s_uiInterface, &options, (void *)*p_options, [](const EOS_UI_OnShowBlockPlayerCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("ui_interface_show_block_player_callback", ret);
    });
}

void IEOS::ui_interface_show_report_player(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UI_ShowReportPlayerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SHOWREPORTPLAYER_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_UI_ShowReportPlayer(s_uiInterface, &options, (void *)*p_options, [](const EOS_UI_OnShowReportPlayerCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("ui_interface_show_report_player_callback", ret);
    });
}

int IEOS::ui_interface_pause_social_overlay(Ref<RefCounted> p_options) {
    EOS_UI_PauseSocialOverlayOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_PAUSESOCIALOVERLAY_API_LATEST;
    options.bIsPaused = VARIANT_TO_EOS_BOOL(p_options->get("is_paused"));

    return static_cast<int>(EOS_UI_PauseSocialOverlay(s_uiInterface, &options));
}

bool IEOS::ui_interface_is_social_overlay_paused(Ref<RefCounted> p_options) {
    EOS_UI_IsSocialOverlayPausedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_ISSOCIALOVERLAYPAUSED_API_LATEST;

    return EOSG_GET_BOOL(EOS_UI_IsSocialOverlayPaused(s_uiInterface, &options));
}

int IEOS::ui_interface_set_toggle_friends_button(Ref<RefCounted> p_options) {
    int button_combination = p_options->get("button_combination");

    EOS_UI_SetToggleFriendsButtonOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SETTOGGLEFRIENDSBUTTON_API_LATEST;
    options.ButtonCombination = static_cast<EOS_UI_EInputStateButtonFlags>(button_combination);

    return static_cast<int>(EOS_UI_SetToggleFriendsButton(s_uiInterface, &options));
}

int IEOS::ui_interface_get_toggle_friends_button(Ref<RefCounted> p_options) {
    EOS_UI_GetToggleFriendsButtonOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_GETTOGGLEFRIENDSBUTTON_API_LATEST;

    return static_cast<int>(EOS_UI_GetToggleFriendsButton(s_uiInterface, &options));
}

bool IEOS::ui_interface_is_valid_button_combination(int p_button_combination) {
    return EOSG_GET_BOOL(EOS_UI_IsValidButtonCombination(s_uiInterface, static_cast<EOS_UI_EInputStateButtonFlags>(p_button_combination)));
}

int IEOS::ui_interface_report_input_state(Ref<RefCounted> p_options) {
    int button_down_flags = p_options->get("button_down_flags");
    int mouse_pos_x = p_options->get("mouse_pos_x");
    int mouse_pos_y = p_options->get("mouse_pos_y");
    int gamepad_index = p_options->get("gamepad_index");
    float left_stick_x = p_options->get("left_stick_x");
    float left_stick_y = p_options->get("left_stick_y");
    float right_stick_x = p_options->get("right_stick_x");
    float right_stick_y = p_options->get("right_stick_y");
    float left_trigger = p_options->get("left_trigger");
    float right_trigger = p_options->get("right_trigger");

    EOS_UI_ReportInputStateOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_REPORTINPUTSTATE_API_LATEST;
    options.ButtonDownFlags = static_cast<EOS_UI_EInputStateButtonFlags>(button_down_flags);
    options.bAcceptIsFaceButtonRight = VARIANT_TO_EOS_BOOL(p_options->get("accept_is_face_button_right"));
    options.bMouseButtonDown = VARIANT_TO_EOS_BOOL(p_options->get("mouse_button_down"));
    options.MousePosX = static_cast<uint32_t>(mouse_pos_x);
    options.MousePosY = static_cast<uint32_t>(mouse_pos_y);
    options.GamepadIndex = static_cast<uint32_t>(gamepad_index);
    options.LeftStickX = left_stick_x;
    options.LeftStickY = left_stick_y;
    options.RightStickX = right_stick_x;
    options.RightStickY = right_stick_y;
    options.LeftTrigger = left_trigger;
    options.RightTrigger = right_trigger;

    return static_cast<int>(EOS_UI_ReportInputState(s_uiInterface, &options));
}

int IEOS::ui_interface_pre_present(Ref<RefCounted> p_options) {
    EOS_UI_PrePresentOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_PREPRESENT_API_LATEST;

    return static_cast<int>(EOS_UI_PrePresent(s_uiInterface, &options));
}

void IEOS::ui_interface_show_native_profile(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_UI_ShowNativeProfileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_UI_SHOWNATIVEPROFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_UI_ShowNativeProfile(s_uiInterface, &options, (void *)*p_options, [](const EOS_UI_ShowNativeProfileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("ui_interface_show_native_profile_callback", ret);
    });
}
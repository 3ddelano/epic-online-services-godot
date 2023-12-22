#include "ieos.h"

Dictionary IEOS::sessions_interface_copy_active_session_details(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_CopyActiveSessionHandleOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_COPYACTIVESESSIONHANDLE_API_LATEST;
    options.SessionName = session_name.get_data();

    EOS_HActiveSession outActiveSession = nullptr;
    EOS_EResult res = EOS_Sessions_CopyActiveSessionHandle(s_sessionsInterface, &options, &outActiveSession);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["active_session"] = eosg_sessions_active_session_to_wrapper(outActiveSession);
    return ret;
}

Dictionary IEOS::sessions_interface_copy_session_details_by_invite_id(Ref<RefCounted> p_options) {
    CharString invite_id = VARIANT_TO_CHARSTRING(p_options->get("invite_id"));

    EOS_Sessions_CopySessionHandleByInviteIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEBYINVITEID_API_LATEST;
    options.InviteId = invite_id.get_data();

    EOS_HSessionDetails outSessionHandle = nullptr;
    EOS_EResult res = EOS_Sessions_CopySessionHandleByInviteId(s_sessionsInterface, &options, &outSessionHandle);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_details"] = eosg_sessions_session_details_to_wrapper(outSessionHandle);
    return ret;
}

Dictionary IEOS::sessions_interface_copy_session_details_by_ui_event_id(Ref<RefCounted> p_options) {
    int ui_event_id = p_options->get("ui_event_id");

    EOS_Sessions_CopySessionHandleByUiEventIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEBYUIEVENTID_API_LATEST;
    options.UiEventId = static_cast<EOS_UI_EventId>(ui_event_id);

    EOS_HSessionDetails outSessionHandle = nullptr;
    EOS_EResult res = EOS_Sessions_CopySessionHandleByUiEventId(s_sessionsInterface, &options, &outSessionHandle);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_details"] = eosg_sessions_session_details_to_wrapper(outSessionHandle);
    return ret;
}

Dictionary IEOS::sessions_interface_copy_session_details_for_presence(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Sessions_CopySessionHandleForPresenceOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEFORPRESENCE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    EOS_HSessionDetails outSessionHandle = nullptr;
    EOS_EResult res = EOS_Sessions_CopySessionHandleForPresence(s_sessionsInterface, &options, &outSessionHandle);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_details"] = eosg_sessions_session_details_to_wrapper(outSessionHandle);
    return ret;
}

Dictionary IEOS::sessions_interface_create_session_modification(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));
    CharString bucket_id = VARIANT_TO_CHARSTRING(p_options->get("bucket_id"));
    int max_players = p_options->get("max_players");
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    bool presence_enabled = p_options->get("presence_enabled");
    CharString session_id = VARIANT_TO_CHARSTRING(p_options->get("session_id"));
    bool sanctions_enabled = p_options->get("sanctions_enabled");
    Array allowed_platform_ids = p_options->get("allowed_platform_ids");

    EOS_Sessions_CreateSessionModificationOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST;
    options.SessionName = session_name.get_data();
    options.BucketId = bucket_id.get_data();
    options.MaxPlayers = static_cast<uint32_t>(max_players);
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.bPresenceEnabled = presence_enabled ? EOS_TRUE : EOS_FALSE;
    options.SessionId = session_id.get_data();
    options.bSanctionsEnabled = sanctions_enabled ? EOS_TRUE : EOS_FALSE;
    options.AllowedPlatformIdsCount = allowed_platform_ids.size();
    if (allowed_platform_ids.size() > 0) {
        uint32_t *allowed_platform_ids_array = (uint32_t *)malloc(sizeof(uint32_t) * allowed_platform_ids.size());
        for (int i = 0; i < allowed_platform_ids.size(); i++) {
            allowed_platform_ids_array[i] = allowed_platform_ids[i];
        }
        options.AllowedPlatformIds = allowed_platform_ids_array;
    }

    EOS_HSessionModification outSessionModificationHandle = nullptr;
    EOS_EResult res = EOS_Sessions_CreateSessionModification(s_sessionsInterface, &options, &outSessionModificationHandle);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_modification"] = eosg_sessions_session_modification_to_wrapper(outSessionModificationHandle);
    return ret;
}

Dictionary IEOS::sessions_interface_create_session_search(Ref<RefCounted> p_options) {
    int max_search_results = p_options->get("max_search_results");

    EOS_Sessions_CreateSessionSearchOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_CREATESESSIONSEARCH_API_LATEST;
    options.MaxSearchResults = static_cast<uint32_t>(max_search_results);

    EOS_HSessionSearch outSessionSearchHandle = nullptr;
    EOS_EResult res = EOS_Sessions_CreateSessionSearch(s_sessionsInterface, &options, &outSessionSearchHandle);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_search"] = eosg_sessions_session_search_to_wrapper(outSessionSearchHandle);
    return ret;
}

int IEOS::sessions_interface_dump_session_state(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_DumpSessionStateOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_DUMPSESSIONSTATE_API_LATEST;
    options.SessionName = session_name.get_data();

    return static_cast<int>(EOS_Sessions_DumpSessionState(s_sessionsInterface, &options));
}

Dictionary IEOS::sessions_interface_get_invite_id_by_index(Ref<RefCounted> p_options) {
    int index = p_options->get("index");

    EOS_Sessions_GetInviteIdByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_GETINVITEIDBYINDEX_API_LATEST;
    options.Index = static_cast<uint32_t>(index);

    char *outBuffer = (char *)memalloc(sizeof(char) * (EOS_SESSIONS_INVITEID_MAX_LENGTH + 1));
    int32_t outBufferLength = 0;
    EOS_EResult res = EOS_Sessions_GetInviteIdByIndex(s_sessionsInterface, &options, outBuffer, &outBufferLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["invite_id"] = String(outBuffer);
    return ret;
}

int IEOS::sessions_interface_is_user_in_session(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Sessions_IsUserInSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_ISUSERINSESSION_API_LATEST;
    options.SessionName = session_name.get_data();
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());

    EOS_EResult res = EOS_Sessions_IsUserInSession(s_sessionsInterface, &options);
    return static_cast<int>(res);
}

Dictionary IEOS::sessions_interface_update_session_modification(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_UpdateSessionModificationOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_UPDATESESSIONMODIFICATION_API_LATEST;
    options.SessionName = session_name.get_data();

    EOS_HSessionModification outSessionModificationHandle = nullptr;
    EOS_EResult res = EOS_Sessions_UpdateSessionModification(s_sessionsInterface, &options, &outSessionModificationHandle);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["session_modification"] = eosg_sessions_session_modification_to_wrapper(outSessionModificationHandle);
    return ret;
}

int IEOS::sessions_interface_get_invite_count(Ref<RefCounted> p_options) {
    EOS_Sessions_GetInviteCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_GETINVITECOUNT_API_LATEST;

    return static_cast<int>(EOS_Sessions_GetInviteCount(s_sessionsInterface, &options));
}

void IEOS::sessions_interface_destroy_session(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_DestroySessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_DESTROYSESSION_API_LATEST;
    options.SessionName = session_name.get_data();
    p_options->reference();

    EOS_Sessions_DestroySession(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_DestroySessionCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("sessions_interface_destroy_session_callback", ret);
    });
}

void IEOS::sessions_interface_end_session(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_EndSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_ENDSESSION_API_LATEST;
    options.SessionName = session_name.get_data();
    p_options->reference();

    EOS_Sessions_EndSession(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_EndSessionCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("sessions_interface_end_session_callback", ret);
    });
}

void IEOS::sessions_interface_join_session(Ref<RefCounted> p_options) {
    Ref<EOSGSessionDetails> session_details = Object::cast_to<EOSGSessionDetails>(p_options->get("session_details"));
    ERR_FAIL_NULL_MSG(session_details, "Error joining session. JoinSessionOptions.session_details is null.");
    ERR_FAIL_NULL_MSG(session_details->get_internal(), "Error joining session. EOSGSessionDetails is null.");

    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    bool presence_enabled = p_options->get("presence_enabled");

    EOS_Sessions_JoinSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_JOINSESSION_API_LATEST;
    options.SessionName = session_name.get_data();
    options.SessionHandle = session_details->get_internal();
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.bPresenceEnabled = presence_enabled ? EOS_TRUE : EOS_FALSE;
    p_options->reference();

    EOS_Sessions_JoinSession(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_JoinSessionCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("sessions_interface_join_session_callback", ret);
    });
}

void IEOS::sessions_interface_query_invites(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Sessions_QueryInvitesOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_QUERYINVITES_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_Sessions_QueryInvites(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_QueryInvitesCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("sessions_interface_query_invites_callback", ret);
    });
}

void IEOS::sessions_interface_register_players(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));
    TypedArray<String> players_to_register = p_options->get("players_to_register");

    EOS_Sessions_RegisterPlayersOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_REGISTERPLAYERS_API_LATEST;
    options.SessionName = session_name.get_data();
    options.PlayersToRegisterCount = players_to_register.size();
    EOS_ProductUserId *players_to_register_array = (EOS_ProductUserId *)malloc(sizeof(EOS_ProductUserId) * players_to_register.size());
    for (int i = 0; i < players_to_register.size(); i++) {
        players_to_register_array[i] = eosg_string_to_product_user_id(VARIANT_TO_CHARSTRING(players_to_register[i]).get_data());
    }
    options.PlayersToRegister = players_to_register_array;
    p_options->reference();

    EOS_Sessions_RegisterPlayers(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_RegisterPlayersCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");

        Array registered_players_array = Array();
        for (int i = 0; i < data->RegisteredPlayersCount; i++) {
            registered_players_array.append(eosg_product_user_id_to_string(data->RegisteredPlayers[i]));
        }
        ret["registered_players"] = registered_players_array;

        Array sanctioned_players_array = Array();
        for (int i = 0; i < data->SanctionedPlayersCount; i++) {
            sanctioned_players_array.append(eosg_product_user_id_to_string(data->SanctionedPlayers[i]));
        }
        ret["sanctioned_players"] = sanctioned_players_array;

        IEOS::get_singleton()->emit_signal("sessions_interface_register_players_callback", ret);
    });
}

void IEOS::sessions_interface_reject_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString invite_id = VARIANT_TO_CHARSTRING(p_options->get("invite_id"));

    EOS_Sessions_RejectInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_REJECTINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.InviteId = invite_id.get_data();
    p_options->reference();

    EOS_Sessions_RejectInvite(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_RejectInviteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("sessions_interface_reject_invite_callback", ret);
    });
}

void IEOS::sessions_interface_send_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_SendInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_SENDINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.SessionName = session_name.get_data();
    p_options->reference();

    EOS_Sessions_SendInvite(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_SendInviteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("sessions_interface_send_invite_callback", ret);
    });
}

void IEOS::sessions_interface_start_session(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));

    EOS_Sessions_StartSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_STARTSESSION_API_LATEST;
    options.SessionName = session_name.get_data();
    p_options->reference();

    EOS_Sessions_StartSession(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_StartSessionCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("sessions_interface_start_session_callback", ret);
    });
}

void IEOS::sessions_interface_unregister_players(Ref<RefCounted> p_options) {
    CharString session_name = VARIANT_TO_CHARSTRING(p_options->get("session_name"));
    TypedArray<String> players_to_unregister = p_options->get("players_to_unregister");

    EOS_Sessions_UnregisterPlayersOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_UNREGISTERPLAYERS_API_LATEST;
    options.SessionName = session_name.get_data();
    options.PlayersToUnregisterCount = players_to_unregister.size();
    EOS_ProductUserId *players_to_unregister_array = (EOS_ProductUserId *)malloc(sizeof(EOS_ProductUserId) * players_to_unregister.size());
    for (int i = 0; i < players_to_unregister.size(); i++) {
        players_to_unregister_array[i] = eosg_string_to_product_user_id(VARIANT_TO_CHARSTRING(players_to_unregister[i]).get_data());
    }
    options.PlayersToUnregister = players_to_unregister_array;
    p_options->reference();

    EOS_Sessions_UnregisterPlayers(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_UnregisterPlayersCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        Array unregistered_players_array = Array();
        for (int i = 0; i < data->UnregisteredPlayersCount; i++) {
            unregistered_players_array.append(eosg_product_user_id_to_string(data->UnregisteredPlayers[i]));
        }
        ret["unregistered_players"] = unregistered_players_array;

        IEOS::get_singleton()->emit_signal("sessions_interface_unregister_players_callback", ret);
    });
}

void IEOS::sessions_interface_update_session(Ref<RefCounted> p_options) {
    Ref<EOSGSessionModification> session_modification = Object::cast_to<EOSGSessionModification>(p_options->get("session_modification"));
    ERR_FAIL_NULL_MSG(session_modification, "Error updating session. UpdateSessionOptions.session_modification is null.");
    ERR_FAIL_NULL_MSG(session_modification->get_internal(), "Error updating session. EOSGSessionModification is null.");

    EOS_Sessions_UpdateSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
    options.SessionModificationHandle = session_modification->get_internal();
    p_options->reference();

    EOS_Sessions_UpdateSession(s_sessionsInterface, &options, (void *)*p_options, [](const EOS_Sessions_UpdateSessionCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["session_name"] = EOSG_GET_STRING(data->SessionName);
        ret["session_id"] = EOSG_GET_STRING(data->SessionId);
        IEOS::get_singleton()->emit_signal("sessions_interface_update_session_callback", ret);
    });
}

#include "eosg_lobby_details.h"
#include "ieos.h"
using namespace godot;

void IEOS::lobby_interface_create_lobby(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString bucket_id = VARIANT_TO_CHARSTRING(p_options->get("bucket_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_CreateLobbyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_CREATELOBBY_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.MaxLobbyMembers = static_cast<uint32_t>(static_cast<int>(p_options->get("max_lobby_members")));
    options.PermissionLevel = static_cast<EOS_ELobbyPermissionLevel>(static_cast<int>(p_options->get("permission_level")));
    options.bPresenceEnabled = VARIANT_TO_EOS_BOOL(p_options->get("presence_enabled"));
    options.bAllowInvites = VARIANT_TO_EOS_BOOL(p_options->get("allow_invites"));
    options.BucketId = bucket_id.get_data();
    options.bDisableHostMigration = VARIANT_TO_EOS_BOOL(p_options->get("disable_host_migration"));
    options.bEnableRTCRoom = VARIANT_TO_EOS_BOOL(p_options->get("enable_rtc_room"));

    Variant local_rtc_options = p_options->get("local_rtc_options");
    if (options.bEnableRTCRoom && local_rtc_options.get_type() != Variant::NIL) {
        EOS_Lobby_LocalRTCOptions localRTCOptions = eosg_variant_to_lobby_local_rtc_options(local_rtc_options);
        options.LocalRTCOptions = &localRTCOptions;
    }
    if (lobby_id.length() != 0) {
        options.LobbyId = lobby_id.get_data();
    }
    options.bEnableJoinById = VARIANT_TO_EOS_BOOL(p_options->get("enable_join_by_id"));
    options.bRejoinAfterKickRequiresInvite = VARIANT_TO_EOS_BOOL(p_options->get("rejoin_after_kick_requires_invite"));
    p_options->reference();

    EOS_Lobby_CreateLobby(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_CreateLobbyCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_create_lobby_callback", ret);
    });
}

void IEOS::lobby_interface_destroy_lobby(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_DestroyLobbyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_DESTROYLOBBY_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    p_options->reference();

    EOS_Lobby_DestroyLobby(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_DestroyLobbyCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_destroy_lobby_callback", ret);
    });
}

void IEOS::lobby_interface_join_lobby(Ref<RefCounted> p_options) {
    Ref<EOSGLobbyDetails> p_lobby_details = Object::cast_to<EOSGLobbyDetails>(p_options->get("lobby_details"));
    ERR_FAIL_NULL_MSG(p_lobby_details, "Error joining lobby. JoinLobbyOptions.lobby_details is null.");
    ERR_FAIL_NULL_MSG(p_lobby_details->get_internal(), "Error joining lobby. EOSGLobbyDetails is null.");

    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Lobby_JoinLobbyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_JOINLOBBY_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.bPresenceEnabled = VARIANT_TO_EOS_BOOL(p_options->get("presence_enabled"));
    Variant local_rtc_options = p_options->get("local_rtc_options");
    if (local_rtc_options.get_type() != Variant::NIL) {
        EOS_Lobby_LocalRTCOptions localRTCOptions = eosg_variant_to_lobby_local_rtc_options(local_rtc_options);
        options.LocalRTCOptions = &localRTCOptions;
    }
    options.LobbyDetailsHandle = p_lobby_details->get_internal();
    p_options->reference();

    EOS_Lobby_JoinLobby(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_JoinLobbyCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_join_lobby_callback", ret);
    });
}

void IEOS::lobby_interface_join_lobby_by_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_JoinLobbyByIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_JOINLOBBYBYID_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    options.bPresenceEnabled = VARIANT_TO_EOS_BOOL(p_options->get("presence_enabled"));
    Variant local_rtc_options = p_options->get("local_rtc_options");
    if (local_rtc_options.get_type() != Variant::NIL) {
        EOS_Lobby_LocalRTCOptions localRTCOptions = eosg_variant_to_lobby_local_rtc_options(local_rtc_options);
        options.LocalRTCOptions = &localRTCOptions;
    }
    p_options->reference();

    EOS_Lobby_JoinLobbyById(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_JoinLobbyByIdCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_join_lobby_by_id_callback", ret);
    });
}

void IEOS::lobby_interface_leave_lobby(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_LeaveLobbyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_LEAVELOBBY_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    p_options->reference();

    EOS_Lobby_LeaveLobby(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_LeaveLobbyCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_leave_lobby_callback", ret);
    });
}

Dictionary IEOS::lobby_interface_update_lobby_modification(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_UpdateLobbyModificationOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();

    EOS_HLobbyModification outLobbyModification = nullptr;
    EOS_EResult res = EOS_Lobby_UpdateLobbyModification(s_lobbyInterface, &options, &outLobbyModification);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_modification"] = eosg_lobby_lobby_modification_to_wrapper(outLobbyModification);
    return ret;
}

void IEOS::lobby_interface_update_lobby(Ref<RefCounted> p_options) {
    Ref<EOSGLobbyModification> lobby_modification = Object::cast_to<EOSGLobbyModification>(p_options->get("lobby_modification"));
    ERR_FAIL_NULL_MSG(lobby_modification, "Error updating lobby. UpdateLobbyOptions.lobby_modification is null.");
    ERR_FAIL_NULL_MSG(lobby_modification->get_internal(), "Error updating lobby. EOSGLobbyModification is null.");

    EOS_Lobby_UpdateLobbyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_UPDATELOBBY_API_LATEST;
    options.LobbyModificationHandle = lobby_modification->get_internal();
    p_options->reference();
    lobby_modification->reference();

    EOS_Lobby_UpdateLobby(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_UpdateLobbyCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();

        Ref<EOSGLobbyModification> got_lobby_modification = Object::cast_to<EOSGLobbyModification>(client_data->get("lobby_modification"));
        got_lobby_modification->unreference();

        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_update_lobby_callback", ret);
    });
}

void IEOS::lobby_interface_promote_member(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_PromoteMemberOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_PROMOTEMEMBER_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    p_options->reference();

    EOS_Lobby_PromoteMember(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_PromoteMemberCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_promote_member_callback", ret);
    });
}

void IEOS::lobby_interface_kick_member(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_KickMemberOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_KICKMEMBER_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    p_options->reference();

    EOS_Lobby_KickMember(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_KickMemberCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_kick_member_callback", ret);
    });
}

void IEOS::lobby_interface_hard_mute_member(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_HardMuteMemberOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_HARDMUTEMEMBER_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    options.bHardMute = VARIANT_TO_EOS_BOOL(p_options->get("hard_mute"));
    p_options->reference();

    EOS_Lobby_HardMuteMember(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_HardMuteMemberCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("lobby_interface_hard_mute_member_callback", ret);
    });
}

void IEOS::lobby_interface_send_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_SendInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_SENDINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.LobbyId = lobby_id.get_data();
    p_options->reference();

    EOS_Lobby_SendInvite(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_SendInviteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_send_invite_callback", ret);
    });
}

void IEOS::lobby_interface_reject_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString invite_id = VARIANT_TO_CHARSTRING(p_options->get("invite_id"));

    EOS_Lobby_RejectInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_REJECTINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.InviteId = invite_id.get_data();
    p_options->reference();

    EOS_Lobby_RejectInvite(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_RejectInviteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["invite_id"] = EOSG_GET_STRING(data->InviteId);
        IEOS::get_singleton()->emit_signal("lobby_interface_reject_invite_callback", ret);
    });
}

void IEOS::lobby_interface_query_invites(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Lobby_QueryInvitesOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_QUERYINVITES_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_Lobby_QueryInvites(s_lobbyInterface, &options, (void *)*p_options, [](const EOS_Lobby_QueryInvitesCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("lobby_interface_query_invites_callback", ret);
    });
}

int IEOS::lobby_interface_get_invite_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Lobby_GetInviteCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_GETINVITECOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    return static_cast<int>(EOS_Lobby_GetInviteCount(s_lobbyInterface, &options));
}

Dictionary IEOS::lobby_interface_get_invite_id_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Lobby_GetInviteIdByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_GETINVITEIDBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Index = static_cast<uint32_t>(static_cast<int>(p_options->get("index")));

    char *outBuffer = (char *)memalloc(EOS_LOBBY_INVITEID_MAX_LENGTH + 1);
    int outBufferSize = EOS_LOBBY_INVITEID_MAX_LENGTH + 1;
    EOS_EResult result = EOS_Lobby_GetInviteIdByIndex(s_lobbyInterface, &options, outBuffer, &outBufferSize);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(result);
    ret["invite_id"] = EOSG_GET_STRING(outBuffer);
    return ret;
}

Dictionary IEOS::lobby_interface_create_lobby_search(Ref<RefCounted> p_options) {
    EOS_Lobby_CreateLobbySearchOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST;
    options.MaxResults = static_cast<uint32_t>(static_cast<int>(p_options->get("max_results")));

    EOS_HLobbySearch outLobbySearch = nullptr;
    EOS_EResult res = EOS_Lobby_CreateLobbySearch(s_lobbyInterface, &options, &outLobbySearch);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_search"] = eosg_lobby_lobby_search_to_wrapper(outLobbySearch);
    return ret;
}

Dictionary IEOS::lobby_interface_copy_lobby_details_by_invite_id(Ref<RefCounted> p_options) {
    CharString invite_id = VARIANT_TO_CHARSTRING(p_options->get("invite_id"));

    EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYINVITEID_API_LATEST;
    options.InviteId = invite_id.get_data();

    EOS_HLobbyDetails outLobbyDetails = nullptr;
    EOS_EResult res = EOS_Lobby_CopyLobbyDetailsHandleByInviteId(s_lobbyInterface, &options, &outLobbyDetails);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_details"] = eosg_lobby_lobby_details_to_wrapper(outLobbyDetails);
    return ret;
}

Dictionary IEOS::lobby_interface_copy_lobby_details_by_ui_event_id(Ref<RefCounted> p_options) {
    EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYUIEVENTID_API_LATEST;
    options.UiEventId = static_cast<uint64_t>(static_cast<int>(p_options->get("ui_event_id")));

    EOS_HLobbyDetails outLobbyDetails = nullptr;
    EOS_EResult res = EOS_Lobby_CopyLobbyDetailsHandleByUiEventId(s_lobbyInterface, &options, &outLobbyDetails);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_details"] = eosg_lobby_lobby_details_to_wrapper(outLobbyDetails);
    return ret;
}

Dictionary IEOS::lobby_interface_copy_lobby_details(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_CopyLobbyDetailsHandleOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();

    EOS_HLobbyDetails outLobbyDetails = nullptr;
    EOS_EResult res = EOS_Lobby_CopyLobbyDetailsHandle(s_lobbyInterface, &options, &outLobbyDetails);

    Variant lobby_details = eosg_lobby_lobby_details_to_wrapper(outLobbyDetails);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["lobby_details"] = lobby_details;
    return ret;
}

Dictionary IEOS::lobby_interface_get_rtc_room_name(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_GetRTCRoomNameOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_GETRTCROOMNAME_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();

    char *outBuffer = (char *)memalloc(256); // TODO: what is the actual max size for the outBuffer?
    uint32_t outBufferSize = 256;
    EOS_EResult res = EOS_Lobby_GetRTCRoomName(s_lobbyInterface, &options, outBuffer, &outBufferSize);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["rtc_room_name"] = EOSG_GET_STRING(outBuffer);

    return ret;
}

Dictionary IEOS::lobby_interface_is_rtc_room_connected(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_IsRTCRoomConnectedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_ISRTCROOMCONNECTED_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();

    EOS_Bool outIsConnected = EOS_FALSE;
    EOS_EResult res = EOS_Lobby_IsRTCRoomConnected(s_lobbyInterface, &options, &outIsConnected);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["is_connected"] = EOSG_GET_BOOL(outIsConnected);
    return ret;
}

Dictionary IEOS::lobby_interface_get_connect_string(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString lobby_id = VARIANT_TO_CHARSTRING(p_options->get("lobby_id"));

    EOS_Lobby_GetConnectStringOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_GETCONNECTSTRING_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.LobbyId = lobby_id.get_data();

    char *OutBuffer = (char *)memalloc(EOS_LOBBY_GETCONNECTSTRING_BUFFER_SIZE + 1);
    uint32_t OutBufferLength = 0;
    EOS_EResult res = EOS_Lobby_GetConnectString(s_lobbyInterface, &options, OutBuffer, &OutBufferLength);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    if (res == EOS_EResult::EOS_Success) {
        ret["connect_string"] = EOSG_GET_STRING(OutBuffer);
    } else {
        ret["connect_string"] = String("");
    }

    return ret;
}

Dictionary IEOS::lobby_interface_parse_connect_string(Ref<RefCounted> p_options) {
    CharString connect_string = VARIANT_TO_CHARSTRING(p_options->get("connect_string"));

    EOS_Lobby_ParseConnectStringOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_PARSECONNECTSTRING_API_LATEST;
    options.ConnectString = connect_string.get_data();

    char *OutBuffer = (char *)memalloc(EOS_LOBBY_PARSECONNECTSTRING_BUFFER_SIZE + 1);
    uint32_t OutBufferLength = 0;
    EOS_EResult res = EOS_Lobby_ParseConnectString(s_lobbyInterface, &options, OutBuffer, &OutBufferLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    if (res == EOS_EResult::EOS_Success) {
        ret["lobby_id"] = EOSG_GET_STRING(OutBuffer);
    } else {
        ret["lobby_id"] = String("");
    }

    return ret;
}
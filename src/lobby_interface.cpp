#include "ieos.h"
#include "lobby_details.h"
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
    // Todo: handle options.LocalRTCOptions
    if (lobby_id.length() != 0) {
        options.LobbyId = lobby_id.get_data();
    }
    options.bEnableJoinById = VARIANT_TO_EOS_BOOL(p_options->get("enable_join_by_id"));
    options.bRejoinAfterKickRequiresInvite = VARIANT_TO_EOS_BOOL(p_options->get("rejoin_after_kick_requires_invite"));
    p_options->reference();

    EOS_Lobby_CreateLobby(s_lobbyInterface, &options, (void*)*p_options, [](const EOS_Lobby_CreateLobbyCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_create_lobby_callback", ret);
    });
    return;
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

    EOS_Lobby_DestroyLobby(s_lobbyInterface, &options, (void*)*p_options, [](const EOS_Lobby_DestroyLobbyCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_destroy_lobby_callback", ret);
    });
    return;
}

void IEOS::lobby_interface_join_lobby(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    Ref<LobbyDetailsEOSG> p_lobby_details = Object::cast_to<LobbyDetailsEOSG>(p_options->get("lobby_details"));

    EOS_Lobby_JoinLobbyOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBY_JOINLOBBY_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.bPresenceEnabled = VARIANT_TO_EOS_BOOL(p_options->get("presence_enabled"));
    // TODO: handle options.LocalRTCOptions
    options.LobbyDetailsHandle = p_lobby_details->get_internal();
    p_options->reference();

    EOS_Lobby_JoinLobby(s_lobbyInterface, &options, (void*)*p_options, [](const EOS_Lobby_JoinLobbyCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["lobby_id"] = EOSG_GET_STRING(data->LobbyId);
        IEOS::get_singleton()->emit_signal("lobby_interface_join_lobby_callback", ret);
    });
    return;
}

void IEOS::lobby_interface_join_lobby_by_id(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_leave_lobby(Ref<RefCounted> p_options) {
    return;
}

int IEOS::lobby_interface_update_lobby_modification(Ref<RefCounted> p_options) {
    return -1;
}

void IEOS::lobby_interface_update_lobby(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_promote_member(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_kick_member(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_hard_mute_member(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_send_invite(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_reject_invite(Ref<RefCounted> p_options) {
    return;
}

void IEOS::lobby_interface_query_invites(Ref<RefCounted> p_options) {
    return;
}

int IEOS::lobby_interface_get_invite_count(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_get_invite_id_by_index(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_create_lobby_search(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_copy_lobby_details_handle_by_invite_id(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_copy_lobby_details_handle_by_ui_event_id(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_copy_lobby_details_handle(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_get_rtc_room_name(Ref<RefCounted> p_options) {
    return -1;
}

int IEOS::lobby_interface_is_rtc_room_connected(Ref<RefCounted> p_options) {
    return -1;
}

// int EOS_LobbyModification_SetBucketId(EOS_HLobbyModification Handle, const EOS_LobbyModification_SetBucketIdOptions* Options);
// int EOS_LobbyModification_SetPermissionLevel(EOS_HLobbyModification Handle, const EOS_LobbyModification_SetPermissionLevelOptions* Options);
// int EOS_LobbyModification_SetMaxMembers(EOS_HLobbyModification Handle, const EOS_LobbyModification_SetMaxMembersOptions* Options);
// int EOS_LobbyModification_SetInvitesAllowed(EOS_HLobbyModification Handle, const EOS_LobbyModification_SetInvitesAllowedOptions* Options);
// int EOS_LobbyModification_AddAttribute(EOS_HLobbyModification Handle, const EOS_LobbyModification_AddAttributeOptions* Options);
// int EOS_LobbyModification_RemoveAttribute(EOS_HLobbyModification Handle, const EOS_LobbyModification_RemoveAttributeOptions* Options);
// int EOS_LobbyModification_AddMemberAttribute(EOS_HLobbyModification Handle, const EOS_LobbyModification_AddMemberAttributeOptions* Options);
// int EOS_LobbyModification_RemoveMemberAttribute(EOS_HLobbyModification Handle, const EOS_LobbyModification_RemoveMemberAttributeOptions* Options);
// String EOS_LobbyDetails_GetLobbyOwner(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_GetLobbyOwnerOptions* Options);
// int EOS_LobbyDetails_CopyInfo(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_CopyInfoOptions* Options, EOS_LobbyDetails_Info** OutLobbyDetailsInfo);
// int EOS_LobbyDetails_GetAttributeCount(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_GetAttributeCountOptions* Options);
// int EOS_LobbyDetails_CopyAttributeByIndex(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_CopyAttributeByIndexOptions* Options, EOS_Lobby_Attribute** OutAttribute);
// int EOS_LobbyDetails_CopyAttributeByKey(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_CopyAttributeByKeyOptions* Options, EOS_Lobby_Attribute** OutAttribute);
// int EOS_LobbyDetails_GetMemberCount(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_GetMemberCountOptions* Options);
// String EOS_LobbyDetails_GetMemberByIndex(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_GetMemberByIndexOptions* Options);
// int EOS_LobbyDetails_GetMemberAttributeCount(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_GetMemberAttributeCountOptions* Options);
// int EOS_LobbyDetails_CopyMemberAttributeByIndex(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_CopyMemberAttributeByIndexOptions* Options, EOS_Lobby_Attribute** OutAttribute);
// int EOS_LobbyDetails_CopyMemberAttributeByKey(EOS_HLobbyDetails Handle, const EOS_LobbyDetails_CopyMemberAttributeByKeyOptions* Options, EOS_Lobby_Attribute** OutAttribute);
// void IEOS::lobby_interfaceSearch_find(Ref<RefCounted> p_options);
// int EOS_LobbySearch_SetLobbyId(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetLobbyIdOptions* Options);
// int EOS_LobbySearch_SetTargetUserId(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetTargetUserIdOptions* Options);
// int EOS_LobbySearch_SetParameter(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetParameterOptions* Options);
// int EOS_LobbySearch_RemoveParameter(EOS_HLobbySearch Handle, const EOS_LobbySearch_RemoveParameterOptions* Options);
// int EOS_LobbySearch_SetMaxResults(EOS_HLobbySearch Handle, const EOS_LobbySearch_SetMaxResultsOptions* Options);
// int EOS_LobbySearch_GetSearchResultCount(EOS_HLobbySearch Handle, const EOS_LobbySearch_GetSearchResultCountOptions* Options);
// int EOS_LobbySearch_CopySearchResultByIndex(EOS_HLobbySearch Handle, const EOS_LobbySearch_CopySearchResultByIndexOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle);
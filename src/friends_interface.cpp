#include "ieos.h"
using namespace std;

void IEOS::friends_interface_accept_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Friends_AcceptInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_ACCEPTINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_Friends_AcceptInvite(s_friendsInterface, &options, (void*)*p_options, [](const EOS_Friends_AcceptInviteCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("friends_interface_accept_invite_callback", ret);
    });
    return;
}

String IEOS::friends_interface_get_friend_at_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Friends_GetFriendAtIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.Index = static_cast<int32_t>(p_options->get("index"));

    return eosg_epic_account_id_to_string(EOS_Friends_GetFriendAtIndex(s_friendsInterface, &options));
}

int IEOS::friends_interface_get_friends_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Friends_GetFriendsCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return static_cast<int>(EOS_Friends_GetFriendsCount(s_friendsInterface, &options));
}

int IEOS::friends_interface_get_status(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Friends_GetStatusOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());

    return static_cast<int>(EOS_Friends_GetStatus(s_friendsInterface, &options));
}

void IEOS::friends_interface_query_friends(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Friends_QueryFriendsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_QUERYFRIENDS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    p_options->reference();

    EOS_Friends_QueryFriends(s_friendsInterface, &options, (void*)*p_options, [](const EOS_Friends_QueryFriendsCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("friends_interface_query_friends_callback", ret);
    });
    return;
}

void IEOS::friends_interface_reject_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Friends_RejectInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_REJECTINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_Friends_RejectInvite(s_friendsInterface, &options, (void*)*p_options, [](const EOS_Friends_RejectInviteCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("friends_interface_reject_invite_callback", ret);
    });
}

void IEOS::friends_interface_send_invite(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Friends_SendInviteOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_FRIENDS_SENDINVITE_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TargetUserId = eosg_string_to_epic_account_id(target_user_id.get_data());
    p_options->reference();

    EOS_Friends_SendInvite(s_friendsInterface, &options, (void*)*p_options, [](const EOS_Friends_SendInviteCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_epic_account_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("friends_interface_send_invite_callback", ret);
    });
    return;
}

int IEOS::friends_interface_get_blocked_users_count(Ref<RefCounted> p_options){
	CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

	EOS_Friends_GetBlockedUsersCountOptions options;
	memset(&options, 0, sizeof(options));
	options.ApiVersion = EOS_FRIENDS_GETBLOCKEDUSERSCOUNT_API_LATEST;
	options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

	return static_cast<int>(EOS_Friends_GetBlockedUsersCount(s_friendsInterface, &options));
}


String IEOS::friends_interface_get_blocked_user_at_index(Ref<RefCounted> p_options){
	CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
	int index = static_cast<int>(p_options->get("index"));

	EOS_Friends_GetBlockedUserAtIndexOptions options;
	memset(&options, 0, sizeof(options));
	options.ApiVersion = EOS_FRIENDS_GETBLOCKEDUSERATINDEX_API_LATEST;
	options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
	options.Index = static_cast<int32_t>(index);

	return eosg_epic_account_id_to_string(EOS_Friends_GetBlockedUserAtIndex(s_friendsInterface, &options));
}
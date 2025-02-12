#include "godot_cpp/variant/packed_byte_array.hpp"
#include "ieos.h"

using namespace godot;

void IEOS::sanctions_interface_query_active_player_sanctions(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_sanctionsInterface);
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Sanctions_QueryActivePlayerSanctionsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    EOS_Sanctions_QueryActivePlayerSanctions(s_sanctionsInterface, &options, (void *)*p_options, [](const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["target_user_id"] = eosg_product_user_id_to_string(data->TargetUserId);
        IEOS::get_singleton()->emit_signal("sanctions_interface_query_active_player_sanctions_callback", ret);
    });
}

int IEOS::sanctions_interface_get_player_sanction_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_sanctionsInterface, 0);
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));

    EOS_Sanctions_GetPlayerSanctionCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SANCTIONS_GETPLAYERSANCTIONCOUNT_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());

    return static_cast<int>(EOS_Sanctions_GetPlayerSanctionCount(s_sanctionsInterface, &options));
}

Dictionary IEOS::sanctions_interface_copy_player_sanction_by_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_sanctionsInterface, {});
    CharString target_user_id = VARIANT_TO_CHARSTRING(p_options->get("target_user_id"));
    int sanction_index = p_options->get("sanction_index");

    EOS_Sanctions_CopyPlayerSanctionByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SANCTIONS_COPYPLAYERSANCTIONBYINDEX_API_LATEST;
    options.TargetUserId = eosg_string_to_product_user_id(target_user_id.get_data());
    options.SanctionIndex = sanction_index;

    EOS_Sanctions_PlayerSanction *outSanction = nullptr;
    EOS_EResult result = EOS_Sanctions_CopyPlayerSanctionByIndex(s_sanctionsInterface, &options, &outSanction);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(result);
    ret["sanction"] = eosg_sanctions_player_sanction_to_dict_and_release(outSanction);
    return ret;
}

void IEOS::sanctions_interface_create_player_sanction_appeal(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_sanctionsInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString reference_id = VARIANT_TO_CHARSTRING(p_options->get("reference_id"));
    int reason = p_options->get("reason");

    EOS_Sanctions_CreatePlayerSanctionAppealOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SANCTIONS_CREATEPLAYERSANCTIONAPPEAL_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.ReferenceId = reference_id.get_data();
    options.Reason = static_cast<EOS_ESanctionAppealReason>(reason);

    EOS_Sanctions_CreatePlayerSanctionAppeal(s_sanctionsInterface, &options, (void *)*p_options, [](const EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["reference_id"] = EOSG_GET_STRING(data->ReferenceId);
        IEOS::get_singleton()->emit_signal("sanctions_interface_create_player_sanction_appeal_callback", ret);
    });
}

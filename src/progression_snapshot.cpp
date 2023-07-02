#include "ieos.h"
using namespace std;

Dictionary IEOS::progression_snapshot_interface_begin_snapshot(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_ProgressionSnapshot_BeginSnapshotOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_BEGINSNAPSHOT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    uint32_t outSnapshotId = 0;
    EOS_EResult res = EOS_ProgressionSnapshot_BeginSnapshot(s_progressionSnapshotInterface, &options, &outSnapshotId);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["snapshot_id"] = outSnapshotId;
    return ret;
}

int IEOS::progression_snapshot_interface_add_progression(Ref<RefCounted> p_options) {
    CharString key = VARIANT_TO_CHARSTRING(p_options->get("key"));
    CharString value = VARIANT_TO_CHARSTRING(p_options->get("value"));

    EOS_ProgressionSnapshot_AddProgressionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_ADDPROGRESSION_API_LATEST;
    options.SnapshotId = static_cast<uint32_t>(static_cast<int>(p_options->get("snapshot_id")));
    options.Key = key.get_data();
    options.Value = value.get_data();

    return static_cast<int>(EOS_ProgressionSnapshot_AddProgression(s_progressionSnapshotInterface, &options));
}

void IEOS::progression_snapshot_interface_submit_snapshot(Ref<RefCounted> p_options) {
    EOS_ProgressionSnapshot_SubmitSnapshotOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_SUBMITSNAPSHOT_API_LATEST;
    options.SnapshotId = static_cast<uint32_t>(static_cast<int>(p_options->get("snapshot_id")));
    p_options->reference();

    EOS_ProgressionSnapshot_SubmitSnapshot(s_progressionSnapshotInterface, &options, (void*)*p_options, [](const EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["snapshot_id"] = static_cast<int>(data->SnapshotId);
        IEOS::get_singleton()->emit_signal("progression_snapshot_interface_submit_snapshot_callback", ret);
    });
    return;
}

void IEOS::progression_snapshot_interface_delete_snapshot(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_ProgressionSnapshot_DeleteSnapshotOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_DELETESNAPSHOT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_ProgressionSnapshot_DeleteSnapshot(s_progressionSnapshotInterface, &options, (void*)*p_options, [](const EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo* data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted*>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("progression_snapshot_interface_delete_snapshot_callback", ret);
    });

    return;
}

int IEOS::progression_snapshot_interface_end_snapshot(Ref<RefCounted> p_options) {
    EOS_ProgressionSnapshot_EndSnapshotOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_ENDSNAPSHOT_API_LATEST;
    options.SnapshotId = static_cast<uint32_t>(static_cast<int>(p_options->get("snapshot_id")));

    return static_cast<int>(EOS_ProgressionSnapshot_EndSnapshot(s_progressionSnapshotInterface, &options));
}

#include "ieos.h"
using namespace godot;

int IEOS::anticheat_client_interface_begin_session(Ref<RefCounted> p_options) {
    CharString p_local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    int p_mode = p_options->get("mode");

    EOS_AntiCheatClient_BeginSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_BEGINSESSION_API_LATEST;
    options.Mode = static_cast<EOS_EAntiCheatClientMode>(p_mode);
    options.LocalUserId = eosg_string_to_product_user_id(p_local_user_id);

    return static_cast<int>(EOS_AntiCheatClient_BeginSession(s_antiCheatClientInterface, &options));
};

int IEOS::anticheat_client_interface_end_session(Ref<RefCounted> p_options) {
    EOS_AntiCheatClient_EndSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_ENDSESSION_API_LATEST;

    return static_cast<int>(EOS_AntiCheatClient_EndSession(s_antiCheatClientInterface, &options));
}

int IEOS::anticheat_client_interface_add_external_integrity_catalog(Ref<RefCounted> p_options) {
    CharString p_path_to_bin_file = VARIANT_TO_CHARSTRING(p_options->get("path_to_bin_file"));
    EOS_AntiCheatClient_AddExternalIntegrityCatalogOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_ADDEXTERNALINTEGRITYCATALOG_API_LATEST;
    options.PathToBinFile = ProjectSettings::get_singleton()->globalize_path(p_path_to_bin_file.get_data()).utf8();

    return static_cast<int>(EOS_AntiCheatClient_AddExternalIntegrityCatalog(s_antiCheatClientInterface, &options));
}

int IEOS::anticheat_client_interface_receive_message_from_server(Ref<RefCounted> p_options) {
    PackedByteArray p_data = p_options->get("data");

    void *data = memalloc(p_data.size());
    std::memcpy(data, p_data.ptr(), p_data.size());

    EOS_AntiCheatClient_ReceiveMessageFromServerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMSERVER_API_LATEST;
    options.DataLengthBytes = static_cast<uint32_t>(p_data.size());
    options.Data = data;

    return static_cast<int>(EOS_AntiCheatClient_ReceiveMessageFromServer(s_antiCheatClientInterface, &options));
}

Dictionary IEOS::anticheat_client_interface_get_protect_message_output_length(Ref<RefCounted> p_options) {
    int p_data_length_bytes = p_options->get("data_length_bytes");

    EOS_AntiCheatClient_GetProtectMessageOutputLengthOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_GETPROTECTMESSAGEOUTPUTLENGTH_API_LATEST;
    options.DataLengthBytes = static_cast<uint32_t>(p_data_length_bytes);

    uint32_t outBufferSizeBytes = 0;

    EOS_EResult res = EOS_AntiCheatClient_GetProtectMessageOutputLength(s_antiCheatClientInterface, &options, &outBufferSizeBytes);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["buffer_size_bytes"] = 0;
    if (res == EOS_EResult::EOS_Success) {
        ret["buffer_size_bytes"] = outBufferSizeBytes;
    }

    return ret;
}

Dictionary IEOS::anticheat_client_interface_protect_message(Ref<RefCounted> p_options) {
    PackedByteArray p_data = p_options->get("data");
    int data_length_bytes = p_data.size();
    void *data = memalloc(data_length_bytes);
    std::memcpy(data, p_data.ptr(), data_length_bytes);
    int out_buffer_size_bytes = p_options->get("out_buffer_size_bytes");

    EOS_AntiCheatClient_ProtectMessageOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_PROTECTMESSAGE_API_LATEST;
    options.DataLengthBytes = static_cast<uint32_t>(data_length_bytes);
    options.Data = data;
    options.OutBufferSizeBytes = static_cast<uint32_t>(out_buffer_size_bytes);

    uint32_t outBytesWritten = 0;
    void *outBuffer = (void *)memalloc(out_buffer_size_bytes + 1);

    EOS_EResult res = EOS_AntiCheatClient_ProtectMessage(s_antiCheatClientInterface, &options, &outBuffer, &outBytesWritten);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["out_buffer"] = Variant();
    ret["out_bytes_written"] = 0;
    if (res == EOS_EResult::EOS_Success) {
        ret["out_buffer"] = outBuffer;
        ret["out_bytes_written"] = outBytesWritten;
    }
    return ret;
}

Dictionary IEOS::anticheat_client_interface_unprotect_message(Ref<RefCounted> p_options) {
    PackedByteArray p_data = p_options->get("data");
    int data_length_bytes = p_data.size();
    void *data = memalloc(data_length_bytes);
    std::memcpy(data, p_data.ptr(), data_length_bytes);

    int out_buffer_size_bytes = p_options->get("out_buffer_size_bytes");

    EOS_AntiCheatClient_UnprotectMessageOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_UNPROTECTMESSAGE_API_LATEST;
    options.DataLengthBytes = static_cast<uint32_t>(data_length_bytes);
    options.Data = data;
    options.OutBufferSizeBytes = static_cast<uint32_t>(out_buffer_size_bytes);

    uint32_t outBytesWritten = 0;
    void *outBuffer = (void *)memalloc(out_buffer_size_bytes + 1);

    EOS_EResult res = EOS_AntiCheatClient_UnprotectMessage(s_antiCheatClientInterface, &options, &outBuffer, &outBytesWritten);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["out_buffer"] = Variant();
    ret["out_bytes_written"] = 0;
    if (res == EOS_EResult::EOS_Success) {
        ret["out_buffer"] = outBuffer;
        ret["out_bytes_written"] = outBytesWritten;
    }
    return ret;
}

int IEOS::anticheat_client_interface_register_peer(Ref<RefCounted> p_options) {
    CharString p_peer_handle = VARIANT_TO_CHARSTRING(p_options->get("peer_handle"));
    int p_client_type = p_options->get("client_type");
    int p_client_platform = p_options->get("client_platform");
    int p_authentication_timeout = p_options->get("authentication_timeout");
    CharString p_ip_address = VARIANT_TO_CHARSTRING(p_options->get("ip_address"));
    CharString p_peer_product_user_id = VARIANT_TO_CHARSTRING(p_options->get("peer_product_user_id"));

    EOS_AntiCheatClient_RegisterPeerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_REGISTERPEER_API_LATEST;
    options.PeerHandle = _anticheat_player_id_to_handle(p_peer_handle);
    options.ClientType = static_cast<EOS_EAntiCheatCommonClientType>(p_client_type);
    options.ClientPlatform = static_cast<EOS_EAntiCheatCommonClientPlatform>(p_client_platform);
    options.AuthenticationTimeout = static_cast<uint32_t>(p_authentication_timeout);
    if (p_ip_address.size() > 1) {
        options.IpAddress = p_ip_address.get_data();
    }
    options.PeerProductUserId = eosg_string_to_product_user_id(p_peer_product_user_id);

    return static_cast<int>(EOS_AntiCheatClient_RegisterPeer(s_antiCheatClientInterface, &options));
}

int IEOS::anticheat_client_interface_unregister_peer(Ref<RefCounted> p_options) {
    CharString p_peer_handle = VARIANT_TO_CHARSTRING(p_options->get("peer_handle"));

    EOS_AntiCheatClient_UnregisterPeerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_UNREGISTERPEER_API_LATEST;
    options.PeerHandle = _anticheat_player_id_to_handle(p_peer_handle);

    return static_cast<int>(EOS_AntiCheatClient_UnregisterPeer(s_antiCheatClientInterface, &options));
}

int IEOS::anticheat_client_interface_receive_message_from_peer(Ref<RefCounted> p_options) {
    CharString p_peer_handle = VARIANT_TO_CHARSTRING(p_options->get("peer_handle"));
    PackedByteArray p_data = p_options->get("data");

    void *data = memalloc(p_data.size());
    std::memcpy(data, p_data.ptr(), p_data.size());

    EOS_AntiCheatClient_ReceiveMessageFromPeerOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMPEER_API_LATEST;
    options.DataLengthBytes = static_cast<uint32_t>(p_data.size());
    options.Data = data;
    options.PeerHandle = _anticheat_player_id_to_handle(p_peer_handle);

    return static_cast<int>(EOS_AntiCheatClient_ReceiveMessageFromPeer(s_antiCheatClientInterface, &options));
}

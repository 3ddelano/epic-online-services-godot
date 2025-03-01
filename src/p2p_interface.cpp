#include "ieos.h"

void IEOS::p2p_interface_query_nat_type() {
    ERR_FAIL_NULL(s_p2pInterface);
    EOS_P2P_QueryNATTypeOptions options = { 0 };
    options.ApiVersion = EOS_P2P_QUERYNATTYPE_API_LATEST;
    EOS_P2P_QueryNATType(s_p2pInterface, &options, nullptr, [](const EOS_P2P_OnQueryNATTypeCompleteInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        ret["nat_type"] = static_cast<int>(data->NATType);
        get_singleton()->emit_signal("p2p_interface_query_nat_type_callback", ret);
    });
}

Dictionary IEOS::p2p_interface_get_nat_type() {
    ERR_FAIL_NULL_V(s_p2pInterface, Dictionary());
    EOS_P2P_GetNATTypeOptions options = { 0 };
    options.ApiVersion = EOS_P2P_GETNATTYPE_API_LATEST;

    EOS_ENATType nat_type = EOS_ENATType::EOS_NAT_Unknown;
    EOS_EResult res = EOS_P2P_GetNATType(s_p2pInterface, &options, &nat_type);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    if (res == EOS_EResult::EOS_Success) {
        ret["nat_type"] = static_cast<int>(nat_type);
    } else {
        ret["nat_type"] = static_cast<int>(EOS_ENATType::EOS_NAT_Unknown);
    }

    return ret;
}

int IEOS::p2p_interface_set_relay_control(int control) {
    ERR_FAIL_NULL_V(s_p2pInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_ERelayControl relay_control = static_cast<EOS_ERelayControl>(control);
    EOS_P2P_SetRelayControlOptions options = { 0 };
    options.ApiVersion = EOS_P2P_SETRELAYCONTROL_API_LATEST;
    options.RelayControl = relay_control;
    return static_cast<int>(EOS_P2P_SetRelayControl(s_p2pInterface, &options));
}

Dictionary IEOS::p2p_interface_get_relay_control() {
    ERR_FAIL_NULL_V(s_p2pInterface, Dictionary());
    EOS_P2P_GetRelayControlOptions options = { 0 };
    options.ApiVersion = EOS_P2P_GETRELAYCONTROL_API_LATEST;

    EOS_ERelayControl relay_control;
    EOS_EResult res = EOS_P2P_GetRelayControl(s_p2pInterface, &options, &relay_control);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    if (res == EOS_EResult::EOS_Success) {
        ret["relay_control"] = static_cast<int>(relay_control);
    } else {
        ret["relay_control"] = static_cast<int>(EOS_ERelayControl::EOS_RC_NoRelays);
    }
    return ret;
}

int IEOS::p2p_interface_set_port_range(Ref<RefCounted> options) {
    ERR_FAIL_NULL_V(s_p2pInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    int port = options->get("port");
    int max_additional_ports_to_try = options->get("max_additional_ports_to_try");
    EOS_P2P_SetPortRangeOptions set_port_range_options = { 0 };
    set_port_range_options.ApiVersion = EOS_P2P_SETPORTRANGE_API_LATEST;
    set_port_range_options.Port = port;
    set_port_range_options.MaxAdditionalPortsToTry = max_additional_ports_to_try;
    return static_cast<int>(EOS_P2P_SetPortRange(s_p2pInterface, &set_port_range_options));
}

Dictionary IEOS::p2p_interface_get_port_range() {
    ERR_FAIL_NULL_V(s_p2pInterface, {});
    EOS_P2P_GetPortRangeOptions options = { 0 };
    options.ApiVersion = EOS_P2P_GETPORTRANGE_API_LATEST;

    uint16_t port;
    uint16_t max_additional_ports_to_try;
    EOS_EResult res = EOS_P2P_GetPortRange(s_p2pInterface, &options, &port, &max_additional_ports_to_try);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    if (res != EOS_EResult::EOS_Success) {
        ret["port"] = port;
        ret["max_additional_ports_to_try"] = max_additional_ports_to_try;
    } else {
        ret["port"] = 0;
        ret["max_additional_ports_to_try"] = 0;
    }
    return ret;
}

int IEOS::p2p_interface_set_packet_queue_size(Ref<RefCounted> options) {
    ERR_FAIL_NULL_V(s_p2pInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    uint64_t incoming_packet_queue_max_size_bytes = options->get("incoming_packet_queue_max_size_bytes");
    uint64_t outgoing_packet_queue_max_size_bytes = options->get("outgoing_packet_queue_max_size_bytes");
    EOS_P2P_SetPacketQueueSizeOptions set_packet_queue_options;
    set_packet_queue_options.ApiVersion = EOS_P2P_SETPACKETQUEUESIZE_API_LATEST;
    set_packet_queue_options.IncomingPacketQueueMaxSizeBytes = incoming_packet_queue_max_size_bytes;
    set_packet_queue_options.OutgoingPacketQueueMaxSizeBytes = outgoing_packet_queue_max_size_bytes;
    return static_cast<int>(EOS_P2P_SetPacketQueueSize(s_p2pInterface, &set_packet_queue_options));
}

Dictionary IEOS::p2p_interface_get_packet_queue_info() {
    ERR_FAIL_NULL_V(s_p2pInterface, {});
    EOS_P2P_GetPacketQueueInfoOptions options = { 0 };
    options.ApiVersion = EOS_P2P_GETPACKETQUEUEINFO_API_LATEST;

    EOS_P2P_PacketQueueInfo info;
    EOS_EResult res = EOS_P2P_GetPacketQueueInfo(s_p2pInterface, &options, &info);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["incoming_packet_queue_current_packet_count"] = info.IncomingPacketQueueCurrentPacketCount;
    ret["incoming_packet_queue_current_size_bytes"] = info.IncomingPacketQueueCurrentSizeBytes;
    ret["incoming_packet_queue_max_size_bytes"] = info.IncomingPacketQueueMaxSizeBytes;
    ret["outgoing_packet_queue_current_packet_count"] = info.OutgoingPacketQueueCurrentPacketCount;
    ret["outgoing_packet_queue_current_size_bytes"] = info.OutgoingPacketQueueCurrentSizeBytes;
    ret["outgoing_packet_queue_max_size_bytes"] = info.OutgoingPacketQueueMaxSizeBytes;
    return ret;
}

EOS_EResult IEOS::_p2p_send_packet(const EOS_P2P_SendPacketOptions *options) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_SendPacket(s_p2pInterface, options);
    return result;
}

EOS_EResult IEOS::_p2p_receive_packet(const EOS_P2P_ReceivePacketOptions *options, void *out_packet_data, uint32_t *out_packet_size,
        uint8_t *out_channel, EOS_ProductUserId *out_remote_user, EOS_P2P_SocketId *out_socket) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_ReceivePacket(s_p2pInterface, options, out_remote_user, out_socket,
            out_channel, out_packet_data, out_packet_size);
    return result;
}

EOS_EResult IEOS::_p2p_accept_connection(const EOS_P2P_AcceptConnectionOptions *options) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_AcceptConnection(s_p2pInterface, options);
    return result;
}

EOS_EResult IEOS::_p2p_close_connection(const EOS_P2P_CloseConnectionOptions *options) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_CloseConnection(s_p2pInterface, options);
    return result;
}

EOS_EResult IEOS::_p2p_close_all_connections(const EOS_P2P_CloseConnectionsOptions *options) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_CloseConnections(s_p2pInterface, options);
    return result;
}

EOS_EResult IEOS::_p2p_get_next_packet_size(const EOS_P2P_GetNextReceivedPacketSizeOptions *options, uint32_t *out_size) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_GetNextReceivedPacketSize(s_p2pInterface, options, out_size);
    return result;
}

EOS_EResult IEOS::_p2p_clear_packet_queue(const EOS_P2P_ClearPacketQueueOptions *options) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_EResult::EOS_InvalidParameters);
    EOS_EResult result = EOS_P2P_ClearPacketQueue(s_p2pInterface, options);
    return result;
}

EOS_NotificationId IEOS::_p2p_add_notify_peer_connection_established(const EOS_P2P_AddNotifyPeerConnectionEstablishedOptions *options,
        EOS_P2P_OnPeerConnectionEstablishedCallback callback) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_INVALID_NOTIFICATIONID);
    EOS_NotificationId callback_id = EOS_P2P_AddNotifyPeerConnectionEstablished(s_p2pInterface, options, nullptr, callback);
    return callback_id;
}

EOS_NotificationId IEOS::_p2p_add_notify_peer_connection_closed(const EOS_P2P_AddNotifyPeerConnectionClosedOptions *options,
        EOS_P2P_OnRemoteConnectionClosedCallback callback) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_INVALID_NOTIFICATIONID);
    EOS_NotificationId callback_id = EOS_P2P_AddNotifyPeerConnectionClosed(s_p2pInterface, options, nullptr, callback);
    return callback_id;
}

EOS_NotificationId IEOS::_p2p_add_notify_peer_connection_request(const EOS_P2P_AddNotifyPeerConnectionRequestOptions *options,
        EOS_P2P_OnIncomingConnectionRequestCallback callback) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_INVALID_NOTIFICATIONID);
    EOS_NotificationId callback_id = EOS_P2P_AddNotifyPeerConnectionRequest(s_p2pInterface, options, nullptr, callback);
    return callback_id;
}

EOS_NotificationId IEOS::_p2p_add_notify_peer_connection_interrupted(const EOS_P2P_AddNotifyPeerConnectionInterruptedOptions *options,
        EOS_P2P_OnPeerConnectionInterruptedCallback callback) {
    ERR_FAIL_NULL_V(s_p2pInterface, EOS_INVALID_NOTIFICATIONID);
    EOS_NotificationId callback_id = EOS_P2P_AddNotifyPeerConnectionInterrupted(s_p2pInterface, options, nullptr, callback);
    return callback_id;
}

void IEOS::_p2p_remove_notify_peer_connection_established(EOS_NotificationId callback_id) {
    ERR_FAIL_NULL(s_p2pInterface);
    EOS_P2P_RemoveNotifyPeerConnectionEstablished(s_p2pInterface, callback_id);
}

void IEOS::_p2p_remove_notify_peer_connection_interrupted(EOS_NotificationId callback_id) {
    ERR_FAIL_NULL(s_p2pInterface);
    EOS_P2P_RemoveNotifyPeerConnectionInterrupted(s_p2pInterface, callback_id);
}

void IEOS::_p2p_remove_notify_peer_connection_closed(EOS_NotificationId callback_id) {
    ERR_FAIL_NULL(s_p2pInterface);
    EOS_P2P_RemoveNotifyPeerConnectionClosed(s_p2pInterface, callback_id);
}

void IEOS::_p2p_remove_notify_peer_connection_request(EOS_NotificationId callback_id) {
    ERR_FAIL_NULL(s_p2pInterface);
    EOS_P2P_RemoveNotifyPeerConnectionRequest(s_p2pInterface, callback_id);
}
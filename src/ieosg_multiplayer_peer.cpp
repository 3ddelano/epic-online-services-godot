#include "ieos.h"
#include "ieosg_multiplayer_peer.h"

using namespace godot;

#define CHECK_IS_SINGLETON_INSTANCE ERR_FAIL_COND_V_MSG(is_singleton == false, ERR_UNCONFIGURED, "This instance is not the singleton instance. Do not create multiple instances of EOSGMultiplayerPeer. Use EOSGMultiplayerPeer.get_singleton() instead.")

EOS_ProductUserId IEOSGMultiplayerPeer::s_local_user_id = nullptr;
IEOSGMultiplayerPeer* IEOSGMultiplayerPeer::singleton = nullptr;

IEOSGMultiplayerPeer* IEOSGMultiplayerPeer::get_singleton() {
    return singleton;
}

void IEOSGMultiplayerPeer::_bind_methods() {
    ClassDB::bind_static_method("IEOSGMultiplayerPeer", D_METHOD("set_local_user_id"), &IEOSGMultiplayerPeer::set_local_user_id);
    ClassDB::bind_static_method("IEOSGMultiplayerPeer", D_METHOD("get_local_user_id"), &IEOSGMultiplayerPeer::get_local_user_id);

    ClassDB::bind_method(D_METHOD("create_server"), &IEOSGMultiplayerPeer::create_server);
    ClassDB::bind_method(D_METHOD("create_client"), &IEOSGMultiplayerPeer::create_client);
    ClassDB::bind_method(D_METHOD("get_peer_info"), &IEOSGMultiplayerPeer::get_peer_info);
    ClassDB::bind_method(D_METHOD("find_peer_id"), &IEOSGMultiplayerPeer::find_peer_id);
    ClassDB::bind_method(D_METHOD("get_all_sockets"), &IEOSGMultiplayerPeer::get_all_sockets);
    ClassDB::bind_method(D_METHOD("set_allow_delayed_delivery"), &IEOSGMultiplayerPeer::set_allow_delayed_delivery);
    ClassDB::bind_method(D_METHOD("is_allowing_delayed_delivery"), &IEOSGMultiplayerPeer::is_allowing_delayed_delivery);
    ClassDB::bind_method(D_METHOD("is_auto_accepting_connection_requests"), &IEOSGMultiplayerPeer::is_auto_accepting_connection_requests);
    ClassDB::bind_method(D_METHOD("set_auto_accept_connection_requests"), &IEOSGMultiplayerPeer::set_auto_accept_connection_requests);
    ClassDB::bind_method(D_METHOD("has_socket"), &IEOSGMultiplayerPeer::has_socket);
    ClassDB::bind_method(D_METHOD("find_all_connecetion_requests_for_user"), &IEOSGMultiplayerPeer::find_all_connecetion_requests_for_user);
    ClassDB::bind_method(D_METHOD("find_all_connecetion_requests_for_socket"), &IEOSGMultiplayerPeer::find_all_connecetion_requests_for_socket);
    ClassDB::bind_method(D_METHOD("get_all_connection_requests"), &IEOSGMultiplayerPeer::get_all_connection_requests);
    ClassDB::bind_method(D_METHOD("has_peer"), &IEOSGMultiplayerPeer::has_peer);
    ClassDB::bind_method(D_METHOD("accept_connection_request"), &IEOSGMultiplayerPeer::accept_connection_request);
    ClassDB::bind_method(D_METHOD("deny_connection_request"), &IEOSGMultiplayerPeer::deny_connection_request);
    ClassDB::bind_method(D_METHOD("accept_all_connection_requests"), &IEOSGMultiplayerPeer::accept_all_connection_requests);
    ClassDB::bind_method(D_METHOD("deny_all_connection_requests"), &IEOSGMultiplayerPeer::deny_all_connection_requests);
    ClassDB::bind_method(D_METHOD("get_active_mode"), &IEOSGMultiplayerPeer::get_active_mode);
    ClassDB::bind_method(D_METHOD("get_all_peers"), &IEOSGMultiplayerPeer::get_all_peers);

    ADD_SIGNAL(MethodInfo("p2p_peer_connection_established", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("p2p_peer_connection_interrupted", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("p2p_peer_connection_closed", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("p2p_incoming_connection_request", PropertyInfo(Variant::DICTIONARY, "callback_data")));
}

Error IEOSGMultiplayerPeer::create_server(const String &socket_id) {
    CHECK_IS_SINGLETON_INSTANCE;
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create server. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create server. Multiplayer instance is already active.");

    //Create a socket where we will be listening for connections
    EOS_P2P_SocketId socket;
    socket.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
    strncpy_s(socket.SocketName, socket_id.utf8(), socket_id.length());
    sockets.push_back(socket);

    unique_id = 1;
    active_mode = MODE_SERVER;
    connection_status = CONNECTION_CONNECTED;

    if(!_add_server_callbacks()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create server.");
    }

    return OK;
}

Error IEOSGMultiplayerPeer::create_client(const String &socket_id, const String &remote_user_id) {
    CHECK_IS_SINGLETON_INSTANCE;
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create client. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create client. Multiplayer instance is already active.");

    EOS_P2P_SocketId socket;
    socket.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
    strncpy_s(socket.SocketName, socket_id.utf8(), socket_id.length());
    sockets.push_back(socket);

    unique_id = generate_unique_id();
    active_mode = MODE_CLIENT;
    connection_status = CONNECTION_CONNECTING;

    //send connection request to server
    EOS_P2P_SendPacketOptions send_packet_options;
    send_packet_options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    send_packet_options.LocalUserId = s_local_user_id;
    send_packet_options.RemoteUserId = eosg_string_to_product_user_id(remote_user_id.utf8());
    send_packet_options.bAllowDelayedDelivery = EOS_TRUE;
    send_packet_options.bDisableAutoAcceptConnection = EOS_FALSE;
    send_packet_options.Channel = CH_RELIABLE;
    send_packet_options.Data = "";
    send_packet_options.DataLengthBytes = 1;
    send_packet_options.Reliability = EOS_EPacketReliability::EOS_PR_ReliableUnordered;
    send_packet_options.SocketId = &sockets[0];

    EOS_EResult result = IEOS::get_singleton()->p2p_send_packet(&send_packet_options);

    if(result != EOS_EResult::EOS_Success) {
        _close();
    }

    ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_InvalidParameters, ERR_CANT_CONNECT, "Failed to send connection request to server. Invalid parameters");
    ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_LimitExceeded, ERR_CANT_CONNECT, "Failed to send connection request to server. Packet is either too large or the packet queue is full.");
    ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_NoConnection, ERR_CANT_CONNECT, "Failed to send connection request to server. No connection");

    if (!_add_client_callbacks()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create client.");
    }

    return OK;
}

Error IEOSGMultiplayerPeer::create_mesh() {
    CHECK_IS_SINGLETON_INSTANCE;
    return OK;
}

Error IEOSGMultiplayerPeer::create_mesh_socket(const String &socket_id) {
    CHECK_IS_SINGLETON_INSTANCE;
    return OK;
}

Error IEOSGMultiplayerPeer::add_mesh_peer(int p_id, const String &socket_id) {
    CHECK_IS_SINGLETON_INSTANCE;
    return OK;
}

Array IEOSGMultiplayerPeer::get_all_connection_requests() {
    Array ret = Array();
    for (EOSGConnectionRequest &connection_request: pending_connection_requests) {
        Dictionary connection_request_data;
        connection_request_data["remote_user"] = eosg_product_user_id_to_string(connection_request.remote_user);
        String socket_name = connection_request.socket->SocketName;
        connection_request_data["socket"] = socket_name;
        ret.push_back(connection_request_data);
    }
    return ret;
}

Array IEOSGMultiplayerPeer::find_all_connecetion_requests_for_user(const String &user_id) {
    Array ret = Array();
    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(connection_request.remote_user);
        if (remote_user_id_str == user_id) {
            Dictionary connection_request_data;
            connection_request_data["remote_user"] = remote_user_id_str;
            String socket_name = connection_request.socket->SocketName;
            connection_request_data["socket"] = socket_name;
            ret.push_back(connection_request_data);
        }
    }
    return ret;
}

Array IEOSGMultiplayerPeer::find_all_connecetion_requests_for_socket(const String &socket_id) {
    Array ret = Array();
    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        String socket_name = connection_request.socket->SocketName;
        if (socket_id == socket_name) {
            Dictionary connection_request_data;
            connection_request_data["remote_user"] = eosg_product_user_id_to_string(connection_request.remote_user);
            connection_request_data["socket"] = socket_name;
            ret.push_back(connection_request_data);
        }
    }
    return ret;
}

Dictionary IEOSGMultiplayerPeer::get_peer_info(int p_id) {
    Dictionary ret = Dictionary();
    if (!peers.has(p_id)) {
        return ret;
    }

    EOSGPeerInfo peer_info = peers.get(p_id);
    ret["user_id"] = eosg_product_user_id_to_string(peer_info.user_id);

    Array socket_names;
    for (const EOS_P2P_SocketId *sockets : peer_info.sockets) {
        socket_names.push_back(sockets->SocketName);
    }
    ret["sockets"] = socket_names;

    return ret;
}

int IEOSGMultiplayerPeer::find_peer_id(const String &user_id) {
    for (KeyValue<uint32_t, EOSGPeerInfo> &E : peers) {
        EOSGPeerInfo peer_info = E.value;
        String user_id_str = eosg_product_user_id_to_string(peer_info.user_id);
        if (user_id == user_id_str) {
            return E.key;
        }
    }
    return 0;
}

bool IEOSGMultiplayerPeer::has_peer(const String &user_id) {
    if (find_peer_id(user_id) != 0) {
        return true;
    }
    return false;
}

Array IEOSGMultiplayerPeer::get_all_sockets() {
    Array ret;
    for (EOS_P2P_SocketId &socket : sockets) {
        String socket_name = socket.SocketName;
        ret.push_back(socket_name);
    }
    return ret;
}

Dictionary IEOSGMultiplayerPeer::get_all_peers() {
    Dictionary ret;
    for (KeyValue<uint32_t, EOSGPeerInfo> &E : peers) {
        EOSGPeerInfo peer_info = E.value;
        Dictionary data;
        Array sockets;
        data["user_id"] = eosg_product_user_id_to_string(peer_info.user_id);
        for (const EOS_P2P_SocketId* socket_id : peer_info.sockets) {
            String socket_name = socket_id->SocketName;
            sockets.push_back(socket_name);
        }
        data["sockets"] = sockets;
        ret[E.key] = data;
    }
    return ret;
}

void IEOSGMultiplayerPeer::set_allow_delayed_delivery(bool allow) {
    allow_delayed_delivery = allow;
}

bool IEOSGMultiplayerPeer::is_allowing_delayed_delivery() {
    return allow_delayed_delivery;
}

void IEOSGMultiplayerPeer::set_auto_accept_connection_requests(bool enable) {
    auto_accept_connection_requests = enable;
}

bool IEOSGMultiplayerPeer::is_auto_accepting_connection_requests() {
    return auto_accept_connection_requests;
}

bool IEOSGMultiplayerPeer::has_socket(const String &find_socket) {
    for (EOS_P2P_SocketId socket : sockets) {
        String socket_name = socket.SocketName;
        if (socket_name == find_socket) {
            return true;
        }
    }
    return false;
}

void IEOSGMultiplayerPeer::accept_connection_request(const String &remote_user, const String &socket_id) {
    ERR_FAIL_COND_MSG(active_mode == MODE_NONE, "Cannot accept connection requests when multiplayer instance is not active.");
    ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Clients are not allowed to accept connection requests.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot accept connection requests. Local user id has not been set.");

    EOSGConnectionRequest connection_request;
    if (!_find_connection_request(remote_user, socket_id, connection_request)) return;

    EOS_P2P_AcceptConnectionOptions options;
    options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = connection_request.remote_user;
    options.SocketId = connection_request.socket;
    EOS_EResult result = IEOS::get_singleton()->p2p_accept_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to accept connection request! Invalid parameters.");
}

void IEOSGMultiplayerPeer::deny_connection_request(const String &remote_user, const String &socket_id) {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Failed to deny connection request. Local user id not set");
    if (active_mode == MODE_NONE || active_mode == MODE_CLIENT) return;
    EOSGConnectionRequest connection_request;
    if(!_find_connection_request(remote_user, socket_id, connection_request)) return;

    EOS_P2P_CloseConnectionOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = connection_request.remote_user;
    options.SocketId = connection_request.socket;
    EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to deny connection. Invalid parameters.");

    pending_connection_requests.erase(connection_request);
}

void IEOSGMultiplayerPeer::accept_all_connection_requests() {
    if (pending_connection_requests.size() == 0) return;

    ERR_FAIL_COND_MSG(active_mode == MODE_NONE, "Cannot accept connection requests when multiplayer instance is not active.");
    ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Clients are not allowed to accept connection requests.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot accept connection requests. Local user id has not been set.");

    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        EOS_P2P_AcceptConnectionOptions options;
        options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
        options.LocalUserId = s_local_user_id;
        options.RemoteUserId = connection_request.remote_user;
        options.SocketId = connection_request.socket;
        EOS_EResult result = IEOS::get_singleton()->p2p_accept_connection(&options);

        ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to accept connection! Invalid parameters.");
    }
}

void IEOSGMultiplayerPeer::deny_all_connection_requests() {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Failed to deny connection requests. Local user id not set");
    if (active_mode == MODE_NONE || active_mode == MODE_CLIENT) return;

    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        EOS_P2P_CloseConnectionOptions options;
        options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
        options.LocalUserId = s_local_user_id;
        options.RemoteUserId = connection_request.remote_user;
        options.SocketId = connection_request.socket;
        EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);

        ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to deny connection. Invalid parameters.");
    }

    pending_connection_requests.clear();
}

int IEOSGMultiplayerPeer::get_active_mode() {
    return static_cast<int>(active_mode);
}

Error IEOSGMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) {
    CHECK_IS_SINGLETON_INSTANCE;
    ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

    current_packet = incoming_packets.front()->get();
    incoming_packets.pop_front();

    *r_buffer = current_packet.get_payload();
    *r_buffer_size = current_packet.payload_size();

    return OK;
}

Error IEOSGMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) {
    CHECK_IS_SINGLETON_INSTANCE;
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(!_is_active(), ERR_UNCONFIGURED, "The multiplayer instance isn't currently active.");
    ERR_FAIL_COND_V_MSG(connection_status != CONNECTION_CONNECTED, ERR_UNCONFIGURED, "The multiplayer instance isn't currently connected");
    ERR_FAIL_COND_V_MSG(target_peer != 0 && !peers.has(ABS(target_peer)), ERR_INVALID_PARAMETER, vformat("Invalid target peer: %d", target_peer));
	ERR_FAIL_COND_V(active_mode == MODE_CLIENT && !peers.has(1), ERR_BUG);

    uint8_t channel;
    uint8_t tr_channel = _get_transfer_channel();
    channel = CH_RELIABLE;
    EOS_EPacketReliability reliability;

    //todo:: Research more into this. The transfer modes don't match because packet reliability is a bit different in EOS. Whats the best way to approach this?...
    switch (_get_transfer_mode())
    {
        case TRANSFER_MODE_UNRELIABLE: {
            reliability = EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
            channel = CH_UNRELIABLE;
        } break;
        case TRANSFER_MODE_UNRELIABLE_ORDERED: {
            //EOS Doesn't have a EOS_PR_UnreliableOrdered.
            reliability = EOS_EPacketReliability::EOS_PR_ReliableOrdered;
            channel = CH_ORDERED;
        } break;
        case TRANSFER_MODE_RELIABLE: {
            reliability = EOS_EPacketReliability::EOS_PR_ReliableUnordered;
            channel = CH_RELIABLE;
        } break;
    }
    if (tr_channel > 0) {
		channel = CH_MAX + tr_channel - 1;
	}

    EOSGPacket packet;
    packet.set_sender(unique_id);
    packet.set_reliability(reliability);
    packet.set_channel(channel);
    packet.set_event(EVENT_STORE_PACKET);
    packet.store_payload(p_buffer, p_buffer_size);
    packet.prepare();

    Error result;
    if (_is_server())
    {
        if (target_peer == 0) {
            result = _broadcast(packet);
        } else if (target_peer < 0) {
            int exclude = ABS(target_peer);
            result = _broadcast(packet, exclude);
        } else {
            result = _send_to(peers[target_peer].user_id, packet, &sockets[0]);
        }
    } else if (active_mode == MODE_CLIENT) {
        ERR_FAIL_COND_V(peers.has(1), ERR_BUG);
        result = _send_to(peers[1].user_id, packet, &sockets[0]); //send to the server
    } else { //TODO:: Logic for mesh

    }

    return result;
}

int32_t IEOSGMultiplayerPeer::_get_available_packet_count() const {
    return incoming_packets.size();
}

int32_t IEOSGMultiplayerPeer::_get_max_packet_size() const {
    return EOS_P2P_MAX_PACKET_SIZE;
}

PackedByteArray IEOSGMultiplayerPeer::_get_packet_script() {
    return PackedByteArray();
}

Error IEOSGMultiplayerPeer::_put_packet_script(const PackedByteArray &p_buffer) {
    return Error::OK;
}

int32_t IEOSGMultiplayerPeer::_get_packet_channel() const {
    EOSGPacket packet = incoming_packets.front()->get();
    return packet.get_channel();
}

MultiplayerPeer::TransferMode IEOSGMultiplayerPeer::_get_packet_mode() const {
    EOSGPacket packet = incoming_packets.front()->get();
    return _convert_eos_reliability_to_transfer_mode(packet.get_reliability());
}

void IEOSGMultiplayerPeer::_set_transfer_channel(int32_t p_channel) {
    transfer_channel = p_channel;
}

int32_t IEOSGMultiplayerPeer::_get_transfer_channel() const {
    return transfer_channel;
}

void IEOSGMultiplayerPeer::_set_transfer_mode(MultiplayerPeer::TransferMode p_mode) {
    transfer_mode = p_mode;
}

MultiplayerPeer::TransferMode IEOSGMultiplayerPeer::_get_transfer_mode() const {
    return transfer_mode;
}

void IEOSGMultiplayerPeer::_set_target_peer(int32_t p_peer) {
    target_peer = p_peer;
}

int32_t IEOSGMultiplayerPeer::_get_packet_peer() const {
    EOSGPacket packet = incoming_packets.front()->get();
    return packet.get_sender();
}

bool IEOSGMultiplayerPeer::_is_server() const {
    return active_mode == MODE_SERVER;
}

void IEOSGMultiplayerPeer::_poll() {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Local user id is not set");
    ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");

    EOS_P2P_GetNextReceivedPacketSizeOptions packet_size_options;
    packet_size_options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
    packet_size_options.LocalUserId = s_local_user_id;
    packet_size_options.RequestedChannel = nullptr;
    uint32_t max_packet_size;
    EOS_EResult result = IEOS::get_singleton()->p2p_get_next_packet_size(&packet_size_options, &max_packet_size);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to get packet size! Invalid parameters.");

    if (result == EOS_EResult::EOS_NotFound) { //No packets found
        return;
    }

    EOS_P2P_ReceivePacketOptions options;
    options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.MaxDataSizeBytes = 4096; //TODO: Turn this number into a constant.
    options.RequestedChannel = nullptr;

    std::vector<uint8_t> packet_data;
    packet_data.resize(max_packet_size);
    uint32_t buffer_size;
    uint8_t channel;
    EOS_P2P_SocketId socket;
    EOS_ProductUserId remote_user;
    result = IEOS::get_singleton()->p2p_receive_packet(&options, packet_data.data(), &buffer_size, &channel, &remote_user, &socket);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to get packet! Invalid parameters.");
    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_NotFound, "Failed to get packet! Packet not found.");

    Event event = static_cast<Event>(packet_data.data()[INDEX_EVENT_TYPE]);
    switch (event) {
        case Event::EVENT_STORE_PACKET: {
            uint32_t peer_id = *reinterpret_cast<uint32_t*>(packet_data.data() + INDEX_PEER_ID);
            if (!peers.has(peer_id)) {
                return; //ignore the packet
            }

            EOS_EPacketReliability reliability = static_cast<EOS_EPacketReliability>(packet_data.data()[INDEX_TRANSFER_MODE]);

            EOSGPacket packet;
            packet.store_payload(packet_data.data() + INDEX_PAYLOAD_DATA, buffer_size - EOSGPacket::PACKET_HEADER_SIZE);
            packet.set_event(event);
            packet.set_sender(peer_id);
            packet.set_reliability(reliability);
            packet.set_channel(channel);

            incoming_packets.push_back(packet);
            break;
        }
        case Event::EVENT_RECIEVE_PEER_ID: {
            ERR_FAIL_COND_MSG(!_is_server() && active_mode != MODE_MESH, "A client has recieved an EVENT_RECIEVE_PEER_ID packet. This should not have happened!");
            const EOS_P2P_SocketId *found_socket = _get_socket(socket.SocketName);
            ERR_FAIL_NULL_MSG(found_socket, "Failed to recieve peer id. Socket that client was attempting to connect to was not found.");
            uint32_t peer_id = *reinterpret_cast<uint32_t*>(packet_data.data() + INDEX_PEER_ID);

            if (active_mode == MODE_MESH && peers.has(peer_id)) {
                peers[peer_id].sockets.push_back(found_socket);
            } else {
                ERR_FAIL_COND_MSG(peers.has(peer_id), "Did not add peer. Peer was already added.");
                EOSGPeerInfo peer_info;
                peer_info.user_id = remote_user;
                peer_info.sockets.push_back(found_socket);
                peers.insert(peer_id, peer_info);
            }
            emit_signal("peer_connected", peer_id);
            break;
        }
        default:
            ERR_FAIL_MSG("Invalid packet event.");
            break;
    }
}

void IEOSGMultiplayerPeer::_close() {
    if (!_is_active()) {
		return;
	}

    EOS_P2P_CloseConnectionsOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTIONS_API_LATEST;
    options.LocalUserId = s_local_user_id;

    for (EOS_P2P_SocketId &socket : sockets) {
        options.SocketId = &socket;
        EOS_EResult result = IEOS::get_singleton()->p2p_close_all_connections(&options);
        ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to close connections. Invalid parameters.");
    }

    active_mode = MODE_NONE;
    connection_status = CONNECTION_DISCONNECTED;
    sockets.clear();
    peers.clear();
    incoming_packets.clear();
    unique_id = 0;
    _remove_all_callbacks();
    set_refuse_new_connections(false);
}

void IEOSGMultiplayerPeer::_disconnect_peer(int32_t p_peer, bool p_force) {
    ERR_FAIL_COND(!_is_active() || !peers.has(p_peer));
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot close connection. Local user id is not set");

    EOSGPeerInfo peer = peers.get(p_peer);
    for (const EOS_P2P_SocketId *socket : peer.sockets) {
        _disconnect_remote_user(peer.user_id, *socket);
    }

    if (p_force && peers.has(p_peer)) {
        peers.erase(p_peer);
        emit_signal("peer_disconnected", p_peer);
    }
}

int32_t IEOSGMultiplayerPeer::_get_unique_id() const {
    return unique_id;
}

void IEOSGMultiplayerPeer::_set_refuse_new_connections(bool p_enable) {
    refusing_connections = p_enable;
}

bool IEOSGMultiplayerPeer::_is_refusing_new_connections() const {
    return refusing_connections;
}

bool IEOSGMultiplayerPeer::_is_server_relay_supported() const {
    return active_mode == MODE_SERVER || active_mode == MODE_CLIENT;
}

MultiplayerPeer::ConnectionStatus IEOSGMultiplayerPeer::_get_connection_status() const {
    return connection_status;
}

void IEOSGMultiplayerPeer::set_local_user_id(const String& p_local_user_id) { 
    CharString local_user_id = p_local_user_id.utf8();
    s_local_user_id = eosg_string_to_product_user_id(local_user_id);
}

String IEOSGMultiplayerPeer::get_local_user_id() {
    String local_user_id = eosg_product_user_id_to_string(s_local_user_id);
    return local_user_id;
}

Error IEOSGMultiplayerPeer::_broadcast(const EOSGPacket &packet, int exclude) {
    CHECK_IS_SINGLETON_INSTANCE;
    EOS_P2P_SendPacketOptions options;
    options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.SocketId = &sockets[0];
    options.Channel = packet.get_channel();
    options.DataLengthBytes = packet.packet_size();
    options.Data = packet.get_packet();
    options.bAllowDelayedDelivery = allow_delayed_delivery;
    options.Reliability = packet.get_reliability();
    options.bDisableAutoAcceptConnection = EOS_FALSE;

    for (KeyValue<uint32_t, EOSGPeerInfo> &E : peers) {
        if (E.key == exclude) {
            continue;
        }

        EOSGPeerInfo peer = E.value;

        options.RemoteUserId = peer.user_id;
        EOS_EResult result = IEOS::get_singleton()->p2p_send_packet(&options);

        ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_InvalidParameters, ERR_INVALID_PARAMETER, "Failed to send packet! Invalid parameters.");
        ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_LimitExceeded, FAILED, "Failed to send packet! Packet is either too large or outgoing packet queue is full.");
        ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_NoConnection, ERR_CANT_CONNECT, "Failed to send packet! No connection.");
    }

    return OK;
}

Error IEOSGMultiplayerPeer::_send_to(const EOS_ProductUserId &remote_peer, const EOSGPacket &packet, const EOS_P2P_SocketId *socket) {
    CHECK_IS_SINGLETON_INSTANCE;
    EOS_P2P_SendPacketOptions options;
    options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_peer;
    options.SocketId = socket;
    options.Channel = packet.get_channel();
    options.DataLengthBytes = packet.packet_size();
    options.Data = packet.get_packet();
    options.bAllowDelayedDelivery = allow_delayed_delivery;
    options.Reliability = packet.get_reliability();
    options.bDisableAutoAcceptConnection = EOS_FALSE;

    EOS_EResult result = IEOS::get_singleton()->p2p_send_packet(&options);

    ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_InvalidParameters, ERR_INVALID_PARAMETER, "Failed to send packet! Invalid parameters.");
    ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_LimitExceeded, FAILED, "Failed to send packet! Packet is either too large or outgoing packet queue is full.");
    ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_NoConnection, ERR_CANT_CONNECT, "Failed to send packet! No connection.");

    return OK;
}

bool IEOSGMultiplayerPeer::_find_connection_request(const String &remote_user, const String &socket_id, EOSGConnectionRequest &out_request) {
    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(connection_request.remote_user);
        String socket_name = connection_request.socket->SocketName;
        if (remote_user == remote_user_id_str && socket_id == socket_name) {
            out_request = connection_request;
            return true;
        }
    }
    return false;
}

void IEOSGMultiplayerPeer::_remove_all_callbacks() {
    if (connection_established_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_established(connection_established_callback_id);
        connection_established_callback_id = EOS_INVALID_NOTIFICATIONID;
    }

    if (connection_interrupted_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_interrupted(connection_interrupted_callback_id);
        connection_interrupted_callback_id = EOS_INVALID_NOTIFICATIONID;
    }

    if (connection_closed_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_closed(connection_closed_callback_id);
        connection_closed_callback_id = EOS_INVALID_NOTIFICATIONID;
    }

    if (incoming_connection_request_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_request(incoming_connection_request_callback_id);
        incoming_connection_request_callback_id = EOS_INVALID_NOTIFICATIONID;
    }
}

bool IEOSGMultiplayerPeer::_add_server_callbacks() {
    ERR_FAIL_COND_V_MSG(sockets.size() != 1, false, "Failed to add server callbacks. There must be exactly one socket.");

    EOS_P2P_AddNotifyPeerConnectionEstablishedOptions connection_established_options;
    connection_established_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST;
    connection_established_options.LocalUserId = s_local_user_id;
    connection_established_options.SocketId = &sockets[0];
    connection_established_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_established(&connection_established_options, 
        IEOSGMultiplayerPeer::_on_peer_connection_established);
    ERR_FAIL_COND_V_MSG(connection_established_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection established callback.");

    EOS_P2P_AddNotifyPeerConnectionInterruptedOptions connection_interrupted_options;
    connection_interrupted_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST;
    connection_interrupted_options.LocalUserId = s_local_user_id;
    connection_interrupted_options.SocketId = &sockets[0];
    connection_interrupted_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_interrupted(&connection_interrupted_options,
        IEOSGMultiplayerPeer::_on_peer_connection_interrupted);
    ERR_FAIL_COND_V_MSG(connection_interrupted_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection interrupted callback.");

    EOS_P2P_AddNotifyPeerConnectionClosedOptions connection_closed_options;
    connection_closed_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
    connection_closed_options.LocalUserId = s_local_user_id;
    connection_closed_options.SocketId = &sockets[0];
    connection_closed_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_closed(&connection_closed_options,
        IEOSGMultiplayerPeer::_on_remote_connection_closed);
    ERR_FAIL_COND_V_MSG(connection_closed_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection closed callback.");
    
    EOS_P2P_AddNotifyPeerConnectionRequestOptions connection_request_options;
    connection_request_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
    connection_request_options.LocalUserId = s_local_user_id;
    connection_request_options.SocketId = &sockets[0];
    incoming_connection_request_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_request(&connection_request_options,
        IEOSGMultiplayerPeer::_on_incoming_connection_request);
    ERR_FAIL_COND_V_MSG(incoming_connection_request_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection request callback.");

    return true;
}

bool IEOSGMultiplayerPeer::_add_client_callbacks() {
    ERR_FAIL_COND_V_MSG(sockets.size() != 1, false, "Failed to add client callbacks. There must be exactly one socket.");

    EOS_P2P_AddNotifyPeerConnectionEstablishedOptions connection_established_options;
    connection_established_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST;
    connection_established_options.LocalUserId = s_local_user_id;
    connection_established_options.SocketId = &sockets[0];
    connection_established_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_established(&connection_established_options, 
        IEOSGMultiplayerPeer::_on_peer_connection_established);
    ERR_FAIL_COND_V_MSG(connection_established_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection established callback.");

    EOS_P2P_AddNotifyPeerConnectionInterruptedOptions connection_interrupted_options;
    connection_interrupted_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST;
    connection_interrupted_options.LocalUserId = s_local_user_id;
    connection_interrupted_options.SocketId = &sockets[0];
    connection_interrupted_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_interrupted(&connection_interrupted_options,
        IEOSGMultiplayerPeer::_on_peer_connection_interrupted);
    ERR_FAIL_COND_V_MSG(connection_interrupted_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection interrupted callback.");

    EOS_P2P_AddNotifyPeerConnectionClosedOptions connection_closed_options;
    connection_closed_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
    connection_closed_options.LocalUserId = s_local_user_id;
    connection_closed_options.SocketId = &sockets[0];
    connection_closed_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_closed(&connection_closed_options,
        IEOSGMultiplayerPeer::_on_remote_connection_closed);
    ERR_FAIL_COND_V_MSG(connection_closed_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection closed callback.");

    return true;
}

EOS_EPacketReliability IEOSGMultiplayerPeer::_convert_transfer_mode_to_eos_reliability(TransferMode mode) const {
    switch (mode) {
        case TRANSFER_MODE_UNRELIABLE:
            return EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
        case TRANSFER_MODE_UNRELIABLE_ORDERED:
            return EOS_EPacketReliability::EOS_PR_ReliableOrdered;
        case TRANSFER_MODE_RELIABLE:
            return EOS_EPacketReliability::EOS_PR_ReliableUnordered;
        default:
            return EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
    }
}

MultiplayerPeer::TransferMode IEOSGMultiplayerPeer::_convert_eos_reliability_to_transfer_mode(EOS_EPacketReliability reliability) const {
    switch (reliability) {
        case EOS_EPacketReliability::EOS_PR_UnreliableUnordered:
            return TRANSFER_MODE_UNRELIABLE;
        case EOS_EPacketReliability::EOS_PR_ReliableUnordered:
            return TRANSFER_MODE_RELIABLE;
        case EOS_EPacketReliability::EOS_PR_ReliableOrdered:
            return TRANSFER_MODE_UNRELIABLE_ORDERED;
        default:
            return TRANSFER_MODE_UNRELIABLE;
    }
}

const EOS_P2P_SocketId* IEOSGMultiplayerPeer::_get_socket(const String &socket_name) const {
    List<EOS_P2P_SocketId>::ConstIterator it = sockets.begin();
    for (; it != sockets.end(); ++it) {
        if (it->SocketName == socket_name) {
            return &(*it);
        }
    }
    return nullptr;
}

void IEOSGMultiplayerPeer::_disconnect_remote_user(const EOS_ProductUserId &remote_user, const EOS_P2P_SocketId &socket) {
    EOS_P2P_CloseConnectionOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_user;
    options.SocketId = &socket;
    EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);
    
    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to close peer connection. Invalid parameters.");
}

void EOS_CALL IEOSGMultiplayerPeer::_on_peer_connection_established(const EOS_P2P_OnPeerConnectionEstablishedInfo *data) {
    if (singleton->active_mode == MODE_CLIENT && data->ConnectionType == EOS_EConnectionEstablishedType::EOS_CET_NewConnection) {
        EOSGPeerInfo server_peer;
        server_peer.user_id = data->RemoteUserId;
        const EOS_P2P_SocketId *found_socket = singleton->_get_socket(data->SocketId->SocketName);

        if (found_socket == nullptr) {
            singleton->_close();
            ERR_FAIL_MSG("Failed to add server as peer. Socket was not found. This shouldn't have happened.");
        }

        server_peer.sockets.push_back(found_socket);
        singleton->peers.insert(1, server_peer);

        EOSGPacket packet;
        packet.set_event(EVENT_RECIEVE_PEER_ID);
        packet.set_channel(CH_RELIABLE);
        packet.set_reliability(EOS_EPacketReliability::EOS_PR_ReliableUnordered);
        packet.set_sender(singleton->unique_id);
        packet.prepare();

        //send peer id to the server
        Error result = singleton->_send_to(data->RemoteUserId, packet, data->SocketId);

        if(result != OK) {
            singleton->_close();
            return;
        }
        get_singleton()->connection_status = CONNECTION_CONNECTED;
    }

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    String socket_name = data->SocketId->SocketName;
    int connection_type = static_cast<int>(data->ConnectionType);
    int network_type = static_cast<int>(data->NetworkType);

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    ret["connection_type"] = connection_type;
    ret["network_type"] = network_type;
    singleton->emit_signal("p2p_peer_connection_established", ret);
}

void EOS_CALL IEOSGMultiplayerPeer::_on_peer_connection_interrupted(const EOS_P2P_OnPeerConnectionInterruptedInfo *data) {
    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    String socket_name = data->SocketId->SocketName;
    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    singleton->emit_signal("p2p_peer_connection_interrupted", ret);
}

void EOS_CALL IEOSGMultiplayerPeer::_on_incoming_connection_request(const EOS_P2P_OnIncomingConnectionRequestInfo *data) {
    ERR_FAIL_NULL_MSG(singleton, "Failed. EOSGMultiplayerPeer singleton is null.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Local user id has not been set.");

    if (singleton->active_mode == MODE_CLIENT || singleton->is_refusing_new_connections()) return;
    String socket_name = data->SocketId->SocketName;
    if (!singleton->has_socket(socket_name)) return; //Ignore the connection request unless it's a socket we're actively listening for.

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);

    EOSGConnectionRequest connection_request;
    connection_request.remote_user = data->RemoteUserId;
    connection_request.socket = data->SocketId;
    singleton->pending_connection_requests.push_back(connection_request);

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    singleton->emit_signal("p2p_incoming_connection_request", ret);

    if(!singleton->is_auto_accepting_connection_requests()) return;

    singleton->accept_connection_request(remote_user_id_str, socket_name);
}

void EOS_CALL IEOSGMultiplayerPeer::_on_remote_connection_closed(const EOS_P2P_OnRemoteConnectionClosedInfo *data) {
    ERR_FAIL_NULL_MSG(singleton, "Failed. EOSGMultiplayerPeer singleton is null.");

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    String socket_name = data->SocketId->SocketName;
    int reason = static_cast<int>(data->Reason);

    //attempt to remove connection request if there was one.
    EOSGConnectionRequest connection_request;
    if (singleton->_find_connection_request(remote_user_id_str, socket_name, connection_request)) {
            singleton->pending_connection_requests.erase(connection_request);
    }

    //Attempt to remove peer;
    int peer_id = singleton->find_peer_id(remote_user_id_str);
    if (peer_id != 0) {
        singleton->peers.erase(peer_id);
        singleton->emit_signal("peer_disconnected", peer_id);
    }

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    ret["reason"] = reason;
    singleton->emit_signal("p2p_peer_connection_closed", ret);
}

IEOSGMultiplayerPeer::IEOSGMultiplayerPeer() {
    ERR_FAIL_COND_MSG(singleton != nullptr, "Multiple instances of EOSGMultiplayerPeer is not allowed. Use EOSGMultiplayerPeer.get_singleton() instead.");
    singleton = this;
    is_singleton = true;
}
IEOSGMultiplayerPeer::~IEOSGMultiplayerPeer() {
    if (singleton != this) {
        return;
    }

    singleton = nullptr;
}

void IEOSGMultiplayerPeer::EOSGPacket::prepare() {
    packet.get()->data()[INDEX_EVENT_TYPE] = static_cast<uint8_t>(event);
    packet.get()->data()[INDEX_TRANSFER_MODE] = static_cast<uint8_t>(reliability);
    memcpy(packet.get()->data() + INDEX_PEER_ID, &from, sizeof(int));
}

void IEOSGMultiplayerPeer::EOSGPacket::store_payload(const uint8_t *payload_data, const uint32_t payload_size_bytes) {
        size_bytes = payload_size_bytes + PACKET_HEADER_SIZE;
        packet.get()->clear();
        packet.get()->resize(size_bytes);
        memcpy(packet.get()->data() + INDEX_PAYLOAD_DATA, payload_data, payload_size_bytes);
}
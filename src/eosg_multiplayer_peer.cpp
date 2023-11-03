#include "ieos.h"
#include "eosg_multiplayer_peer.h"
#include "eosg_packet_peer_mediator.h"

using namespace godot;

EOS_ProductUserId EOSGMultiplayerPeer::s_local_user_id = nullptr;
HashMap<String, EOSGMultiplayerPeer*> EOSGMultiplayerPeer::active_peers = HashMap<String, EOSGMultiplayerPeer*>();

void EOSGMultiplayerPeer::_bind_methods() {
    ClassDB::bind_static_method("EOSGMultiplayerPeer", D_METHOD("set_local_user_id"), &EOSGMultiplayerPeer::set_local_user_id);
    ClassDB::bind_static_method("EOSGMultiplayerPeer", D_METHOD("get_local_user_id"), &EOSGMultiplayerPeer::get_local_user_id);

    ClassDB::bind_method(D_METHOD("create_server", "socket_id"), &EOSGMultiplayerPeer::create_server);
    ClassDB::bind_method(D_METHOD("create_client", "socket_id", "remote_user_id"), &EOSGMultiplayerPeer::create_client);
    ClassDB::bind_method(D_METHOD("create_mesh", "socket_ids"), &EOSGMultiplayerPeer::create_mesh, PackedStringArray());
    ClassDB::bind_method(D_METHOD("add_mesh_peer", "remote_user_id", "socket_id"), &EOSGMultiplayerPeer::add_mesh_peer);
    ClassDB::bind_method(D_METHOD("get_peer_id", "remote_user_id"), &EOSGMultiplayerPeer::get_peer_id);
    ClassDB::bind_method(D_METHOD("set_allow_delayed_delivery", "allow"), &EOSGMultiplayerPeer::set_allow_delayed_delivery);
    ClassDB::bind_method(D_METHOD("is_allowing_delayed_delivery"), &EOSGMultiplayerPeer::is_allowing_delayed_delivery);
    ClassDB::bind_method(D_METHOD("is_auto_accepting_connection_requests"), &EOSGMultiplayerPeer::is_auto_accepting_connection_requests);
    ClassDB::bind_method(D_METHOD("set_auto_accept_connection_requests", "enable"), &EOSGMultiplayerPeer::set_auto_accept_connection_requests);
    ClassDB::bind_method(D_METHOD("get_all_connecetion_requests_for_user", "remote_user_id"), &EOSGMultiplayerPeer::get_all_connecetion_requests_for_user);
    ClassDB::bind_method(D_METHOD("get_all_connection_requests"), &EOSGMultiplayerPeer::get_all_connection_requests);
    ClassDB::bind_method(D_METHOD("has_peer", "peer_id"), &EOSGMultiplayerPeer::has_peer);
    ClassDB::bind_method(D_METHOD("has_peer", "remote_user_id"), &EOSGMultiplayerPeer::has_peer);
    ClassDB::bind_method(D_METHOD("accept_connection_request", "remote_user_id", "socket_id"), &EOSGMultiplayerPeer::accept_connection_request);
    ClassDB::bind_method(D_METHOD("deny_connection_request", "remote_user_id", "socket_id"), &EOSGMultiplayerPeer::deny_connection_request);
    ClassDB::bind_method(D_METHOD("accept_all_connection_requests"), &EOSGMultiplayerPeer::accept_all_connection_requests);
    ClassDB::bind_method(D_METHOD("deny_all_connection_requests"), &EOSGMultiplayerPeer::deny_all_connection_requests);
    ClassDB::bind_method(D_METHOD("get_active_mode"), &EOSGMultiplayerPeer::get_active_mode);
    ClassDB::bind_method(D_METHOD("get_all_peers"), &EOSGMultiplayerPeer::get_all_peers);

    ADD_SIGNAL(MethodInfo("p2p_peer_connection_established", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("p2p_peer_connection_interrupted", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("p2p_peer_connection_closed", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("p2p_incoming_connection_request", PropertyInfo(Variant::DICTIONARY, "callback_data")));
}

Error EOSGMultiplayerPeer::create_server(const String &socket_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create server. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create server. Multiplayer instance is already active.");

    //Create a socket where we will be listening for connections
    socket = EOSGSocket(socket_id);
    active_peers.insert(socket.get_socket_name(), this);
    EOSGPacketPeerMediator::get_singleton()->register_socket(socket.get_socket_name());

    unique_id = 1;
    active_mode = MODE_SERVER;
    connection_status = CONNECTION_CONNECTED;

    if(!_add_server_callbacks()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create server.");
    }

    return OK;
}

Error EOSGMultiplayerPeer::create_client(const String &socket_id, const String &remote_user_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create client. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create client. Multiplayer instance is already active.");

    //Create the socket we are trying to connect to
    socket = EOSGSocket(socket_id);
    active_peers.insert(socket.get_socket_name(), this);
    EOSGPacketPeerMediator::get_singleton()->register_socket(socket.get_socket_name());

    unique_id = generate_unique_id();
    active_mode = MODE_CLIENT;
    connection_status = CONNECTION_CONNECTING;

    //send connection request to server
    EOSGPacket packet;
    packet.set_event(EVENT_RECIEVE_PEER_ID);
    packet.set_channel(CH_RELIABLE);
    packet.set_reliability(EOS_EPacketReliability::EOS_PR_ReliableUnordered);
    packet.set_sender(unique_id);
    packet.prepare();

    //send peer id to the server
    Error result = _send_to(eosg_string_to_product_user_id(remote_user_id.utf8()), packet);

    if(result != OK) {
        _close();
        return result;
    }

    if (!_add_client_callbacks()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create client.");
    }

    return OK;
}

Error EOSGMultiplayerPeer::create_mesh(const String &socket_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create mesh. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create mesh. Multiplayer instance is already active.");

    //Create a socket where we will be listening for connections
    socket = EOSGSocket(socket_id);
    active_peers.insert(socket.get_socket_name(), this);

    unique_id = generate_unique_id();
    active_mode = MODE_MESH;
    connection_status = CONNECTION_CONNECTED;

    if(!_add_server_callbacks()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create server.");
    }

    return OK;
}

Error EOSGMultiplayerPeer::add_mesh_peer(const String &remote_user_id, const String &socket_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to add mesh socket. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(active_mode != MODE_MESH, ERR_UNCONFIGURED, "Failed to add mesh socket. Multiplayer instance is not in mesh mode.");
    ERR_FAIL_COND_V_MSG(has_user_id(remote_user_id), ERR_ALREADY_EXISTS, "Failed to add mesh peer. Already connected to peer");

    //send connection request to peer
    EOSGPacket packet;
    packet.set_event(EVENT_RECIEVE_PEER_ID);
    packet.set_channel(CH_RELIABLE);
    packet.set_reliability(EOS_EPacketReliability::EOS_PR_ReliableUnordered);
    packet.set_sender(unique_id);
    packet.prepare();

    _send_to(eosg_string_to_product_user_id(remote_user_id.utf8()), packet);

    return OK;
}

Array EOSGMultiplayerPeer::get_all_connection_requests() {
    Array ret = Array();
    for (EOSGConnectionRequest &connection_request: pending_connection_requests) {
        Dictionary connection_request_data;
        connection_request_data["remote_user_id"] = eosg_product_user_id_to_string(connection_request.remote_user);
        String socket_name = connection_request.socket.SocketName;
        connection_request_data["socket"] = socket_name;
        ret.push_back(connection_request_data);
    }
    return ret;
}

Array EOSGMultiplayerPeer::get_all_connecetion_requests_for_user(const String &user_id) {
    Array ret = Array();
    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(connection_request.remote_user);
        if (remote_user_id_str == user_id) {
            Dictionary connection_request_data;
            connection_request_data["remote_user_id"] = remote_user_id_str;
            String socket_name = connection_request.socket.SocketName;
            connection_request_data["socket"] = socket_name;
            ret.push_back(connection_request_data);
        }
    }
    return ret;
}

String EOSGMultiplayerPeer::get_peer_user_id(int p_id) {
    String ret = "";
    if (!peers.has(p_id)) {
        return ret;
    }
    EOS_ProductUserId user_id = peers.get(p_id);
    ret = eosg_product_user_id_to_string(user_id);
    return ret;
}

int EOSGMultiplayerPeer::get_peer_id(const String &remote_user_id) {
    for (KeyValue<uint32_t, EOS_ProductUserId> &E : peers) {
        EOS_ProductUserId user_id = E.value;
        String user_id_str = eosg_product_user_id_to_string(user_id);
        if (remote_user_id == user_id_str) {
            return E.key;
        }
    }
    return 0;
}

bool EOSGMultiplayerPeer::has_peer(int peer_id) {
    return peers.has(peer_id);
}

bool EOSGMultiplayerPeer::has_user_id(const String &remote_user_id) {
    for (KeyValue<uint32_t, EOS_ProductUserId> &E : peers) {
        String peer_user_id = eosg_product_user_id_to_string(E.value);
        if (remote_user_id != peer_user_id) continue;
        return true;
    }
    return false;
}

void EOSGMultiplayerPeer::_clear_peer_packet_queue(int p_id) {
    ERR_FAIL_COND_MSG(!peers.has(p_id), "Failed to clear packet queue for peer. Peer was not found.");

    socket.clear_packets_from_peer(p_id);
    String remote_user_id = eosg_product_user_id_to_string(peers[p_id]);
    EOSGPacketPeerMediator::get_singleton()->clear_packets_from_remote_user(socket.get_socket_name(), remote_user_id);
}

Dictionary EOSGMultiplayerPeer::get_all_peers() {
    Dictionary ret;
    for (KeyValue<uint32_t, EOS_ProductUserId> &E : peers) {
        EOS_ProductUserId user_id = E.value;
        ret[E.key] = eosg_product_user_id_to_string(user_id);
    }
    return ret;
}

void EOSGMultiplayerPeer::set_allow_delayed_delivery(bool allow) {
    allow_delayed_delivery = allow;
}

bool EOSGMultiplayerPeer::is_allowing_delayed_delivery() {
    return allow_delayed_delivery;
}

void EOSGMultiplayerPeer::set_auto_accept_connection_requests(bool enable) {
    auto_accept_connection_requests = enable;
}

bool EOSGMultiplayerPeer::is_auto_accepting_connection_requests() {
    return auto_accept_connection_requests;
}

void EOSGMultiplayerPeer::accept_connection_request(const String &remote_user, const String &socket_id) {
    ERR_FAIL_COND_MSG(active_mode == MODE_NONE, "Cannot accept connection requests when multiplayer instance is not active.");
    ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Clients are not allowed to accept connection requests.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot accept connection requests. Local user id has not been set.");

    EOSGConnectionRequest connection_request;
    if (!_find_connection_request(remote_user, socket_id, connection_request)) return;

    EOS_P2P_AcceptConnectionOptions options;
    options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = connection_request.remote_user;
    options.SocketId = &connection_request.socket;
    EOS_EResult result = IEOS::get_singleton()->p2p_accept_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to accept connection request! Invalid parameters.");

    pending_connection_requests.erase(connection_request);
}

void EOSGMultiplayerPeer::deny_connection_request(const String &remote_user, const String &socket_id) {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Failed to deny connection request. Local user id not set");
    if (active_mode == MODE_NONE || active_mode == MODE_CLIENT) return;
    EOSGConnectionRequest connection_request;
    if(!_find_connection_request(remote_user, socket_id, connection_request)) return;

    EOS_P2P_CloseConnectionOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = connection_request.remote_user;
    options.SocketId = &connection_request.socket;
    EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to deny connection. Invalid parameters.");

    pending_connection_requests.erase(connection_request);
}

void EOSGMultiplayerPeer::accept_all_connection_requests() {
    if (pending_connection_requests.size() == 0) return;

    ERR_FAIL_COND_MSG(active_mode == MODE_NONE, "Cannot accept connection requests when multiplayer instance is not active.");
    ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Clients are not allowed to accept connection requests.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot accept connection requests. Local user id has not been set.");

    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        EOS_P2P_AcceptConnectionOptions options;
        options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
        options.LocalUserId = s_local_user_id;
        options.RemoteUserId = connection_request.remote_user;
        options.SocketId = &connection_request.socket;
        EOS_EResult result = IEOS::get_singleton()->p2p_accept_connection(&options);

        ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to accept connection! Invalid parameters.");
    }
}

void EOSGMultiplayerPeer::deny_all_connection_requests() {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Failed to deny connection requests. Local user id not set");
    if (active_mode == MODE_NONE || active_mode == MODE_CLIENT) return;

    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        EOS_P2P_CloseConnectionOptions options;
        options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
        options.LocalUserId = s_local_user_id;
        options.RemoteUserId = connection_request.remote_user;
        options.SocketId = &connection_request.socket;
        EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);

        ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to deny connection. Invalid parameters.");
    }

    pending_connection_requests.clear();
}

int EOSGMultiplayerPeer::get_active_mode() {
    return static_cast<int>(active_mode);
}

Error EOSGMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) {
    current_packet = socket.pop_packet();

    *r_buffer = current_packet.get_payload();
    *r_buffer_size = current_packet.payload_size();

    return OK;
}

Error EOSGMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(!_is_active(), ERR_UNCONFIGURED, "The multiplayer instance isn't currently active.");
    ERR_FAIL_COND_V_MSG(connection_status != CONNECTION_CONNECTED, ERR_UNCONFIGURED, "The multiplayer instance isn't currently connected");
    ERR_FAIL_COND_V_MSG(target_peer != 0 && !peers.has(ABS(target_peer)), ERR_INVALID_PARAMETER, vformat("Invalid target peer: %d", target_peer));
    ERR_FAIL_COND_V_MSG(p_buffer_size > _get_max_packet_size(), ERR_UNAVAILABLE, "Failed to send packet. Packet size exceeds limits.");
	ERR_FAIL_COND_V(active_mode == MODE_CLIENT && !peers.has(1), ERR_BUG);

    uint8_t channel;
    uint8_t tr_channel = _get_transfer_channel();
    channel = CH_RELIABLE;
    EOS_EPacketReliability reliability = EOS_EPacketReliability::EOS_PR_UnreliableUnordered;

    /*IMPORTANT NOTE: EOS does not support a unreliable ordered transfer mode. 
    This is why there is no TRANSFER_MODE_UNRELIABLE_ORDERED listed here.
    EOSGMultiplayerPeer prints a warning and automatically sets the transfer
    mode to TRANSFER_MODE_RELIABLE if the user tries to set to unreliable ordred.
    See _set_transer_mode()*/
    switch (_get_transfer_mode())
    {
        case TRANSFER_MODE_UNRELIABLE: {
            reliability = EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
            channel = CH_UNRELIABLE;
        } break;
        case TRANSFER_MODE_RELIABLE: {
            reliability = EOS_EPacketReliability::EOS_PR_ReliableOrdered;
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
            result = _send_to(peers[target_peer], packet);
        }
    } else if (active_mode == MODE_CLIENT) {
        result = _send_to(peers[1], packet); //send to the server
    } else { //TODO:: Logic for mesh

    }
    return result;
}

int32_t EOSGMultiplayerPeer::_get_available_packet_count() const {
    return socket.get_packet_count();
}

int32_t EOSGMultiplayerPeer::_get_max_packet_size() const {
    return EOS_P2P_MAX_PACKET_SIZE;
}

PackedByteArray EOSGMultiplayerPeer::_get_packet_script() {
    return PackedByteArray();
}

Error EOSGMultiplayerPeer::_put_packet_script(const PackedByteArray &p_buffer) {
    return Error::OK;
}

int32_t EOSGMultiplayerPeer::_get_packet_channel() const {
    return socket.get_packet_channel();
}

MultiplayerPeer::TransferMode EOSGMultiplayerPeer::_get_packet_mode() const {
    return _convert_eos_reliability_to_transfer_mode(socket.get_packet_reliability());
}

void EOSGMultiplayerPeer::_set_transfer_channel(int32_t p_channel) {
    transfer_channel = p_channel;
}

int32_t EOSGMultiplayerPeer::_get_transfer_channel() const {
    return transfer_channel;
}

void EOSGMultiplayerPeer::_set_transfer_mode(MultiplayerPeer::TransferMode p_mode) {
    if (p_mode == TRANSFER_MODE_UNRELIABLE_ORDERED) {
        WARN_PRINT("EOS does not support unreliable ordered. Setting to reliable instead.");
        transfer_mode = TRANSFER_MODE_RELIABLE;
        return;
    }
    transfer_mode = p_mode;
}

MultiplayerPeer::TransferMode EOSGMultiplayerPeer::_get_transfer_mode() const {
    return transfer_mode;
}

void EOSGMultiplayerPeer::_set_target_peer(int32_t p_peer) {
    target_peer = p_peer;
}

int32_t EOSGMultiplayerPeer::_get_packet_peer() const {
    return socket.get_packet_peer();
}

bool EOSGMultiplayerPeer::_is_server() const {
    return active_mode == MODE_SERVER;
}

void EOSGMultiplayerPeer::_poll() {
    ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");

    PacketData packet_data;
    bool success = EOSGPacketPeerMediator::get_singleton()->poll_next_packet(socket.get_socket_name(), &packet_data);

    if (!success) return; //theres no avaialable packets to pull

    PackedByteArray *data_ptr = packet_data.get_packet();
    Event event = static_cast<Event>(data_ptr->ptrw()[INDEX_EVENT_TYPE]);
    switch (event) {
        case Event::EVENT_STORE_PACKET: {
            uint32_t peer_id = *reinterpret_cast<uint32_t*>(data_ptr->ptrw() + INDEX_PEER_ID);
            if (!peers.has(peer_id)) {
                return; //ignore the packet if we don't have the peer
            }

            EOS_EPacketReliability reliability = static_cast<EOS_EPacketReliability>(data_ptr->ptrw()[INDEX_TRANSFER_MODE]);

            EOSGPacket packet;
            packet.store_payload(data_ptr->ptrw() + INDEX_PAYLOAD_DATA, packet_data.size() - EOSGPacket::PACKET_HEADER_SIZE);
            packet.set_event(event);
            packet.set_sender(peer_id);
            packet.set_reliability(reliability);
            packet.set_channel(packet_data.get_channel());
            
            socket.push_packet(packet);
            break;
        }
        case Event::EVENT_RECIEVE_PEER_ID: {
            ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Client has recieved an EVENT_RECIEVE_PEER_ID packet. This should not have happened.");

            uint32_t peer_id = *reinterpret_cast<uint32_t*>(data_ptr->ptrw() + INDEX_PEER_ID);
            EOS_ProductUserId remote_user = eosg_string_to_product_user_id(packet_data.get_sender().utf8());
            if (peer_id < 2 || peers.has(peer_id)) {
                _disconnect_remote_user(remote_user); //Invalid peer id. reject the peer.
                return;
            }
            peers.insert(peer_id, remote_user);

            emit_signal("peer_connected", peer_id);
            break;
        }
    }
}

void EOSGMultiplayerPeer::_close() {
    if (!_is_active()) {
		return;
	}

    EOSGPacketPeerMediator::get_singleton()->unregister_socket(socket.get_socket_name());
    active_peers.erase(socket.get_socket_name());
    socket.close();
    active_mode = MODE_NONE;
    connection_status = CONNECTION_DISCONNECTED;
    peers.clear();
    pending_connection_requests.clear();
    unique_id = 0;
    set_refuse_new_connections(false);
}

void EOSGMultiplayerPeer::_disconnect_peer(int32_t p_peer, bool p_force) {
    ERR_FAIL_COND(!_is_active() || !peers.has(p_peer));
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot close connection. Local user id is not set");

    EOS_ProductUserId user_id = peers.get(p_peer);
    _disconnect_remote_user(user_id);

    if (p_force && peers.has(p_peer)) {
        peers.erase(p_peer);
        emit_signal("peer_disconnected", p_peer);
    }
}

int32_t EOSGMultiplayerPeer::_get_unique_id() const {
    return unique_id;
}

void EOSGMultiplayerPeer::_set_refuse_new_connections(bool p_enable) {
    refusing_connections = p_enable;
}

bool EOSGMultiplayerPeer::_is_refusing_new_connections() const {
    return refusing_connections;
}

bool EOSGMultiplayerPeer::_is_server_relay_supported() const {
    return active_mode == MODE_SERVER || active_mode == MODE_CLIENT;
}

MultiplayerPeer::ConnectionStatus EOSGMultiplayerPeer::_get_connection_status() const {
    return connection_status;
}

void EOSGMultiplayerPeer::set_local_user_id(const String& p_local_user_id) { 
    CharString local_user_id = p_local_user_id.utf8();
    s_local_user_id = eosg_string_to_product_user_id(local_user_id);
}

String EOSGMultiplayerPeer::get_local_user_id() {
    if (s_local_user_id == nullptr) return "";
    String local_user_id = eosg_product_user_id_to_string(s_local_user_id);
    return local_user_id;
}

Error EOSGMultiplayerPeer::_broadcast(const EOSGPacket &packet, int exclude) {
    ERR_FAIL_COND_V_MSG(packet.packet_size() > _get_max_packet_size(), ERR_OUT_OF_MEMORY, "Failed to send packet. Packet exceeds max size limits.");

    EOS_P2P_SendPacketOptions options;
    options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.Channel = packet.get_channel();
    options.DataLengthBytes = packet.packet_size();
    options.Data = packet.get_packet();
    options.bAllowDelayedDelivery = allow_delayed_delivery;
    options.Reliability = packet.get_reliability();
    options.bDisableAutoAcceptConnection = EOS_FALSE;
    options.SocketId = socket.get_socket_id();

    for (KeyValue<uint32_t, EOS_ProductUserId> &E : peers) {
        if (E.key == exclude) {
            continue;
        }

        options.RemoteUserId = E.value;
        EOS_EResult result = IEOS::get_singleton()->p2p_send_packet(&options);

        ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_InvalidParameters, ERR_INVALID_PARAMETER, "Failed to send packet! Invalid parameters.");
        ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_LimitExceeded, FAILED, "Failed to send packet! Packet is either too large or outgoing packet queue is full.");
        ERR_FAIL_COND_V_MSG(result == EOS_EResult::EOS_NoConnection, ERR_CANT_CONNECT, "Failed to send packet! No connection.");
    }

    return OK;
}

Error EOSGMultiplayerPeer::_send_to(const EOS_ProductUserId &remote_peer, const EOSGPacket &packet) {
    ERR_FAIL_COND_V_MSG(packet.packet_size() > _get_max_packet_size(), ERR_OUT_OF_MEMORY, "Failed to send packet. Packet exceeds max size limits.");

    EOS_P2P_SendPacketOptions options;
    options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_peer;
    options.SocketId = socket.get_socket_id();
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

bool EOSGMultiplayerPeer::_find_connection_request(const String &remote_user, const String &socket_id, EOSGConnectionRequest &out_request) {
    for (EOSGConnectionRequest &connection_request : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(connection_request.remote_user);
        String socket_name = connection_request.socket.SocketName;
        if (remote_user == remote_user_id_str && socket_id == socket_name) {
            out_request = connection_request;
            return true;
        }
    }
    return false;
}

bool EOSGMultiplayerPeer::_add_server_callbacks() {

    if (socket.add_connection_established_callback() && socket.add_connection_interrupted_callback() && 
        socket.add_connection_closed_callback() && socket.add_incoming_connection_request_callback()) {
        return true;
    }
    return true;
}

bool EOSGMultiplayerPeer::_add_client_callbacks() {

    if (socket.add_connection_established_callback() && socket.add_connection_interrupted_callback() && 
        socket.add_connection_closed_callback()) {
        return true;
    }
    return false;
}

EOS_EPacketReliability EOSGMultiplayerPeer::_convert_transfer_mode_to_eos_reliability(TransferMode mode) const {
    switch (mode) {
        case TRANSFER_MODE_UNRELIABLE:
            return EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
        case TRANSFER_MODE_UNRELIABLE_ORDERED:
            return EOS_EPacketReliability::EOS_PR_ReliableOrdered;
        case TRANSFER_MODE_RELIABLE:
            return EOS_EPacketReliability::EOS_PR_ReliableOrdered;
        default:
            return EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
    }
}

MultiplayerPeer::TransferMode EOSGMultiplayerPeer::_convert_eos_reliability_to_transfer_mode(EOS_EPacketReliability reliability) const {
    switch (reliability) {
        case EOS_EPacketReliability::EOS_PR_UnreliableUnordered:
            return TRANSFER_MODE_UNRELIABLE;
        case EOS_EPacketReliability::EOS_PR_ReliableOrdered:
            return TRANSFER_MODE_RELIABLE;
        default:
            return TRANSFER_MODE_UNRELIABLE;
    }
}

void EOSGMultiplayerPeer::_disconnect_remote_user(const EOS_ProductUserId &remote_user) {
    EOS_P2P_CloseConnectionOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_user;
    options.SocketId = socket.get_socket_id();
    EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);
    
    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to close peer connection. Invalid parameters.");
}

void EOS_CALL EOSGMultiplayerPeer::_on_peer_connection_established(const EOS_P2P_OnPeerConnectionEstablishedInfo *data) {
    String socket_name = data->SocketId->SocketName;
    ERR_FAIL_COND_MSG(!active_peers.has(socket_name), vformat("Callback _on_peer_connection_established has failed. Active peer could not be found for socket \"%s\"", socket_name));
    EOSGMultiplayerPeer *peer_instance = active_peers[socket_name];
    
    if (data->ConnectionType == EOS_EConnectionEstablishedType::EOS_CET_NewConnection) {
        if (peer_instance->active_mode == MODE_CLIENT) {
            EOS_ProductUserId server_peer = data->RemoteUserId;
            peer_instance->peers.insert(1, server_peer);
            peer_instance->connection_status = CONNECTION_CONNECTED;
            peer_instance->emit_signal("peer_connected", 1);
        }
    }

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    int connection_type = static_cast<int>(data->ConnectionType);
    int network_type = static_cast<int>(data->NetworkType);

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    ret["connection_type"] = connection_type;
    ret["network_type"] = network_type;
    peer_instance->emit_signal("p2p_peer_connection_established", ret);
}

void EOS_CALL EOSGMultiplayerPeer::_on_peer_connection_interrupted(const EOS_P2P_OnPeerConnectionInterruptedInfo *data) {
    String socket_name = data->SocketId->SocketName;
    ERR_FAIL_COND_MSG(!active_peers.has(socket_name), vformat("Callback _on_peer_connection_established has failed. Active peer could not be found for socket \"%s\"", socket_name));
    EOSGMultiplayerPeer *peer_instance = active_peers[socket_name];

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    peer_instance->emit_signal("p2p_peer_connection_interrupted", ret);
}

void EOS_CALL EOSGMultiplayerPeer::_on_incoming_connection_request(const EOS_P2P_OnIncomingConnectionRequestInfo *data) {
    String socket_name = data->SocketId->SocketName;
    ERR_FAIL_COND_MSG(!active_peers.has(socket_name), vformat("Callback _on_peer_connection_established has failed. Active peer could not be found for socket \"%s\"", socket_name));
    EOSGMultiplayerPeer *peer_instance = active_peers[socket_name];

    if (peer_instance->active_mode == MODE_CLIENT || peer_instance->is_refusing_new_connections()) return;

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);

    EOSGConnectionRequest connection_request;
    connection_request.remote_user = data->RemoteUserId;
    connection_request.socket = *data->SocketId;
    peer_instance->pending_connection_requests.push_back(connection_request);

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    peer_instance->emit_signal("p2p_incoming_connection_request", ret);

    if(!peer_instance->is_auto_accepting_connection_requests()) return;

    peer_instance->accept_connection_request(remote_user_id_str, socket_name);
}

void EOS_CALL EOSGMultiplayerPeer::_on_remote_connection_closed(const EOS_P2P_OnRemoteConnectionClosedInfo *data) {
    String socket_name = data->SocketId->SocketName;
    ERR_FAIL_COND_MSG(!active_peers.has(socket_name), vformat("Callback _on_peer_connection_established has failed. Active peer could not be found for socket \"%s\"", socket_name));
    EOSGMultiplayerPeer *peer_instance = active_peers[socket_name];

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    int reason = static_cast<int>(data->Reason);

    //attempt to remove connection request if there was one.
    EOSGConnectionRequest connection_request;
    if (peer_instance->_find_connection_request(remote_user_id_str, socket_name, connection_request)) {
            peer_instance->pending_connection_requests.erase(connection_request);
    }

    //Attempt to remove peer;
    int peer_id = peer_instance->get_peer_id(remote_user_id_str);
    if (peer_id != 0) {
        peer_instance->peers.erase(peer_id);
        peer_instance->_clear_peer_packet_queue(peer_id);
        peer_instance->emit_signal("peer_disconnected", peer_id);
    }

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = socket_name;
    ret["reason"] = reason;
    peer_instance->emit_signal("p2p_peer_connection_closed", ret);
}

EOSGMultiplayerPeer::~EOSGMultiplayerPeer() {
    if (active_mode != MODE_NONE) {
        _close();
    }
}

void EOSGMultiplayerPeer::EOSGPacket::prepare() {
    packet.get()->ptrw()[INDEX_EVENT_TYPE] = static_cast<uint8_t>(event);
    packet.get()->ptrw()[INDEX_TRANSFER_MODE] = static_cast<uint8_t>(reliability);
    memcpy(packet.get()->ptrw() + INDEX_PEER_ID, &from, sizeof(int));
}

void EOSGMultiplayerPeer::EOSGPacket::store_payload(const uint8_t *payload_data, const uint32_t payload_size_bytes) {
    size_bytes = payload_size_bytes + PACKET_HEADER_SIZE;
    packet.get()->clear();
    packet.get()->resize(size_bytes);
    memcpy(packet.get()->ptrw() + INDEX_PAYLOAD_DATA, payload_data, payload_size_bytes);
}

bool EOSGMultiplayerPeer::EOSGSocket::add_connection_established_callback() {
    EOS_P2P_AddNotifyPeerConnectionEstablishedOptions connection_established_options;
    connection_established_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST;
    connection_established_options.LocalUserId = s_local_user_id;
    connection_established_options.SocketId = &socket;
    connection_established_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_established(&connection_established_options, 
        EOSGMultiplayerPeer::_on_peer_connection_established);
    ERR_FAIL_COND_V_MSG(connection_established_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection established callback.");

    return true;
}

bool EOSGMultiplayerPeer::EOSGSocket::add_connection_interrupted_callback() {
    EOS_P2P_AddNotifyPeerConnectionInterruptedOptions connection_interrupted_options;
    connection_interrupted_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST;
    connection_interrupted_options.LocalUserId = s_local_user_id;
    connection_interrupted_options.SocketId = &socket;
    connection_interrupted_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_interrupted(&connection_interrupted_options,
        EOSGMultiplayerPeer::_on_peer_connection_interrupted);
    ERR_FAIL_COND_V_MSG(connection_interrupted_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection interrupted callback.");

    return true;
}

bool EOSGMultiplayerPeer::EOSGSocket::add_connection_closed_callback() {
    EOS_P2P_AddNotifyPeerConnectionClosedOptions connection_closed_options;
    connection_closed_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
    connection_closed_options.LocalUserId = s_local_user_id;
    connection_closed_options.SocketId = &socket;
    connection_closed_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_closed(&connection_closed_options,
        EOSGMultiplayerPeer::_on_remote_connection_closed);
    ERR_FAIL_COND_V_MSG(connection_closed_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection closed callback.");

    return true;
}

bool EOSGMultiplayerPeer::EOSGSocket::add_incoming_connection_request_callback() {
    EOS_P2P_AddNotifyPeerConnectionRequestOptions connection_request_options;
    connection_request_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
    connection_request_options.LocalUserId = s_local_user_id;
    connection_request_options.SocketId = &socket;
    incoming_connection_request_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_request(&connection_request_options,
        EOSGMultiplayerPeer::_on_incoming_connection_request);
    ERR_FAIL_COND_V_MSG(incoming_connection_request_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection request callback.");

    return true;
}

void EOSGMultiplayerPeer::EOSGSocket::close() {
    clear_packet_queue();

    if (connection_established_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_established(connection_established_callback_id);
    }
    if (connection_interrupted_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_interrupted(connection_interrupted_callback_id);
    }
    if (connection_closed_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_closed(connection_closed_callback_id);
    }
    if (incoming_connection_request_callback_id != EOS_INVALID_NOTIFICATIONID) {
        IEOS::get_singleton()->p2p_remove_notify_peer_connection_request(incoming_connection_request_callback_id);
    }

    EOS_P2P_CloseConnectionsOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTIONS_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.SocketId = &socket;
    IEOS::get_singleton()->p2p_close_all_connections(&options);
}

void EOSGMultiplayerPeer::EOSGSocket::clear_packets_from_peer(int p_peer) {
    List<List<EOSGPacket>::Element*> del;
    for (List<EOSGPacket>::Element *e = incoming_packets.front(); e != nullptr; e = e->next()) {
        if (e->get().get_sender() != p_peer) continue;
        del.push_back(e);
    }
    for (List<EOSGPacket>::Element *e : del) {
        e->erase();
    }
}

// bool EOSGMultiplayerPeer::EOSGSocket::operator ==(const EOSGSocket &rhs) {
//     String lhs_socket_name = rhs.get_socket_id()->SocketName;
//     String rhs_socket_name = this->socket.SocketName;
//     return lhs_socket_name == rhs_socket_name;
// }

bool EOSGMultiplayerPeer::EOSGConnectionRequest::operator ==(const EOSGConnectionRequest &rhs) {
    String rhs_remote_id = eosg_product_user_id_to_string(rhs.remote_user);
    String rhs_socket_name = rhs.socket.SocketName;
    String lhs_remote_id = eosg_product_user_id_to_string(this->remote_user);
    String lhs_socket_name = this->socket.SocketName;
    return rhs_remote_id == lhs_remote_id && rhs_socket_name == lhs_socket_name;
}
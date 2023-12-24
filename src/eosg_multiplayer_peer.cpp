/****************************************
 * EOSGMultiplayerPeer
 * Author: Dallin Lovin aka LowFire
 * Description: Multiplayer peer that uses the EOS P2P Interface to allow users to
 * establish direct peer-to-peer connections with other players over the internet.
 * NAT punchthrough is handled automatically by the P2P interface and a relay server
 * is used if direct connection is not possible. EOSGMultiplayerPeer allows users to
 * create either a client, server, or mesh instance. A server opens a socket and actively listens
 * for incoming connection requests. When a connection request is received, a server can either
 * accept or reject the connection request. Servers auto accept connection requests by default.
 * Clients can only connect to a server instance and cannot accept connection requests
 * from anyone. Clients are only allowed to be connected to a single peer and that peer
 * must be a server. Mesh instances can arbitrarily connect to each other using a common
 * socket id. They can also connect to a server instance. The socket id must be the same on
 * all mesh instances if they are to connect to each other. Once connected, mesh instances can
 * send packets to each other just like a server and clients, but those packets will not be relayed. A mesh instance can only send
 * packets to peers they are directly connected to.
 ****************************************/

#include "eosg_multiplayer_peer.h"
#include "eosg_packet_peer_mediator.h"
#include "ieos.h"

using namespace godot;

EOS_ProductUserId EOSGMultiplayerPeer::s_local_user_id = nullptr;

void EOSGMultiplayerPeer::_bind_methods() {
    ClassDB::bind_static_method("EOSGMultiplayerPeer", D_METHOD("get_local_user_id"), &EOSGMultiplayerPeer::get_local_user_id);

    ClassDB::bind_method(D_METHOD("create_server", "socket_id"), &EOSGMultiplayerPeer::create_server);
    ClassDB::bind_method(D_METHOD("create_client", "socket_id", "remote_user_id"), &EOSGMultiplayerPeer::create_client);
    ClassDB::bind_method(D_METHOD("create_mesh", "socket_id"), &EOSGMultiplayerPeer::create_mesh);
    ClassDB::bind_method(D_METHOD("add_mesh_peer", "remote_user_id"), &EOSGMultiplayerPeer::add_mesh_peer);
    ClassDB::bind_method(D_METHOD("get_socket"), &EOSGMultiplayerPeer::get_socket);
    ClassDB::bind_method(D_METHOD("get_peer_id", "remote_user_id"), &EOSGMultiplayerPeer::get_peer_id);
    ClassDB::bind_method(D_METHOD("set_allow_delayed_delivery", "allow"), &EOSGMultiplayerPeer::set_allow_delayed_delivery);
    ClassDB::bind_method(D_METHOD("is_allowing_delayed_delivery"), &EOSGMultiplayerPeer::is_allowing_delayed_delivery);
    ClassDB::bind_method(D_METHOD("is_auto_accepting_connection_requests"), &EOSGMultiplayerPeer::is_auto_accepting_connection_requests);
    ClassDB::bind_method(D_METHOD("set_auto_accept_connection_requests", "enable"), &EOSGMultiplayerPeer::set_auto_accept_connection_requests);
    ClassDB::bind_method(D_METHOD("get_all_connection_requests"), &EOSGMultiplayerPeer::get_all_connection_requests);
    ClassDB::bind_method(D_METHOD("has_peer", "peer_id"), &EOSGMultiplayerPeer::has_peer);
    ClassDB::bind_method(D_METHOD("has_user_id", "remote_user_id"), &EOSGMultiplayerPeer::has_user_id);
    ClassDB::bind_method(D_METHOD("accept_connection_request", "remote_user_id"), &EOSGMultiplayerPeer::accept_connection_request);
    ClassDB::bind_method(D_METHOD("deny_connection_request", "remote_user_id"), &EOSGMultiplayerPeer::deny_connection_request);
    ClassDB::bind_method(D_METHOD("accept_all_connection_requests"), &EOSGMultiplayerPeer::accept_all_connection_requests);
    ClassDB::bind_method(D_METHOD("deny_all_connection_requests"), &EOSGMultiplayerPeer::deny_all_connection_requests);
    ClassDB::bind_method(D_METHOD("get_active_mode"), &EOSGMultiplayerPeer::get_active_mode);
    ClassDB::bind_method(D_METHOD("get_all_peers"), &EOSGMultiplayerPeer::get_all_peers);
    ClassDB::bind_method(D_METHOD("get_peer_user_id"), &EOSGMultiplayerPeer::get_peer_user_id);
    ClassDB::bind_method(D_METHOD("is_polling"), &EOSGMultiplayerPeer::is_polling);
    ClassDB::bind_method(D_METHOD("set_is_polling", "polling"), &EOSGMultiplayerPeer::set_is_polling);

    ADD_SIGNAL(MethodInfo("peer_connection_established", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("peer_connection_interrupted", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("peer_connection_closed", PropertyInfo(Variant::DICTIONARY, "callback_data")));
    ADD_SIGNAL(MethodInfo("incoming_connection_request", PropertyInfo(Variant::DICTIONARY, "callback_data")));
}

/****************************************
 * create_server
 * Parameters:
 *   socket_id - The socket id the server uses to listen for connection requests.
 * Description: Creates a server instance using the given socket id.
 ****************************************/
Error EOSGMultiplayerPeer::create_server(const String &socket_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create server. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create server. Multiplayer instance is already active.");

    unique_id = 1;
    active_mode = MODE_SERVER;
    connection_status = CONNECTION_CONNECTED;
    polling = true;
    set_refuse_new_connections(false);

    //Create a socket where we will be listening for connections
    socket = EOSGSocket(socket_id);
    if (socket.get_name().is_empty()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create server.");
    }

    bool success = EOSGPacketPeerMediator::get_singleton()->register_peer(this);

    if (!success) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create server.");
    }

    return OK;
}

/****************************************
 * create_client
 * Parameters:
 *   socket_id - The socket id of the server the client is trying to connect to.
 *   remote_user_id - The user id of the server the client is trying to connect to.
 * Description: Creates a client instance and sends a connection request to the server using
 * the socket id and remote user id. Clients are only allowed to connect to servers. If the
 * remote user is another client or a mesh instance, connection will fail.
 ****************************************/
Error EOSGMultiplayerPeer::create_client(const String &socket_id, const String &remote_user_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create client. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create client. Multiplayer instance is already active.");

    polling = true;
    set_refuse_new_connections(false);

    //Create the socket we are trying to connect to
    socket = EOSGSocket(socket_id);
    if (socket.get_name().is_empty()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create client.");
    }

    bool success = EOSGPacketPeerMediator::get_singleton()->register_peer(this);

    if (!success) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create client.");
    }

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

    if (result != OK) {
        _close();
        return result;
    }
    return OK;
}

/****************************************
 * create_mesh
 * Parameters:
 *   socket_id - The socket id the mesh instance uses to listen for connection requests.
 * Description: Creates a mesh instance using the given socket id. Other mesh instances can send
 * connection requests to this instance using the socket id.
 ****************************************/
Error EOSGMultiplayerPeer::create_mesh(const String &socket_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to create mesh. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "Failed to create mesh. Multiplayer instance is already active.");

    unique_id = generate_unique_id();
    active_mode = MODE_MESH;
    connection_status = CONNECTION_CONNECTED;
    polling = true;

    //Create a socket where we will be listening for connections
    socket = EOSGSocket(socket_id);
    if (socket.get_name().is_empty()) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create mesh.");
    }

    bool success = EOSGPacketPeerMediator::get_singleton()->register_peer(this);

    if (!success) {
        _close();
        ERR_FAIL_V_MSG(ERR_CANT_CREATE, "Failed to create mesh.");
    }

    return OK;
}

/****************************************
 * add_mesh_peer
 * Parameters:
 *   remote_user_id - The remote user id of the other mesh instance this mesh instance is trying to
 * connect to.
 * Description: Sends a connection request to another mesh instance using the remote user id. If the connection
 * request is accepted, the other instance is added as a peer.
 ****************************************/
Error EOSGMultiplayerPeer::add_mesh_peer(const String &remote_user_id) {
    ERR_FAIL_NULL_V_MSG(s_local_user_id, ERR_UNCONFIGURED, "Failed to add mesh peer. Local user id has not been set.");
    ERR_FAIL_COND_V_MSG(active_mode != MODE_MESH, ERR_UNCONFIGURED, "Failed to add mesh peer. Multiplayer instance is not in mesh mode.");
    ERR_FAIL_COND_V_MSG(has_user_id(remote_user_id), ERR_ALREADY_EXISTS, "Failed to add mesh peer. Already connected to peer");

    //send connection request to peer
    EOSGPacket packet;
    packet.set_event(EVENT_MESH_CONNECTION_REQUEST);
    packet.set_channel(CH_RELIABLE);
    packet.set_reliability(EOS_EPacketReliability::EOS_PR_ReliableUnordered);
    packet.set_sender(unique_id);
    packet.prepare();

    _send_to(eosg_string_to_product_user_id(remote_user_id.utf8()), packet);

    return OK;
}

/****************************************
 * get_socket
 * Description: Returns the socket id of this multiplayer instance.
 ****************************************/
String EOSGMultiplayerPeer::get_socket() const {
    return socket.get_name();
}

/****************************************
 * get_all_connection_requests
 * Description: Returns all connection requests currently pending. Returns an empty
 * array if there are none.
 ****************************************/
Array EOSGMultiplayerPeer::get_all_connection_requests() {
    Array ret = Array();
    for (const EOS_ProductUserId &remote_user : pending_connection_requests) {
        ret.push_back(eosg_product_user_id_to_string(remote_user));
    }
    return ret;
}

/****************************************
 * get_peer_user_id
 * Parameters:
 *   p_id - The peer id of the peer to return the user id of.
 * Description: Returns the remote user id of the given peer. Returns an empty string
 * if the peer could not be found.
 ****************************************/
String EOSGMultiplayerPeer::get_peer_user_id(int p_id) {
    String ret = "";
    if (!peers.has(p_id)) {
        return ret;
    }
    EOS_ProductUserId user_id = peers.get(p_id);
    ret = eosg_product_user_id_to_string(user_id);
    return ret;
}

/****************************************
 * get_peer_id
 * Parameters:
 *   remote_user_id - The remote user id used to find a peer.
 * Description: Retrieves the peer id of a peer using their remote user id. Returns 0
 * if no peer with a matching remote user id could be found.
 ****************************************/
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

/****************************************
 * get_peer_id
 * Parameters:
 *   peer_id - The peer id of the peer to find.
 * Description: Returns whether or not this multiplayer instance has the given
 * peer.
 ****************************************/
bool EOSGMultiplayerPeer::has_peer(int peer_id) {
    return peers.has(peer_id);
}

/****************************************
 * has_user_id
 * Parameters:
 *   remote_user_id - The remote user id to look for.
 * Description: Returns whether or not this multiplayer instance has a peer with the given
 * remote user id.
 ****************************************/
bool EOSGMultiplayerPeer::has_user_id(const String &remote_user_id) {
    for (KeyValue<uint32_t, EOS_ProductUserId> &E : peers) {
        String peer_user_id = eosg_product_user_id_to_string(E.value);
        if (remote_user_id != peer_user_id)
            continue;
        return true;
    }
    return false;
}

/****************************************
 * _clear_peer_packet_queue
 * Parameters:
 *   peer_id - The peer id of the peer to clear the packet from.
 * Description: Clears all packets sent by the given peer.
 ****************************************/
void EOSGMultiplayerPeer::_clear_peer_packet_queue(int p_id) {
    ERR_FAIL_COND_MSG(!peers.has(p_id), "Failed to clear packet queue for peer. Peer was not found.");

    socket.clear_packets_from_peer(p_id);
    String remote_user_id = eosg_product_user_id_to_string(peers[p_id]);
    EOSGPacketPeerMediator::get_singleton()->clear_packets_from_remote_user(socket.get_name(), remote_user_id);
}

/****************************************
 * get_all_peers
 * Description: Gets all connected peers. Returns a dictionary, using the peer id as key
 * and the peer's remote user id as value.
 ****************************************/
Dictionary EOSGMultiplayerPeer::get_all_peers() {
    Dictionary ret;
    for (KeyValue<uint32_t, EOS_ProductUserId> &E : peers) {
        EOS_ProductUserId user_id = E.value;
        ret[E.key] = eosg_product_user_id_to_string(user_id);
    }
    return ret;
}

/****************************************
 * set_allowed_delayed_delivery
 * Parameters:
 *   allow - bool used to set allowed delivery.
 * Description: Set whether or not delayed delivery should be allowed on all sent packets.
 * This means that packets will be delayed in their delivery if a connenetion with the other
 * peer has not been estalished yet, otherwise the packets will be dropped.
 ****************************************/
void EOSGMultiplayerPeer::set_allow_delayed_delivery(bool allow) {
    allow_delayed_delivery = allow;
}

/****************************************
 * is_allowing_delayed_delivery
 * Description: Returns whether or not delayed deliver is turned on.
 ****************************************/
bool EOSGMultiplayerPeer::is_allowing_delayed_delivery() {
    return allow_delayed_delivery;
}

/****************************************
 * set_auto_accept_connection_requests
 * Parameters:
 *   enable - bool used to set auto accept connection requests.
 * Description: Set whether or not to auto accept connection requests when they are
 * recieved. If this is off, connection requests are put into a list to be accepted
 * or denied later if desired.
 ****************************************/
void EOSGMultiplayerPeer::set_auto_accept_connection_requests(bool enable) {
    auto_accept_connection_requests = enable;
}

/****************************************
 * is_auto_accepting_connection_requests
 * Description: Returns whether or not auto accepting connection requests is on.
 ****************************************/
bool EOSGMultiplayerPeer::is_auto_accepting_connection_requests() {
    return auto_accept_connection_requests;
}

/****************************************
 * accept_connection_request
 * Parameters:
 *   remote_user - The remote user id of the peer to accept a connection request from.
 * Description: Accepts a connection request from the given peer. If the connection request is accepted,
 * the peer will establish a connection with this multiplayer instance and peer id's will be exchanged.
 * The method does nothing if no connection request could be found using the given remote user id.
 ****************************************/
void EOSGMultiplayerPeer::accept_connection_request(const String &remote_user) {
    ERR_FAIL_COND_MSG(active_mode == MODE_NONE, "Cannot accept connection requests when multiplayer instance is not active.");
    ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Clients are not allowed to accept connection requests.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot accept connection requests. Local user id has not been set.");

    EOS_ProductUserId remote_user_id;
    if (!_find_connection_request(remote_user, remote_user_id))
        return;

    EOS_P2P_AcceptConnectionOptions options;
    options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_user_id;
    options.SocketId = socket.get_id();
    EOS_EResult result = IEOS::get_singleton()->p2p_accept_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to accept connection request! Invalid parameters.");

    pending_connection_requests.erase(remote_user_id);
}

/****************************************
 * deny_connection_request
 * Parameters:
 *   remote_user - The remote user id of the peer to deny a connection request from.
 * Description: Denies a connection request from the given peer. The connection request is removed
 * from the list of pending connection requests when denied and the connection is closed with the peer.
 ****************************************/
void EOSGMultiplayerPeer::deny_connection_request(const String &remote_user) {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Failed to deny connection request. Local user id not set");
    if (active_mode == MODE_NONE || active_mode == MODE_CLIENT)
        return;
    EOS_ProductUserId remote_user_id;
    if (!_find_connection_request(remote_user, remote_user_id))
        return;

    EOS_P2P_CloseConnectionOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_user_id;
    options.SocketId = socket.get_id();
    EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to deny connection. Invalid parameters.");

    pending_connection_requests.erase(remote_user_id);
}

/****************************************
 * accept_all_connection_requests
 * Description: Accepts all connection requests currently pending.
 ****************************************/
void EOSGMultiplayerPeer::accept_all_connection_requests() {
    if (pending_connection_requests.size() == 0)
        return;

    ERR_FAIL_COND_MSG(active_mode == MODE_NONE, "Cannot accept connection requests when multiplayer instance is not active.");
    ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT, "Clients are not allowed to accept connection requests.");
    ERR_FAIL_NULL_MSG(s_local_user_id, "Cannot accept connection requests. Local user id has not been set.");

    for (const EOS_ProductUserId &remote_user_id : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(remote_user_id);
        accept_connection_request(remote_user_id_str);
    }
}

/****************************************
 * deny_all_connection_requests
 * Description: Denies all connection requests currently pending.
 ****************************************/
void EOSGMultiplayerPeer::deny_all_connection_requests() {
    ERR_FAIL_NULL_MSG(s_local_user_id, "Failed to deny connection requests. Local user id not set");
    if (active_mode == MODE_NONE || active_mode == MODE_CLIENT)
        return;

    for (const EOS_ProductUserId &remote_user_id : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(remote_user_id);
        deny_connection_request(remote_user_id_str);
    }

    pending_connection_requests.clear();
}

/****************************************
 * get_active_mode
 * Description: Returns the active mode of the multiplayer peer, which could be either be MODE_CLIENT,
 * MODE_SERVER, MODE_MESH, or MODE_NONE. MODE_NONE is returned if the multiplayer instance is not
 * currently active.
 ****************************************/
int EOSGMultiplayerPeer::get_active_mode() {
    return static_cast<int>(active_mode);
}

/****************************************
 * _get_packet
 *   Parameters:
 *   r_buffer - out parameter that returns packet's raw byte data.
 *   r_buffer_size - out parameter that returns the packet's total size in bytes.
 * Description: This method is called by the MultiplayerAPI when it tries to poll the next available packet
 * from this multiplayer instance. The packet data and the packet size is returned to the MultiplayerAPI using
 * the r_buffer and r_buffer_size out parameters.
 ****************************************/
Error EOSGMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) {
    current_packet = socket.pop_packet();

    *r_buffer = current_packet.get_payload();
    *r_buffer_size = current_packet.payload_size();

    return OK;
}

/****************************************
 * _put_packet
 *   Parameters:
 *   r_buffer - The packet's raw byte data being sent by the MultiplayerAPI
 *   r_buffer_size - The size in bytes of the packet being sent.
 * Description: This method is called by the MultiplayerAPI when it tries to send a packet to
 * a connected peer. The packet's event type, transfer mode, and sender's peer id are put into
 * a packet header before sending. The payload, which is the data being sent by the MultiplayerAPI,
 * is appended after the packet header. Once the packet has been prepared, it is sent to the intended
 * peer.
 ****************************************/
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

    /*IMPORTANT NOTE: EOS does not support an unreliable ordered transfer mode,
    so it has been left out. EOSGMultiplayerPeer prints a warning and automatically sets the transfer
    mode to TRANSFER_MODE_RELIABLE if the user tries to set to unreliable ordred.
    See _set_transer_mode()*/
    switch (_get_transfer_mode()) {
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
    if (_is_server() || active_mode == MODE_MESH) {
        if (target_peer == 0) {
            result = _broadcast(packet);
        } else if (target_peer < 0) {
            int exclude = ABS(target_peer);
            result = _broadcast(packet, exclude);
        } else {
            result = _send_to(peers[target_peer], packet);
        }
    } else { //We're a client
        result = _send_to(peers[1], packet); //send to the server
    }
    return result;
}

/****************************************
 * _get_available_packet_count
 * Description: Called by MultiplayerAPI to query how
 * many packets are currently queued. The MultiplayerAPI calls this method
 * every time it polls to determine if there are packets to recieve.
 ****************************************/
int32_t EOSGMultiplayerPeer::_get_available_packet_count() const {
    return socket.get_packet_count();
}

/****************************************
 * _get_max_packet_size
 * Description: Called by MultiplayerAPI to query how
 * large packets are allowed to be.
 ****************************************/
int32_t EOSGMultiplayerPeer::_get_max_packet_size() const {
    return EOS_P2P_MAX_PACKET_SIZE;
}

/****************************************
 * _get_packet_channel
 * Description: Called by MultiplayerAPI to get the channel of next
 * avaialble packet in the packet queue.
 ****************************************/
int32_t EOSGMultiplayerPeer::_get_packet_channel() const {
    return socket.get_packet_channel();
}

/****************************************
 * _get_packet_mode
 * Description: Called by MultiplayerAPI to get the transfer mode of next
 * avaialble packet in the packet queue.
 ****************************************/
MultiplayerPeer::TransferMode EOSGMultiplayerPeer::_get_packet_mode() const {
    return _convert_eos_reliability_to_transfer_mode(socket.get_packet_reliability());
}

/****************************************
 * _set_transfer_channel
 * Parameters:
 *   p_channel - The channel to set to.
 * Description: Called by MultiplayerAPI to set the transfer channel of this
 * multiplayer instance.
 ****************************************/
void EOSGMultiplayerPeer::_set_transfer_channel(int32_t p_channel) {
    transfer_channel = p_channel;
}

/****************************************
 * _get_transfer_channel
 * Description: Called by MultiplayerAPI to get the transfer channel of
 * this multiplayer instance.
 ****************************************/
int32_t EOSGMultiplayerPeer::_get_transfer_channel() const {
    return transfer_channel;
}

/****************************************
 * _set_transfer_mode
 * Parameters:
 *   p_mode - The transfer mode to set to.
 * Description: Called by MultiplayerAPI to set the transfer mode of
 * this multiplayer instance. Unreliable ordered is not supported so the transfer mode
 * is set to reliable in that case.
 ****************************************/
void EOSGMultiplayerPeer::_set_transfer_mode(MultiplayerPeer::TransferMode p_mode) {
    if (p_mode == TRANSFER_MODE_UNRELIABLE_ORDERED) {
        WARN_PRINT("EOSGMultiplayerPeer does not support unreliable ordered. Setting to reliable instead.");
        transfer_mode = TRANSFER_MODE_RELIABLE;
        return;
    }
    transfer_mode = p_mode;
}

/****************************************
 * _get_transfer_mode
 * Description: Called by MultiplayerAPI to get the transfer mode of
 * this multiplayer instance.
 ****************************************/
MultiplayerPeer::TransferMode EOSGMultiplayerPeer::_get_transfer_mode() const {
    return transfer_mode;
}

/****************************************
 * _set_target_peer
 * Parameters:
 *   p_peer - The target peer
 * Description: Called by MultiplayerAPI to set the target peer to
 * send packets to. If this is set to 0, packets are broadcasted to all connected to peers.
 * If the value is negative, packets are broadcasted to all peers except for the peer identified
 * by the absolute value of the negative id.
 ****************************************/
void EOSGMultiplayerPeer::_set_target_peer(int32_t p_peer) {
    target_peer = p_peer;
}

/****************************************
 * _get_packet_peer
 * Description: Called by MultiplayerAPI to get the peer id of the peer
 * who sent the next queued packet.
 ****************************************/
int32_t EOSGMultiplayerPeer::_get_packet_peer() const {
    return socket.get_packet_peer();
}

/****************************************
 * _is_server
 * Description: Called by MultiplayerAPI to determine if this instance is a server.
 ****************************************/
bool EOSGMultiplayerPeer::_is_server() const {
    return active_mode == MODE_SERVER;
}

/****************************************
 * _poll
 * Description: Called by MultiplayerAPI every network frame to poll queued packets inside EOSGPacketPeerMediator.
 * Packets are recieved first from EOSGPacketPeerMediator. Once polled, the event type of each packet
 * are retrieved from the first byte of the packet. If the event type is EVENT_RECIEVED_PEER_ID,
 * the sender peer id contained inside the packet header is added to the list of connected peers
 * for this mutliplayer instance. This usually happens only once when peers first establish a connection.
 * If the event is EVENT_STORE_PACKET, the packet is queued into the socket's packet queue to be
 * retrieved by the MultiplayerAPI later (See _get_packet()). If the event is EVENT_MESH_CONNECTION_REQUEST,
 * then do nothing. This event is just to notify the multiplayer instance that the packet has been used to
 * establish a connection between two mesh instances and nothing needs to be done with the packet.
 ****************************************/
void EOSGMultiplayerPeer::_poll() {
    ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");
    if (!polling && !EOSGPacketPeerMediator::get_singleton()->next_packet_is_peer_id_packet(socket.get_name()))
        return;
    if (EOSGPacketPeerMediator::get_singleton()->get_packet_count_for_socket(socket.get_name()) == 0)
        return; //No packets available

    List<PacketData> packets;
    PacketData next_packet;
    if (!polling) { //The next packet should be a peer id packet if we're at this point
        while (EOSGPacketPeerMediator::get_singleton()->next_packet_is_peer_id_packet(socket.get_name())) {
            EOSGPacketPeerMediator::get_singleton()->poll_next_packet(socket.get_name(), &next_packet);
            packets.push_back(next_packet);
        }
    } else {
        while (EOSGPacketPeerMediator::get_singleton()->poll_next_packet(socket.get_name(), &next_packet)) {
            packets.push_back(next_packet);
        }
    }

    //process all the packets
    for (PacketData &packet_data : packets) {
        PackedByteArray *data_ptr = packet_data.get_data();
        Event event = static_cast<Event>(data_ptr->ptrw()[INDEX_EVENT_TYPE]);
        switch (event) {
            case Event::EVENT_STORE_PACKET: {
                uint32_t peer_id = *reinterpret_cast<uint32_t *>(data_ptr->ptrw() + INDEX_PEER_ID);
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
                ERR_FAIL_COND_MSG(active_mode == MODE_CLIENT && connection_status == CONNECTION_CONNECTED, "Client has recieved a EVENT_RECIEVE_PEER_ID packet when already connected. This shouldn't have happened!");

                uint32_t peer_id = *reinterpret_cast<uint32_t *>(data_ptr->ptrw() + INDEX_PEER_ID);
                if (active_mode == MODE_CLIENT && peer_id != 1) {
                    _close();
                    ERR_FAIL_MSG("Failed to connect. Instance is not a server.");
                }

                EOS_ProductUserId remote_user = eosg_string_to_product_user_id(packet_data.get_sender().utf8());
                if (peer_id < 1 || peers.has(peer_id) || unique_id == peer_id) {
                    _disconnect_remote_user(remote_user); //Invalid peer id. reject the peer.
                    break;
                }

                if (has_user_id(packet_data.get_sender())) {
                    //Peer may have reconnected with a new multiplayer instance. Remove their old peer id.
                    int old_id = get_peer_id(packet_data.get_sender());
                    peers.erase(old_id);
                    emit_signal("peer_disconnected", old_id);
                }

                peers.insert(peer_id, remote_user);
                if (active_mode == MODE_CLIENT) {
                    connection_status = CONNECTION_CONNECTED;
                }

                emit_signal("peer_connected", peer_id);
                break;
            }
            case Event::EVENT_MESH_CONNECTION_REQUEST:
                break;
        }
    }
}

/****************************************
 * _close
 * Description: Called when the multiplayer instance closes. Cleans everything up, closes connections with all peers,
 * and resets the state of the multiplayer instance.
 ****************************************/
void EOSGMultiplayerPeer::_close() {
    if (!_is_active()) {
        return;
    }

    polling = false; //To prevent any further packets from coming in when we close the socket.
    socket.close();
    active_mode = MODE_NONE;
    connection_status = CONNECTION_DISCONNECTED;
    pending_connection_requests.clear();
    unique_id = 0;
    set_refuse_new_connections(false);

    if (peers.is_empty()) { //Go ahead and unregister if there were no peers connected
        EOSGPacketPeerMediator::get_singleton()->unregister_peer(this);
    }
}

/****************************************
 * _disconnect_peer
 * Parameters:
 *   p_peer - The peer to disconnect
 *   p_force - Whether or not we should force disconnection with the peer.
 * Description: Disconnects the given peer. The peer is removed from the list of connected peers
 * when the connection closed. If p_force is set to true, the peer is removed from the list immediately
 * instead of waiting for a confirmed disconnection.
 ****************************************/
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

/****************************************
 * _get_unique_id
 * Description: returns the peer id of this multiplayer instance.
 ****************************************/
int32_t EOSGMultiplayerPeer::_get_unique_id() const {
    return unique_id;
}

/****************************************
 * _set_refuse_new_connections
 * Parameters:
 *   p_enable - bool that sets refusing new connections.
 * Description: Sets whether or not new connections are refused. Setting this to true means
 * that all connection requests are automatically denied. This overrides auto accepting connection requests.
 ****************************************/
void EOSGMultiplayerPeer::_set_refuse_new_connections(bool p_enable) {
    refusing_connections = p_enable;
}

/****************************************
 * _is_refuse_new_connections
 * Description: Returns whether or not new connections are refused.
 ****************************************/
bool EOSGMultiplayerPeer::_is_refusing_new_connections() const {
    return refusing_connections;
}

/****************************************
 * _is_server_relayed_supported
 * Description: Called by MultiplayerAPI to determine if if this multiplayer instance
 * supports packet relaying. If the instance is either a server or a connected client, then
 * relaying is supported. This is not the case for mesh instances.
 ****************************************/
bool EOSGMultiplayerPeer::_is_server_relay_supported() const {
    return active_mode == MODE_SERVER || active_mode == MODE_CLIENT;
}

/****************************************
 * _get_connection_status
 * Description: Returns the connection status of this multiplayer instance. Can be either
 * CONNECTION_DISCONNECTED, CONNECTION_CONNECTING, or CONNECTION_CONNECTED.
 ****************************************/
MultiplayerPeer::ConnectionStatus EOSGMultiplayerPeer::_get_connection_status() const {
    return connection_status;
}

/****************************************
 * set_local_user_id
 * Description: Static method that sets the local user id for the game. This is called when
 * players first log into the connect interface. This needs to be done for the multiplayer instance
 * to work.
 ****************************************/
void EOSGMultiplayerPeer::set_local_user_id(const String &p_local_user_id) {
    CharString local_user_id = p_local_user_id.utf8();
    s_local_user_id = eosg_string_to_product_user_id(local_user_id);
}

/****************************************
 * get_local_user_id
 * Description: Returns the currently set local user id. Returns an empty string if it was not set.
 ****************************************/
String EOSGMultiplayerPeer::get_local_user_id() {
    if (s_local_user_id == nullptr)
        return "";
    String local_user_id = eosg_product_user_id_to_string(s_local_user_id);
    return local_user_id;
}

/****************************************
 * _broadcast
 * Parameters:
 *   packet - The packet to be broadcasted.
 *   exclude - The peer that is excluded from receiving the packet.
 * Description: Broadcast the given packet to all connected peers, except for the peer given by the
 * exclude parameter.
 ****************************************/
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
    options.SocketId = socket.get_id();

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

/****************************************
 * _send_to
 * Parameters:
 *   remote_peer - The peer to send the packet to.
 *   packet - The packet being sent.
 * Description: Sends a packet to the given peer identified by the product user id.
 ****************************************/
Error EOSGMultiplayerPeer::_send_to(const EOS_ProductUserId &remote_peer, const EOSGPacket &packet) {
    ERR_FAIL_COND_V_MSG(packet.packet_size() > _get_max_packet_size(), ERR_OUT_OF_MEMORY, "Failed to send packet. Packet exceeds max size limits.");

    EOS_P2P_SendPacketOptions options;
    options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_peer;
    options.SocketId = socket.get_id();
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

/****************************************
 * _find_connection_request
 * Parameters:
 *   remote_user - The user to look for a connection request.
 *   out_request - An out parameter that returns the found connection request.
 * Description: Looks for a connection request in the pending connection request list for the given remote user.
 * Method returns true if a connection request was found for the user. False otherwise.
 ****************************************/
bool EOSGMultiplayerPeer::_find_connection_request(const String &remote_user, EOS_ProductUserId &out_request) {
    for (const EOS_ProductUserId &remote_user_id : pending_connection_requests) {
        String remote_user_id_str = eosg_product_user_id_to_string(remote_user_id);
        if (remote_user == remote_user_id_str) {
            out_request = remote_user_id;
            return true;
        }
    }
    return false;
}

/****************************************
 * _convert_transfer_mode_to_eos_reliability
 * Parameters:
 *   mode - The transfer mode to convert.
 * Description: A helper function that converts the given transfer mode to matching packet reliability.
 * If TRANSFER_MODE_UNRELIABLE_ORDED is passed, the function returns EOS_PR_ReliableOrdered because EOS
 * does not support unreliable ordered
 ****************************************/
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

/****************************************
 * _convert_eos_reliability_to_transfer_mode
 * Parameters:
 *   reliability - The packet reliability to convert.
 * Description: A helper function that converts the given packet reliability to a matching transfer mode.
 ****************************************/
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

/****************************************
 * _disconnect_remote_user
 * Parameters:
 *   remote_user - The remote user id of the user to close the connection.
 * Description: Closes the connection with the given remote user. This is done on the EOS side
 * and is called right before the peer with the remote user id is removed from the multiplayer
 * instance (see _disconnect_peer()).
 ****************************************/
void EOSGMultiplayerPeer::_disconnect_remote_user(const EOS_ProductUserId &remote_user) {
    EOS_P2P_CloseConnectionOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.RemoteUserId = remote_user;
    options.SocketId = socket.get_id();
    EOS_EResult result = IEOS::get_singleton()->p2p_close_connection(&options);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to close peer connection. Invalid parameters.");
}

/****************************************
 * _peer_connection_established_callback
 * Parameters:
 *   data - data retrieved from the connection established callback.
 * Description: A callback that is called by EOSGPacketPeerMediator when it receives a peer connection
 * established notification. The data contains the local user id of this instance, the remote user id of
 * peer who just connected, the socket id that the remote peer connected to, the connection type (which tells
 * you whether it was a new connection or a re-connection), and the network type (which tells you if it is a direct
 * connection or a relay server is being used.) When a connection is established for the first time and the instance
 * is not a client, a packet will be sent back to the newly connected peer with the EVENT_RECIEVE_PEER_ID event.
 * This is how servers and mesh instances peers exchange their peer ids with newly connected peers.
 ****************************************/
void EOSGMultiplayerPeer::peer_connection_established_callback(const EOS_P2P_OnPeerConnectionEstablishedInfo *data) {
    if (data->ConnectionType == EOS_EConnectionEstablishedType::EOS_CET_NewConnection &&
            active_mode != MODE_CLIENT) { //We're either a server or mesh
        //Send peer id to connected peer
        EOSGPacket packet;
        packet.set_event(EVENT_RECIEVE_PEER_ID);
        packet.set_channel(CH_RELIABLE);
        packet.set_reliability(EOS_EPacketReliability::EOS_PR_ReliableUnordered);
        packet.set_sender(unique_id);
        packet.prepare();

        Error result = _send_to(data->RemoteUserId, packet);
    }

    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    int connection_type = static_cast<int>(data->ConnectionType);
    int network_type = static_cast<int>(data->NetworkType);

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = data->SocketId->SocketName;
    ret["connection_type"] = connection_type;
    ret["network_type"] = network_type;
    emit_signal("peer_connection_established", ret);
}

/****************************************
 * remote_connection_closed_callback
 * Parameters:
 *   data - data retrieved from the connection closed callback.
 * Description: A callback that is called by EOSGPacketPeerMediator when it receives a remote connection
 * closed notification. The data contains the local user id of this instance, the remote user id of
 * peer whose connection has closed, the socket id that the remote peer was previously connected to,
 * and the reason for the disconnection (Which could be a LOT of different reasons, like timeouts, failed
 * connections, remotely disconnected, etc.). When a connection is closed, the instance checks if there
 * was a connection request associated with the connection. If there was, then remove the connection request
 * from the pending connection requests. If the peer was already previously connected, remove the peer
 * from the instance. If this instance is a client when the callback is called it either means the client
 * has disconnected locally or connection with the server has failed.
 ****************************************/
void EOSGMultiplayerPeer::remote_connection_closed_callback(const EOS_P2P_OnRemoteConnectionClosedInfo *data) {
    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    int reason = static_cast<int>(data->Reason);

    //attempt to remove connection request if there was one.
    EOS_ProductUserId remote_user_id;
    if (_find_connection_request(remote_user_id_str, remote_user_id)) {
        pending_connection_requests.erase(remote_user_id);
    }

    //Attempt to remove peer;
    int peer_id = get_peer_id(remote_user_id_str);
    if (peer_id != 0) {
        _clear_peer_packet_queue(peer_id);
        peers.erase(peer_id);
        emit_signal("peer_disconnected", peer_id);
    }

    //If this callback is called when we're a client and the reason isn't due to the local user closing the connection,
    //it probably means the connection to the server has failed. Close the peer in this case.
    if (active_mode == MODE_CLIENT && data->Reason != EOS_EConnectionClosedReason::EOS_CCR_ClosedByLocalUser) {
        EOSGPacketPeerMediator::get_singleton()->unregister_peer(this);
        _close();
    }

    //The peer has closed their connection. Once all peers have been removed, unregister from the mediator
    if (connection_status == CONNECTION_DISCONNECTED && peers.is_empty()) {
        EOSGPacketPeerMediator::get_singleton()->unregister_peer(this);
    }

    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = data->SocketId->SocketName;
    ret["reason"] = reason;
    emit_signal("peer_connection_closed", ret);
}

/****************************************
 * peer_connection_interrupted_callback
 * Parameters:
 *   data - data retrieved from the connection interrupted callback.
 * Description: A callback that is called by EOSGPacketPeerMediator when it receives a peer connection
 * interrupted notification. Callback only emits a signal containing the data when it is called.
 ****************************************/
void EOSGMultiplayerPeer::peer_connection_interrupted_callback(const EOS_P2P_OnPeerConnectionInterruptedInfo *data) {
    String local_user_id_str = eosg_product_user_id_to_string(data->LocalUserId);
    String remote_user_id_str = eosg_product_user_id_to_string(data->RemoteUserId);
    Dictionary ret;
    ret["local_user_id"] = local_user_id_str;
    ret["remote_user_id"] = remote_user_id_str;
    ret["socket"] = data->SocketId->SocketName;
    emit_signal("peer_connection_interrupted", ret);
}

/****************************************
 * connection_request_callback
 * Parameters:
 *   data - Contains data about the connection request received.
 * Description: A callback that is called by EOSGPacketPeerMediator when it receives a connection request
 * for this multiplayer instance. If this instance is a server or a mesh, the connection request is pushed
 * to the list of pending connection requests. If this instance is a client, the connection request is ignored.
 * If auto-accepting connection requests is set to true, the connection request is accepted immediately.
 ****************************************/
void EOSGMultiplayerPeer::connection_request_callback(const ConnectionRequestData &data) {
    if (active_mode == MODE_CLIENT || is_refusing_new_connections())
        return;

    EOS_ProductUserId remote_user_id = eosg_string_to_product_user_id(data.remote_user_id.utf8());

    pending_connection_requests.push_back(remote_user_id);

    Dictionary ret;
    ret["local_user_id"] = data.local_user_id;
    ret["remote_user_id"] = data.remote_user_id;
    ret["socket"] = String(data.socket_name);
    emit_signal("incoming_connection_request", ret);

    if (!is_auto_accepting_connection_requests())
        return;

    accept_connection_request(data.remote_user_id);
}

/****************************************
 * ~EOSGMultiplayerPeer
 * Description: Destructor. Closes the multiplayer instance if it is still active.
 ****************************************/
EOSGMultiplayerPeer::~EOSGMultiplayerPeer() {
    if (active_mode != MODE_NONE) {
        _close();
    }
}

/****************************************
 * EOSGPacket::prepare
 * Description: Prepares the packet for sending. Allocates memory for the packet if not done so already.
 * Sets the packet header.
 ****************************************/
void EOSGMultiplayerPeer::EOSGPacket::prepare() {
    if (packet == nullptr) {
        _alloc_packet();
    }

    packet->ptrw()[INDEX_EVENT_TYPE] = static_cast<uint8_t>(event);
    packet->ptrw()[INDEX_TRANSFER_MODE] = static_cast<uint8_t>(reliability);
    memcpy(packet->ptrw() + INDEX_PEER_ID, &from, sizeof(int));
}

/****************************************
 * EOSGPacket::store_payload
 * Parameters:
 *   payload_data - pointer to the payload data to be stored in the packet.
 *   payload_size_bytes - size of the payload in bytes.
 * Description: Stores the passed payload data into the packet. Allocates the packet to the correct
 * size so that it can fit the payload.
 ****************************************/
void EOSGMultiplayerPeer::EOSGPacket::store_payload(const uint8_t *payload_data, const uint32_t payload_size_bytes) {
    if (packet == nullptr) {
        _alloc_packet(payload_size_bytes + PACKET_HEADER_SIZE);
    }
    if (packet->size() != payload_size_bytes + PACKET_HEADER_SIZE) {
        size_bytes = payload_size_bytes + PACKET_HEADER_SIZE;
        packet->resize(size_bytes);
    }
    memcpy(packet->ptrw() + INDEX_PAYLOAD_DATA, payload_data, payload_size_bytes);
}

/****************************************
 * EOSGSocket::close
 * Description: Closes the socket. Closes all connections with peers who are currently
 * connected. Clears the packet queue.
 ****************************************/
void EOSGMultiplayerPeer::EOSGSocket::close() {
    clear_packet_queue();

    EOS_P2P_CloseConnectionsOptions options;
    options.ApiVersion = EOS_P2P_CLOSECONNECTIONS_API_LATEST;
    options.LocalUserId = s_local_user_id;
    options.SocketId = &socket;
    IEOS::get_singleton()->p2p_close_all_connections(&options);
}

/****************************************
 * EOSGSocket::close
 * Description: Closes the socket. Closes all connections with peers who are currently
 * connected. Clears the packet queue.
 ****************************************/
void EOSGMultiplayerPeer::EOSGSocket::clear_packets_from_peer(int p_peer) {
    List<List<EOSGPacket>::Element *> del;
    for (List<EOSGPacket>::Element *e = incoming_packets.front(); e != nullptr; e = e->next()) {
        if (e->get().get_sender() != p_peer)
            continue;
        del.push_back(e);
    }
    for (List<EOSGPacket>::Element *e : del) {
        e->erase();
    }
}

/****************************************
 * EOSGSocket::_socket_id_is_valid
 * Parameters:
 *  socket_id: The socket id to validate.
 * Description: Validates a socket id. Checks if there are any invalid characters.
 * Also checks if the socket id is empty and if it meets the length requirements.
 * Socket id's can only contain alpha-numeric characters. If there
 * are any invalid characters in the socket id or if it's empty or exceeds the
 * character length limit, the method returns false.
 ****************************************/
bool EOSGMultiplayerPeer::EOSGSocket::_socket_id_is_valid(const String &socket_id) {
    if (socket_id.is_empty())
        return false; //socket id should not be empty

    if (socket_id.length() >= EOS_P2P_SOCKETID_SOCKETNAME_SIZE) {
        return false; //socket id should not be longer than EOS_P2P_SOCKETID_SOCKETNAME_SIZE - 1 characters
    }

    //The ranges of certain characters in the ascii table.
    int numeric_range_min = 48;
    int numeric_range_max = 57;
    int alpha_capitalized_range_min = 65;
    int alpha_capitalized_range_max = 90;
    int alpha_lowercase_range_min = 97;
    int alpha_lowercase_range_max = 122;

    CharString str = socket_id.ascii();
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= numeric_range_min && str[i] <= numeric_range_max) {
            continue;
        } else if (str[i] >= alpha_capitalized_range_min && str[i] <= alpha_capitalized_range_max) {
            continue;
        } else if (str[i] >= alpha_lowercase_range_min && str[i] <= alpha_lowercase_range_max) {
            continue;
        } else {
            return false; //Invalid character found
        }
    }
    return true;
}
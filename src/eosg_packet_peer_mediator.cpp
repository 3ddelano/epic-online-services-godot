/****************************************
* EOSGPacketPeerMediator
* Author: Dallin Lovin aka LowFire
* Description: Manages EOSG multiplayer instances when they are active.
* Multiplayer instances register their socket id with the mediator when 
* they become active and unregister their socket id when they close.
* The mediator recieves packets from the EOS P2P interface every process
* frame and sorts those packets according to their destination socket so
* that the appropriate multiplayer instance can poll them later. The mediator
* recieves EOS notifications and fowards it to the appropriate multiplayer
* instance according to the socket the notification was receieved from. 
* Mediator manages incoming connection requests and fowards them to the
* appropriate multiplayer instance according to the socket id of the
* connection request. If there is no matching socket from any of the active
* multiplayer instances, the mediator will hold onto the connection request
* until either a muliplayer instance opens with a matching socket or until
* the connection request times out.
****************************************/

#include "eosg_packet_peer_mediator.h"
#include "godot_cpp/classes/rendering_server.hpp"
#include "utils.h"

EOSGPacketPeerMediator *EOSGPacketPeerMediator::singleton = nullptr;

void EOSGPacketPeerMediator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_process_frame"), &EOSGPacketPeerMediator::_on_process_frame);
	ClassDB::bind_method(D_METHOD("_on_connect_interface_login"), &EOSGPacketPeerMediator::_on_connect_interface_login);
	ClassDB::bind_method(D_METHOD("get_total_packet_count"), &EOSGPacketPeerMediator::get_total_packet_count);
	ClassDB::bind_method(D_METHOD("get_sockets"), &EOSGPacketPeerMediator::get_sockets);
	ClassDB::bind_method(D_METHOD("get_packet_count_for_socket", "socket_id"), &EOSGPacketPeerMediator::get_packet_count_for_socket);
	ClassDB::bind_method(D_METHOD("has_socket"), &EOSGPacketPeerMediator::has_socket);
	ClassDB::bind_method(D_METHOD("get_packet_count_from_remote_user"), &EOSGPacketPeerMediator::get_packet_count_from_remote_user);
	ClassDB::bind_method(D_METHOD("get_queue_size_limit"), &EOSGPacketPeerMediator::get_queue_size_limit);
	ClassDB::bind_method(D_METHOD("set_queue_size_limit", "limit"), &EOSGPacketPeerMediator::set_queue_size_limit);
	ClassDB::bind_method(D_METHOD("get_connection_request_count"), &EOSGPacketPeerMediator::get_connection_request_count);

	ADD_SIGNAL(MethodInfo("packet_queue_full"));
	ADD_SIGNAL(MethodInfo("connection_request_received", PropertyInfo(Variant::DICTIONARY, "callback_data")));
	ADD_SIGNAL(MethodInfo("connection_request_removed", PropertyInfo(Variant::DICTIONARY, "callback_data")));
}

/****************************************
* _on_process_frame
* Description: Method is connected to the game main loop's process signal so
* that it can execute every process frame (see _init()). Checks if there are
* any packets available from the incoming packet queue. If there are, recieves
* the packet and sorts it into separate queues according to it's destination socket. Packets that
* are peer id packets (packets with EVENT_RECIEVE_PEER_ID) and pushed to the front. Packets will
* stop being polled if the queue size limit is reached.
****************************************/
void EOSGPacketPeerMediator::_on_process_frame() {
	if (EOSGMultiplayerPeer::get_local_user_id().is_empty()) return;
	if (socket_packet_queues.size() == 0) return;
	if (get_total_packet_count() >= max_queue_size) return;

	String local_user_id_str = EOSGMultiplayerPeer::get_local_user_id();
	EOS_ProductUserId local_user_id = eosg_string_to_product_user_id(local_user_id_str.utf8());
	EOS_P2P_GetNextReceivedPacketSizeOptions packet_size_options;
    packet_size_options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
    packet_size_options.LocalUserId = local_user_id;
    packet_size_options.RequestedChannel = nullptr;
    uint32_t max_packet_size;

	EOS_P2P_ReceivePacketOptions recieve_packet_options;
	recieve_packet_options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
	recieve_packet_options.LocalUserId = local_user_id;
	recieve_packet_options.MaxDataSizeBytes = EOS_P2P_MAX_PACKET_SIZE;
	recieve_packet_options.RequestedChannel = nullptr;

	bool next_packet_available = true;
	EOS_EResult result = EOS_EResult::EOS_Success;

	do {
		result = IEOS::get_singleton()->p2p_get_next_packet_size(&packet_size_options, &max_packet_size);

		ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to get packet size! Invalid parameters.");

		if (result == EOS_EResult::EOS_Success) {
			next_packet_available = true;
		}
		else {
			next_packet_available = false;
		}

		if (next_packet_available) {
			PackedByteArray packet_data;
			packet_data.resize(max_packet_size);
			uint32_t buffer_size;
			uint8_t channel;
			EOS_P2P_SocketId socket;
			EOS_ProductUserId remote_user;
			result = IEOS::get_singleton()->p2p_receive_packet(&recieve_packet_options, packet_data.ptrw(), &buffer_size, &channel, &remote_user, &socket);
			String socket_id_str = socket.SocketName;

			ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to get packet! Invalid parameters.");
			ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_NotFound, "Failed to get packet! Packet is too large. This should not have happened.");

			if (!socket_packet_queues.has(socket_id_str)) return; //invalid socket. Drop the packet.

			PacketData packet;
			packet.store(packet_data.ptrw(), max_packet_size);
			packet.set_channel(channel);
			packet.set_sender(remote_user);
			uint8_t event = packet.get_data()->ptrw()[0];
			if (event == 1) {
					socket_packet_queues[socket_id_str].push_front(packet);
			} else {
					socket_packet_queues[socket_id_str].push_back(packet);
			}
			if (get_total_packet_count() >= max_queue_size) {
				emit_signal("packet_queue_full");
				break;
			}
		}
	} while (next_packet_available);
}

/****************************************
* poll_next_packet
* Parameters:
*   socket_id - The socket to poll a packet from.
*   out_packet - An out parameter that returns the polled packet.
* Description: Polls the next packet available for the given socket.
* Returns true if a packet has been successfully polled. False otherwise.
****************************************/
bool EOSGPacketPeerMediator::poll_next_packet(const String &socket_id, PacketData *out_packet) {
	if (!socket_packet_queues.has(socket_id)) return false;
	if (socket_packet_queues[socket_id].size() == 0) return false;

	PacketData next_packet = socket_packet_queues[socket_id].front()->get();
	*out_packet = next_packet;
	socket_packet_queues[socket_id].pop_front();
	return true;
}

/****************************************
* register_peer
* Parameters:
*   peer - The peer to be registered with the mediator.
* Description: Registers a peer and it's socket with the mediator.
* Once registered, a peer can recieve packets, EOS notifications, and connection requests.
****************************************/
bool EOSGPacketPeerMediator::register_peer(EOSGMultiplayerPeer *peer) {
	ERR_FAIL_COND_V_MSG(!initialized, false, "Failed to register peer. EOSGPacketPeerMediator has not been initialized. Call EOSGPacketPeerMediator.init() before starting a multiplayer instance.");
	ERR_FAIL_COND_V_MSG(peer->get_socket().is_empty(), false, "Failed to register peer. Peer is not active.");
	ERR_FAIL_COND_V_MSG(active_peers.has(peer->get_socket()), false, "Failed to register peer. This peer has already been registered.");

	active_peers.insert(peer->get_socket(), peer);
	socket_packet_queues.insert(peer->get_socket(), List<PacketData>());

	_forward_pending_connection_requests(peer);

	return true;
}

/****************************************
* unregister_peer
* Parameters:
*   peer - The peer to be unregistered with the mediator.
* Description: Unregisteres a peer and it's socket with the mediator.
* Peers can no longer recieved packets, notifications, or connection requests once this is done.
* unregistration usually happens when a peer closes.
****************************************/
void EOSGPacketPeerMediator::unregister_peer(EOSGMultiplayerPeer *peer) {
	ERR_FAIL_COND_MSG(!active_peers.has(peer->get_socket()), "Failed to unregister peer. This peer has not been previously registered.");

	clear_packet_queue(peer->get_socket());
	socket_packet_queues.erase(peer->get_socket());
	active_peers.erase(peer->get_socket());
}

/****************************************
* clear_packet_queue
* Parameters:
*   socket_id - The socket to clear packets from.
* Description: Removes all packets queued for the given socket.
****************************************/
void EOSGPacketPeerMediator::clear_packet_queue(const String &socket_id) {
	ERR_FAIL_COND_MSG(!socket_packet_queues.has(socket_id), vformat("Failed to clear packet queue for socket \"%s\". Socket was not registered.", socket_id));

	socket_packet_queues[socket_id].clear();
}

/****************************************
* clear_packets_from_remote_user
* Parameters:
*   socket_id - The socket to clear packets from.
*	remote_user_id - The user to remove packets from.
* Description: Removes all packets queued for the given socket and from the given remote user.
* This is usually called when a peer disconnects. All packets from that peer are removed.
****************************************/
void EOSGPacketPeerMediator::clear_packets_from_remote_user(const String &socket_id, const String &remote_user_id) {
	ERR_FAIL_COND_MSG(!socket_packet_queues.has(socket_id), vformat("Failed to clear packet queue for socket \"%s\". Socket was not registered.", socket_id));

	List<List<PacketData>::Element*> del;
	for (List<PacketData>::Element *e = socket_packet_queues[socket_id].front(); e != nullptr; e = e->next()) {
		if (e->get().get_sender() != remote_user_id) continue;
		del.push_back(e);
	}
	for (List<PacketData>::Element *e : del) {
		e->erase();
	}
}

/****************************************
* _init
* Description: Initialized EOSGPacketPeerMediator. Connects _on_process_frame to the
* main loop's process signal. Add's EOS callbacks so that it can recieve notifications.
****************************************/
void EOSGPacketPeerMediator::_init() {
	ERR_FAIL_COND_MSG(EOSGMultiplayerPeer::get_local_user_id().is_empty(), "Failed to initialize EOSGPacketPeerMediator. Local user id has not been set.");
	if (initialized) return;

	MainLoop *main_loop = Engine::get_singleton()->get_main_loop();
	ERR_FAIL_COND_MSG(!main_loop->has_signal("process_frame"), "Failed to initialize EOSGPacketPeerMediator. Main loop does not have the process_frame() signal.");
	main_loop->connect("process_frame", process_frame_callback);

		//Register callbacks
	_add_connection_closed_callback();
	_add_connection_established_callback();
	_add_connection_interrupted_callback();
	_add_connection_request_callback();

	initialized = true;
}

/****************************************
* _terminate
* Description: terminates EOSGPacketPeerMediator. Disconnects from the
* main loop's process signal. Removes all EOS callbacks.
****************************************/
void EOSGPacketPeerMediator::_terminate() {
	if (!initialized) return;

	MainLoop *main_loop = Engine::get_singleton()->get_main_loop();
	main_loop->disconnect("process_frame", process_frame_callback);

	//Unregister callbacks
	IEOS::get_singleton()->p2p_remove_notify_peer_connection_established(connection_established_callback_id);
	IEOS::get_singleton()->p2p_remove_notify_peer_connection_interrupted(connection_interrupted_callback_id);
	IEOS::get_singleton()->p2p_remove_notify_peer_connection_closed(connection_closed_callback_id);
	IEOS::get_singleton()->p2p_remove_notify_peer_connection_request(connection_request_callback_id);

	initialized = false;
}

/****************************************
* get_packet_count_from_remote_user
* Parameters:
*	remote_user - The user to count packets for.
* 	socket_id - The socket to count packets for.
* Description: Counts the number of packets from the given remote user and for
* the given socket. Returns the packet count.
****************************************/
int EOSGPacketPeerMediator::get_packet_count_from_remote_user(const String &remote_user, const String &socket_id) {
	ERR_FAIL_COND_V_MSG(!socket_packet_queues.has(socket_id), 0, vformat("Failed to get packet count for remote user. Socket \"%s\" does not exist", socket_id));
	int ret = 0;
	for (PacketData &data : socket_packet_queues[socket_id]) {
		if (data.get_sender() == remote_user) {
			ret++;
		}
	}
	return ret;
}

/****************************************
* next_packet_is_peer_id_packet
* Parameters:
* 	socket_id - The socket to check
* Description: Checks if there is a peer id packet queued for the given socket.
* Returns true if there is, false otherwise.
****************************************/
bool EOSGPacketPeerMediator::next_packet_is_peer_id_packet(const String &socket_id) {
	ERR_FAIL_COND_V_MSG(!socket_packet_queues.has(socket_id), false, "Failed to check next packet. Socket \"%s\" does not exist.");
	if (socket_packet_queues[socket_id].size() == 0) return false;
	PacketData packet = socket_packet_queues[socket_id][0];
	uint8_t event = packet.get_data()->ptrw()[0];
	if (event == 1) return true;
	return false;
}

/****************************************
* _on_peer_connection_established
* Parameters:
* 	data - Data returned from the notification
* Description: An EOS callback that is called when connection is established with a peer.
* Forwards the data to the appropriate multiplayer instance using the socket id provided in the data.
****************************************/
void EOS_CALL EOSGPacketPeerMediator::_on_peer_connection_established(const EOS_P2P_OnPeerConnectionEstablishedInfo *data) {
	String socket_id = data->SocketId->SocketName;
	if (!singleton->active_peers.has(socket_id)) return;
	singleton->active_peers[socket_id]->peer_connection_established_callback(data);
}

/****************************************
* _on_peer_connection_interrupted
* Parameters:
* 	data - Data returned from the notification
* Description: An EOS callback that is called when connection with a peer is interrupted.
* Forwards the data to the appropriate multiplayer instance using the socket id provided in the data.
****************************************/
void EOS_CALL EOSGPacketPeerMediator::_on_peer_connection_interrupted(const EOS_P2P_OnPeerConnectionInterruptedInfo *data) {
	String socket_id = data->SocketId->SocketName;
	if (!singleton->active_peers.has(socket_id)) return;
	singleton->active_peers[socket_id]->peer_connection_interrupted_callback(data);
}

/****************************************
* _on_remote_connection_closed
* Parameters:
* 	data - Data returned from the notification
* Description: An EOS callback that is called when connection with a peer is closed.
* Checks to see if there were any connection requests associated with the closed connection.
* If so, it removes that connection request. Forwards the data to the appropriate multiplayer instance using 
* the socket id provided in the data.
****************************************/
void EOS_CALL EOSGPacketPeerMediator::_on_remote_connection_closed(const EOS_P2P_OnRemoteConnectionClosedInfo *data) {
	String socket_name = data->SocketId->SocketName;
	//Check if there are any connection requests that need to be removed.
	List<ConnectionRequestData>::Element *e = singleton->pending_connection_requests.front();
	for (; e != nullptr; e = e->next()) {
		String request_remote_user_id = e->get().remote_user_id.utf8();
		String closed_remote_user_id = eosg_product_user_id_to_string(data->RemoteUserId);
		String request_socket_name = e->get().socket_name;
		if (request_remote_user_id == closed_remote_user_id && socket_name == request_socket_name) {
			Dictionary ret;
			ret["local_user_id"] = e->get().local_user_id;
			ret["remote_user_id"] = e->get().remote_user_id;
			ret["socket_id"] = e->get().socket_name;
			singleton->emit_signal("connection_request_removed", ret);
			e->erase();
			break;
		}
	}
	if (!singleton->active_peers.has(socket_name)) return;
	singleton->active_peers[socket_name]->remote_connection_closed_callback(data);
}

/****************************************
* _on_incoming_connection_request
* Parameters:
* 	data - Data returned from the notification
* Description: An EOS callback that is called when a connection request is recieved.
* Checks if there are any peers available to recieve the connection request using
* the destination socket id. If there isn't, stores the connection request for later.
* If there is, forward the connection request to that multiplayer instance.
****************************************/
void EOS_CALL EOSGPacketPeerMediator::_on_incoming_connection_request(const EOS_P2P_OnIncomingConnectionRequestInfo *data) {
	ConnectionRequestData request_data;
	request_data.local_user_id = eosg_product_user_id_to_string(data->LocalUserId);
	request_data.remote_user_id = eosg_product_user_id_to_string(data->RemoteUserId);
	request_data.socket_name = data->SocketId->SocketName;
	if (!singleton->active_peers.has(request_data.socket_name)) {
		//Hold onto the connection request just in case a socket does get opened with this socket id
		singleton->pending_connection_requests.push_back(request_data);
		Dictionary ret;
		ret["local_user_id"] = request_data.local_user_id;
		ret["remote_user_id"] = request_data.remote_user_id;
		ret["socket_id"] = request_data.socket_name;
		singleton->emit_signal("connection_request_received", ret);
		return;
	}
	singleton->active_peers[request_data.socket_name]->connection_request_callback(request_data);
}

/****************************************
* _on_connect_interface_login
* Parameters:
* 	data - Contains info about the login.
* Description: Called when the user logs into the connect interface. Sets the
* local user id recieved from the login and initialized EOSGPacketPeerMediator.
****************************************/
void EOSGPacketPeerMediator::_on_connect_interface_login(Dictionary data) {
	String local_user_id = data["local_user_id"].operator godot::String();
    if (local_user_id != "") {
        EOSGMultiplayerPeer::set_local_user_id(local_user_id);
        EOSGPacketPeerMediator::get_singleton()->_init();
    } else {
		ERR_FAIL_MSG("Local user id was not set on connect interface login.");
	}
	IEOS::get_singleton()->disconnect("connect_interface_login_callback", connect_interface_login_callback);
}

/****************************************
* _on_connect_interface_login
* Parameters:
* 	data - Contains info about the login.
* Description: Called when the user logs into the connect interface. Sets the
* local user id recieved from the login and initialized EOSGPacketPeerMediator.
****************************************/
bool EOSGPacketPeerMediator::_add_connection_established_callback() {
	String local_user_id_str = EOSGMultiplayerPeer::get_local_user_id();
	EOS_ProductUserId local_user_id = eosg_string_to_product_user_id(local_user_id_str.utf8());
    EOS_P2P_AddNotifyPeerConnectionEstablishedOptions connection_established_options;
    connection_established_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST;
    connection_established_options.LocalUserId = local_user_id;
    connection_established_options.SocketId = nullptr;
    connection_established_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_established(&connection_established_options, 
        _on_peer_connection_established);
    ERR_FAIL_COND_V_MSG(connection_established_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection established callback.");
	return true;
}

/****************************************
* _add_connection_interrupted_callback
* Description: Adds the connection interrupted callback. This is called
* in _init()
****************************************/
bool EOSGPacketPeerMediator::_add_connection_interrupted_callback() {
	String local_user_id_str = EOSGMultiplayerPeer::get_local_user_id();
	EOS_ProductUserId local_user_id = eosg_string_to_product_user_id(local_user_id_str.utf8());
    EOS_P2P_AddNotifyPeerConnectionInterruptedOptions connection_interrupted_options;
    connection_interrupted_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST;
    connection_interrupted_options.LocalUserId = local_user_id;
    connection_interrupted_options.SocketId = nullptr;
    connection_interrupted_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_interrupted(&connection_interrupted_options,
        _on_peer_connection_interrupted);
    ERR_FAIL_COND_V_MSG(connection_interrupted_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection interrupted callback.");
	return true;
}

/****************************************
* _add_connection_closed_callback
* Description: Adds the connection closed callback. This is called
* in _init()
****************************************/
bool EOSGPacketPeerMediator::_add_connection_closed_callback() {
	String local_user_id_str = EOSGMultiplayerPeer::get_local_user_id();
	EOS_ProductUserId local_user_id = eosg_string_to_product_user_id(local_user_id_str.utf8());
    EOS_P2P_AddNotifyPeerConnectionClosedOptions connection_closed_options;
    connection_closed_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
    connection_closed_options.LocalUserId = local_user_id;
    connection_closed_options.SocketId = nullptr;
    connection_closed_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_closed(&connection_closed_options,
        _on_remote_connection_closed);
    ERR_FAIL_COND_V_MSG(connection_closed_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection closed callback.");
	return true;
}

/****************************************
* _add_connection_request_callback
* Description: Adds the connection request callback. This is called
* in _init()
****************************************/
bool EOSGPacketPeerMediator::_add_connection_request_callback() {
	String local_user_id_str = EOSGMultiplayerPeer::get_local_user_id();
	EOS_ProductUserId local_user_id = eosg_string_to_product_user_id(local_user_id_str.utf8());
    EOS_P2P_AddNotifyPeerConnectionRequestOptions connection_request_options;
    connection_request_options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
    connection_request_options.LocalUserId = local_user_id;
    connection_request_options.SocketId = nullptr;
    connection_request_callback_id = IEOS::get_singleton()->p2p_add_notify_peer_connection_request(&connection_request_options,
        _on_incoming_connection_request);
    ERR_FAIL_COND_V_MSG(connection_request_callback_id == EOS_INVALID_NOTIFICATIONID, false, "Failed to add connection request callback.");
	return true;
}

/****************************************
* _forward_pending_connection_requests
* Parameters:
*	peer - The peer to forward connection requests to.
* Description: Attempts to foward any pending connection requests to the given multiplayer instance.
* If none of the pending requests match the multiplayer instance's socket, then no connection requests
* are forwarded.
****************************************/
void EOSGPacketPeerMediator::_forward_pending_connection_requests(EOSGMultiplayerPeer *peer) {
	List<ConnectionRequestData>::Element *e = pending_connection_requests.front();
	List<List<ConnectionRequestData>::Element*> del;
	for(; e != nullptr; e = e->next()) {
		if (peer->get_socket() != e->get().socket_name) continue;
		peer->connection_request_callback(e->get());
		del.push_back(e);
	}
	for(List<ConnectionRequestData>::Element* e : del) {
		Dictionary ret;
		ret["local_user_id"] = e->get().local_user_id;
		ret["remote_user_id"] = e->get().remote_user_id;
		ret["socket_id"] = e->get().socket_name;
		emit_signal("connection_request_removed", ret);
		e->erase();
	}
}

/****************************************
* EOSGPacketPeerMediator
* Description: Default constructor. Sets the singleton. Connects
* to the connect interface login callback so that the class knows
* when to initialize.
****************************************/
EOSGPacketPeerMediator::EOSGPacketPeerMediator() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "EOSGPacketPeerMediator already initialized");
	singleton = this;

	process_frame_callback = Callable(this, "_on_process_frame");

	connect_interface_login_callback = Callable(this, "_on_connect_interface_login");
	IEOS::get_singleton()->connect("connect_interface_login_callback", connect_interface_login_callback);
}

/****************************************
* !EOSGPacketPeerMediator
* Description: Destructor. Sets singleton to null.
****************************************/
EOSGPacketPeerMediator::~EOSGPacketPeerMediator() {
	if (singleton != this) return;
	singleton = nullptr;
}
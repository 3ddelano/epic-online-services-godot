#include "eosg_packet_peer_mediator.h"
#include "utils.h"

EOSGPacketPeerMediator *EOSGPacketPeerMediator::singleton = nullptr;

void EOSGPacketPeerMediator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_process_frame"), &EOSGPacketPeerMediator::_on_process_frame);

	ADD_SIGNAL(MethodInfo("packet_queue_full"));
}

void EOSGPacketPeerMediator::_on_process_frame() {
	ERR_FAIL_COND_MSG(EOSGMultiplayerPeer::get_local_user_id().is_empty(), "Local user id is not set");
	if (registered_sockets.size() == 0) return;
	if (total_bytes_stored >= MAX_QUEUE_SIZE_BYTES) return;

	String local_user_id_str = EOSGMultiplayerPeer::get_local_user_id();
	EOS_ProductUserId local_user_id = eosg_string_to_product_user_id(local_user_id_str.utf8());
	EOS_P2P_GetNextReceivedPacketSizeOptions packet_size_options;
    packet_size_options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
    packet_size_options.LocalUserId = local_user_id;
    packet_size_options.RequestedChannel = nullptr;
    uint32_t max_packet_size;
    EOS_EResult result = IEOS::get_singleton()->p2p_get_next_packet_size(&packet_size_options, &max_packet_size);

    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to get packet size! Invalid parameters.");

    if (result == EOS_EResult::EOS_NotFound) return; //No packets found

	total_bytes_stored += max_packet_size;
	if (total_bytes_stored >= MAX_QUEUE_SIZE_BYTES) {
		emit_signal("packet_queue_full");
	}

	EOS_P2P_ReceivePacketOptions options;
    options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
    options.LocalUserId = local_user_id;
    options.MaxDataSizeBytes = EOS_P2P_MAX_PACKET_SIZE;
    options.RequestedChannel = nullptr;

	PackedByteArray packet_data;
    packet_data.resize(max_packet_size);
    uint32_t buffer_size;
    uint8_t channel;
    EOS_P2P_SocketId socket;
    EOS_ProductUserId remote_user;
    result = IEOS::get_singleton()->p2p_receive_packet(&options, packet_data.ptrw(), &buffer_size, &channel, &remote_user, &socket);
	String socket_id_str = socket.SocketName;

	ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_InvalidParameters, "Failed to get packet! Invalid parameters.");
    ERR_FAIL_COND_MSG(result == EOS_EResult::EOS_NotFound, "Failed to get packet! Packet is too large. This should not have happened.");

	if (!registered_sockets.has(socket_id_str)) return; //invalid socket. Drop the packet.

	PacketData packet;
	packet.store(packet_data.ptrw(), max_packet_size);
	packet.set_channel(channel);
	packet.set_sender(remote_user);
	registered_sockets[socket_id_str].push_back(packet);
}

bool EOSGPacketPeerMediator::poll_next_packet(const String &socket_id, PacketData *out_packet) {
	if (!registered_sockets.has(socket_id)) return false;
	if (registered_sockets[socket_id].size() == 0) return false;

	PacketData next_packet = registered_sockets[socket_id].front()->get();
	*out_packet = next_packet;
	total_bytes_stored -= next_packet.size();
	registered_sockets[socket_id].pop_front();
	return true;
}

void EOSGPacketPeerMediator::register_socket(const String &socket_id) {
	ERR_FAIL_COND_MSG(socket_id.is_empty(), "Failed to register socket. Socket id is invalid. Provided an empty string.");
	ERR_FAIL_COND_MSG(registered_sockets.has(socket_id), vformat("Failed to register socket. socket \"%s\" has already been registered.", socket_id));

	registered_sockets.insert(socket_id, List<PacketData>());
}

void EOSGPacketPeerMediator::unregister_socket(const String &socket_id) {
	ERR_FAIL_COND_MSG(!registered_sockets.has(socket_id), vformat("Failed to unregister socket. Socket \"%s\" does not exist.", socket_id));

	clear_packet_queue(socket_id);
	registered_sockets.erase(socket_id);
}

void EOSGPacketPeerMediator::clear_packet_queue(const String &socket_id) {
	ERR_FAIL_COND_MSG(!registered_sockets.has(socket_id), vformat("Failed to clear packet queue for socket \"%s\". Socket was not registered.", socket_id));

	for (PacketData &packet : registered_sockets[socket_id]) {
		total_bytes_stored -= packet.size();
	}
	registered_sockets[socket_id].clear();
}

void EOSGPacketPeerMediator::clear_packets_from_remote_user(const String &socket_id, const String &remote_user_id) {
	ERR_FAIL_COND_MSG(!registered_sockets.has(socket_id), vformat("Failed to clear packet queue for socket \"%s\". Socket was not registered.", socket_id));

	List<List<PacketData>::Element*> del;
	for (List<PacketData>::Element *e = registered_sockets[socket_id].front(); e != nullptr; e = e->next()) {
		if (e->get().get_sender() != remote_user_id) continue;
		del.push_back(e);
	}
	for (List<PacketData>::Element *e : del) {
		total_bytes_stored -= e->get().size();
		e->erase();
	}
}

EOSGPacketPeerMediator::EOSGPacketPeerMediator() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "EOSGPacketPeerMediator already initialized");

	singleton = this;
	MainLoop *main_loop = Engine::get_singleton()->get_main_loop();
	ERR_FAIL_COND_MSG(!main_loop->has_signal("process_frame"), "Failed to initialize EOSGPacketPeerMediator. Main loop does not have the process_frame() signal.");
	Callable callback(this, "_on_procces_frame");
	main_loop->connect("process_frame", callback);
}

EOSGPacketPeerMediator::~EOSGPacketPeerMediator() {
		if (singleton != this) return;
		singleton = nullptr;
}
#include "eosg_packet_peer_mediator.h"
#include "godot_cpp/classes/rendering_server.hpp"
#include "utils.h"

EOSGPacketPeerMediator *EOSGPacketPeerMediator::singleton = nullptr;

void EOSGPacketPeerMediator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_process_frame"), &EOSGPacketPeerMediator::_on_process_frame);
	ClassDB::bind_method(D_METHOD("get_total_packet_count"), &EOSGPacketPeerMediator::get_total_packet_count);
	ClassDB::bind_method(D_METHOD("get_sockets"), &EOSGPacketPeerMediator::get_sockets);
	ClassDB::bind_method(D_METHOD("get_packet_count_for_socket", "socket_id"), &EOSGPacketPeerMediator::get_packet_count_for_socket);
	ClassDB::bind_method(D_METHOD("has_socket"), &EOSGPacketPeerMediator::has_socket);
	ClassDB::bind_method(D_METHOD("get_packet_count_from_remote_user"), &EOSGPacketPeerMediator::get_packet_count_from_remote_user);
	ClassDB::bind_method(D_METHOD("get_queue_size_limit"), &EOSGPacketPeerMediator::get_queue_size_limit);
	ClassDB::bind_method(D_METHOD("set_queue_size_limit", "limit"), &EOSGPacketPeerMediator::set_queue_size_limit);

	ADD_SIGNAL(MethodInfo("packet_queue_full"));
}

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
			}
		}
	} while (next_packet_available);
}

bool EOSGPacketPeerMediator::poll_next_packet(const String &socket_id, PacketData *out_packet) {
	if (!socket_packet_queues.has(socket_id)) return false;
	if (socket_packet_queues[socket_id].size() == 0) return false;

	PacketData next_packet = socket_packet_queues[socket_id].front()->get();
	*out_packet = next_packet;
	socket_packet_queues[socket_id].pop_front();
	return true;
}

bool EOSGPacketPeerMediator::register_socket(const String &socket_id) {
	ERR_FAIL_COND_V_MSG(socket_id.is_empty(), false, "Failed to register socket. Socket id is invalid. Provided an empty string.");
	ERR_FAIL_COND_V_MSG(socket_packet_queues.has(socket_id), false, vformat("Failed to register socket. socket \"%s\" has already been registered.", socket_id));

	if (!initialized) {
		initialized = _init();
		ERR_FAIL_COND_V(!initialized, false);
	}
	socket_packet_queues.insert(socket_id, List<PacketData>());
	return true;
}

void EOSGPacketPeerMediator::unregister_socket(const String &socket_id) {
	ERR_FAIL_COND_MSG(!socket_packet_queues.has(socket_id), vformat("Failed to unregister socket. Socket \"%s\" does not exist.", socket_id));

	clear_packet_queue(socket_id);
	socket_packet_queues.erase(socket_id);
}

void EOSGPacketPeerMediator::clear_packet_queue(const String &socket_id) {
	ERR_FAIL_COND_MSG(!socket_packet_queues.has(socket_id), vformat("Failed to clear packet queue for socket \"%s\". Socket was not registered.", socket_id));

	socket_packet_queues[socket_id].clear();
}

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

bool EOSGPacketPeerMediator::_init() {
	MainLoop *main_loop = Engine::get_singleton()->get_main_loop();
	ERR_FAIL_COND_V_MSG(!main_loop->has_signal("process_frame"), false, "Failed to initialize EOSGPacketPeerMediator. Main loop does not have the process_frame() signal.");
	Callable process_frame_callback = Callable(this, "_on_process_frame");
	main_loop->connect("process_frame", process_frame_callback);
	return true;
}

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

bool EOSGPacketPeerMediator::next_packet_is_peer_id_packet(const String &socket_id) {
	ERR_FAIL_COND_V_MSG(!socket_packet_queues.has(socket_id), false, "Failed to check next packet. Socket \"%s\" does not exist.");
	if (socket_packet_queues[socket_id].size() == 0) return false;
	PacketData packet = socket_packet_queues[socket_id][0];
	uint8_t event = packet.get_data()->ptrw()[0];
	if (event == 1) return true;
	return false;
}

EOSGPacketPeerMediator::EOSGPacketPeerMediator() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "EOSGPacketPeerMediator already initialized");
	singleton = this;
}

EOSGPacketPeerMediator::~EOSGPacketPeerMediator() {
		if (singleton != this) return;
		singleton = nullptr;
}
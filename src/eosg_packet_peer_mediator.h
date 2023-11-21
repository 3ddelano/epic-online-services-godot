#include "godot_cpp/classes/ref_counted.hpp"
#include "eosg_multiplayer_peer.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

namespace godot
{
struct PacketData {
	private:
	std::shared_ptr<PackedByteArray> data;
	String remote_user_id;
	int size_bytes = 0;
	int channel = 0;

	public:
	void store(uint8_t *packet, int size_bytes) {
		if (data == nullptr) {
			data = std::make_shared<PackedByteArray>();
		}
		this->size_bytes = size_bytes;
		data->resize(size_bytes);
		memcpy(data->ptrw(), packet, size_bytes);
	}

	int size() {
		return size_bytes;
	}

	int get_channel() {
		return channel;
	}

	String get_sender() {
		return remote_user_id;
	}

	void set_channel(int channel) {
		this->channel = channel;
	}

	void set_sender(EOS_ProductUserId sender) {
		remote_user_id = eosg_product_user_id_to_string(sender);
	}

	PackedByteArray *get_data() {
		return data.get();
	}
};

class EOSGPacketPeerMediator : public RefCounted {
	GDCLASS(EOSGPacketPeerMediator, RefCounted)

	private:
	static EOSGPacketPeerMediator *singleton;
	Callable process_frame_callback;
	Callable connect_interface_login_callback;
	static void _bind_methods();

	HashMap<String, EOSGMultiplayerPeer*> active_peers;
	HashMap<String, List<PacketData>> socket_packet_queues;
	List<ConnectionRequestData> pending_connection_requests;
	int max_queue_size = 5000;
	bool initialized = false;

	void _on_process_frame();
	void _init();
	void _terminate();

	static void EOS_CALL _on_peer_connection_established(const EOS_P2P_OnPeerConnectionEstablishedInfo *data);
	static void EOS_CALL _on_peer_connection_interrupted(const EOS_P2P_OnPeerConnectionInterruptedInfo *data);
	static void EOS_CALL _on_remote_connection_closed(const EOS_P2P_OnRemoteConnectionClosedInfo *data);
	static void EOS_CALL _on_incoming_connection_request(const EOS_P2P_OnIncomingConnectionRequestInfo *data);
	void _on_connect_interface_login(Dictionary data);
	bool _add_connection_established_callback();
	bool _add_connection_closed_callback();
	bool _add_connection_interrupted_callback();
	bool _add_connection_request_callback();
	void _pass_on_pending_connection_requests(EOSGMultiplayerPeer *peer);

	EOS_NotificationId connection_established_callback_id = EOS_INVALID_NOTIFICATIONID;
	EOS_NotificationId connection_interrupted_callback_id = EOS_INVALID_NOTIFICATIONID;
	EOS_NotificationId connection_closed_callback_id = EOS_INVALID_NOTIFICATIONID;
	EOS_NotificationId connection_request_callback_id = EOS_INVALID_NOTIFICATIONID;

	public:

	static EOSGPacketPeerMediator *get_singleton(){
		return singleton;
	}

	int get_total_packet_count() {
		int ret = 0;
		for (KeyValue<String, List<PacketData>> &E : socket_packet_queues){
			ret += E.value.size();
		}
		return ret;
	}

	int get_packet_count_for_socket(const String &socket_id) {
		ERR_FAIL_COND_V_MSG(!socket_packet_queues.has(socket_id), 0, "Failed to get packet count for socket \"%s\". Socket does not exist.");
		return socket_packet_queues[socket_id].size();
	}

	Array get_sockets() {
		Array ret;
		for (KeyValue<String, List<PacketData>> &E : socket_packet_queues) {
			ret.push_back(E.key);
		}
		return ret;
	}

	bool has_socket(const String &socket_id) {
		return socket_packet_queues.has(socket_id);
	}

	int get_queue_size_limit() {
		return max_queue_size;
	}

	void set_queue_size_limit(int limit) {
		ERR_FAIL_COND_MSG(limit < 1, "Cannot set queue size limit. Limit must be greater than 0");
		max_queue_size = limit;
	}

	int get_connection_request_count() {
		return pending_connection_requests.size();
	}

	int get_packet_count_from_remote_user(const String &remote_user, const String &socket_id);
	bool poll_next_packet(const String &socket_id, PacketData *out_packet);
	bool next_packet_is_peer_id_packet(const String &socket_id);
	bool register_peer(EOSGMultiplayerPeer *peer);
	void unregister_peer(EOSGMultiplayerPeer *peer);
	void clear_packet_queue(const String &socket_id);
	void clear_packets_from_remote_user(const String &socket_id, const String &remote_user_id);

	EOSGPacketPeerMediator();
	~EOSGPacketPeerMediator();
};
} //namespace godot
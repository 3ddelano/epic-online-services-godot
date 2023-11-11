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
	static void _bind_methods();

	HashMap<String, List<PacketData>> socket_packet_queues;
	int max_queue_size = 5000;
	bool initialized = false;

	void _on_process_frame();
	bool _init();

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

	int get_packet_count_from_remote_user(const String &remote_user, const String &socket_id);
	bool poll_next_packet(const String &socket_id, PacketData *out_packet);
	bool next_packet_is_peer_id_packet(const String &socket_id);
	bool register_socket(const String &socket_id);
	void unregister_socket(const String &socket_id);
	void clear_packet_queue(const String &socket_id);
	void clear_packets_from_remote_user(const String &socket_id, const String &remote_user_id);

	EOSGPacketPeerMediator();
	~EOSGPacketPeerMediator();
};
} //namespace godot
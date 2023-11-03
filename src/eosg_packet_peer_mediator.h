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

	PackedByteArray *get_packet() {
		return data.get();
	}

	PacketData() {
		data = std::make_shared<PackedByteArray>();
	}
};
class EOSGPacketPeerMediator : public RefCounted {
	GDCLASS(EOSGPacketPeerMediator, RefCounted)

	private:
	static EOSGPacketPeerMediator *singleton;
	static void _bind_methods();

	HashMap<String, List<PacketData>> registered_sockets;
	int total_bytes_stored = 0;
	const int MAX_QUEUE_SIZE_BYTES = 5242880;

	void _on_process_frame();

	public:
	static EOSGPacketPeerMediator *get_singleton(){
		return singleton;
	}

	bool poll_next_packet(const String &socket_id, PacketData *out_packet);
	void register_socket(const String &socket_id);
	void unregister_socket(const String &socket_id);
	void clear_packet_queue(const String &socket_id);
	void clear_packets_from_remote_user(const String &socket_id, const String &remote_user_id);

	EOSGPacketPeerMediator();
	~EOSGPacketPeerMediator();
};
} //namespace godot
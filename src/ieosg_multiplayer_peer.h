#pragma once

#include "godot_cpp/templates/hash_map.hpp"
#include "godot_cpp/classes/multiplayer_peer_extension.hpp"
#include "eos_p2p.h"
#include "utils.h"
#include <memory>
#include <vector>

namespace godot
{
class IEOSGMultiplayerPeer : public MultiplayerPeerExtension {
    GDCLASS(IEOSGMultiplayerPeer, MultiplayerPeerExtension)
	
	private:
	enum {
		INDEX_EVENT_TYPE = 0,
		INDEX_TRANSFER_MODE = 1,
		INDEX_PEER_ID = 2,
		INDEX_PAYLOAD_DATA = 6,
	};

	enum {
		CH_RELIABLE = 0,
		CH_UNRELIABLE = 1,
		CH_MAX = 2,
	};

	enum Mode {
		MODE_NONE,
		MODE_SERVER,
		MODE_CLIENT,
		MODE_MESH,
	};

	enum Event {
		EVENT_STORE_PACKET,
		EVENT_RECIEVE_PEER_ID,
	};

	struct EOSGPeerInfo {
		EOS_ProductUserId user_id;
		List<const EOS_P2P_SocketId*> sockets;
	};

	struct EOSGPacket {
		private:
		std::shared_ptr<std::vector<uint8_t>> packet;
		uint8_t channel = 0;
		int32_t size_bytes;
		EOS_EPacketReliability reliability;
		Event event;
		int from = 0;

		public:

		static const int PACKET_HEADER_SIZE = 6;

		void prepare();
		void store_payload(const uint8_t *payload_date, const uint32_t payload_size_bytes);

		int payload_size() const {
			return size_bytes - PACKET_HEADER_SIZE;
		}

		int packet_size() const {
			return size_bytes;
		}

		uint8_t* get_payload() const {
			if (size_bytes == PACKET_HEADER_SIZE) { //There's no payload, just the packet header
				return nullptr;
			}
			return packet.get()->data() + INDEX_PAYLOAD_DATA;
		}

		uint8_t* get_packet() const {
			return packet.get()->data();
		}

		EOS_EPacketReliability get_reliability() const {
			return reliability;
		}

		void set_reliability(EOS_EPacketReliability reliability) {
			this->reliability = reliability;
		}

		uint8_t get_channel() const {
			return channel;
		}

		void set_channel(uint8_t channel) {
			this->channel = channel;
		}

		Event get_event() const {
			return event;
		}

		void set_event(Event event) {
			this->event = event;
		}

		int get_sender() const {
			return from;
		}

		void set_sender(int p_id) {
			from = p_id;
		}

		EOSGPacket() {
			packet = std::make_shared<std::vector<uint8_t>>();
			packet.get()->resize(PACKET_HEADER_SIZE);
			size_bytes = PACKET_HEADER_SIZE;
		}
	};

	struct EOSGConnectionRequest {
		EOS_ProductUserId remote_user;
		EOS_P2P_SocketId socket;

		bool operator == (const EOSGConnectionRequest &lhs) {
			String lhs_remote_id = eosg_product_user_id_to_string(lhs.remote_user);
			String lhs_socket_name = lhs.socket.SocketName;
			String rhs_remote_id = eosg_product_user_id_to_string(this->remote_user);
			String rhs_socket_name = this->socket.SocketName;
			return rhs_remote_id == lhs_remote_id && rhs_socket_name == lhs_socket_name;
		}
	};

	_FORCE_INLINE_ bool _is_active() const { return active_mode != MODE_NONE; }
	
	Error _broadcast(const EOSGPacket &packet, int exclude = 0);
	Error _send_to(const EOS_ProductUserId &remote_peer, const EOSGPacket &packet, const EOS_P2P_SocketId *socket);
	bool _find_connection_request(const String &remote_user, const String &socket_id, EOSGConnectionRequest &out_request);
	void _remove_all_callbacks();
	bool _add_server_mesh_callbacks();
	bool _add_client_callbacks();
	const EOS_P2P_SocketId* _get_socket(const String &socket_name) const;
	EOS_EPacketReliability _convert_transfer_mode_to_eos_reliability(TransferMode mode) const;
	TransferMode _convert_eos_reliability_to_transfer_mode(EOS_EPacketReliability reliability) const;
	void _disconnect_remote_user(const EOS_ProductUserId &remote_user, const EOS_P2P_SocketId &socket);
	void _remove_socket(const String &socket_id);

	static void EOS_CALL _on_peer_connection_established(const EOS_P2P_OnPeerConnectionEstablishedInfo *data);
	static void EOS_CALL _on_peer_connection_interrupted(const EOS_P2P_OnPeerConnectionInterruptedInfo *data);
	static void EOS_CALL _on_incoming_connection_request(const EOS_P2P_OnIncomingConnectionRequestInfo *data);
	static void EOS_CALL _on_remote_connection_closed(const EOS_P2P_OnRemoteConnectionClosedInfo *data);

	static IEOSGMultiplayerPeer *singleton;
	static EOS_ProductUserId s_local_user_id;
	bool is_singleton = false;

	List<EOSGPacket> incoming_packets;
	EOSGPacket current_packet;

	EOS_ProductUserId target_user_id;
	uint32_t unique_id;
	int target_peer = 0;
	ConnectionStatus connection_status = CONNECTION_DISCONNECTED;
	Mode active_mode = MODE_NONE;
	EOS_Bool allow_delayed_delivery = EOS_TRUE;
	bool auto_accept_connection_requests = false;
	TransferMode transfer_mode = TransferMode::TRANSFER_MODE_RELIABLE;
	uint32_t transfer_channel = CH_RELIABLE;
	bool refusing_connections = false;

	HashMap<uint32_t, EOSGPeerInfo> peers;
	List<EOS_P2P_SocketId> sockets;
	List<EOSGConnectionRequest> pending_connection_requests;

	EOS_NotificationId connection_established_callback_id = EOS_INVALID_NOTIFICATIONID;
	EOS_NotificationId connection_interrupted_callback_id = EOS_INVALID_NOTIFICATIONID;
	EOS_NotificationId incoming_connection_request_callback_id = EOS_INVALID_NOTIFICATIONID;
	EOS_NotificationId connection_closed_callback_id = EOS_INVALID_NOTIFICATIONID;

    static void _bind_methods();

    public:
	static IEOSGMultiplayerPeer* get_singleton();
	static void set_local_user_id(const String& p_local_user_id);
	static String get_local_user_id();

	Error create_server(const String &socket_id);
	Error create_client(const String &socket_id, const String &remote_user_id);
	Error create_mesh(const PackedStringArray &sockets);
	Error add_mesh_socket(const String &socket_id);
	void close_mesh_socket(const String &socket_id);
	Error add_mesh_peer(const String &remote_user, const String &socket_id);

	Array find_all_connecetion_requests_for_user(const String &user_id);
	Array find_all_connecetion_requests_for_socket(const String &socket_id);
	Array get_all_connection_requests();
	Dictionary get_peer_info(int p_id);
	int find_peer_id(const String &user_id);
	bool has_peer(int peer_id);
	Array get_all_sockets();
	Dictionary get_all_peers();
	void set_allow_delayed_delivery(bool allow);
	bool is_allowing_delayed_delivery();
	void set_auto_accept_connection_requests(bool enable);
	bool is_auto_accepting_connection_requests();
	bool has_socket(const String &socket);
	void accept_connection_request(const String &remote_user, const String &socket_id);
	void deny_connection_request(const String &remote_user, const String &socket_id);
	void accept_all_connection_requests();
	void deny_all_connection_requests();
	int get_active_mode();

    virtual Error _get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) override;
	virtual Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size);
	virtual int32_t _get_available_packet_count() const override;
	virtual int32_t _get_max_packet_size() const override;
	virtual PackedByteArray _get_packet_script() override;
	virtual Error _put_packet_script(const PackedByteArray &p_buffer) override;
	virtual int32_t _get_packet_channel() const override;
	virtual MultiplayerPeer::TransferMode _get_packet_mode() const override;
	virtual void _set_transfer_channel(int32_t p_channel) override;
	virtual int32_t _get_transfer_channel() const override;
	virtual void _set_transfer_mode(MultiplayerPeer::TransferMode p_mode) override;
	virtual MultiplayerPeer::TransferMode _get_transfer_mode() const override;
	virtual void _set_target_peer(int32_t p_peer) override;
	virtual int32_t _get_packet_peer() const override;
	virtual bool _is_server() const override;
	virtual void _poll() override;
	virtual void _close() override;
	virtual void _disconnect_peer(int32_t p_peer, bool p_force = false) override;
	virtual int32_t _get_unique_id() const override;
	virtual void _set_refuse_new_connections(bool p_enable) override;
	virtual bool _is_refusing_new_connections() const override;
	virtual bool _is_server_relay_supported() const override;
	virtual MultiplayerPeer::ConnectionStatus _get_connection_status() const override;

	IEOSGMultiplayerPeer();
	~IEOSGMultiplayerPeer();
};
} //namespace godot
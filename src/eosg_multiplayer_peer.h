#pragma once

#include "eos_p2p.h"
#include "godot_cpp/classes/multiplayer_peer_extension.hpp"
#include "godot_cpp/templates/hash_map.hpp"
#include "utils.h"
#include <memory>

namespace godot {

struct ConnectionRequestData {
    String socket_name;
    String remote_user_id;
    String local_user_id;
};

class EOSGMultiplayerPeer : public MultiplayerPeerExtension {
    GDCLASS(EOSGMultiplayerPeer, MultiplayerPeerExtension)

private:
    enum Event {
        EVENT_STORE_PACKET,
        EVENT_RECIEVE_PEER_ID,
        EVENT_MESH_CONNECTION_REQUEST
    };

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

    class EOSGPacket {
    private:
        std::shared_ptr<PackedByteArray> packet;
        uint8_t channel = 0;
        int32_t size_bytes = 0;
        EOS_EPacketReliability reliability;
        Event event;
        int from = 0;

        void _alloc_packet(int size_bytes = PACKET_HEADER_SIZE) {
            packet = std::make_shared<PackedByteArray>();
            packet->resize(size_bytes);
            this->size_bytes = size_bytes;
        }

    public:
        static const int PACKET_HEADER_SIZE = 6;

        void prepare();
        void store_payload(const uint8_t *payload_data, const uint32_t payload_size_bytes);

        int payload_size() const {
            return size_bytes - PACKET_HEADER_SIZE;
        }

        int packet_size() const {
            return size_bytes;
        }

        uint8_t *get_payload() const {
            if (size_bytes == 0 || size_bytes == PACKET_HEADER_SIZE) {
                return nullptr; //Return nullptr if there's no payload.
            }
            return packet->ptrw() + INDEX_PAYLOAD_DATA;
        }

        uint8_t *get_packet() const {
            if (packet.get() == nullptr) {
                return nullptr; //Return nullptr if the packed has not been allocated
            }
            return packet->ptrw();
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
    };

    class EOSGSocket {
    private:
        EOS_P2P_SocketId socket;
        List<EOSGPacket> incoming_packets;

    public:
        const EOS_P2P_SocketId *get_id() const {
            return &socket;
        }

        String get_name() const {
            return socket.SocketName;
        }

        void push_packet(EOSGPacket packet) {
            incoming_packets.push_back(packet);
        }

        EOSGPacket pop_packet() {
            EOSGPacket ret = incoming_packets.front()->get();
            incoming_packets.pop_front();
            return ret;
        }

        void clear_packet_queue() {
            incoming_packets.clear();
        }

        bool has_packet() const {
            return incoming_packets.size() != 0;
        }

        int get_packet_count() const {
            return incoming_packets.size();
        }

        EOS_EPacketReliability get_packet_reliability() const {
            EOSGPacket packet = incoming_packets.front()->get();
            return packet.get_reliability();
        }

        int32_t get_packet_channel() const {
            EOSGPacket packet = incoming_packets.front()->get();
            return packet.get_channel();
        }

        int32_t get_packet_peer() const {
            EOSGPacket packet = incoming_packets.front()->get();
            return packet.get_sender();
        }

        void close();
        void clear_packets_from_peer(int p_peer);
        bool _socket_id_is_valid(const String &socket_id);

        EOSGSocket() {}

        EOSGSocket(const EOS_P2P_SocketId &socket) {
            this->socket = socket;
        }

        EOSGSocket(const String &socket_name) {
            memset(socket.SocketName, 0, sizeof(socket.SocketName));
            ERR_FAIL_COND_MSG(!_socket_id_is_valid(socket_name), "Failed to create socket. Socket id is not valid.\nNOTE: Socket id cannot be empty, must only have alpha-numeric characters, and must not be longer than 32 characters");
            socket.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
            STRNCPY_S(socket.SocketName, EOS_P2P_SOCKETID_SOCKETNAME_SIZE, socket_name.utf8(), socket_name.length());
        }
    };

    _FORCE_INLINE_ bool _is_active() const { return active_mode != MODE_NONE; }

    Error _broadcast(const EOSGPacket &packet, int exclude = 0);
    Error _send_to(const EOS_ProductUserId &remote_peer, const EOSGPacket &packet);
    bool _find_connection_request(const String &remote_user, EOS_ProductUserId &out_request);
    EOS_EPacketReliability _convert_transfer_mode_to_eos_reliability(TransferMode mode) const;
    TransferMode _convert_eos_reliability_to_transfer_mode(EOS_EPacketReliability reliability) const;
    void _disconnect_remote_user(const EOS_ProductUserId &remote_user);
    void _clear_peer_packet_queue(int p_id);

    static EOS_ProductUserId s_local_user_id;

    EOSGPacket current_packet;
    uint32_t unique_id;
    int target_peer = 0;
    ConnectionStatus connection_status = CONNECTION_DISCONNECTED;
    Mode active_mode = MODE_NONE;
    EOS_Bool allow_delayed_delivery = EOS_TRUE;
    bool auto_accept_connection_requests = true;
    TransferMode transfer_mode = TransferMode::TRANSFER_MODE_RELIABLE;
    uint32_t transfer_channel = CH_RELIABLE;
    bool refusing_connections = false;
    bool polling = false;

    HashMap<uint32_t, EOS_ProductUserId> peers;

    EOSGSocket socket;
    List<EOS_ProductUserId> pending_connection_requests;

    static void _bind_methods();

public:
    static void set_local_user_id(const String &p_local_user_id);
    static String get_local_user_id();

    void peer_connection_established_callback(const EOS_P2P_OnPeerConnectionEstablishedInfo *data);
    void remote_connection_closed_callback(const EOS_P2P_OnRemoteConnectionClosedInfo *data);
    void peer_connection_interrupted_callback(const EOS_P2P_OnPeerConnectionInterruptedInfo *data);
    void connection_request_callback(const ConnectionRequestData &data);

    Error create_server(const String &socket_id);
    Error create_client(const String &socket_id, const String &remote_user_id);
    Error create_mesh(const String &socket_id);
    Error add_mesh_peer(const String &remote_user);

    String get_socket() const;
    Array get_all_connection_requests();
    String get_peer_user_id(int p_id);
    int get_peer_id(const String &user_id);
    bool has_peer(int peer_id);
    bool has_user_id(const String &remote_user_id);
    Dictionary get_all_peers();
    void set_allow_delayed_delivery(bool allow);
    bool is_allowing_delayed_delivery();
    void set_auto_accept_connection_requests(bool enable);
    bool is_auto_accepting_connection_requests();
    void accept_connection_request(const String &remote_user);
    void deny_connection_request(const String &remote_user);
    void accept_all_connection_requests();
    void deny_all_connection_requests();
    int get_active_mode();

    bool is_polling() {
        return polling;
    }

    void set_is_polling(bool polling) {
        this->polling = polling;
    }

    virtual Error _get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) override;
    virtual Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size);
    virtual int32_t _get_available_packet_count() const override;
    virtual int32_t _get_max_packet_size() const override;
    //virtual PackedByteArray _get_packet_script() override;
    //virtual Error _put_packet_script(const PackedByteArray &p_buffer) override;
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

    EOSGMultiplayerPeer(){};
    ~EOSGMultiplayerPeer();
};
} //namespace godot
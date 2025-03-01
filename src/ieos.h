#pragma once
#include "eos_achievements.h"
#include "eos_anticheatclient.h"
#include "eos_anticheatserver.h"
#include "eos_auth.h"
#include "eos_custominvites.h"
#include "eos_ecom.h"
#include "eos_friends.h"
#include "eos_kws.h"
#include "eos_leaderboards.h"
#include "eos_lobby.h"
#include "eos_logging.h"
#include "eos_metrics.h"
#include "eos_mods.h"
#include "eos_p2p.h"
#include "eos_playerdatastorage.h"
#include "eos_presence.h"
#include "eos_progressionsnapshot.h"
#include "eos_reports.h"
#include "eos_rtc.h"
#include "eos_rtc_audio.h"
#include "eos_rtc_data.h"
#include "eos_sanctions.h"
#include "eos_sdk.h"
#include "eos_sessions.h"
#include "eos_stats.h"
#include "eos_titlestorage.h"
#include "eos_ui.h"
#include "eos_userinfo.h"
#include "eos_version.h"
#include "eosg_file_transfer_request.h"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/os.hpp"
#include "godot_cpp/classes/project_settings.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "unordered_map"
#include "utils.h"
#ifdef _WIN32
#include "Windows/eos_Windows.h"
#endif

namespace godot {

class IEOS : public RefCounted {
    GDCLASS(IEOS, RefCounted)

    static IEOS *singleton;

protected:
    static void _bind_methods();

    EOS_HAchievements s_achievementsInterface = nullptr;
    EOS_HAuth s_authInterface = nullptr;
    EOS_HConnect s_connectInterface = nullptr;
    EOS_HCustomInvites s_customInvitesInterface = nullptr;
    EOS_HEcom s_ecomInterface = nullptr;
    EOS_HFriends s_friendsInterface = nullptr;
    EOS_HKWS s_kwsInterface = nullptr;
    EOS_HLeaderboards s_leaderboardsInterface = nullptr;
    EOS_HLobby s_lobbyInterface = nullptr;
    EOS_HMetrics s_metricsInterface = nullptr;
    EOS_HMods s_modsInterface = nullptr;
    EOS_HP2P s_p2pInterface = nullptr;
    EOS_HPlatform s_platformInterface = nullptr;
    EOS_HPlayerDataStorage s_playerDataStorageInterface = nullptr;
    EOS_HPresence s_presenceInterface = nullptr;
    EOS_HProgressionSnapshot s_progressionSnapshotInterface = nullptr;
    EOS_HReports s_reportsInterface = nullptr;
    EOS_HRTC s_rtcInterface = nullptr;
    EOS_HRTCAudio s_rtcAudioInterface = nullptr;
    EOS_HRTCData s_rtcDataInterface = nullptr;
    EOS_HSanctions s_sanctionsInterface = nullptr;
    EOS_HSessions s_sessionsInterface = nullptr;
    EOS_HStats s_statsInterface = nullptr;
    EOS_HTitleStorage s_titleStorageInterface = nullptr;
    EOS_HUI s_uiInterface = nullptr;
    EOS_HUserInfo s_userInfoInterface = nullptr;
    EOS_HAntiCheatServer s_antiCheatServerInterface = nullptr;
    EOS_HAntiCheatClient s_antiCheatClientInterface = nullptr;

    std::unordered_map<std::string, void *> anticheat_client_map; // {String: String} key: hash value: string

public:
    bool isEOSValid = false; // is true if EOS Platform is initialized

    static IEOS *get_singleton();

    IEOS();
    ~IEOS();

    static void tick();
    bool is_operation_complete(int result_code);

    // -----
    // Platform Interface
    // -----
    bool platform_interface_create(Ref<RefCounted> options);
    Dictionary platform_interface_get_active_country_code(const String &local_user_id);
    Dictionary platform_interface_get_active_locale_code(const String &local_user_id);
    Dictionary platform_interface_get_desktop_crossplay_status_info();
    Dictionary platform_interface_get_override_country_code();
    Dictionary platform_interface_get_override_locale_code();
    int platform_interface_check_for_launcher_and_restart();
    int platform_interface_get_application_status();
    int platform_interface_get_network_status();
    int platform_interface_initialize(Ref<RefCounted> options);
    int platform_interface_set_application_status(int status);
    int platform_interface_set_network_status(int status);
    int platform_interface_set_override_country_code(const String &country_code);
    int platform_interface_set_override_locale_code(const String &locale_code);
    int platform_interface_shutdown();
    void platform_interface_release();

    // -----
    // Logging Interface
    // -----
    int logging_interface_set_log_level(int category, int level);

    // -----
    // Version Interface
    // -----
    Dictionary version_interface_get_constants();
    String version_interface_get_version();

    // -----
    // Auth interface
    // -----
    Dictionary auth_interface_copy_id_token(Ref<RefCounted> options);
    Dictionary auth_interface_copy_user_auth_token(Ref<RefCounted> options, const String &local_user_id);
    Dictionary auth_interface_get_selected_account_id(const String &local_user_id);
    int auth_interface_get_logged_in_accounts_count();
    int auth_interface_get_login_status(const String &local_user_id);
    int auth_interface_get_merged_accounts_count(const String &local_user_id);
    String auth_interface_get_logged_in_account_by_index(int index);
    String auth_interface_get_merged_account_by_index(const String &local_user_id, int index);
    void auth_interface_delete_persistent_auth(Ref<RefCounted> options);
    void auth_interface_link_account(Ref<RefCounted> options);
    void auth_interface_login(Ref<RefCounted> options);
    void auth_interface_logout(Ref<RefCounted> options);
    void auth_interface_query_id_token(Ref<RefCounted> options);
    void auth_interface_verify_id_token(Ref<RefCounted> options);
    void auth_interface_verify_user_auth(Ref<RefCounted> _options);

    // -----
    // Connect Interface
    // -----
    Dictionary connect_interface_copy_id_token(Ref<RefCounted> options);
    Dictionary connect_interface_copy_product_user_external_account_by_account_id(Ref<RefCounted> options);
    Dictionary connect_interface_copy_product_user_external_account_by_account_type(Ref<RefCounted> options);
    Dictionary connect_interface_copy_product_user_external_account_by_index(Ref<RefCounted> options);
    Dictionary connect_interface_copy_product_user_info(Ref<RefCounted> options);
    Dictionary connect_interface_get_product_user_id_mapping(Ref<RefCounted> options);
    int connect_interface_get_logged_in_users_count();
    int connect_interface_get_login_status(const String &local_user_id);
    int connect_interface_get_product_user_external_account_count(Ref<RefCounted> options);
    String connect_interface_get_external_account_mapping(Ref<RefCounted> options);
    String connect_interface_get_logged_in_user_by_index(int index);
    void connect_interface_create_device_id(Ref<RefCounted> options);
    void connect_interface_create_user(Ref<RefCounted> options);
    void connect_interface_delete_device_id(Ref<RefCounted> options);
    void connect_interface_link_account(Ref<RefCounted> options);
    void connect_interface_login(Ref<RefCounted> options);
    void connect_interface_logout(Ref<RefCounted> options);
    void connect_interface_query_product_user_id_mappings(Ref<RefCounted> options);
    void connect_interface_query_external_account_mappings(Ref<RefCounted> options);
    void connect_interface_transfer_device_id_account(Ref<RefCounted> options);
    void connect_interface_unlink_account(Ref<RefCounted> options);
    void connect_interface_verify_id_token(Ref<RefCounted> options);

    // -----
    // Achievements Interface
    // -----
    Dictionary achievements_interface_copy_achievement_definition_v2_by_achievement_id(Ref<RefCounted> options);
    Dictionary achievements_interface_copy_achievement_definition_v2_by_index(Ref<RefCounted> options);
    Dictionary achievements_interface_copy_player_achievement_by_achievement_id(Ref<RefCounted> options);
    Dictionary achievements_interface_copy_player_achievement_by_index(Ref<RefCounted> options);
    int achievements_interface_get_achievement_definition_count(Ref<RefCounted> options);
    int achievements_interface_get_player_achievement_count(Ref<RefCounted> options);
    void achievements_interface_query_definitions(Ref<RefCounted> options);
    void achievements_interface_query_player_achievements(Ref<RefCounted> options);
    void achievements_interface_unlock_achievements(Ref<RefCounted> options);

    // -----
    // Custom Invites Interface
    // -----
    int custom_invites_interface_finalize_invite(Ref<RefCounted> options);
    int custom_invites_interface_set_custom_invite(Ref<RefCounted> options);
    void custom_invites_interface_accept_request_to_join(Ref<RefCounted> options);
    void custom_invites_interface_reject_request_to_join(Ref<RefCounted> options);
    void custom_invites_interface_send_custom_invite(Ref<RefCounted> options);
    void custom_invites_interface_send_request_to_join(Ref<RefCounted> options);

    // -----
    // Stats Interface
    // -----
    Dictionary stats_interface_copy_stat_by_index(Ref<RefCounted> options);
    Dictionary stats_interface_copy_stat_by_name(Ref<RefCounted> options);
    int stats_interface_get_stats_count(Ref<RefCounted> options);
    void stats_interface_ingest_stat(Ref<RefCounted> options);
    void stats_interface_query_stats(Ref<RefCounted> options);

    // -----
    // Leaderboards Interface
    // -----
    Dictionary leaderboards_interface_copy_leaderboard_definition_by_index(Ref<RefCounted> options);
    Dictionary leaderboards_interface_copy_leaderboard_definition_by_leaderboard_id(Ref<RefCounted> options);
    Dictionary leaderboards_interface_copy_leaderboard_record_by_index(Ref<RefCounted> options);
    Dictionary leaderboards_interface_copy_leaderboard_record_by_user_id(Ref<RefCounted> options);
    Dictionary leaderboards_interface_copy_leaderboard_user_score_by_index(Ref<RefCounted> options);
    Dictionary leaderboards_interface_copy_leaderboard_user_score_by_user_id(Ref<RefCounted> options);
    int leaderboards_interface_get_leaderboard_definition_count(Ref<RefCounted> options);
    int leaderboards_interface_get_leaderboard_record_count(Ref<RefCounted> options);
    int leaderboards_interface_get_leaderboard_user_score_count(Ref<RefCounted> options);
    void leaderboards_interface_query_leaderboard_definitions(Ref<RefCounted> options);
    void leaderboards_interface_query_leaderboard_ranks(Ref<RefCounted> options);
    void leaderboards_interface_query_leaderboard_user_scores(Ref<RefCounted> options);

    // -----
    // Friends Interface
    // -----
    int friends_interface_get_blocked_users_count(Ref<RefCounted> options);
    int friends_interface_get_friends_count(Ref<RefCounted> options);
    int friends_interface_get_status(Ref<RefCounted> options);
    String friends_interface_get_blocked_user_at_index(Ref<RefCounted> options);
    String friends_interface_get_friend_at_index(Ref<RefCounted> options);
    void friends_interface_accept_invite(Ref<RefCounted> options);
    void friends_interface_query_friends(Ref<RefCounted> options);
    void friends_interface_reject_invite(Ref<RefCounted> options);
    void friends_interface_send_invite(Ref<RefCounted> options);

    // -----
    // Userinfo Interface
    // -----
    Dictionary user_info_interface_copy_best_display_name_with_platform(Ref<RefCounted> options);
    Dictionary user_info_interface_copy_best_display_name(Ref<RefCounted> options);
    Dictionary user_info_interface_copy_external_user_info_by_account_id(Ref<RefCounted> options);
    Dictionary user_info_interface_copy_external_user_info_by_account_type(Ref<RefCounted> options);
    Dictionary user_info_interface_copy_external_user_info_by_index(Ref<RefCounted> options);
    Dictionary user_info_interface_copy_user_info(Ref<RefCounted> options);
    int user_info_interface_get_external_user_info_count(Ref<RefCounted> options);
    int user_info_interface_get_local_platform_type(Ref<RefCounted> options);
    void user_info_interface_query_user_info_by_display_name(Ref<RefCounted> options);
    void user_info_interface_query_user_info_by_external_account(Ref<RefCounted> options);
    void user_info_interface_query_user_info(Ref<RefCounted> options);

    // -----
    // Ecom Interface
    // -----
    Dictionary ecom_interface_copy_entitlement_by_id(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_entitlement_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_entitlement_by_name_and_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_item_by_id(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_item_image_info_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_item_release_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_last_redeemed_entitlement_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_offer_by_id(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_offer_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_offer_image_info_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_offer_item_by_index(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_transaction_by_id(Ref<RefCounted> options);
    Dictionary ecom_interface_copy_transaction_by_index(Ref<RefCounted> options);
    int ecom_interface_get_entitlements_by_name_count(Ref<RefCounted> options);
    int ecom_interface_get_entitlements_count(Ref<RefCounted> options);
    int ecom_interface_get_item_image_info_count(Ref<RefCounted> options);
    int ecom_interface_get_item_release_count(Ref<RefCounted> options);
    int ecom_interface_get_last_redeemed_entitlements_count(Ref<RefCounted> options);
    int ecom_interface_get_offer_count(Ref<RefCounted> options);
    int ecom_interface_get_offer_image_info_count(Ref<RefCounted> options);
    int ecom_interface_get_offer_item_count(Ref<RefCounted> options);
    int ecom_interface_get_transaction_count(Ref<RefCounted> options);
    void ecom_interface_checkout(Ref<RefCounted> options);
    void ecom_interface_query_entitlements(Ref<RefCounted> options);
    void ecom_interface_query_offers(Ref<RefCounted> options);
    void ecom_interface_query_ownership_token(Ref<RefCounted> options);
    void ecom_interface_query_ownership(Ref<RefCounted> options);
    void ecom_interface_redeem_entitlements(Ref<RefCounted> options);
    void ecom_interface_query_entitlement_token(Ref<RefCounted> options);
    void ecom_interface_query_ownership_by_sandbox_ids(Ref<RefCounted> options);

    // -----
    // UI Interface
    // -----
    bool ui_interface_get_friends_exclusive_input(Ref<RefCounted> options);
    bool ui_interface_get_friends_visible(Ref<RefCounted> options);
    bool ui_interface_is_social_overlay_paused(Ref<RefCounted> options);
    bool ui_interface_is_valid_button_combination(int button_combination);
    bool ui_interface_is_valid_key_combination(int key_combination);
    int ui_interface_acknowledge_event_id(Ref<RefCounted> options);
    int ui_interface_get_notification_location_preference();
    int ui_interface_get_toggle_friends_button(Ref<RefCounted> options);
    int ui_interface_get_toggle_friends_key(Ref<RefCounted> options);
    int ui_interface_pause_social_overlay(Ref<RefCounted> options);
    int ui_interface_pre_present(Ref<RefCounted> options);
    int ui_interface_report_input_state(Ref<RefCounted> options);
    int ui_interface_set_display_preference(Ref<RefCounted> options);
    int ui_interface_set_toggle_friends_button(Ref<RefCounted> options);
    int ui_interface_set_toggle_friends_key(Ref<RefCounted> options);
    void ui_interface_hide_friends(Ref<RefCounted> options);
    void ui_interface_show_block_player(Ref<RefCounted> options);
    void ui_interface_show_friends(Ref<RefCounted> options);
    void ui_interface_show_native_profile(Ref<RefCounted> options);
    void ui_interface_show_report_player(Ref<RefCounted> options);

    // -----
    // KWS Interface
    // -----
    Dictionary kws_interface_copy_permission_by_index(Ref<RefCounted> options);
    Dictionary kws_interface_get_permission_by_key(Ref<RefCounted> options);
    int kws_interface_get_permissions_count(Ref<RefCounted> options);
    void kws_interface_create_user(Ref<RefCounted> options);
    void kws_interface_query_age_gate(Ref<RefCounted> options);
    void kws_interface_query_permissions(Ref<RefCounted> options);
    void kws_interface_request_permissions(Ref<RefCounted> options);
    void kws_interface_update_parent_email(Ref<RefCounted> options);

    // -----
    // Metrics Interface
    // -----
    int metrics_interface_begin_player_session(Ref<RefCounted> options);
    int metrics_interface_end_player_session(Ref<RefCounted> options);

    // -----
    // Mods Interface
    // -----
    Dictionary mods_interface_copy_mod_info(Ref<RefCounted> options);
    void mods_interface_enumerate_mods(Ref<RefCounted> options);
    void mods_interface_install_mod(Ref<RefCounted> options);
    void mods_interface_uninstall_mod(Ref<RefCounted> options);
    void mods_interface_update_mod(Ref<RefCounted> options);

    // -----
    // Reports Interface
    // -----
    void reports_interface_send_player_behavior_report(Ref<RefCounted> options);

    // -----
    // Progression Snapshot Interface
    // -----
    Dictionary progression_snapshot_interface_begin_snapshot(Ref<RefCounted> options);
    int progression_snapshot_interface_add_progression(Ref<RefCounted> options);
    int progression_snapshot_interface_end_snapshot(Ref<RefCounted> options);
    void progression_snapshot_interface_delete_snapshot(Ref<RefCounted> options);
    void progression_snapshot_interface_submit_snapshot(Ref<RefCounted> options);

    // -----
    // Presence Interface
    // -----
    bool presence_interface_has_presence(Ref<RefCounted> options);
    Dictionary presence_interface_copy_presence(Ref<RefCounted> options);
    Dictionary presence_interface_create_presence_modification(Ref<RefCounted> options);
    Dictionary presence_interface_get_join_info(Ref<RefCounted> options);
    void presence_interface_query_presence(Ref<RefCounted> options);
    void presence_interface_set_presence(Ref<RefCounted> options);

    // -----
    // Lobby Interface
    // -----
    Dictionary lobby_interface_copy_lobby_details_by_invite_id(Ref<RefCounted> options);
    Dictionary lobby_interface_copy_lobby_details_by_ui_event_id(Ref<RefCounted> options);
    Dictionary lobby_interface_copy_lobby_details(Ref<RefCounted> options);
    Dictionary lobby_interface_create_lobby_search(Ref<RefCounted> options);
    Dictionary lobby_interface_get_connect_string(Ref<RefCounted> options);
    Dictionary lobby_interface_get_invite_id_by_index(Ref<RefCounted> options);
    Dictionary lobby_interface_get_rtc_room_name(Ref<RefCounted> options);
    Dictionary lobby_interface_is_rtc_room_connected(Ref<RefCounted> options);
    Dictionary lobby_interface_parse_connect_string(Ref<RefCounted> options);
    Dictionary lobby_interface_update_lobby_modification(Ref<RefCounted> options);
    int lobby_interface_get_invite_count(Ref<RefCounted> options);
    void lobby_interface_create_lobby(Ref<RefCounted> options);
    void lobby_interface_destroy_lobby(Ref<RefCounted> options);
    void lobby_interface_hard_mute_member(Ref<RefCounted> options);
    void lobby_interface_join_lobby_by_id(Ref<RefCounted> options);
    void lobby_interface_join_lobby(Ref<RefCounted> options);
    void lobby_interface_join_rtc_room(Ref<RefCounted> options);
    void lobby_interface_kick_member(Ref<RefCounted> options);
    void lobby_interface_leave_lobby(Ref<RefCounted> options);
    void lobby_interface_leave_rtc_room(Ref<RefCounted> options);
    void lobby_interface_promote_member(Ref<RefCounted> options);
    void lobby_interface_query_invites(Ref<RefCounted> options);
    void lobby_interface_reject_invite(Ref<RefCounted> options);
    void lobby_interface_send_invite(Ref<RefCounted> options);
    void lobby_interface_update_lobby(Ref<RefCounted> options);

    // -----
    // P2P Interface
    // -----

    // Available in Godot functions
    Dictionary p2p_interface_get_packet_queue_info();
    Dictionary p2p_interface_get_port_range();
    Dictionary p2p_interface_get_nat_type();
    Dictionary p2p_interface_get_relay_control();
    void p2p_interface_query_nat_type();
    int p2p_interface_set_packet_queue_size(Ref<RefCounted> options);
    int p2p_interface_set_port_range(Ref<RefCounted> options);
    int p2p_interface_set_relay_control(int control);

    // Not available in Godot. Called by EOSGMultiplayerPeer
    EOS_EResult _p2p_accept_connection(const EOS_P2P_AcceptConnectionOptions *options);
    EOS_EResult _p2p_clear_packet_queue(const EOS_P2P_ClearPacketQueueOptions *options);
    EOS_EResult _p2p_close_all_connections(const EOS_P2P_CloseConnectionsOptions *options);
    EOS_EResult _p2p_close_connection(const EOS_P2P_CloseConnectionOptions *options);
    EOS_EResult _p2p_get_next_packet_size(const EOS_P2P_GetNextReceivedPacketSizeOptions *options, uint32_t *out_size);
    EOS_EResult _p2p_receive_packet(const EOS_P2P_ReceivePacketOptions *options, void *out_packet_data, uint32_t *out_packet_size, uint8_t *out_channel, EOS_ProductUserId *remote_user, EOS_P2P_SocketId *out_socket);
    EOS_EResult _p2p_send_packet(const EOS_P2P_SendPacketOptions *options);
    EOS_NotificationId _p2p_add_notify_peer_connection_closed(const EOS_P2P_AddNotifyPeerConnectionClosedOptions *options, EOS_P2P_OnRemoteConnectionClosedCallback callback);
    EOS_NotificationId _p2p_add_notify_peer_connection_established(const EOS_P2P_AddNotifyPeerConnectionEstablishedOptions *options, EOS_P2P_OnPeerConnectionEstablishedCallback callback);
    EOS_NotificationId _p2p_add_notify_peer_connection_interrupted(const EOS_P2P_AddNotifyPeerConnectionInterruptedOptions *options, EOS_P2P_OnPeerConnectionInterruptedCallback callback);
    EOS_NotificationId _p2p_add_notify_peer_connection_request(const EOS_P2P_AddNotifyPeerConnectionRequestOptions *options, EOS_P2P_OnIncomingConnectionRequestCallback callback);
    void _p2p_remove_notify_peer_connection_closed(EOS_NotificationId callback_id);
    void _p2p_remove_notify_peer_connection_established(EOS_NotificationId callback_id);
    void _p2p_remove_notify_peer_connection_interrupted(EOS_NotificationId callback_id);
    void _p2p_remove_notify_peer_connection_request(EOS_NotificationId callback_id);

    // -----
    // PlayerDataStorage Interface
    // -----
    Dictionary playerdatastorage_interface_copy_file_metadata_at_index(Ref<RefCounted> options);
    Dictionary playerdatastorage_interface_copy_file_metadata_by_filename(Ref<RefCounted> options);
    Dictionary playerdatastorage_interface_get_file_metadata_count(Ref<RefCounted> options);
    int playerdatastorage_interface_delete_cache(Ref<RefCounted> options);
    Variant playerdatastorage_interface_read_file(Ref<RefCounted> options);
    Variant playerdatastorage_interface_write_file(Ref<RefCounted> options);
    void playerdatastorage_interface_delete_file(Ref<RefCounted> options);
    void playerdatastorage_interface_duplicate_file(Ref<RefCounted> options);
    void playerdatastorage_interface_query_file_list(Ref<RefCounted> options);
    void playerdatastorage_interface_query_file(Ref<RefCounted> options);

    // -----
    // TitleStorage Interface
    // -----
    Dictionary titlestorage_interface_copy_file_metadata_at_index(Ref<RefCounted> options);
    Dictionary titlestorage_interface_copy_file_metadata_by_filename(Ref<RefCounted> options);
    int titlestorage_interface_delete_cache(Ref<RefCounted> options);
    int titlestorage_interface_get_file_metadata_count(Ref<RefCounted> options);
    Variant titlestorage_interface_read_file(Ref<RefCounted> options);
    void titlestorage_interface_query_file_list(Ref<RefCounted> options);
    void titlestorage_interface_query_file(Ref<RefCounted> options);

    // -----
    // Sanctions Interface
    // -----
    Dictionary sanctions_interface_copy_player_sanction_by_index(Ref<RefCounted> options);
    int sanctions_interface_get_player_sanction_count(Ref<RefCounted> options);
    void sanctions_interface_query_active_player_sanctions(Ref<RefCounted> options);
    void sanctions_interface_create_player_sanction_appeal(Ref<RefCounted> options);

    // -----
    // Sessions Interface
    // -----
    Dictionary sessions_interface_copy_active_session_details(Ref<RefCounted> options);
    Dictionary sessions_interface_copy_session_details_by_invite_id(Ref<RefCounted> options);
    Dictionary sessions_interface_copy_session_details_by_ui_event_id(Ref<RefCounted> options);
    Dictionary sessions_interface_copy_session_details_for_presence(Ref<RefCounted> options);
    Dictionary sessions_interface_create_session_modification(Ref<RefCounted> options);
    Dictionary sessions_interface_create_session_search(Ref<RefCounted> options);
    Dictionary sessions_interface_get_invite_id_by_index(Ref<RefCounted> options);
    Dictionary sessions_interface_update_session_modification(Ref<RefCounted> options);
    int sessions_interface_dump_session_state(Ref<RefCounted> options);
    int sessions_interface_get_invite_count(Ref<RefCounted> options);
    int sessions_interface_is_user_in_session(Ref<RefCounted> options);
    void sessions_interface_destroy_session(Ref<RefCounted> options);
    void sessions_interface_end_session(Ref<RefCounted> options);
    void sessions_interface_join_session(Ref<RefCounted> options);
    void sessions_interface_query_invites(Ref<RefCounted> options);
    void sessions_interface_register_players(Ref<RefCounted> options);
    void sessions_interface_reject_invite(Ref<RefCounted> options);
    void sessions_interface_send_invite(Ref<RefCounted> options);
    void sessions_interface_start_session(Ref<RefCounted> options);
    void sessions_interface_unregister_players(Ref<RefCounted> options);
    void sessions_interface_update_session(Ref<RefCounted> options);

    // -----
    // RTCAudio Interface
    // -----
    Dictionary rtc_audio_interface_copy_input_device_information_by_index(Ref<RefCounted> options);
    Dictionary rtc_audio_interface_copy_output_device_information_by_index(Ref<RefCounted> options);
    int rtc_audio_interface_add_notify_audio_before_render(Ref<RefCounted> options);
    int rtc_audio_interface_add_notify_audio_before_send(Ref<RefCounted> options);
    int rtc_audio_interface_add_notify_audio_input_state(Ref<RefCounted> options);
    int rtc_audio_interface_add_notify_audio_output_state(Ref<RefCounted> options);
    int rtc_audio_interface_add_notify_participant_updated(Ref<RefCounted> options);
    int rtc_audio_interface_get_input_devices_count(Ref<RefCounted> options);
    int rtc_audio_interface_get_output_devices_count(Ref<RefCounted> options);
    int rtc_audio_interface_send_audio(Ref<RefCounted> options);
    void rtc_audio_interface_query_input_devices_information(Ref<RefCounted> options);
    void rtc_audio_interface_query_output_devices_information(Ref<RefCounted> options);
    void rtc_audio_interface_register_platform_user(Ref<RefCounted> options);
    void rtc_audio_interface_remove_notify_audio_before_render(int notification_id);
    void rtc_audio_interface_remove_notify_audio_before_send(int notification_id);
    void rtc_audio_interface_remove_notify_audio_input_state(int notification_id);
    void rtc_audio_interface_remove_notify_audio_output_state(int notification_id);
    void rtc_audio_interface_remove_notify_participant_updated(int notification_id);
    void rtc_audio_interface_set_input_device_settings(Ref<RefCounted> options);
    void rtc_audio_interface_set_output_device_settings(Ref<RefCounted> options);
    void rtc_audio_interface_unregister_platform_user(Ref<RefCounted> options);
    void rtc_audio_interface_update_participant_volume(Ref<RefCounted> options);
    void rtc_audio_interface_update_receiving(Ref<RefCounted> options);
    void rtc_audio_interface_update_receiving_volume(Ref<RefCounted> options);
    void rtc_audio_interface_update_sending(Ref<RefCounted> options);
    void rtc_audio_interface_update_sending_volume(Ref<RefCounted> options);

    // -----
    // RTCData Interface
    // -----
    int rtc_data_interface_add_notify_data_received(Ref<RefCounted> options);
    int rtc_data_interface_add_notify_participant_updated(Ref<RefCounted> options);
    int rtc_data_interface_send_data(Ref<RefCounted> options);
    void rtc_data_interface_remove_notify_data_received(int notification_id);
    void rtc_data_interface_remove_notify_participant_updated(int notification_id);
    void rtc_data_interface_update_receiving(Ref<RefCounted> options);
    void rtc_data_interface_update_sending(Ref<RefCounted> options);

    // -----
    // RTC Interface
    // -----
    // TODO: impl rtc_interface_get_data_interface
    int rtc_interface_add_notify_disconnected(Ref<RefCounted> options);
    int rtc_interface_add_notify_participant_status_changed(Ref<RefCounted> options);
    int rtc_interface_add_notify_room_statistics_updated(Ref<RefCounted> options);
    int rtc_interface_set_room_setting(Ref<RefCounted> options);
    int rtc_interface_set_setting(Ref<RefCounted> options);
    void rtc_interface_block_participant(Ref<RefCounted> options);
    void rtc_interface_join_room(Ref<RefCounted> options);
    void rtc_interface_leave_room(Ref<RefCounted> options);
    void rtc_interface_remove_notify_disconnected(int notification_id);
    void rtc_interface_remove_notify_participant_status_changed(int notification_id);
    void rtc_interface_remove_notify_room_statistics_updated(int notification_id);

    // -----
    // AntiCheatServer Interface
    // -----
    int anticheat_server_interface_begin_session(Ref<RefCounted> options);
    int anticheat_server_interface_end_session(Ref<RefCounted> options);
    int anticheat_server_interface_register_client(Ref<RefCounted> options);
    int anticheat_server_interface_unregister_client(Ref<RefCounted> options);
    int anticheat_server_interface_receive_message_from_client(Ref<RefCounted> options);
    int anticheat_server_interface_set_client_details(Ref<RefCounted> options);
    int anticheat_server_interface_set_game_session_id(Ref<RefCounted> options);
    int anticheat_server_interface_set_client_network_state(Ref<RefCounted> options);
    Dictionary anticheat_server_interface_get_protect_message_output_length(Ref<RefCounted> options);
    Dictionary anticheat_server_interface_protect_message(Ref<RefCounted> options);
    Dictionary anticheat_server_interface_unprotect_message(Ref<RefCounted> options);
    int anticheat_server_interface_register_event(Ref<RefCounted> options);
    int anticheat_server_interface_log_event(Ref<RefCounted> options);
    int anticheat_server_interface_log_game_round_start(Ref<RefCounted> options);
    int anticheat_server_interface_log_game_round_end(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_spawn(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_despawn(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_revive(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_tick(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_use_weapon(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_use_ability(Ref<RefCounted> options);
    int anticheat_server_interface_log_player_take_damage(Ref<RefCounted> options);

    // -----
    // AntiCheatClient Interface
    // -----
    int anticheat_client_interface_begin_session(Ref<RefCounted> options);
    int anticheat_client_interface_end_session(Ref<RefCounted> options);
    int anticheat_client_interface_add_external_integrity_catalog(Ref<RefCounted> options);
    int anticheat_client_interface_receive_message_from_server(Ref<RefCounted> options);
    Dictionary anticheat_client_interface_get_protect_message_output_length(Ref<RefCounted> options);
    Dictionary anticheat_client_interface_protect_message(Ref<RefCounted> options);
    Dictionary anticheat_client_interface_unprotect_message(Ref<RefCounted> options);
    int anticheat_client_interface_register_peer(Ref<RefCounted> options);
    int anticheat_client_interface_unregister_peer(Ref<RefCounted> options);
    int anticheat_client_interface_receive_message_from_peer(Ref<RefCounted> options);

    void *_anticheat_player_id_to_handle(CharString p_player_id) {
        std::string player_id = p_player_id.get_data();

        if (anticheat_client_map.count(player_id) == 0) {
            anticheat_client_map[player_id] = malloc(1);
        }
        return anticheat_client_map[player_id];
    }

    String _anticheat_player_handle_to_id(void *p_player_handle) {
        for (auto const &[key, val] : anticheat_client_map) {
            if (val == p_player_handle) {
                return String(key.c_str());
            }
        }
        return String("");
    }
};

} // namespace godot

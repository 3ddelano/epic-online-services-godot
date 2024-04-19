#include "ieos.h"

using namespace godot;

#define IEOS_BIND_METHOD(m_name) ClassDB::bind_method(D_METHOD(#m_name), &IEOS::m_name)
#define IEOS_BIND_SIGNAL(m_name) ADD_SIGNAL(MethodInfo(#m_name, PropertyInfo(Variant::DICTIONARY, "callback_data")))

void IEOS::_bind_methods() {
    ClassDB::bind_static_method("IEOS", D_METHOD("tick"), &IEOS::tick);
    IEOS_BIND_METHOD(is_operation_complete);
    IEOS_BIND_METHOD(shutdown);

    // EOS Methods
    IEOS_BIND_METHOD(achievements_interface_copy_achievement_definition_v2_by_achievement_id);
    IEOS_BIND_METHOD(achievements_interface_copy_achievement_definition_v2_by_index);
    IEOS_BIND_METHOD(achievements_interface_copy_player_achievement_by_achievement_id);
    IEOS_BIND_METHOD(achievements_interface_copy_player_achievement_by_index);
    IEOS_BIND_METHOD(achievements_interface_get_achievement_definition_count);
    IEOS_BIND_METHOD(achievements_interface_get_player_achievement_count);
    IEOS_BIND_METHOD(achievements_interface_query_definitions);
    IEOS_BIND_METHOD(achievements_interface_query_player_achievements);
    IEOS_BIND_METHOD(achievements_interface_unlock_achievements);
    IEOS_BIND_METHOD(auth_interface_copy_id_token);
    IEOS_BIND_METHOD(auth_interface_copy_user_auth_token);
    IEOS_BIND_METHOD(auth_interface_delete_persistent_auth);
    IEOS_BIND_METHOD(auth_interface_get_logged_in_account_by_index);
    IEOS_BIND_METHOD(auth_interface_get_logged_in_accounts_count);
    IEOS_BIND_METHOD(auth_interface_get_login_status);
    IEOS_BIND_METHOD(auth_interface_get_merged_account_by_index);
    IEOS_BIND_METHOD(auth_interface_get_merged_accounts_count);
    IEOS_BIND_METHOD(auth_interface_get_selected_account_id);
    IEOS_BIND_METHOD(auth_interface_link_account);
    IEOS_BIND_METHOD(auth_interface_login);
    IEOS_BIND_METHOD(auth_interface_logout);
    IEOS_BIND_METHOD(auth_interface_query_id_token);
    IEOS_BIND_METHOD(auth_interface_verify_id_token);
    IEOS_BIND_METHOD(auth_interface_verify_user_auth);
    IEOS_BIND_METHOD(connect_interface_copy_id_token);
    IEOS_BIND_METHOD(connect_interface_copy_product_user_external_account_by_account_id);
    IEOS_BIND_METHOD(connect_interface_copy_product_user_external_account_by_account_type);
    IEOS_BIND_METHOD(connect_interface_copy_product_user_external_account_by_index);
    IEOS_BIND_METHOD(connect_interface_copy_product_user_info);
    IEOS_BIND_METHOD(connect_interface_create_device_id);
    IEOS_BIND_METHOD(connect_interface_create_user);
    IEOS_BIND_METHOD(connect_interface_delete_device_id);
    IEOS_BIND_METHOD(connect_interface_get_external_account_mapping);
    IEOS_BIND_METHOD(connect_interface_get_logged_in_user_by_index);
    IEOS_BIND_METHOD(connect_interface_get_logged_in_users_count);
    IEOS_BIND_METHOD(connect_interface_get_login_status);
    IEOS_BIND_METHOD(connect_interface_get_product_user_external_account_count);
    IEOS_BIND_METHOD(connect_interface_get_product_user_id_mapping);
    IEOS_BIND_METHOD(connect_interface_link_account);
    IEOS_BIND_METHOD(connect_interface_login);
    IEOS_BIND_METHOD(connect_interface_logout);
    IEOS_BIND_METHOD(connect_interface_query_product_user_id_mappings);
    IEOS_BIND_METHOD(connect_interface_transfer_device_id_account);
    IEOS_BIND_METHOD(connect_interface_unlink_account);
    IEOS_BIND_METHOD(connect_interface_verify_id_token);
    IEOS_BIND_METHOD(custom_invites_interface_accept_request_to_join);
    IEOS_BIND_METHOD(custom_invites_interface_finalize_invite);
    IEOS_BIND_METHOD(custom_invites_interface_reject_request_to_join);
    IEOS_BIND_METHOD(custom_invites_interface_send_custom_invite);
    IEOS_BIND_METHOD(custom_invites_interface_send_request_to_join);
    IEOS_BIND_METHOD(custom_invites_interface_set_custom_invite);
    IEOS_BIND_METHOD(ecom_interface_checkout);
    IEOS_BIND_METHOD(ecom_interface_copy_entitlement_by_id);
    IEOS_BIND_METHOD(ecom_interface_copy_entitlement_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_entitlement_by_name_and_index);
    IEOS_BIND_METHOD(ecom_interface_copy_item_by_id);
    IEOS_BIND_METHOD(ecom_interface_copy_item_image_info_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_item_release_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_last_redeemed_entitlement_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_offer_by_id);
    IEOS_BIND_METHOD(ecom_interface_copy_offer_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_offer_image_info_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_offer_item_by_index);
    IEOS_BIND_METHOD(ecom_interface_copy_transaction_by_id);
    IEOS_BIND_METHOD(ecom_interface_copy_transaction_by_index);
    IEOS_BIND_METHOD(ecom_interface_get_entitlements_by_name_count);
    IEOS_BIND_METHOD(ecom_interface_get_entitlements_count);
    IEOS_BIND_METHOD(ecom_interface_get_item_image_info_count);
    IEOS_BIND_METHOD(ecom_interface_get_item_release_count);
    IEOS_BIND_METHOD(ecom_interface_get_last_redeemed_entitlements_count);
    IEOS_BIND_METHOD(ecom_interface_get_offer_count);
    IEOS_BIND_METHOD(ecom_interface_get_offer_image_info_count);
    IEOS_BIND_METHOD(ecom_interface_get_offer_item_count);
    IEOS_BIND_METHOD(ecom_interface_get_transaction_count);
    IEOS_BIND_METHOD(ecom_interface_query_entitlement_token);
    IEOS_BIND_METHOD(ecom_interface_query_entitlements);
    IEOS_BIND_METHOD(ecom_interface_query_offers);
    IEOS_BIND_METHOD(ecom_interface_query_ownership);
    IEOS_BIND_METHOD(ecom_interface_query_ownership_by_sandbox_ids);
    IEOS_BIND_METHOD(ecom_interface_query_ownership_token);
    IEOS_BIND_METHOD(ecom_interface_redeem_entitlements);
    IEOS_BIND_METHOD(friends_interface_accept_invite);
    IEOS_BIND_METHOD(friends_interface_get_blocked_user_at_index);
    IEOS_BIND_METHOD(friends_interface_get_blocked_users_count);
    IEOS_BIND_METHOD(friends_interface_get_friend_at_index);
    IEOS_BIND_METHOD(friends_interface_get_friends_count);
    IEOS_BIND_METHOD(friends_interface_get_status);
    IEOS_BIND_METHOD(friends_interface_query_friends);
    IEOS_BIND_METHOD(friends_interface_reject_invite);
    IEOS_BIND_METHOD(friends_interface_send_invite);
    IEOS_BIND_METHOD(kws_interface_copy_permission_by_index);
    IEOS_BIND_METHOD(kws_interface_create_user);
    IEOS_BIND_METHOD(kws_interface_get_permission_by_key);
    IEOS_BIND_METHOD(kws_interface_get_permissions_count);
    IEOS_BIND_METHOD(kws_interface_query_age_gate);
    IEOS_BIND_METHOD(kws_interface_query_permissions);
    IEOS_BIND_METHOD(kws_interface_request_permissions);
    IEOS_BIND_METHOD(kws_interface_update_parent_email);
    IEOS_BIND_METHOD(leaderboards_interface_copy_leaderboard_definition_by_index);
    IEOS_BIND_METHOD(leaderboards_interface_copy_leaderboard_definition_by_leaderboard_id);
    IEOS_BIND_METHOD(leaderboards_interface_copy_leaderboard_record_by_index);
    IEOS_BIND_METHOD(leaderboards_interface_copy_leaderboard_record_by_user_id);
    IEOS_BIND_METHOD(leaderboards_interface_copy_leaderboard_user_score_by_index);
    IEOS_BIND_METHOD(leaderboards_interface_copy_leaderboard_user_score_by_user_id);
    IEOS_BIND_METHOD(leaderboards_interface_get_leaderboard_definition_count);
    IEOS_BIND_METHOD(leaderboards_interface_get_leaderboard_record_count);
    IEOS_BIND_METHOD(leaderboards_interface_get_leaderboard_user_score_count);
    IEOS_BIND_METHOD(leaderboards_interface_query_leaderboard_definitions);
    IEOS_BIND_METHOD(leaderboards_interface_query_leaderboard_ranks);
    IEOS_BIND_METHOD(leaderboards_interface_query_leaderboard_user_scores);
    IEOS_BIND_METHOD(lobby_interface_copy_lobby_details);
    IEOS_BIND_METHOD(lobby_interface_copy_lobby_details_by_invite_id);
    IEOS_BIND_METHOD(lobby_interface_copy_lobby_details_by_ui_event_id);
    IEOS_BIND_METHOD(lobby_interface_create_lobby);
    IEOS_BIND_METHOD(lobby_interface_create_lobby_search);
    IEOS_BIND_METHOD(lobby_interface_destroy_lobby);
    IEOS_BIND_METHOD(lobby_interface_get_connect_string);
    IEOS_BIND_METHOD(lobby_interface_get_invite_count);
    IEOS_BIND_METHOD(lobby_interface_get_invite_id_by_index);
    IEOS_BIND_METHOD(lobby_interface_get_rtc_room_name);
    IEOS_BIND_METHOD(lobby_interface_hard_mute_member);
    IEOS_BIND_METHOD(lobby_interface_is_rtc_room_connected);
    IEOS_BIND_METHOD(lobby_interface_join_lobby);
    IEOS_BIND_METHOD(lobby_interface_join_lobby_by_id);
    IEOS_BIND_METHOD(lobby_interface_kick_member);
    IEOS_BIND_METHOD(lobby_interface_leave_lobby);
    IEOS_BIND_METHOD(lobby_interface_parse_connect_string);
    IEOS_BIND_METHOD(lobby_interface_promote_member);
    IEOS_BIND_METHOD(lobby_interface_query_invites);
    IEOS_BIND_METHOD(lobby_interface_reject_invite);
    IEOS_BIND_METHOD(lobby_interface_send_invite);
    IEOS_BIND_METHOD(lobby_interface_update_lobby);
    IEOS_BIND_METHOD(lobby_interface_update_lobby_modification);
    IEOS_BIND_METHOD(logging_interface_set_log_level);
    IEOS_BIND_METHOD(metrics_interface_begin_player_session);
    IEOS_BIND_METHOD(metrics_interface_end_player_session);
    IEOS_BIND_METHOD(mods_interface_copy_mod_info);
    IEOS_BIND_METHOD(mods_interface_enumerate_mods);
    IEOS_BIND_METHOD(mods_interface_install_mod);
    IEOS_BIND_METHOD(mods_interface_uninstall_mod);
    IEOS_BIND_METHOD(mods_interface_update_mod);
    IEOS_BIND_METHOD(p2p_interface_get_nat_type);
    IEOS_BIND_METHOD(p2p_interface_get_packet_queue_info);
    IEOS_BIND_METHOD(p2p_interface_get_port_range);
    IEOS_BIND_METHOD(p2p_interface_get_relay_control);
    IEOS_BIND_METHOD(p2p_interface_query_nat_type);
    IEOS_BIND_METHOD(p2p_interface_set_packet_queue_size);
    IEOS_BIND_METHOD(p2p_interface_set_port_range);
    IEOS_BIND_METHOD(p2p_interface_set_relay_control);
    IEOS_BIND_METHOD(platform_interface_check_for_launcher_and_restart);
    IEOS_BIND_METHOD(platform_interface_create);
    IEOS_BIND_METHOD(platform_interface_get_active_country_code);
    IEOS_BIND_METHOD(platform_interface_get_active_locale_code);
    IEOS_BIND_METHOD(platform_interface_get_application_status);
    IEOS_BIND_METHOD(platform_interface_get_desktop_crossplay_status_info);
    IEOS_BIND_METHOD(platform_interface_get_network_status);
    IEOS_BIND_METHOD(platform_interface_get_override_country_code);
    IEOS_BIND_METHOD(platform_interface_get_override_locale_code);
    IEOS_BIND_METHOD(platform_interface_initialize);
    IEOS_BIND_METHOD(platform_interface_release);
    IEOS_BIND_METHOD(platform_interface_set_application_status);
    IEOS_BIND_METHOD(platform_interface_set_network_status);
    IEOS_BIND_METHOD(platform_interface_set_override_country_code);
    IEOS_BIND_METHOD(platform_interface_set_override_locale_code);
    IEOS_BIND_METHOD(platform_interface_shutdown);
    IEOS_BIND_METHOD(playerdatastorage_interface_copy_file_metadata_at_index);
    IEOS_BIND_METHOD(playerdatastorage_interface_copy_file_metadata_by_filename);
    IEOS_BIND_METHOD(playerdatastorage_interface_delete_cache);
    IEOS_BIND_METHOD(playerdatastorage_interface_delete_file);
    IEOS_BIND_METHOD(playerdatastorage_interface_duplicate_file);
    IEOS_BIND_METHOD(playerdatastorage_interface_get_file_metadata_count);
    IEOS_BIND_METHOD(playerdatastorage_interface_query_file);
    IEOS_BIND_METHOD(playerdatastorage_interface_query_file_list);
    IEOS_BIND_METHOD(playerdatastorage_interface_read_file);
    IEOS_BIND_METHOD(playerdatastorage_interface_write_file);
    IEOS_BIND_METHOD(presence_interface_copy_presence);
    IEOS_BIND_METHOD(presence_interface_create_presence_modification);
    IEOS_BIND_METHOD(presence_interface_get_join_info);
    IEOS_BIND_METHOD(presence_interface_has_presence);
    IEOS_BIND_METHOD(presence_interface_query_presence);
    IEOS_BIND_METHOD(presence_interface_set_presence);
    IEOS_BIND_METHOD(progression_snapshot_interface_add_progression);
    IEOS_BIND_METHOD(progression_snapshot_interface_begin_snapshot);
    IEOS_BIND_METHOD(progression_snapshot_interface_delete_snapshot);
    IEOS_BIND_METHOD(progression_snapshot_interface_end_snapshot);
    IEOS_BIND_METHOD(progression_snapshot_interface_submit_snapshot);
    IEOS_BIND_METHOD(reports_interface_send_player_behavior_report);
    IEOS_BIND_METHOD(rtc_audio_interface_add_notify_audio_before_render);
    IEOS_BIND_METHOD(rtc_audio_interface_add_notify_audio_before_send);
    IEOS_BIND_METHOD(rtc_audio_interface_add_notify_audio_input_state);
    IEOS_BIND_METHOD(rtc_audio_interface_add_notify_audio_output_state);
    IEOS_BIND_METHOD(rtc_audio_interface_add_notify_participant_updated);
    IEOS_BIND_METHOD(rtc_audio_interface_copy_input_device_information_by_index);
    IEOS_BIND_METHOD(rtc_audio_interface_copy_output_device_information_by_index);
    IEOS_BIND_METHOD(rtc_audio_interface_get_input_devices_count);
    IEOS_BIND_METHOD(rtc_audio_interface_get_output_devices_count);
    IEOS_BIND_METHOD(rtc_audio_interface_query_input_devices_information);
    IEOS_BIND_METHOD(rtc_audio_interface_query_output_devices_information);
    IEOS_BIND_METHOD(rtc_audio_interface_register_platform_user);
    IEOS_BIND_METHOD(rtc_audio_interface_remove_notify_audio_before_render);
    IEOS_BIND_METHOD(rtc_audio_interface_remove_notify_audio_before_send);
    IEOS_BIND_METHOD(rtc_audio_interface_remove_notify_audio_input_state);
    IEOS_BIND_METHOD(rtc_audio_interface_remove_notify_audio_output_state);
    IEOS_BIND_METHOD(rtc_audio_interface_remove_notify_participant_updated);
    IEOS_BIND_METHOD(rtc_audio_interface_send_audio);
    IEOS_BIND_METHOD(rtc_audio_interface_set_input_device_settings);
    IEOS_BIND_METHOD(rtc_audio_interface_set_output_device_settings);
    IEOS_BIND_METHOD(rtc_audio_interface_unregister_platform_user);
    IEOS_BIND_METHOD(rtc_audio_interface_update_participant_volume);
    IEOS_BIND_METHOD(rtc_audio_interface_update_receiving);
    IEOS_BIND_METHOD(rtc_audio_interface_update_receiving_volume);
    IEOS_BIND_METHOD(rtc_audio_interface_update_sending);
    IEOS_BIND_METHOD(rtc_audio_interface_update_sending_volume);
    IEOS_BIND_METHOD(rtc_interface_add_notify_disconnected);
    IEOS_BIND_METHOD(rtc_interface_add_notify_participant_status_changed);
    IEOS_BIND_METHOD(rtc_interface_add_notify_room_statistics_updated);
    IEOS_BIND_METHOD(rtc_interface_block_participant);
    IEOS_BIND_METHOD(rtc_interface_join_room);
    IEOS_BIND_METHOD(rtc_interface_leave_room);
    IEOS_BIND_METHOD(rtc_interface_remove_notify_disconnected);
    IEOS_BIND_METHOD(rtc_interface_remove_notify_participant_status_changed);
    IEOS_BIND_METHOD(rtc_interface_remove_notify_room_statistics_updated);
    IEOS_BIND_METHOD(rtc_interface_set_room_setting);
    IEOS_BIND_METHOD(rtc_interface_set_setting);
    IEOS_BIND_METHOD(sanctions_interface_copy_player_sanction_by_index);
    IEOS_BIND_METHOD(sanctions_interface_get_player_sanction_count);
    IEOS_BIND_METHOD(sanctions_interface_query_active_player_sanctions);
    IEOS_BIND_METHOD(sanctions_interface_create_player_sanction_appeal);
    IEOS_BIND_METHOD(sessions_interface_copy_active_session_details);
    IEOS_BIND_METHOD(sessions_interface_copy_session_details_by_invite_id);
    IEOS_BIND_METHOD(sessions_interface_copy_session_details_by_ui_event_id);
    IEOS_BIND_METHOD(sessions_interface_copy_session_details_for_presence);
    IEOS_BIND_METHOD(sessions_interface_create_session_modification);
    IEOS_BIND_METHOD(sessions_interface_create_session_search);
    IEOS_BIND_METHOD(sessions_interface_destroy_session);
    IEOS_BIND_METHOD(sessions_interface_dump_session_state);
    IEOS_BIND_METHOD(sessions_interface_end_session);
    IEOS_BIND_METHOD(sessions_interface_get_invite_count);
    IEOS_BIND_METHOD(sessions_interface_get_invite_id_by_index);
    IEOS_BIND_METHOD(sessions_interface_is_user_in_session);
    IEOS_BIND_METHOD(sessions_interface_join_session);
    IEOS_BIND_METHOD(sessions_interface_query_invites);
    IEOS_BIND_METHOD(sessions_interface_register_players);
    IEOS_BIND_METHOD(sessions_interface_reject_invite);
    IEOS_BIND_METHOD(sessions_interface_send_invite);
    IEOS_BIND_METHOD(sessions_interface_start_session);
    IEOS_BIND_METHOD(sessions_interface_unregister_players);
    IEOS_BIND_METHOD(sessions_interface_update_session);
    IEOS_BIND_METHOD(sessions_interface_update_session_modification);
    IEOS_BIND_METHOD(stats_interface_copy_stat_by_index);
    IEOS_BIND_METHOD(stats_interface_copy_stat_by_name);
    IEOS_BIND_METHOD(stats_interface_get_stats_count);
    IEOS_BIND_METHOD(stats_interface_ingest_stat);
    IEOS_BIND_METHOD(stats_interface_query_stats);
    IEOS_BIND_METHOD(titlestorage_interface_copy_file_metadata_at_index);
    IEOS_BIND_METHOD(titlestorage_interface_copy_file_metadata_by_filename);
    IEOS_BIND_METHOD(titlestorage_interface_delete_cache);
    IEOS_BIND_METHOD(titlestorage_interface_get_file_metadata_count);
    IEOS_BIND_METHOD(titlestorage_interface_query_file);
    IEOS_BIND_METHOD(titlestorage_interface_query_file_list);
    IEOS_BIND_METHOD(titlestorage_interface_read_file);
    IEOS_BIND_METHOD(ui_interface_acknowledge_event_id);
    IEOS_BIND_METHOD(ui_interface_get_friends_exclusive_input);
    IEOS_BIND_METHOD(ui_interface_get_friends_visible);
    IEOS_BIND_METHOD(ui_interface_get_notification_location_preference);
    IEOS_BIND_METHOD(ui_interface_get_toggle_friends_button);
    IEOS_BIND_METHOD(ui_interface_get_toggle_friends_key);
    IEOS_BIND_METHOD(ui_interface_hide_friends);
    IEOS_BIND_METHOD(ui_interface_is_social_overlay_paused);
    IEOS_BIND_METHOD(ui_interface_is_valid_button_combination);
    IEOS_BIND_METHOD(ui_interface_is_valid_key_combination);
    IEOS_BIND_METHOD(ui_interface_pause_social_overlay);
    IEOS_BIND_METHOD(ui_interface_pre_present);
    IEOS_BIND_METHOD(ui_interface_report_input_state);
    IEOS_BIND_METHOD(ui_interface_set_display_preference);
    IEOS_BIND_METHOD(ui_interface_set_toggle_friends_button);
    IEOS_BIND_METHOD(ui_interface_set_toggle_friends_key);
    IEOS_BIND_METHOD(ui_interface_show_block_player);
    IEOS_BIND_METHOD(ui_interface_show_friends);
    IEOS_BIND_METHOD(ui_interface_show_native_profile);
    IEOS_BIND_METHOD(ui_interface_show_report_player);
    IEOS_BIND_METHOD(user_info_interface_copy_best_display_name);
    IEOS_BIND_METHOD(user_info_interface_copy_best_display_name_with_platform);
    IEOS_BIND_METHOD(user_info_interface_copy_external_user_info_by_account_id);
    IEOS_BIND_METHOD(user_info_interface_copy_external_user_info_by_account_type);
    IEOS_BIND_METHOD(user_info_interface_copy_external_user_info_by_index);
    IEOS_BIND_METHOD(user_info_interface_copy_user_info);
    IEOS_BIND_METHOD(user_info_interface_get_external_user_info_count);
    IEOS_BIND_METHOD(user_info_interface_get_local_platform_type);
    IEOS_BIND_METHOD(user_info_interface_query_user_info);
    IEOS_BIND_METHOD(user_info_interface_query_user_info_by_display_name);
    IEOS_BIND_METHOD(user_info_interface_query_user_info_by_external_account);
    IEOS_BIND_METHOD(version_interface_get_constants);
    IEOS_BIND_METHOD(version_interface_get_version);

    ADD_SIGNAL(MethodInfo("logging_interface_callback", PropertyInfo(Variant::DICTIONARY, "log_message")));
    ADD_SIGNAL(MethodInfo("p2p_interface_query_nat_type_callback", PropertyInfo(Variant::INT, "nat_type")));
    IEOS_BIND_SIGNAL(achievements_interface_achievements_unlocked_v2_callback);
    IEOS_BIND_SIGNAL(achievements_interface_query_definitions_callback);
    IEOS_BIND_SIGNAL(achievements_interface_query_player_achievements_callback);
    IEOS_BIND_SIGNAL(achievements_interface_unlock_achievements_callback);
    IEOS_BIND_SIGNAL(auth_interface_delete_persistent_auth_callback);
    IEOS_BIND_SIGNAL(auth_interface_link_account_callback);
    IEOS_BIND_SIGNAL(auth_interface_login_callback);
    IEOS_BIND_SIGNAL(auth_interface_login_status_changed);
    IEOS_BIND_SIGNAL(auth_interface_logout_callback);
    IEOS_BIND_SIGNAL(auth_interface_query_id_token_callback);
    IEOS_BIND_SIGNAL(auth_interface_verify_id_token_callback);
    IEOS_BIND_SIGNAL(auth_interface_verify_user_auth_callback);
    IEOS_BIND_SIGNAL(connect_interface_auth_expiration);
    IEOS_BIND_SIGNAL(connect_interface_create_device_id_callback);
    IEOS_BIND_SIGNAL(connect_interface_create_user_callback);
    IEOS_BIND_SIGNAL(connect_interface_delete_device_id_callback);
    IEOS_BIND_SIGNAL(connect_interface_link_account_callback);
    IEOS_BIND_SIGNAL(connect_interface_login_callback);
    IEOS_BIND_SIGNAL(connect_interface_logout_callback);
    IEOS_BIND_SIGNAL(connect_interface_login_status_changed);
    IEOS_BIND_SIGNAL(connect_interface_query_product_user_id_mappings_callback);
    IEOS_BIND_SIGNAL(connect_interface_transfer_device_id_account_callback);
    IEOS_BIND_SIGNAL(connect_interface_unlink_account_callback);
    IEOS_BIND_SIGNAL(connect_interface_verify_id_token_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_accept_request_to_join_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_custom_invite_accepted_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_custom_invite_received_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_custom_invite_rejected_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_reject_request_to_join_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_request_to_join_accepted_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_request_to_join_received_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_request_to_join_rejected_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_request_to_join_response_received_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_send_custom_invite_callback);
    IEOS_BIND_SIGNAL(custom_invites_interface_send_request_to_join_callback);
    IEOS_BIND_SIGNAL(ecom_interface_checkout_callback);
    IEOS_BIND_SIGNAL(ecom_interface_query_entitlement_token_callback);
    IEOS_BIND_SIGNAL(ecom_interface_query_entitlements_callback);
    IEOS_BIND_SIGNAL(ecom_interface_query_offers_callback);
    IEOS_BIND_SIGNAL(ecom_interface_query_ownership_by_sandbox_ids_callback);
    IEOS_BIND_SIGNAL(ecom_interface_query_ownership_callback);
    IEOS_BIND_SIGNAL(ecom_interface_query_ownership_token_callback);
    IEOS_BIND_SIGNAL(ecom_interface_redeem_entitlements_callback);
    IEOS_BIND_SIGNAL(friends_interface_accept_invite_callback);
    IEOS_BIND_SIGNAL(friends_interface_blocked_users_update_callback);
    IEOS_BIND_SIGNAL(friends_interface_friends_update_callback);
    IEOS_BIND_SIGNAL(friends_interface_query_friends_callback);
    IEOS_BIND_SIGNAL(friends_interface_reject_invite_callback);
    IEOS_BIND_SIGNAL(friends_interface_send_invite_callback);
    IEOS_BIND_SIGNAL(kws_interface_create_user_callback);
    IEOS_BIND_SIGNAL(kws_interface_permissions_update_received_callback);
    IEOS_BIND_SIGNAL(kws_interface_query_age_gate_callback);
    IEOS_BIND_SIGNAL(kws_interface_query_permissions_callback);
    IEOS_BIND_SIGNAL(kws_interface_request_permissions_callback);
    IEOS_BIND_SIGNAL(kws_interface_update_parent_email_callback);
    IEOS_BIND_SIGNAL(leaderboards_interface_query_leaderboard_definitions_callback);
    IEOS_BIND_SIGNAL(leaderboards_interface_query_leaderboard_ranks_callback);
    IEOS_BIND_SIGNAL(leaderboards_interface_query_leaderboard_user_scores_callback);
    IEOS_BIND_SIGNAL(lobby_interface_create_lobby_callback);
    IEOS_BIND_SIGNAL(lobby_interface_destroy_lobby_callback);
    IEOS_BIND_SIGNAL(lobby_interface_hard_mute_member_callback);
    IEOS_BIND_SIGNAL(lobby_interface_join_lobby_accepted_callback);
    IEOS_BIND_SIGNAL(lobby_interface_join_lobby_by_id_callback);
    IEOS_BIND_SIGNAL(lobby_interface_join_lobby_callback);
    IEOS_BIND_SIGNAL(lobby_interface_kick_member_callback);
    IEOS_BIND_SIGNAL(lobby_interface_leave_lobby_callback);
    IEOS_BIND_SIGNAL(lobby_interface_leave_lobby_requested_callback);
    IEOS_BIND_SIGNAL(lobby_interface_lobby_invite_accepted_callback);
    IEOS_BIND_SIGNAL(lobby_interface_lobby_invite_received_callback);
    IEOS_BIND_SIGNAL(lobby_interface_lobby_invite_rejected_callback);
    IEOS_BIND_SIGNAL(lobby_interface_lobby_member_status_received_callback);
    IEOS_BIND_SIGNAL(lobby_interface_lobby_member_update_received_callback);
    IEOS_BIND_SIGNAL(lobby_interface_lobby_update_received_callback);
    IEOS_BIND_SIGNAL(lobby_interface_promote_member_callback);
    IEOS_BIND_SIGNAL(lobby_interface_query_invites_callback);
    IEOS_BIND_SIGNAL(lobby_interface_reject_invite_callback);
    IEOS_BIND_SIGNAL(lobby_interface_rtc_room_connection_changed_callback);
    IEOS_BIND_SIGNAL(lobby_interface_send_invite_callback);
    IEOS_BIND_SIGNAL(lobby_interface_send_lobby_native_invite_requested_callback);
    IEOS_BIND_SIGNAL(lobby_interface_update_lobby_callback);
    IEOS_BIND_SIGNAL(lobby_search_find_callback);
    IEOS_BIND_SIGNAL(mods_interface_enumerate_mods_callback);
    IEOS_BIND_SIGNAL(mods_interface_install_mod_callback);
    IEOS_BIND_SIGNAL(mods_interface_uninstall_mod_callback);
    IEOS_BIND_SIGNAL(mods_interface_update_mod_callback);
    IEOS_BIND_SIGNAL(p2p_interface_incoming_packet_queue_full);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_delete_cache_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_delete_file_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_duplicate_file_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_file_transfer_progress_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_query_file_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_query_file_list_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_read_file_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_read_file_data_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_write_file_callback);
    IEOS_BIND_SIGNAL(playerdatastorage_interface_write_file_data_callback);
    IEOS_BIND_SIGNAL(presence_interface_join_game_accepted_callback);
    IEOS_BIND_SIGNAL(presence_interface_presence_changed_callback);
    IEOS_BIND_SIGNAL(presence_interface_query_presence_callback);
    IEOS_BIND_SIGNAL(presence_interface_set_presence_callback);
    IEOS_BIND_SIGNAL(progression_snapshot_interface_delete_snapshot_callback);
    IEOS_BIND_SIGNAL(progression_snapshot_interface_submit_snapshot_callback);
    IEOS_BIND_SIGNAL(reports_interface_send_player_behavior_report_callback);
    IEOS_BIND_SIGNAL(rtc_audio_audio_before_render);
    IEOS_BIND_SIGNAL(rtc_audio_audio_before_send);
    IEOS_BIND_SIGNAL(rtc_audio_audio_devices_changed);
    IEOS_BIND_SIGNAL(rtc_audio_audio_input_state);
    IEOS_BIND_SIGNAL(rtc_audio_audio_output_state);
    IEOS_BIND_SIGNAL(rtc_audio_interface_query_input_devices_information_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_query_output_devices_information_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_register_platform_user_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_set_input_device_settings_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_set_output_device_settings_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_unregister_platform_user_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_update_participant_volume_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_update_receiving_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_update_receiving_volume_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_update_sending_callback);
    IEOS_BIND_SIGNAL(rtc_audio_interface_update_sending_volume_callback);
    IEOS_BIND_SIGNAL(rtc_audio_participant_updated);
    IEOS_BIND_SIGNAL(rtc_interface_block_participant_callback);
    IEOS_BIND_SIGNAL(rtc_interface_disconnected);
    IEOS_BIND_SIGNAL(rtc_interface_join_room_callback);
    IEOS_BIND_SIGNAL(rtc_interface_leave_room_callback);
    IEOS_BIND_SIGNAL(rtc_interface_participant_status_changed);
    IEOS_BIND_SIGNAL(rtc_interface_room_statistics_updated);
    IEOS_BIND_SIGNAL(sanctions_interface_query_active_player_sanctions_callback);
    IEOS_BIND_SIGNAL(sanctions_interface_create_player_sanction_appeal_callback);
    IEOS_BIND_SIGNAL(session_search_find_callback);
    IEOS_BIND_SIGNAL(sessions_interface_destroy_session_callback);
    IEOS_BIND_SIGNAL(sessions_interface_end_session_callback);
    IEOS_BIND_SIGNAL(sessions_interface_join_session_accepted_callback);
    IEOS_BIND_SIGNAL(sessions_interface_join_session_callback);
    IEOS_BIND_SIGNAL(sessions_interface_leave_session_requested_callback);
    IEOS_BIND_SIGNAL(sessions_interface_query_invites_callback);
    IEOS_BIND_SIGNAL(sessions_interface_register_players_callback);
    IEOS_BIND_SIGNAL(sessions_interface_reject_invite_callback);
    IEOS_BIND_SIGNAL(sessions_interface_send_invite_callback);
    IEOS_BIND_SIGNAL(sessions_interface_send_session_native_invite_requested_callback);
    IEOS_BIND_SIGNAL(sessions_interface_session_invite_accepted_callback);
    IEOS_BIND_SIGNAL(sessions_interface_session_invite_received_callback);
    IEOS_BIND_SIGNAL(sessions_interface_session_invite_rejected_callback);
    IEOS_BIND_SIGNAL(sessions_interface_start_session_callback);
    IEOS_BIND_SIGNAL(sessions_interface_unregister_players_callback);
    IEOS_BIND_SIGNAL(sessions_interface_update_session_callback);
    IEOS_BIND_SIGNAL(stats_interface_ingest_stat_callback);
    IEOS_BIND_SIGNAL(stats_interface_query_stats_callback);
    IEOS_BIND_SIGNAL(titlestorage_interface_delete_cache_callback);
    IEOS_BIND_SIGNAL(titlestorage_interface_file_transfer_progress_callback);
    IEOS_BIND_SIGNAL(titlestorage_interface_query_file_callback);
    IEOS_BIND_SIGNAL(titlestorage_interface_query_file_list_callback);
    IEOS_BIND_SIGNAL(titlestorage_interface_read_file_callback);
    IEOS_BIND_SIGNAL(titlestorage_interface_read_file_data_callback);
    IEOS_BIND_SIGNAL(ui_interface_display_settings_updated_callback);
    IEOS_BIND_SIGNAL(ui_interface_hide_friends_callback);
    IEOS_BIND_SIGNAL(ui_interface_memory_monitor_callback);
    IEOS_BIND_SIGNAL(ui_interface_show_block_player_callback);
    IEOS_BIND_SIGNAL(ui_interface_show_friends_callback);
    IEOS_BIND_SIGNAL(ui_interface_show_native_profile_callback);
    IEOS_BIND_SIGNAL(ui_interface_show_report_player_callback);
    IEOS_BIND_SIGNAL(user_info_interface_query_user_info_by_display_name_callback);
    IEOS_BIND_SIGNAL(user_info_interface_query_user_info_by_external_account_callback);
    IEOS_BIND_SIGNAL(user_info_interface_query_user_info_callback);
}

int IEOS::shutdown() {
    EOS_EResult result = EOS_Shutdown();
    return static_cast<int>(result);
}

bool IEOS::is_operation_complete(int p_result_code) {
    return EOSG_GET_BOOL(EOS_EResult_IsOperationComplete(static_cast<EOS_EResult>(p_result_code)));
}

IEOS *IEOS::singleton = nullptr;

IEOS *IEOS::get_singleton() { return singleton; }

IEOS::IEOS() {
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;
}

IEOS::~IEOS() {
    ERR_FAIL_COND(singleton != this);
    if (s_platformInterface != nullptr) {
        EOS_Platform_Release(s_platformInterface);
    }
    singleton = nullptr;
}

void IEOS::tick() {
    if (IEOS::get_singleton()->s_platformInterface != nullptr) {
        EOS_Platform_Tick(IEOS::get_singleton()->s_platformInterface);
    }
}
#include "ieos.h"
using namespace godot;

int IEOS::anticheat_server_interface_begin_session(Ref<RefCounted> p_options) {
    int p_register_timeout_seconds = p_options->get("register_timeout_seconds");
    CharString p_server_name = VARIANT_TO_CHARSTRING(p_options->get("server_name"));
    CharString p_local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_AntiCheatServer_BeginSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_BEGINSESSION_API_LATEST;
    options.RegisterTimeoutSeconds = static_cast<uint32_t>(p_register_timeout_seconds);
    options.ServerName = p_server_name.get_data();
    options.bEnableGameplayData = VARIANT_TO_EOS_BOOL(p_options->get("enable_gameplay_data"));
    if (p_local_user_id.size() > 1) {
        options.LocalUserId = eosg_string_to_product_user_id(p_local_user_id);
    }

    return static_cast<int>(EOS_AntiCheatServer_BeginSession(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_end_session(Ref<RefCounted> p_options) {
    EOS_AntiCheatServer_EndSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_ENDSESSION_API_LATEST;

    return static_cast<int>(EOS_AntiCheatServer_EndSession(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_register_client(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));
    int p_client_type = p_options->get("client_type");
    int p_client_platform = p_options->get("client_platform");
    CharString p_ip_address = VARIANT_TO_CHARSTRING(p_options->get("ip_address"));
    CharString p_user_id = VARIANT_TO_CHARSTRING(p_options->get("user_id"));

    EOS_AntiCheatServer_RegisterClientOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_REGISTERCLIENT_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.ClientType = static_cast<EOS_EAntiCheatCommonClientType>(p_client_type);
    options.ClientPlatform = static_cast<EOS_EAntiCheatCommonClientPlatform>(p_client_platform);
    options.IpAddress = p_ip_address.get_data();
    options.UserId = eosg_string_to_product_user_id(p_user_id);

    return static_cast<int>(EOS_AntiCheatServer_RegisterClient(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_unregister_client(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));

    EOS_AntiCheatServer_UnregisterClientOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_UNREGISTERCLIENT_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);

    return static_cast<int>(EOS_AntiCheatServer_UnregisterClient(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_receive_message_from_client(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));
    PackedByteArray p_data = p_options->get("data");

    void *data = memalloc(p_data.size());
    std::memcpy(data, p_data.ptr(), p_data.size());

    EOS_AntiCheatServer_ReceiveMessageFromClientOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_RECEIVEMESSAGEFROMCLIENT_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.DataLengthBytes = static_cast<uint32_t>(p_data.size());
    options.Data = data;

    return static_cast<int>(EOS_AntiCheatServer_ReceiveMessageFromClient(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_set_client_details(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));
    int p_client_flags = p_options->get("client_flags");
    int p_client_input = p_options->get("client_input");

    EOS_AntiCheatCommon_SetClientDetailsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_SETCLIENTDETAILS_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.ClientFlags = static_cast<EOS_EAntiCheatCommonClientFlags>(p_client_flags);
    options.ClientInputMethod = static_cast<EOS_EAntiCheatCommonClientInput>(p_client_input);

    return static_cast<int>(EOS_AntiCheatServer_SetClientDetails(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_set_game_session_id(Ref<RefCounted> p_options) {
    CharString p_game_session_id = VARIANT_TO_CHARSTRING(p_options->get("game_session_id"));

    EOS_AntiCheatCommon_SetGameSessionIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_SETGAMESESSIONID_API_LATEST;
    options.GameSessionId = p_game_session_id.get_data();

    return static_cast<int>(EOS_AntiCheatServer_SetGameSessionId(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_set_client_network_state(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));

    EOS_AntiCheatServer_SetClientNetworkStateOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_SETCLIENTNETWORKSTATE_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.bIsNetworkActive = VARIANT_TO_EOS_BOOL(p_options->get("is_network_active"));

    return static_cast<int>(EOS_AntiCheatServer_SetClientNetworkState(s_antiCheatServerInterface, &options));
}

Dictionary IEOS::anticheat_server_interface_get_protect_message_output_length(Ref<RefCounted> p_options) {
    int p_data_length_bytes = p_options->get("data_length_bytes");

    EOS_AntiCheatServer_GetProtectMessageOutputLengthOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_GETPROTECTMESSAGEOUTPUTLENGTH_API_LATEST;
    options.DataLengthBytes = static_cast<uint32_t>(p_data_length_bytes);

    uint32_t outBufferSizeBytes = 0;

    EOS_EResult res = EOS_AntiCheatServer_GetProtectMessageOutputLength(s_antiCheatServerInterface, &options, &outBufferSizeBytes);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["buffer_size_bytes"] = 0;
    if (res == EOS_EResult::EOS_Success) {
        ret["buffer_size_bytes"] = outBufferSizeBytes;
    }

    return ret;
}

Dictionary IEOS::anticheat_server_interface_protect_message(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));
    PackedByteArray p_data = p_options->get("data");
    int data_length_bytes = p_data.size();
    void *data = memalloc(data_length_bytes);
    std::memcpy(data, p_data.ptr(), data_length_bytes);
    int out_buffer_size_bytes = p_options->get("out_buffer_size_bytes");

    EOS_AntiCheatServer_ProtectMessageOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_PROTECTMESSAGE_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.DataLengthBytes = static_cast<uint32_t>(data_length_bytes);
    options.Data = data;
    options.OutBufferSizeBytes = static_cast<uint32_t>(out_buffer_size_bytes);

    uint32_t outBytesWritten = 0;
    void *outBuffer = (void *)memalloc(out_buffer_size_bytes + 1);

    EOS_EResult res = EOS_AntiCheatServer_ProtectMessage(s_antiCheatServerInterface, &options, &outBuffer, &outBytesWritten);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["out_buffer"] = Variant();
    ret["out_bytes_written"] = 0;
    if (res == EOS_EResult::EOS_Success) {
        ret["out_buffer"] = outBuffer;
        ret["out_bytes_written"] = outBytesWritten;
    }
    return ret;
}

Dictionary IEOS::anticheat_server_interface_unprotect_message(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));
    PackedByteArray p_data = p_options->get("data");
    int data_length_bytes = p_data.size();
    void *data = memalloc(data_length_bytes);
    std::memcpy(data, p_data.ptr(), data_length_bytes);

    int out_buffer_size_bytes = p_options->get("out_buffer_size_bytes");

    EOS_AntiCheatServer_UnprotectMessageOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATSERVER_UNPROTECTMESSAGE_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.DataLengthBytes = static_cast<uint32_t>(data_length_bytes);
    options.Data = data;
    options.OutBufferSizeBytes = static_cast<uint32_t>(out_buffer_size_bytes);

    uint32_t outBytesWritten = 0;
    void *outBuffer = (void *)memalloc(out_buffer_size_bytes + 1);

    EOS_EResult res = EOS_AntiCheatServer_UnprotectMessage(s_antiCheatServerInterface, &options, &outBuffer, &outBytesWritten);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["out_buffer"] = Variant();
    ret["out_bytes_written"] = 0;
    if (res == EOS_EResult::EOS_Success) {
        ret["out_buffer"] = outBuffer;
        ret["out_bytes_written"] = outBytesWritten;
    }
    return ret;
}

int IEOS::anticheat_server_interface_register_event(Ref<RefCounted> p_options) {
    int p_event_id = p_options->get("event_id");
    CharString p_event_name = VARIANT_TO_CHARSTRING(p_options->get("event_name"));
    int p_event_type = p_options->get("event_type");
    Array p_param_defs = p_options->get("param_defs");

    EOS_AntiCheatCommon_RegisterEventParamDef *paramDefs = (EOS_AntiCheatCommon_RegisterEventParamDef *)memalloc(sizeof(EOS_AntiCheatCommon_RegisterEventParamDef) * p_param_defs.size());
    for (int i = 0; i < p_param_defs.size(); i++) {
        Dictionary paramDef = p_param_defs[i];
        int param_type = paramDef["param_type"];
        CharString param_name = VARIANT_TO_CHARSTRING(paramDef["param_name"]);
        paramDefs[i].ParamType = static_cast<EOS_EAntiCheatCommonEventParamType>(param_type);
        paramDefs[i].ParamName = param_name.get_data();
    }

    EOS_AntiCheatCommon_RegisterEventOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_REGISTEREVENT_API_LATEST;
    options.EventId = static_cast<uint32_t>(p_event_id);
    options.EventName = p_event_name.get_data();
    options.EventType = static_cast<EOS_EAntiCheatCommonEventType>(p_event_type);
    options.ParamDefsCount = p_param_defs.size();
    options.ParamDefs = paramDefs;

    return static_cast<int>(EOS_AntiCheatServer_RegisterEvent(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_event(Ref<RefCounted> p_options) {
    CharString p_client_handle = VARIANT_TO_CHARSTRING(p_options->get("client_handle"));
    int p_event_id = p_options->get("event_id");
    Array p_params = p_options->get("params");

    int params_count = p_params.size();
    EOS_AntiCheatCommon_LogEventParamPair *params = (EOS_AntiCheatCommon_LogEventParamPair *)memalloc(sizeof(EOS_AntiCheatCommon_LogEventParamPair) * params_count);
    for (int i = 0; i < params_count; i++) {
        Variant param = p_params[i];

        if (param.get_type() == Variant::INT) {
            params[i].ParamValueType = EOS_EAntiCheatCommonEventParamType::EOS_ACCEPT_Int64;
            params[i].ParamValue.Int64 = param;
        } else if (param.get_type() == Variant::STRING) {
            params[i].ParamValueType = EOS_EAntiCheatCommonEventParamType::EOS_ACCEPT_String;
            params[i].ParamValue.String = VARIANT_TO_CHARSTRING(param).get_data();
            break;
        } else if (param.get_type() == Variant::VECTOR3) {
            params[i].ParamValueType = EOS_EAntiCheatCommonEventParamType::EOS_ACCEPT_Vector3f;
            Vector3 vec = param;
            params[i].ParamValue.Vec3f.x = vec.x;
            params[i].ParamValue.Vec3f.y = vec.y;
            params[i].ParamValue.Vec3f.z = vec.z;
            break;
        } else if (param.get_type() == Variant::FLOAT) {
            params[i].ParamValueType = EOS_EAntiCheatCommonEventParamType::EOS_ACCEPT_Float;
            params[i].ParamValue.Float = param;
            break;
        } else if (param.get_type() == Variant::QUATERNION) {
            Quaternion quat = param;
            params[i].ParamValueType = EOS_EAntiCheatCommonEventParamType::EOS_ACCEPT_Quat;
            params[i].ParamValue.Quat.x = quat.x;
            params[i].ParamValue.Quat.y = quat.y;
            params[i].ParamValue.Quat.z = quat.z;
            params[i].ParamValue.Quat.w = quat.w;
            break;
        } else {
            ERR_FAIL_V_MSG(static_cast<int>(EOS_EResult::EOS_InvalidParameters), "Invalid param type provided.");
        }
    }

    EOS_AntiCheatCommon_LogEventOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGEVENT_API_LATEST;
    options.ClientHandle = _anticheat_player_id_to_handle(p_client_handle);
    options.EventId = static_cast<uint32_t>(p_event_id);
    options.ParamsCount = params_count;
    options.Params = params;

    return static_cast<int>(EOS_AntiCheatServer_LogEvent(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_game_round_start(Ref<RefCounted> p_options) {
    CharString p_session_identifier = VARIANT_TO_CHARSTRING(p_options->get("session_identifier"));
    CharString p_level_name = VARIANT_TO_CHARSTRING(p_options->get("level_name"));
    CharString p_mode_name = VARIANT_TO_CHARSTRING(p_options->get("mode_name"));
    int p_round_time_seconds = p_options->get("round_time_seconds");
    int p_competition_type = p_options->get("competition_type");

    EOS_AntiCheatCommon_LogGameRoundStartOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGGAMEROUNDSTART_API_LATEST;
    if (p_session_identifier.size() > 1) {
        options.SessionIdentifier = p_session_identifier.get_data();
    }
    if (p_level_name.size() > 1) {
        options.LevelName = p_level_name.get_data();
    }
    if (p_mode_name.size() > 1) {
        options.ModeName = p_mode_name.get_data();
    }
    options.RoundTimeSeconds = static_cast<uint32_t>(p_round_time_seconds);
    options.CompetitionType = static_cast<EOS_EAntiCheatCommonGameRoundCompetitionType>(p_competition_type);

    return static_cast<int>(EOS_AntiCheatServer_LogGameRoundStart(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_game_round_end(Ref<RefCounted> p_options) {
    int p_winning_team_id = p_options->get("winning_team_id");

    EOS_AntiCheatCommon_LogGameRoundEndOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGGAMEROUNDEND_API_LATEST;
    options.WinningTeamId = static_cast<uint32_t>(p_winning_team_id);

    return static_cast<int>(EOS_AntiCheatServer_LogGameRoundEnd(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_spawn(Ref<RefCounted> p_options) {
    CharString p_spawned_player_handle = VARIANT_TO_CHARSTRING(p_options->get("spawned_player_handle"));
    int p_team_id = p_options->get("team_id");
    int p_character_id = p_options->get("character_id");

    EOS_AntiCheatCommon_LogPlayerSpawnOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERSPAWN_API_LATEST;
    options.SpawnedPlayerHandle = _anticheat_player_id_to_handle(p_spawned_player_handle);
    options.TeamId = static_cast<uint32_t>(p_team_id);
    options.CharacterId = static_cast<uint32_t>(p_character_id);

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerSpawn(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_despawn(Ref<RefCounted> p_options) {
    CharString p_despawned_player_handle = VARIANT_TO_CHARSTRING(p_options->get("despawned_player_handle"));

    EOS_AntiCheatCommon_LogPlayerDespawnOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERDESPAWN_API_LATEST;
    options.DespawnedPlayerHandle = _anticheat_player_id_to_handle(p_despawned_player_handle);

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerDespawn(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_revive(Ref<RefCounted> p_options) {
    CharString p_revived_player_handle = VARIANT_TO_CHARSTRING(p_options->get("revived_player_handle"));
    CharString p_reviver_player_handle = VARIANT_TO_CHARSTRING(p_options->get("reviver_player_handle"));

    EOS_AntiCheatCommon_LogPlayerReviveOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERREVIVE_API_LATEST;
    options.RevivedPlayerHandle = _anticheat_player_id_to_handle(p_revived_player_handle);
    options.ReviverPlayerHandle = _anticheat_player_id_to_handle(p_reviver_player_handle);

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerRevive(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_tick(Ref<RefCounted> p_options) {
    CharString p_player_handle = VARIANT_TO_CHARSTRING(p_options->get("player_handle"));
    Vector3 p_player_position = p_options->get("player_position");
    Quaternion p_player_view_rotation = p_options->get("player_view_rotation");
    float p_player_health = p_options->get("player_health");
    int p_player_movement_state = p_options->get("player_movement_state");
    Vector3 p_player_view_position = p_options->get("player_view_position");

    EOS_AntiCheatCommon_LogPlayerTickOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERTICK_API_LATEST;
    options.PlayerHandle = _anticheat_player_id_to_handle(p_player_handle);
    options.PlayerPosition->x = p_player_position.x;
    options.PlayerPosition->y = p_player_position.y;
    options.PlayerPosition->z = p_player_position.z;
    options.PlayerViewRotation->x = p_player_view_rotation.x;
    options.PlayerViewRotation->y = p_player_view_rotation.y;
    options.PlayerViewRotation->z = p_player_view_rotation.z;
    options.PlayerViewRotation->w = p_player_view_rotation.w;
    options.bIsPlayerViewZoomed = VARIANT_TO_EOS_BOOL(p_options->get("is_player_view_zoomed"));
    options.PlayerHealth = p_player_health;
    options.PlayerMovementState = static_cast<EOS_EAntiCheatCommonPlayerMovementState>(p_player_movement_state);
    options.PlayerViewPosition->x = p_player_view_position.x;
    options.PlayerViewPosition->y = p_player_view_position.y;
    options.PlayerViewPosition->z = p_player_view_position.z;

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerTick(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_use_weapon(Ref<RefCounted> p_options) {
    EOS_AntiCheatCommon_LogPlayerUseWeaponData use_weapon_data = eosg_refcounted_to_anticheat_log_player_use_weapon_data(p_options);

    EOS_AntiCheatCommon_LogPlayerUseWeaponOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_API_LATEST;
    options.UseWeaponData = &use_weapon_data;

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerUseWeapon(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_use_ability(Ref<RefCounted> p_options) {
    CharString p_player_handle = VARIANT_TO_CHARSTRING(p_options->get("player_handle"));
    int p_ability_id = p_options->get("ability_id");
    int p_ability_duration_ms = p_options->get("ability_duration_ms");
    int p_ability_cooldown_ms = p_options->get("ability_cooldown_ms");

    EOS_AntiCheatCommon_LogPlayerUseAbilityOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERUSEABILITY_API_LATEST;
    options.PlayerHandle = _anticheat_player_id_to_handle(p_player_handle);
    options.AbilityId = static_cast<uint32_t>(p_ability_id);
    options.AbilityDurationMs = static_cast<uint32_t>(p_ability_duration_ms);
    options.AbilityCooldownMs = static_cast<uint32_t>(p_ability_cooldown_ms);

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerUseAbility(s_antiCheatServerInterface, &options));
}

int IEOS::anticheat_server_interface_log_player_take_damage(Ref<RefCounted> p_options) {
    CharString p_victim_player_handle = VARIANT_TO_CHARSTRING(p_options->get("victim_player_handle"));
    Vector3 p_victim_player_position = p_options->get("victim_player_position");
    Quaternion p_victim_player_view_rotation = p_options->get("victim_player_view_rotation");

    CharString p_attacker_player_handle = VARIANT_TO_CHARSTRING(p_options->get("attacker_player_handle"));
    Vector3 p_attacker_player_position = p_options->get("attacker_player_position");
    Quaternion p_attacker_player_view_rotation = p_options->get("attacker_player_view_rotation");

    float p_damage_taken = p_options->get("damage_taken");
    float p_health_remaining = p_options->get("health_remaining");
    int p_damage_source = p_options->get("damage_source");
    int p_damage_type = p_options->get("damage_type");
    int p_damage_result = p_options->get("damage_result");
    int p_time_since_player_use_weapon_ms = p_options->get("time_since_player_use_weapon_ms");
    Vector3 p_damage_position = p_options->get("damage_position");
    Vector3 p_attacker_player_view_position = p_options->get("attacker_player_view_position");

    Ref<RefCounted> use_weapon_data_options = p_options->get("player_use_weapon_data");
    EOS_AntiCheatCommon_LogPlayerUseWeaponData use_weapon_data = eosg_refcounted_to_anticheat_log_player_use_weapon_data(use_weapon_data_options);

    EOS_AntiCheatCommon_LogPlayerTakeDamageOptions options;
    options.ApiVersion = EOS_ANTICHEATCOMMON_LOGPLAYERTAKEDAMAGE_API_LATEST;
    options.VictimPlayerHandle = _anticheat_player_id_to_handle(p_victim_player_handle);
    options.VictimPlayerPosition->x = p_victim_player_position.x;
    options.VictimPlayerPosition->y = p_victim_player_position.y;
    options.VictimPlayerPosition->z = p_victim_player_position.z;
    options.VictimPlayerViewRotation->x = p_victim_player_view_rotation.x;
    options.VictimPlayerViewRotation->y = p_victim_player_view_rotation.y;
    options.VictimPlayerViewRotation->z = p_victim_player_view_rotation.z;
    options.VictimPlayerViewRotation->w = p_victim_player_view_rotation.w;
    options.AttackerPlayerHandle = _anticheat_player_id_to_handle(p_attacker_player_handle);
    options.AttackerPlayerPosition->x = p_attacker_player_position.x;
    options.AttackerPlayerPosition->y = p_attacker_player_position.y;
    options.AttackerPlayerPosition->z = p_attacker_player_position.z;
    options.AttackerPlayerViewRotation->x = p_attacker_player_view_rotation.x;
    options.AttackerPlayerViewRotation->y = p_attacker_player_view_rotation.y;
    options.AttackerPlayerViewRotation->z = p_attacker_player_view_rotation.z;
    options.AttackerPlayerViewRotation->w = p_attacker_player_view_rotation.w;
    options.bIsHitscanAttack = VARIANT_TO_EOS_BOOL(p_options->get("is_hitscan_attack"));
    options.bHasLineOfSight = VARIANT_TO_EOS_BOOL(p_options->get("has_line_of_sight"));
    options.bIsCriticalHit = VARIANT_TO_EOS_BOOL(p_options->get("is_critical_hit"));
    options.DamageTaken = p_damage_taken;
    options.HealthRemaining = p_health_remaining;
    options.DamageSource = static_cast<EOS_EAntiCheatCommonPlayerTakeDamageSource>(p_damage_source);
    options.DamageType = static_cast<EOS_EAntiCheatCommonPlayerTakeDamageType>(p_damage_type);
    options.DamageResult = static_cast<EOS_EAntiCheatCommonPlayerTakeDamageResult>(p_damage_result);
    options.PlayerUseWeaponData = &use_weapon_data;
    options.TimeSincePlayerUseWeaponMs = static_cast<uint32_t>(p_time_since_player_use_weapon_ms);
    options.DamagePosition->x = p_damage_position.x;
    options.DamagePosition->y = p_damage_position.y;
    options.DamagePosition->z = p_damage_position.z;
    options.AttackerPlayerViewPosition->x = p_attacker_player_view_position.x;
    options.AttackerPlayerViewPosition->y = p_attacker_player_view_position.y;
    options.AttackerPlayerViewPosition->z = p_attacker_player_view_position.z;

    return static_cast<int>(EOS_AntiCheatServer_LogPlayerTakeDamage(s_antiCheatServerInterface, &options));
}

#include "ieos.h"
using namespace std;

int IEOS::metrics_interface_begin_player_session(Ref<RefCounted> p_options) {
    CharString account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));
    CharString display_name = VARIANT_TO_CHARSTRING(p_options->get("display_name"));
    CharString server_ip = VARIANT_TO_CHARSTRING(p_options->get("server_ip"));
    CharString game_session_id = VARIANT_TO_CHARSTRING(p_options->get("game_session_id"));

    EOS_Metrics_BeginPlayerSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_METRICS_BEGINPLAYERSESSION_API_LATEST;
    options.AccountIdType = static_cast<EOS_EMetricsAccountIdType>(static_cast<int>(p_options->get("account_id_type")));
    options.DisplayName = display_name.get_data();
    options.ControllerType = static_cast<EOS_EUserControllerType>(static_cast<int>(p_options->get("controller_type")));

    if (options.AccountIdType == EOS_EMetricsAccountIdType::EOS_MAIT_Epic) {
        options.AccountId.Epic = eosg_string_to_epic_account_id(account_id.get_data());
    } else if (options.AccountIdType == EOS_EMetricsAccountIdType::EOS_MAIT_External) {
        options.AccountId.External = account_id.get_data();
    }
    if (server_ip.length() != 0) {
        options.ServerIp = server_ip.get_data();
    }
    if (game_session_id.length() != 0) {
        options.GameSessionId = game_session_id.get_data();
    }

    return static_cast<int>(EOS_Metrics_BeginPlayerSession(s_metricsInterface, &options));
}

int IEOS::metrics_interface_end_player_session(Ref<RefCounted> p_options) {
    CharString account_id = VARIANT_TO_CHARSTRING(p_options->get("account_id"));

    EOS_Metrics_EndPlayerSessionOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_METRICS_ENDPLAYERSESSION_API_LATEST;
    options.AccountIdType = static_cast<EOS_EMetricsAccountIdType>(static_cast<int>(p_options->get("account_id_type")));

    if (options.AccountIdType == EOS_EMetricsAccountIdType::EOS_MAIT_Epic) {
        options.AccountId.Epic = eosg_string_to_epic_account_id(account_id.get_data());
    } else if (options.AccountIdType == EOS_EMetricsAccountIdType::EOS_MAIT_External) {
        options.AccountId.External = account_id.get_data();
    }

    return static_cast<int>(EOS_Metrics_EndPlayerSession(s_metricsInterface, &options));
}

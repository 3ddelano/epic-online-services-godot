#include "ieos.h"
using namespace std;

void IEOS::reports_interface_send_player_behavior_report(Ref<RefCounted> p_options) {
	ERR_FAIL_NULL(s_reportsInterface);
    CharString reporter_user_id = VARIANT_TO_CHARSTRING(p_options->get("reporter_user_id"));
    CharString reported_user_id = VARIANT_TO_CHARSTRING(p_options->get("reported_user_id"));
    CharString message = VARIANT_TO_CHARSTRING(p_options->get("message"));
    CharString context = VARIANT_TO_CHARSTRING(p_options->get("context"));

    EOS_Reports_SendPlayerBehaviorReportOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_REPORTS_SENDPLAYERBEHAVIORREPORT_API_LATEST;
    options.ReporterUserId = eosg_string_to_product_user_id(reporter_user_id.get_data());
    options.ReportedUserId = eosg_string_to_product_user_id(reported_user_id.get_data());
    options.Category = static_cast<EOS_EPlayerReportsCategory>(static_cast<int>(p_options->get("category")));
    if (message.length() != 0) {
        options.Message = message.get_data();
    }
    if (context.length() != 0) {
        options.Context = context.get_data();
    }
    p_options->reference();

    EOS_Reports_SendPlayerBehaviorReport(s_reportsInterface, &options, (void *)*p_options, [](const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        IEOS::get_singleton()->emit_signal("reports_interface_send_player_behavior_report_callback", ret);
    });
}

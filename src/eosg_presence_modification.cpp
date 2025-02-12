#include "eosg_presence_modification.h"

#include "utils.h"

using namespace godot;

void EOSGPresenceModification::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_status", "new_status"), &EOSGPresenceModification::set_status);
    ClassDB::bind_method(D_METHOD("set_raw_rich_text", "new_raw_rich_text"), &EOSGPresenceModification::set_raw_rich_text);
    ClassDB::bind_method(D_METHOD("set_data", "new_data"), &EOSGPresenceModification::set_data);
    ClassDB::bind_method(D_METHOD("delete_data", "keys"), &EOSGPresenceModification::delete_data);
    ClassDB::bind_method(D_METHOD("set_join_info", "new_join_info"), &EOSGPresenceModification::set_join_info);
}

int EOSGPresenceModification::set_status(int new_status) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_PresenceModification_SetStatusOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_SETPRESENCE_API_LATEST;
    options.Status = static_cast<EOS_Presence_EStatus>(new_status);

    return static_cast<int>(EOS_PresenceModification_SetStatus(m_internal, &options));
}

int EOSGPresenceModification::set_raw_rich_text(const String &p_raw_rich_text) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString new_raw_rich_text = VARIANT_TO_CHARSTRING(p_raw_rich_text);

    EOS_PresenceModification_SetRawRichTextOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_SETRAWRICHTEXT_API_LATEST;
    options.RichText = new_raw_rich_text.get_data();

    return static_cast<int>(EOS_PresenceModification_SetRawRichText(m_internal, &options));
}

int EOSGPresenceModification::set_data(Dictionary p_data) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    int records_count = p_data.keys().size();
    EOS_Presence_DataRecord *records = (EOS_Presence_DataRecord *)memalloc(sizeof(EOS_Presence_DataRecord) * records_count);

    PERSISTENT_CHAR_ARRAY_CREATE(data_keys, data_keys_charstrings, records_count);
    PERSISTENT_CHAR_ARRAY_CREATE(data_values, data_values_charstrings, records_count);
    for (int i = 0; i < records_count; i++) {
        PERSISTENT_CHAR_ARRAY_SET(data_keys, data_keys_charstrings, i, p_data.keys()[i]);
        PERSISTENT_CHAR_ARRAY_SET(data_values, data_values_charstrings, i, p_data[p_data.keys()[i]]);

        records[i].ApiVersion = EOS_PRESENCE_DATARECORD_API_LATEST;
        records[i].Key = data_keys[i];
        records[i].Value = data_values[i];
    }

    EOS_PresenceModification_SetDataOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST;
    options.RecordsCount = static_cast<int32_t>(records_count);
    options.Records = records;

    return static_cast<int>(EOS_PresenceModification_SetData(m_internal, &options));
}

int EOSGPresenceModification::delete_data(Array p_keys) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    int records_count = p_keys.size();
    EOS_PresenceModification_DataRecordId *records = (EOS_PresenceModification_DataRecordId *)memalloc(sizeof(EOS_PresenceModification_DataRecordId) * records_count);

    PERSISTENT_CHAR_ARRAY_CREATE(data_keys, data_keys_charstrings, records_count)
    for (int i = 0; i < records_count; i++) {
        PERSISTENT_CHAR_ARRAY_SET(data_keys, data_keys_charstrings, i, p_keys[i]);

        records[i].ApiVersion = EOS_PRESENCEMODIFICATION_DATARECORDID_API_LATEST;
        records[i].Key = data_keys[i];
    }

    EOS_PresenceModification_DeleteDataOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_DELETEDATA_API_LATEST;
    options.RecordsCount = records_count;
    options.Records = records;

    return static_cast<int>(EOS_PresenceModification_DeleteData(m_internal, &options));
}

int EOSGPresenceModification::set_join_info(const String &p_join_info) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString new_join_info = VARIANT_TO_CHARSTRING(p_join_info);

    EOS_PresenceModification_SetJoinInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCEMODIFICATION_SETJOININFO_API_LATEST;
    options.JoinInfo = new_join_info.get_data();

    return static_cast<int>(EOS_PresenceModification_SetJoinInfo(m_internal, &options));
}
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
    EOS_PresenceModification_SetStatusOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_SETPRESENCE_API_LATEST;
    options.Status = static_cast<EOS_Presence_EStatus>(new_status);

    return static_cast<int>(EOS_PresenceModification_SetStatus(m_internal, &options));
}

int EOSGPresenceModification::set_raw_rich_text(const String &p_raw_rich_text) {
    CharString new_raw_rich_text = VARIANT_TO_CHARSTRING(p_raw_rich_text);

    EOS_PresenceModification_SetRawRichTextOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_SETRAWRICHTEXT_API_LATEST;
    options.RichText = new_raw_rich_text.get_data();

    return static_cast<int>(EOS_PresenceModification_SetRawRichText(m_internal, &options));
}

int EOSGPresenceModification::set_data(Dictionary p_data) {
    int records_count = p_data.keys().size();
    EOS_Presence_DataRecord *records = (EOS_Presence_DataRecord *)memalloc(sizeof(EOS_Presence_DataRecord) * records_count);

    for (int i = 0; i < records_count; i++) {
        String key = p_data.keys()[i];
        String value = p_data[key];

        CharString new_key = VARIANT_TO_CHARSTRING(key);
        CharString new_value = VARIANT_TO_CHARSTRING(value);

        records[i].ApiVersion = EOS_PRESENCE_DATARECORD_API_LATEST;
        records[i].Key = new_key.get_data();
        records[i].Value = new_value.get_data();
    }

    EOS_PresenceModification_SetDataOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST;
    options.RecordsCount = static_cast<int32_t>(records_count);
    options.Records = records;

    return static_cast<int>(EOS_PresenceModification_SetData(m_internal, &options));
}

int EOSGPresenceModification::delete_data(Array p_keys) {
    int records_count = p_keys.size();
    EOS_PresenceModification_DataRecordId *records = (EOS_PresenceModification_DataRecordId *)memalloc(sizeof(EOS_PresenceModification_DataRecordId) * records_count);

    for (int i = 0; i < records_count; i++) {
        String key = p_keys[i];
        CharString p_key = VARIANT_TO_CHARSTRING(key);

        records[i].ApiVersion = EOS_PRESENCEMODIFICATION_DATARECORDID_API_LATEST;
        records[i].Key = p_key.get_data();
    }

    EOS_PresenceModification_DeleteDataOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCE_DELETEDATA_API_LATEST;
    options.RecordsCount = records_count;
    options.Records = records;

    return static_cast<int>(EOS_PresenceModification_DeleteData(m_internal, &options));
}

int EOSGPresenceModification::set_join_info(const String &p_join_info) {
    CharString new_join_info = VARIANT_TO_CHARSTRING(p_join_info);

    EOS_PresenceModification_SetJoinInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PRESENCEMODIFICATION_SETJOININFO_API_LATEST;
    options.JoinInfo = new_join_info.get_data();

    return static_cast<int>(EOS_PresenceModification_SetJoinInfo(m_internal, &options));
}
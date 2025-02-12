#include "eosg_session_modification.h"

#include "utils.h"
using namespace godot;

void EOSGSessionModification::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_attribute", "key", "value", "advertisement_type"), &EOSGSessionModification::add_attribute);
    ClassDB::bind_method(D_METHOD("remove_attribute", "key"), &EOSGSessionModification::remove_attribute);
    ClassDB::bind_method(D_METHOD("set_allowed_platform_ids", "allowed_platform_ids"), &EOSGSessionModification::set_allowed_platform_ids);
    ClassDB::bind_method(D_METHOD("set_bucket_id", "bucket_id"), &EOSGSessionModification::set_bucket_id);
    ClassDB::bind_method(D_METHOD("set_host_address", "host_address"), &EOSGSessionModification::set_host_address);
    ClassDB::bind_method(D_METHOD("set_invites_allowed", "invites_allowed"), &EOSGSessionModification::set_invites_allowed);
    ClassDB::bind_method(D_METHOD("set_max_players", "max_players"), &EOSGSessionModification::set_max_players);
    ClassDB::bind_method(D_METHOD("set_join_in_progress_allowed", "join_in_progress_allowed"), &EOSGSessionModification::set_join_in_progress_allowed);
    ClassDB::bind_method(D_METHOD("set_permission_level", "permission_level"), &EOSGSessionModification::set_permission_level);
}

int EOSGSessionModification::add_attribute(const String &p_key, Variant p_value, int p_visibility) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString key = p_key.utf8();

    EOS_Sessions_AttributeData sessionAttribute;
    memset(&sessionAttribute, 0, sizeof(sessionAttribute));
    sessionAttribute.ApiVersion = EOS_SESSIONS_ATTRIBUTEDATA_API_LATEST;
    sessionAttribute.Key = key.get_data();
	CharString value;

    if (p_value.get_type() == Variant::Type::BOOL) {
        sessionAttribute.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;
        sessionAttribute.Value.AsBool = p_value;
    } else if (p_value.get_type() == Variant::Type::INT) {
        sessionAttribute.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;
        sessionAttribute.Value.AsInt64 = p_value;
    } else if (p_value.get_type() == Variant::Type::FLOAT) {
        sessionAttribute.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;
        sessionAttribute.Value.AsDouble = p_value;
    } else if (p_value.get_type() == Variant::Type::STRING) {
        value = VARIANT_TO_CHARSTRING(p_value);
        sessionAttribute.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;
        sessionAttribute.Value.AsUtf8 = value.get_data();
    }

    EOS_SessionModification_AddAttributeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST;
    options.SessionAttribute = &sessionAttribute;
    options.AdvertisementType = static_cast<EOS_ESessionAttributeAdvertisementType>(p_visibility);

    return static_cast<int>(EOS_SessionModification_AddAttribute(m_internal, &options));
}

int EOSGSessionModification::remove_attribute(const String &p_key) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString key = p_key.utf8();

    EOS_SessionModification_RemoveAttributeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_REMOVEATTRIBUTE_API_LATEST;
    options.Key = key.get_data();

    return static_cast<int>(EOS_SessionModification_RemoveAttribute(m_internal, &options));
}

int EOSGSessionModification::set_allowed_platform_ids(const TypedArray<int> &p_platform_ids) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_SessionModification_SetAllowedPlatformIdsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST;
    options.AllowedPlatformIdsCount = p_platform_ids.size();

    uint32_t *platform_ids_array = (uint32_t *)memalloc(sizeof(uint32_t) * options.AllowedPlatformIdsCount);
    for (int i = 0; i < options.AllowedPlatformIdsCount; i++) {
        platform_ids_array[i] = static_cast<uint32_t>(p_platform_ids[i]);
    }
    options.AllowedPlatformIds = platform_ids_array;

    return static_cast<int>(EOS_SessionModification_SetAllowedPlatformIds(m_internal, &options));
}

int EOSGSessionModification::set_bucket_id(const String &p_bucket_id) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString bucket_id = p_bucket_id.utf8();
    EOS_SessionModification_SetBucketIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETBUCKETID_API_LATEST;
    options.BucketId = bucket_id.get_data();

    return static_cast<int>(EOS_SessionModification_SetBucketId(m_internal, &options));
}

int EOSGSessionModification::set_host_address(const String &p_host_address) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString host_address = p_host_address.utf8();
    EOS_SessionModification_SetHostAddressOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETHOSTADDRESS_API_LATEST;
    options.HostAddress = host_address.get_data();

    return static_cast<int>(EOS_SessionModification_SetHostAddress(m_internal, &options));
}

int EOSGSessionModification::set_invites_allowed(bool p_invites_allowed) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_SessionModification_SetInvitesAllowedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETINVITESALLOWED_API_LATEST;
    options.bInvitesAllowed = p_invites_allowed ? EOS_TRUE : EOS_FALSE;

    return static_cast<int>(EOS_SessionModification_SetInvitesAllowed(m_internal, &options));
}

int EOSGSessionModification::set_max_players(int p_max_players) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_SessionModification_SetMaxPlayersOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETMAXPLAYERS_API_LATEST;
    options.MaxPlayers = static_cast<uint32_t>(p_max_players);

    return static_cast<int>(EOS_SessionModification_SetMaxPlayers(m_internal, &options));
}

int EOSGSessionModification::set_join_in_progress_allowed(bool p_join_in_progress_allowed) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_SessionModification_SetJoinInProgressAllowedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETJOININPROGRESSALLOWED_API_LATEST;
    options.bAllowJoinInProgress = p_join_in_progress_allowed ? EOS_TRUE : EOS_FALSE;

    return static_cast<int>(EOS_SessionModification_SetJoinInProgressAllowed(m_internal, &options));
}

int EOSGSessionModification::set_permission_level(int p_permission_level) {
	ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    EOS_SessionModification_SetPermissionLevelOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_SESSIONMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
    options.PermissionLevel = static_cast<EOS_EOnlineSessionPermissionLevel>(p_permission_level);

    return static_cast<int>(EOS_SessionModification_SetPermissionLevel(m_internal, &options));
}

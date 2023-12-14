#include "eosg_lobby_modification.h"

#include "utils.h"
using namespace godot;

void EOSGLobbyModification::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bucket_id", "bucket_id"), &EOSGLobbyModification::set_bucket_id);
    ClassDB::bind_method(D_METHOD("set_permission_level", "permission_level"), &EOSGLobbyModification::set_permission_level);
    ClassDB::bind_method(D_METHOD("set_max_members", "max_members"), &EOSGLobbyModification::set_max_members);
    ClassDB::bind_method(D_METHOD("set_invites_allowed", "invites_allowed"), &EOSGLobbyModification::set_invites_allowed);
    ClassDB::bind_method(D_METHOD("add_attribute", "key", "value", "visibility"), &EOSGLobbyModification::add_attribute);
    ClassDB::bind_method(D_METHOD("remove_attribute", "key"), &EOSGLobbyModification::remove_attribute);
    ClassDB::bind_method(D_METHOD("add_member_attribute", "key", "value", "visibility"), &EOSGLobbyModification::add_member_attribute);
    ClassDB::bind_method(D_METHOD("remove_member_attribute", "key"), &EOSGLobbyModification::remove_member_attribute);
}

int EOSGLobbyModification::set_bucket_id(const String &p_bucket_id) {
    CharString bucket_id = p_bucket_id.utf8();
    EOS_LobbyModification_SetBucketIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_SETBUCKETID_API_LATEST;
    options.BucketId = bucket_id.get_data();

    return static_cast<int>(EOS_LobbyModification_SetBucketId(m_internal, &options));
}

int EOSGLobbyModification::set_permission_level(int p_permission_level) {
    EOS_LobbyModification_SetPermissionLevelOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
    options.PermissionLevel = static_cast<EOS_ELobbyPermissionLevel>(p_permission_level);

    return static_cast<int>(EOS_LobbyModification_SetPermissionLevel(m_internal, &options));
}

int EOSGLobbyModification::set_max_members(int p_max_members) {
    EOS_LobbyModification_SetMaxMembersOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_SETMAXMEMBERS_API_LATEST;
    options.MaxMembers = static_cast<uint32_t>(p_max_members);

    return static_cast<int>(EOS_LobbyModification_SetMaxMembers(m_internal, &options));
}

int EOSGLobbyModification::set_invites_allowed(bool p_invites_allowed) {
    EOS_LobbyModification_SetInvitesAllowedOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_SETINVITESALLOWED_API_LATEST;
    options.bInvitesAllowed = p_invites_allowed ? EOS_TRUE : EOS_FALSE;

    return static_cast<int>(EOS_LobbyModification_SetInvitesAllowed(m_internal, &options));
}

int EOSGLobbyModification::add_attribute(const String &p_key, Variant p_value, int p_visibility) {
    CharString key = p_key.utf8();

    EOS_Lobby_AttributeData attributeData;
    memset(&attributeData, 0, sizeof(attributeData));
    attributeData.ApiVersion = EOS_LOBBY_ATTRIBUTEDATA_API_LATEST;
    attributeData.Key = key.get_data();

    if (p_value.get_type() == Variant::Type::BOOL) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_BOOLEAN;
        attributeData.Value.AsBool = p_value;
    } else if (p_value.get_type() == Variant::Type::INT) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_INT64;
        attributeData.Value.AsInt64 = p_value;
    } else if (p_value.get_type() == Variant::Type::FLOAT) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_DOUBLE;
        attributeData.Value.AsDouble = p_value;
    } else if (p_value.get_type() == Variant::Type::STRING) {
        CharString value = VARIANT_TO_CHARSTRING(p_value);
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_STRING;
        attributeData.Value.AsUtf8 = value.get_data();
    }

    EOS_LobbyModification_AddAttributeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_ADDATTRIBUTE_API_LATEST;
    options.Attribute = &attributeData;
    options.Visibility = static_cast<EOS_ELobbyAttributeVisibility>(p_visibility);

    return static_cast<int>(EOS_LobbyModification_AddAttribute(m_internal, &options));
}

int EOSGLobbyModification::remove_attribute(const String &p_key) {
    CharString key = p_key.utf8();

    EOS_LobbyModification_RemoveAttributeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_REMOVEATTRIBUTE_API_LATEST;
    options.Key = key.get_data();

    return static_cast<int>(EOS_LobbyModification_RemoveAttribute(m_internal, &options));
}

int EOSGLobbyModification::add_member_attribute(const String &p_key, Variant p_value, int p_visibility) {
    CharString key = p_key.utf8();

    EOS_Lobby_AttributeData attributeData;
    memset(&attributeData, 0, sizeof(attributeData));
    attributeData.ApiVersion = EOS_LOBBY_ATTRIBUTEDATA_API_LATEST;
    attributeData.Key = key.get_data();

    if (p_value.get_type() == Variant::Type::BOOL) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_BOOLEAN;
        attributeData.Value.AsBool = p_value;
    } else if (p_value.get_type() == Variant::Type::INT) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_INT64;
        attributeData.Value.AsInt64 = p_value;
    } else if (p_value.get_type() == Variant::Type::FLOAT) {
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_DOUBLE;
        attributeData.Value.AsDouble = p_value;
    } else if (p_value.get_type() == Variant::Type::STRING) {
        CharString value = VARIANT_TO_CHARSTRING(p_value);
        attributeData.ValueType = EOS_ELobbyAttributeType::EOS_AT_STRING;
        attributeData.Value.AsUtf8 = value.get_data();
    }

    EOS_LobbyModification_AddMemberAttributeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_ADDMEMBERATTRIBUTE_API_LATEST;
    options.Attribute = &attributeData;
    options.Visibility = static_cast<EOS_ELobbyAttributeVisibility>(p_visibility);

    return static_cast<int>(EOS_LobbyModification_AddMemberAttribute(m_internal, &options));
}

int EOSGLobbyModification::remove_member_attribute(const String &p_key) {
    CharString key = p_key.utf8();

    EOS_LobbyModification_RemoveMemberAttributeOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_REMOVEMEMBERATTRIBUTE_API_LATEST;
    options.Key = key.get_data();

    return static_cast<int>(EOS_LobbyModification_RemoveMemberAttribute(m_internal, &options));
}

int EOSGLobbyModification::set_allowed_platform_ids(const TypedArray<int> &p_platform_ids) {
    uint32_t *platform_ids_array = new uint32_t[p_platform_ids.size()];
    for (int i = 0; i < p_platform_ids.size(); i++) {
        platform_ids_array[i] = static_cast<uint32_t>(p_platform_ids[i]);
    }

    EOS_LobbyModification_SetAllowedPlatformIdsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_LOBBYMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST;
    options.AllowedPlatformIds = platform_ids_array;
    options.AllowedPlatformIdsCount = p_platform_ids.size();

    return static_cast<int>(EOS_LobbyModification_SetAllowedPlatformIds(m_internal, &options));
}
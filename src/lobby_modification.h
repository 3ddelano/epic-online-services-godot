#pragma once
#include "eos_lobby.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class LobbyModificationEOSG : public RefCounted {
	GDCLASS(LobbyModificationEOSG, RefCounted)

private:
	EOS_HLobbyModification m_internal = nullptr;
	static void _bind_methods();

public:
	int set_bucket_id(const String &bucket_id);
	int set_permission_level(int permission_level);
	int set_max_members(int max_members);
	int set_invites_allowed(bool invites_allowed);
	int add_attribute(const String &key, Variant value, int visibility);
	int remove_attribute(const String &key);
	int add_member_attribute(const String &key, Variant value, int visibility);
	int remove_member_attribute(const String &key);
	int set_allowed_platform_ids(const TypedArray<int> &platform_ids);

	LobbyModificationEOSG(){};
	~LobbyModificationEOSG() {
		if (m_internal != nullptr) {
			EOS_LobbyModification_Release(m_internal);
		}
	};

	void set_internal(EOS_HLobbyModification p_internal) {
		m_internal = p_internal;
	}

	EOS_HLobbyModification get_internal() {
		return m_internal;
	}
};
} // namespace godot

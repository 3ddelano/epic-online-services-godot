#pragma once
#include "eos_sessions.h"
#include "godot_cpp/classes/ref_counted.hpp"
namespace godot {

class EOSGSessionModification : public RefCounted {
    GDCLASS(EOSGSessionModification, RefCounted)

private:
    EOS_HSessionModification m_internal = nullptr;
    static void _bind_methods();

public:
    int add_attribute(const String &key, Variant value, int advertisement_type);
    int remove_attribute(const String &key);
    int set_allowed_platform_ids(const TypedArray<int> &p_platform_ids);
    int set_bucket_id(const String &bucket_id);
    int set_host_address(const String &host_address);
    int set_invites_allowed(bool invites_allowed);
    int set_join_in_progress_allowed(bool join_in_progress_allowed);
    int set_max_players(int max_players);
    int set_permission_level(int permission_level);

    EOSGSessionModification(){};
    ~EOSGSessionModification() {
        if (m_internal != nullptr) {
            EOS_SessionModification_Release(m_internal);
			m_internal = nullptr;
        }
    };

    void set_internal(EOS_HSessionModification p_internal) {
        m_internal = p_internal;
    }

    EOS_HSessionModification get_internal() {
        return m_internal;
    }
};
} // namespace godot

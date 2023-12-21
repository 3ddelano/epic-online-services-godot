#pragma once
#include "eos_sessions.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGSessionDetails : public RefCounted {
    GDCLASS(EOSGSessionDetails, RefCounted)

private:
    EOS_HSessionDetails m_internal = nullptr;
    static void _bind_methods();

public:
    Dictionary copy_info();
    Dictionary copy_session_attribute_by_index(int attribute_index);
    Dictionary copy_session_attribute_by_key(const String &attribute_key);
    int get_session_attribute_count();

    EOSGSessionDetails(){};
    ~EOSGSessionDetails() {
        if (m_internal != nullptr) {
            EOS_SessionDetails_Release(m_internal);
        }
    };

    void set_internal(EOS_HSessionDetails p_internal) {
        m_internal = p_internal;
    }

    EOS_HSessionDetails get_internal() {
        return m_internal;
    }
};
} // namespace godot

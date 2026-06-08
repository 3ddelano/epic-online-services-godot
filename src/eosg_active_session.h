#pragma once
#include "eos_sessions.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGActiveSession : public RefCounted {
    GDCLASS(EOSGActiveSession, RefCounted)

private:
    EOS_HActiveSession m_internal = nullptr;
    static void _bind_methods();

public:
    Dictionary copy_info();
    String get_registered_player_by_index(int index);
    int get_registered_player_count();

    EOSGActiveSession(){};
    ~EOSGActiveSession() {
        if (m_internal != nullptr) {
            EOS_ActiveSession_Release(m_internal);
			m_internal = nullptr;
        }
    };

    void set_internal(EOS_HActiveSession p_internal) {
        m_internal = p_internal;
    }

    EOS_HActiveSession get_internal() {
        return m_internal;
    }
};
} // namespace godot

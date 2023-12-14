#pragma once
#include "eos_lobby.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/dictionary.hpp"

namespace godot {

class EOSGLobbyDetails : public RefCounted {
    GDCLASS(EOSGLobbyDetails, RefCounted)

private:
    EOS_HLobbyDetails m_internal = nullptr;
    static void _bind_methods();

public:
    String get_lobby_owner();
    Dictionary copy_info();
    int get_attribute_count();
    Dictionary copy_attribute_by_index(int index);
    Dictionary copy_attribute_by_key(const String &key);
    int get_member_count();
    String get_member_by_index(int index);
    int get_member_attribute_count(const String &target_user_id);
    Dictionary copy_member_attribute_by_index(const String &target_user_id, int index);
    Dictionary copy_member_attribute_by_key(const String &target_user_id, const String &key);
    Dictionary copy_member_info(const String &target_user_id);

    EOSGLobbyDetails(){};
    ~EOSGLobbyDetails() {
        if (m_internal != nullptr) {
            EOS_LobbyDetails_Release(m_internal);
        }
    };

    void set_internal(EOS_HLobbyDetails p_internal) {
        m_internal = p_internal;
    }

    EOS_HLobbyDetails get_internal() {
        return m_internal;
    }
};
} // namespace godot

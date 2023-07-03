#pragma once
#include "eos_lobby.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class LobbyDetailsEOSG : public RefCounted {
    GDCLASS(LobbyDetailsEOSG, RefCounted)

   private:
    EOS_HLobbyDetails m_internal = nullptr;
    static void _bind_methods(){};

   public:
    LobbyDetailsEOSG(){};
    ~LobbyDetailsEOSG() {
        if (m_internal != nullptr) {
            EOS_LobbyDetails_Release(m_internal);
        }
    };

    void set_internal(EOS_HLobbyDetails token) {
        m_internal = token;
    }

    EOS_HLobbyDetails get_internal() {
        return m_internal;
    }
};
}  // namespace godot

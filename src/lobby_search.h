#pragma once
#include "eos_lobby.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class LobbySearchEOSG : public RefCounted {
    GDCLASS(LobbySearchEOSG, RefCounted)

   private:
    EOS_HLobbySearch m_internal = nullptr;
    static void _bind_methods();

   public:
    void find(const String& local_user_id);
    int set_lobby_id(const String& lobby_id);
    int set_target_user_id(const String& target_user_id);
    int set_parameter(const String& key, Variant value, int comparison_op);
    int remove_parameter(const String& key, int comparison_op);
    int set_max_results(int max_results);
    int get_search_result_count();
    Dictionary copy_search_result_by_index(int index);

    LobbySearchEOSG(){};
    ~LobbySearchEOSG() {
        if (m_internal != nullptr) {
            EOS_LobbySearch_Release(m_internal);
        }
    };

    void set_internal(EOS_HLobbySearch p_internal) {
        m_internal = p_internal;
    }

    EOS_HLobbySearch get_internal() {
        return m_internal;
    }
};
}  // namespace godot
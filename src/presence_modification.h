#pragma once
#include "eos_presence.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class PresenceModificationEOSG : public RefCounted {
    GDCLASS(PresenceModificationEOSG, RefCounted)

   protected:
    EOS_HPresenceModification m_presence_modification = nullptr;
    static void _bind_methods();

   public:
    int set_status(int new_status);
    int set_raw_rich_text(const String& p_raw_rich_text);
    int set_data(Dictionary p_data);
    int delete_data(Array p_keys);
    int set_join_info(const String& p_join_info);

    PresenceModificationEOSG(){};
    ~PresenceModificationEOSG() {
        if (m_presence_modification != nullptr) {
            EOS_PresenceModification_Release(m_presence_modification);
        }
    };

    void set_presence_modification(EOS_HPresenceModification presence_modification) {
        m_presence_modification = presence_modification;
    }

    EOS_HPresenceModification get_presence_modification() {
        return m_presence_modification;
    }
};

}  // namespace godot
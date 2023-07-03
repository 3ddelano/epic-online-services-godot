#pragma once
#include "eos_common.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class ContinuanceTokenEOSG : public RefCounted {
    GDCLASS(ContinuanceTokenEOSG, RefCounted)

   private:
    EOS_ContinuanceToken m_token = nullptr;
    static void _bind_methods(){};

   public:
    ContinuanceTokenEOSG(){};
    ~ContinuanceTokenEOSG(){};

    void set_token(EOS_ContinuanceToken token) {
        m_token = token;
    }

    EOS_ContinuanceToken get_token() {
        return m_token;
    }
};
}  // namespace godot

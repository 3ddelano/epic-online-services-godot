#pragma once
#include "eos_common.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class ContinuanceTokenEOSG : public RefCounted {
    GDCLASS(ContinuanceTokenEOSG, RefCounted)

private:
    EOS_ContinuanceToken m_internal = nullptr;
    static void _bind_methods(){};

public:
    ContinuanceTokenEOSG(){};
    ~ContinuanceTokenEOSG(){};

    void set_internal(EOS_ContinuanceToken p_internal) {
        m_internal = p_internal;
    }

    EOS_ContinuanceToken get_internal() {
        return m_internal;
    }
};
} // namespace godot

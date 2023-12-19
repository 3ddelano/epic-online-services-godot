#pragma once
#include "eos_ecom.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGTransaction : public RefCounted {
    GDCLASS(EOSGTransaction, RefCounted)

protected:
    EOS_Ecom_HTransaction m_internal = nullptr;
    static void _bind_methods();

public:
    Dictionary copy_entitlement_by_index(int p_entitlement_index);
    int get_entitlement_count();
    String get_id();

    EOSGTransaction(){};
    ~EOSGTransaction() {
        if (m_internal != nullptr) {
            EOS_Ecom_Transaction_Release(m_internal);
        }
    };

    void set_internal(EOS_Ecom_HTransaction p_internal) {
        m_internal = p_internal;
    }

    EOS_Ecom_HTransaction get_internal() {
        return m_internal;
    }
};
} // namespace godot
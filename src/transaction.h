#pragma once
#include "eos_ecom.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class TransactionEOSG : public RefCounted {
    GDCLASS(TransactionEOSG, RefCounted)

   protected:
    EOS_Ecom_HTransaction m_transaction = nullptr;
    static void _bind_methods();

   public:
    String get_id();
    int get_entitlement_count();
    Dictionary copy_entitlement_by_index(int p_entitlement_index);

    TransactionEOSG(){};
    ~TransactionEOSG() {
        if (m_transaction != nullptr) {
            EOS_Ecom_Transaction_Release(m_transaction);
        }
    };

    void set_transaction(EOS_Ecom_HTransaction transaction) {
        m_transaction = transaction;
    }

    EOS_Ecom_HTransaction get_transaction() {
        return m_transaction;
    }
};
}  // namespace godot
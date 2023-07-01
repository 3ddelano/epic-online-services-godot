#pragma once
#include "eos_ecom_types.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class TransactionEOSG : public RefCounted {
    GDCLASS(TransactionEOSG, RefCounted)

   private:
    EOS_Ecom_HTransaction m_transaction = nullptr;
    static void _bind_methods(){};

   public:
    bool isSet = false;
    TransactionEOSG(){};
    ~TransactionEOSG() {
        EOS_Ecom_Transaction_Release(m_transaction);
    };

    void set_transaction(EOS_Ecom_HTransaction transaction) {
        m_transaction = transaction;
        if (transaction == nullptr) {
            isSet = false;
        } else {
            isSet = true;
        }
    }

    EOS_Ecom_HTransaction get_transaction() {
        return m_transaction;
    }
};
}  // namespace godot

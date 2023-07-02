#pragma once
#include "eos_ecom_types.h"
#include "godot_cpp/classes/ref_counted.hpp"
// #include "utils.h"

namespace godot {

class TransactionEOSG : public RefCounted {
    GDCLASS(TransactionEOSG, RefCounted)

   private:
    EOS_Ecom_HTransaction m_transaction = nullptr;
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("get_id"), &TransactionEOSG::get_id);
        ClassDB::bind_method(D_METHOD("get_entitlement_count"), &TransactionEOSG::get_entitlement_count);
        ClassDB::bind_method(D_METHOD("copy_entitlement_by_index", "entitlement_index"), &TransactionEOSG::copy_entitlement_by_index);
    };

   public:
    String get_id() {
        char* outBuffer = (char*)memalloc(256);
        int32_t outLength = 0;
        EOS_Ecom_Transaction_GetTransactionId(m_transaction, outBuffer, &outLength);
        return String(outBuffer);
    }

    int get_entitlement_count() {
        EOS_Ecom_Transaction_GetEntitlementsCountOptions options;
        memset(&options, 0, sizeof(EOS_Ecom_Transaction_GetEntitlementsCountOptions));
        options.ApiVersion = EOS_ECOM_TRANSACTION_GETENTITLEMENTSCOUNT_API_LATEST;

        return static_cast<int>(EOS_Ecom_Transaction_GetEntitlementsCount(m_transaction, &options));
    }

    Dictionary copy_entitlement_by_index(int p_entitlement_index) {
        EOS_Ecom_Transaction_CopyEntitlementByIndexOptions options;
        memset(&options, 0, sizeof(EOS_Ecom_Transaction_CopyEntitlementByIndexOptions));
        options.ApiVersion = EOS_ECOM_TRANSACTION_COPYENTITLEMENTBYINDEX_API_LATEST;
        options.EntitlementIndex = static_cast<uint32_t>(p_entitlement_index);

        EOS_Ecom_Entitlement* outEntitlement;
        EOS_EResult res = EOS_Ecom_Transaction_CopyEntitlementByIndex(m_transaction, &options, &outEntitlement);

        Dictionary ret;
        ret["result_code"] = static_cast<int>(res);
        // TODO: Figure out why the below line gives not found error
        // ret["entitlement"] = eosg_ecom_entitlement_to_dict(outEntitlement);
        EOS_Ecom_Entitlement_Release(outEntitlement);

        return ret;
    }

    bool isSet = false;
    TransactionEOSG(){};
    ~TransactionEOSG() {
        if (m_transaction != nullptr) {
            EOS_Ecom_Transaction_Release(m_transaction);
        }
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
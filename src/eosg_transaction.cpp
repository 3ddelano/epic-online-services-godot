#include "eosg_transaction.h"

#include "utils.h"

using namespace godot;

void EOSGTransaction::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_id"), &EOSGTransaction::get_id);
    ClassDB::bind_method(D_METHOD("get_entitlement_count"), &EOSGTransaction::get_entitlement_count);
    ClassDB::bind_method(D_METHOD("copy_entitlement_by_index", "entitlement_index"), &EOSGTransaction::copy_entitlement_by_index);
};

String EOSGTransaction::get_id() {
    char *outBuffer = (char *)memalloc(256);
    int32_t outLength = 0;
    EOS_Ecom_Transaction_GetTransactionId(m_internal, outBuffer, &outLength);
    return String(outBuffer);
}

int EOSGTransaction::get_entitlement_count() {
    EOS_Ecom_Transaction_GetEntitlementsCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_TRANSACTION_GETENTITLEMENTSCOUNT_API_LATEST;

    return static_cast<int>(EOS_Ecom_Transaction_GetEntitlementsCount(m_internal, &options));
}

Dictionary EOSGTransaction::copy_entitlement_by_index(int p_entitlement_index) {
    EOS_Ecom_Transaction_CopyEntitlementByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_TRANSACTION_COPYENTITLEMENTBYINDEX_API_LATEST;
    options.EntitlementIndex = static_cast<uint32_t>(p_entitlement_index);

    EOS_Ecom_Entitlement *outEntitlement;
    EOS_EResult res = EOS_Ecom_Transaction_CopyEntitlementByIndex(m_internal, &options, &outEntitlement);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["entitlement"] = eosg_ecom_entitlement_to_dict_and_release(outEntitlement);
    return ret;
}
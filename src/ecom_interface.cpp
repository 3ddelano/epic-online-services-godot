#include "ieos.h"
using namespace std;

void IEOS::ecom_interface_checkout(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    String p_override_catalog_namespace = p_options->get("override_catalog_namespace");
    CharString override_catalog_namespace = VARIANT_TO_CHARSTRING(p_options->get("override_catalog_namespace"));

    EOS_Ecom_CheckoutOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_CHECKOUT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    if (!p_override_catalog_namespace.is_empty()) {
        options.OverrideCatalogNamespace = override_catalog_namespace.get_data();
    }

    TypedArray<Dictionary> p_entries = p_options->get("entries");
    options.EntryCount = static_cast<uint32_t>(p_entries.size());
    EOS_Ecom_CheckoutEntry *entries = (EOS_Ecom_CheckoutEntry *)memalloc(sizeof(EOS_Ecom_CheckoutEntry) * options.EntryCount);
    for (int i = 0; i < options.EntryCount; i++) {
        Dictionary entry = p_entries[i];
        CharString offer_id = VARIANT_TO_CHARSTRING(entry["offer_id"]);
        entries[i].ApiVersion = EOS_ECOM_CHECKOUTENTRY_API_LATEST;
        entries[i].OfferId = offer_id.get_data();
    }
    options.Entries = entries;

    p_options->reference();

    EOS_Ecom_Checkout(s_ecomInterface, &options, (void *)*p_options, [](const EOS_Ecom_CheckoutCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["transaction_id"] = String(data->TransactionId);
        IEOS::get_singleton()->emit_signal("ecom_interface_checkout_callback", ret);
    });
}

Dictionary IEOS::ecom_interface_copy_entitlement_by_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString entitlement_id = VARIANT_TO_CHARSTRING(p_options->get("entitlement_id"));

    EOS_Ecom_CopyEntitlementByIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYENTITLEMENTBYID_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.EntitlementId = entitlement_id.get_data();

    EOS_Ecom_Entitlement *outEntitlement = nullptr;
    EOS_EResult res = EOS_Ecom_CopyEntitlementById(s_ecomInterface, &options, &outEntitlement);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["entitlement"] = eosg_ecom_entitlement_to_dict_and_release(outEntitlement);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_entitlement_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_CopyEntitlementByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYENTITLEMENTBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.EntitlementIndex = static_cast<uint32_t>(p_options->get("entitlement_index"));

    EOS_Ecom_Entitlement *outEntitlement = nullptr;
    EOS_EResult res = EOS_Ecom_CopyEntitlementByIndex(s_ecomInterface, &options, &outEntitlement);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["entitlement"] = eosg_ecom_entitlement_to_dict_and_release(outEntitlement);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_entitlement_by_name_and_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString entitlement_name = VARIANT_TO_CHARSTRING(p_options->get("entitlement_name"));

    EOS_Ecom_CopyEntitlementByNameAndIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYENTITLEMENTBYNAMEANDINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.EntitlementName = entitlement_name.get_data();
    options.Index = static_cast<uint32_t>(p_options->get("index"));

    EOS_Ecom_Entitlement *outEntitlement = nullptr;
    EOS_EResult res = EOS_Ecom_CopyEntitlementByNameAndIndex(s_ecomInterface, &options, &outEntitlement);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["entitlement"] = eosg_ecom_entitlement_to_dict_and_release(outEntitlement);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_item_by_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString item_id = VARIANT_TO_CHARSTRING(p_options->get("item_id"));

    EOS_Ecom_CopyItemByIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYITEMBYID_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.ItemId = item_id.get_data();

    EOS_Ecom_CatalogItem *outItem = nullptr;
    EOS_EResult res = EOS_Ecom_CopyItemById(s_ecomInterface, &options, &outItem);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["item"] = eosg_ecom_catalog_item_to_dict_and_release(outItem);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_item_image_info_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString item_id = VARIANT_TO_CHARSTRING(p_options->get("item_id"));

    EOS_Ecom_CopyItemImageInfoByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYITEMIMAGEINFOBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.ItemId = item_id.get_data();
    options.ImageInfoIndex = static_cast<uint32_t>(p_options->get("image_info_index"));

    EOS_Ecom_KeyImageInfo *outImageInfo = nullptr;
    EOS_EResult res = EOS_Ecom_CopyItemImageInfoByIndex(s_ecomInterface, &options, &outImageInfo);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["image_info"] = eosg_ecom_key_image_info_to_dict_and_release(outImageInfo);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_item_release_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString item_id = VARIANT_TO_CHARSTRING(p_options->get("item_id"));

    EOS_Ecom_CopyItemReleaseByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYITEMRELEASEBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.ItemId = item_id.get_data();
    options.ReleaseIndex = static_cast<uint32_t>(p_options->get("release_index"));

    EOS_Ecom_CatalogRelease *outRelease = nullptr;
    EOS_EResult res = EOS_Ecom_CopyItemReleaseByIndex(s_ecomInterface, &options, &outRelease);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["release"] = eosg_ecom_catalog_release_to_dict_and_release(outRelease);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_offer_by_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString offer_id = VARIANT_TO_CHARSTRING(p_options->get("offer_id"));

    EOS_Ecom_CopyOfferByIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYOFFERBYID_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferId = offer_id.get_data();

    EOS_Ecom_CatalogOffer *outOffer = nullptr;
    EOS_EResult res = EOS_Ecom_CopyOfferById(s_ecomInterface, &options, &outOffer);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["offer"] = eosg_ecom_catalog_offer_to_dict_and_release(outOffer);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_offer_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_CopyOfferByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYOFFERBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferIndex = static_cast<uint32_t>(p_options->get("offer_index"));

    EOS_Ecom_CatalogOffer *outOffer = nullptr;
    EOS_EResult res = EOS_Ecom_CopyOfferByIndex(s_ecomInterface, &options, &outOffer);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["offer"] = eosg_ecom_catalog_offer_to_dict_and_release(outOffer);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_offer_image_info_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString offer_id = VARIANT_TO_CHARSTRING(p_options->get("offer_id"));

    EOS_Ecom_CopyOfferImageInfoByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYOFFERIMAGEINFOBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferId = offer_id.get_data();
    options.ImageInfoIndex = static_cast<uint32_t>(p_options->get("image_info_index"));

    EOS_Ecom_KeyImageInfo *outImageInfo = nullptr;
    EOS_EResult res = EOS_Ecom_CopyOfferImageInfoByIndex(s_ecomInterface, &options, &outImageInfo);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["image_info"] = eosg_ecom_key_image_info_to_dict_and_release(outImageInfo);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_offer_item_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString offer_id = VARIANT_TO_CHARSTRING(p_options->get("offer_id"));

    EOS_Ecom_CopyOfferItemByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYOFFERITEMBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferId = offer_id.get_data();
    options.ItemIndex = static_cast<uint32_t>(p_options->get("item_index"));

    EOS_Ecom_CatalogItem *outItem = nullptr;
    EOS_EResult res = EOS_Ecom_CopyOfferItemByIndex(s_ecomInterface, &options, &outItem);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["item"] = eosg_ecom_catalog_item_to_dict_and_release(outItem);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_transaction_by_id(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString transaction_id = VARIANT_TO_CHARSTRING(p_options->get("transaction_id"));

    EOS_Ecom_CopyTransactionByIdOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYTRANSACTIONBYID_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TransactionId = transaction_id.get_data();

    EOS_Ecom_HTransaction outTransaction = nullptr;
    EOS_EResult res = EOS_Ecom_CopyTransactionById(s_ecomInterface, &options, &outTransaction);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["transaction"] = eosg_ecom_transaction_to_wrapper(outTransaction);
    return ret;
}

Dictionary IEOS::ecom_interface_copy_transaction_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_CopyTransactionByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYTRANSACTIONBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.TransactionIndex = static_cast<uint32_t>(p_options->get("transaction_index"));

    EOS_Ecom_HTransaction outTransaction = nullptr;
    EOS_EResult res = EOS_Ecom_CopyTransactionByIndex(s_ecomInterface, &options, &outTransaction);
    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["transaction"] = eosg_ecom_transaction_to_wrapper(outTransaction);
    return ret;
}

int IEOS::ecom_interface_get_entitlements_by_name_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString entitlement_name = VARIANT_TO_CHARSTRING(p_options->get("entitlement_name"));

    EOS_Ecom_GetEntitlementsByNameCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETENTITLEMENTSBYNAMECOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.EntitlementName = entitlement_name.get_data();

    return static_cast<int>(EOS_Ecom_GetEntitlementsByNameCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_entitlements_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_GetEntitlementsCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETENTITLEMENTSCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return static_cast<int>(EOS_Ecom_GetEntitlementsCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_item_image_info_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString offer_id = VARIANT_TO_CHARSTRING(p_options->get("offer_id"));

    EOS_Ecom_GetOfferImageInfoCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETOFFERIMAGEINFOCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferId = offer_id.get_data();

    return static_cast<int>(EOS_Ecom_GetOfferImageInfoCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_item_release_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString item_id = VARIANT_TO_CHARSTRING(p_options->get("item_id"));

    EOS_Ecom_GetItemReleaseCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETITEMRELEASECOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.ItemId = item_id.get_data();

    return static_cast<int>(EOS_Ecom_GetItemReleaseCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_offer_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_GetOfferCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETOFFERCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return static_cast<int>(EOS_Ecom_GetOfferCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_offer_image_info_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString offer_id = VARIANT_TO_CHARSTRING(p_options->get("offer_id"));

    EOS_Ecom_GetOfferImageInfoCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETOFFERIMAGEINFOCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferId = offer_id.get_data();

    return static_cast<int>(EOS_Ecom_GetOfferImageInfoCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_offer_item_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString offer_id = VARIANT_TO_CHARSTRING(p_options->get("offer_id"));

    EOS_Ecom_GetOfferItemCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETOFFERITEMCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.OfferId = offer_id.get_data();

    return static_cast<int>(EOS_Ecom_GetOfferItemCount(s_ecomInterface, &options));
}

int IEOS::ecom_interface_get_transaction_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_GetTransactionCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETTRANSACTIONCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return static_cast<int>(EOS_Ecom_GetTransactionCount(s_ecomInterface, &options));
}

void IEOS::ecom_interface_query_entitlements(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_QueryEntitlementsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_QUERYENTITLEMENTS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    TypedArray<String> p_entitlement_names = p_options->get("entitlement_names");

    EOS_Ecom_EntitlementName *entitlementNames = (EOS_Ecom_EntitlementName *)memalloc(sizeof(EOS_Ecom_EntitlementName) * p_entitlement_names.size());
    for (int i = 0; i < p_entitlement_names.size(); i++) {
        CharString entitlement_name = VARIANT_TO_CHARSTRING(p_entitlement_names[i]);
        entitlementNames[i] = entitlement_name.get_data();
    }
    options.EntitlementNameCount = static_cast<uint32_t>(p_entitlement_names.size());
    options.EntitlementNames = entitlementNames;
    options.bIncludeRedeemed = VARIANT_TO_EOS_BOOL(p_options->get("include_redeemed"));
    p_options->reference();

    EOS_Ecom_QueryEntitlements(s_ecomInterface, &options, (void *)*p_options, [](const EOS_Ecom_QueryEntitlementsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("ecom_interface_query_entitlements_callback", ret);
    });
}

void IEOS::ecom_interface_query_offers(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    String p_override_catalog_namespace = p_options->get("override_catalog_namespace");
    CharString override_catalog_namespace = VARIANT_TO_CHARSTRING(p_options->get("override_catalog_namespace"));

    EOS_Ecom_QueryOffersOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_QUERYOFFERS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    if (!p_override_catalog_namespace.is_empty()) {
        options.OverrideCatalogNamespace = override_catalog_namespace.get_data();
    }
    p_options->reference();

    EOS_Ecom_QueryOffers(s_ecomInterface, &options, (void *)*p_options, [](const EOS_Ecom_QueryOffersCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("ecom_interface_query_offers_callback", ret);
    });
}

void IEOS::ecom_interface_query_ownership(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    String p_catalog_namespace = p_options->get("catalog_namespace");
    CharString catalog_namespace = VARIANT_TO_CHARSTRING(p_options->get("catalog_namespace"));

    TypedArray<String> p_catalog_item_ids = p_options->get("catalog_item_ids");
    EOS_Ecom_CatalogItemId *catalog_item_ids = (EOS_Ecom_CatalogItemId *)memalloc(sizeof(EOS_Ecom_CatalogItemId) * p_catalog_item_ids.size());
    for (int i = 0; i < p_catalog_item_ids.size(); i++) {
        CharString catalog_item_id = VARIANT_TO_CHARSTRING(p_catalog_item_ids[i]);
        catalog_item_ids[i] = catalog_item_id.get_data();
    }

    EOS_Ecom_QueryOwnershipOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_QUERYOWNERSHIP_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.CatalogItemIdCount = static_cast<uint32_t>(p_catalog_item_ids.size());
    options.CatalogItemIds = catalog_item_ids;
    if (!p_catalog_namespace.is_empty()) {
        options.CatalogNamespace = catalog_namespace.get_data();
    }
    p_options->reference();

    EOS_Ecom_QueryOwnership(s_ecomInterface, &options, (void *)*p_options, [](const EOS_Ecom_QueryOwnershipCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("ecom_interface_query_ownership_callback", ret);
    });
}

void IEOS::ecom_interface_query_ownership_token(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    String p_catalog_namespace = p_options->get("catalog_namespace");
    CharString catalog_namespace = VARIANT_TO_CHARSTRING(p_options->get("catalog_namespace"));

    TypedArray<String> p_catalog_item_ids = p_options->get("catalog_item_ids");
    EOS_Ecom_CatalogItemId *catalog_item_ids = (EOS_Ecom_CatalogItemId *)memalloc(sizeof(EOS_Ecom_CatalogItemId) * p_catalog_item_ids.size());
    for (int i = 0; i < p_catalog_item_ids.size(); i++) {
        CharString catalog_item_id = VARIANT_TO_CHARSTRING(p_catalog_item_ids[i]);
        catalog_item_ids[i] = catalog_item_id.get_data();
    }

    EOS_Ecom_QueryOwnershipTokenOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_QUERYOWNERSHIPTOKEN_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.CatalogItemIdCount = static_cast<uint32_t>(p_catalog_item_ids.size());
    options.CatalogItemIds = catalog_item_ids;
    if (!p_catalog_namespace.is_empty()) {
        options.CatalogNamespace = catalog_namespace.get_data();
    }
    p_options->reference();

    EOS_Ecom_QueryOwnershipToken(s_ecomInterface, &options, (void *)*p_options, [](const EOS_Ecom_QueryOwnershipTokenCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["ownership_token"] = EOSG_GET_STRING(data->OwnershipToken);
        IEOS::get_singleton()->emit_signal("ecom_interface_query_ownership_token_callback", ret);
    });
}

void IEOS::ecom_interface_redeem_entitlements(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    TypedArray<String> p_entitlement_ids = p_options->get("entitlement_ids");
    EOS_Ecom_EntitlementId *entitlement_ids = (EOS_Ecom_EntitlementId *)memalloc(sizeof(EOS_Ecom_EntitlementId) * p_entitlement_ids.size());

    EOS_Ecom_RedeemEntitlementsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_REDEEMENTITLEMENTS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.EntitlementIdCount = static_cast<uint32_t>(p_entitlement_ids.size());
    options.EntitlementIds = entitlement_ids;
    p_options->reference();

    EOS_Ecom_RedeemEntitlements(s_ecomInterface, &options, (void *)*p_options, [](const EOS_Ecom_RedeemEntitlementsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["redeemed_entitlement_ids_count"] = data->RedeemedEntitlementIdsCount;
        IEOS::get_singleton()
                ->emit_signal("ecom_interface_redeem_entitlements_callback", ret);
    });
}

int IEOS::ecom_interface_get_last_redeemed_entitlements_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_GetLastRedeemedEntitlementsCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_GETLASTREDEEMEDENTITLEMENTSCOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());

    return static_cast<int>(EOS_Ecom_GetLastRedeemedEntitlementsCount(s_ecomInterface, &options));
}

Dictionary IEOS::ecom_interface_copy_last_redeemed_entitlement_by_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Ecom_CopyLastRedeemedEntitlementByIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_ECOM_COPYLASTREDEEMEDENTITLEMENTBYINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.RedeemedEntitlementIndex = static_cast<uint32_t>(p_options->get("redeemed_entitlement_index"));

    char *outEntitlementId = (char *)memalloc(EOS_ECOM_ENTITLEMENTID_MAX_LENGTH + 1);
    int outEntitlementIdLength = 0;
    EOS_EResult res = EOS_Ecom_CopyLastRedeemedEntitlementByIndex(s_ecomInterface, &options, outEntitlementId, &outEntitlementIdLength);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["entitlement_id"] = EOSG_GET_STRING(outEntitlementId);
    return ret;
}

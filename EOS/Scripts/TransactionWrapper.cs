using Godot;
using Godot.Collections;
using Epic.OnlineServices;
using Epic.OnlineServices.Ecom;

public class TransactionWrapper : Reference
{
    public Dictionary copy_entitlement_by_index(Reference p_options)
    {
        int p_entitlement_index = (int)p_options.Get("entitlement_index");
        var options = new TransactionCopyEntitlementByIndexOptions()
        {
            EntitlementIndex = (uint)p_entitlement_index
        };
        Entitlement? outEntitlement;
        var res = _internalTransaction.CopyEntitlementByIndex(ref options, out outEntitlement);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            var entitlement_dict = new Dictionary(){
                {"entitlement_name", outEntitlement?.EntitlementName.ToString()},
                {"entitlement_id", outEntitlement?.EntitlementId.ToString()},
                {"catalog_item_id", outEntitlement?.CatalogItemId.ToString()},
                {"server_index", outEntitlement?.ServerIndex.ToString()},
                {"redeemed", outEntitlement?.Redeemed},
                {"end_timestamp", (int) outEntitlement?.EndTimestamp},
            };
            ret.Add("entitlement", entitlement_dict);
        }

        return ret;
    }

    public int get_entitlements_count(Reference p_options)
    {
        var options = new TransactionGetEntitlementsCountOptions();
        uint count = _internalTransaction.GetEntitlementsCount(ref options);
        return (int)count;
    }

    public Dictionary get_transaction_id(Reference p_options)
    {
        Utf8String outBuffer;
        Result res = _internalTransaction.GetTransactionId(out outBuffer);
        var ret = new Dictionary(){
            {"result_code", res}
        };
        if (res == Result.Success)
        {
            ret.Add("transaction_id", outBuffer.ToString());
        }
        return ret;
    }

    public TransactionWrapper(Transaction transaction)
    {
        _internalTransaction = transaction;
    }

    public new string GetClass()
    {
        return "TransactionWrapper";
    }

    public override string ToString()
    {
        return "TransactionWrapper(" + _internalTransaction.ToString() + ")";
    }

    public override void _Notification(int what)
    {
        if (what == 1) //Object.NOTIFICATION_PREDELETE
        {
            _internalTransaction.Release();
        }
    }

    public Transaction _internalTransaction;

}
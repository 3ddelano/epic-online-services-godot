#include "ieos.h"
using namespace std;

Dictionary IEOS::mods_interface_copy_mod_info(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_modsInterface, {});
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Mods_CopyModInfoOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_MODS_COPYMODINFO_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.Type = static_cast<EOS_EModEnumerationType>(static_cast<int>(p_options->get("type")));

    EOS_Mods_ModInfo *outEnumeratedMods = nullptr;
    EOS_EResult res = EOS_Mods_CopyModInfo(s_modsInterface, &options, &outEnumeratedMods);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["mods"] = eosg_mods_mod_info_to_dict_and_release(outEnumeratedMods);
    return ret;
}

void IEOS::mods_interface_enumerate_mods(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_modsInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_Mods_EnumerateModsOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_MODS_ENUMERATEMODS_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.Type = static_cast<EOS_EModEnumerationType>(static_cast<int>(p_options->get("type")));
    p_options->reference();

    EOS_Mods_EnumerateMods(s_modsInterface, &options, (void *)*p_options, [](const EOS_Mods_EnumerateModsCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["type"] = static_cast<int>(data->Type);
        IEOS::get_singleton()->emit_signal("mods_interface_enumerate_mods_callback", ret);
    });
}

void IEOS::mods_interface_install_mod(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_modsInterface);
    Dictionary p_mod = p_options->get("mod");
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    CharString mod_namespace_id = VARIANT_TO_CHARSTRING(p_mod["namespace_id"]);
    CharString mod_item_id = VARIANT_TO_CHARSTRING(p_mod["item_id"]);
    CharString mod_artifact_id = VARIANT_TO_CHARSTRING(p_mod["artifact_id"]);
    CharString mod_title = VARIANT_TO_CHARSTRING(p_mod["title"]);
    CharString mod_version = VARIANT_TO_CHARSTRING(p_mod["version"]);

    EOS_Mod_Identifier mod;
    memset(&mod, 0, sizeof(mod));
    mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;
    mod.NamespaceId = mod_namespace_id.get_data();
    mod.ItemId = mod_item_id.get_data();
    mod.ArtifactId = mod_artifact_id.get_data();
    mod.Title = mod_title.get_data();
    mod.Version = mod_version.get_data();

    EOS_Mods_InstallModOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_MODS_INSTALLMOD_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.Mod = &mod;
    options.bRemoveAfterExit = VARIANT_TO_EOS_BOOL(p_options->get("remove_after_exit"));
    p_options->reference();

    EOS_Mods_InstallMod(s_modsInterface, &options, (void *)*p_options, [](const EOS_Mods_InstallModCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["mod"] = eosg_mods_mod_identifier_to_dict(data->Mod);
        IEOS::get_singleton()->emit_signal("mods_interface_install_mod_callback", ret);
    });
}

void IEOS::mods_interface_uninstall_mod(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_modsInterface);
    Dictionary p_mod = p_options->get("mod");
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    CharString mod_namespace_id = VARIANT_TO_CHARSTRING(p_mod["namespace_id"]);
    CharString mod_item_id = VARIANT_TO_CHARSTRING(p_mod["item_id"]);
    CharString mod_artifact_id = VARIANT_TO_CHARSTRING(p_mod["artifact_id"]);
    CharString mod_title = VARIANT_TO_CHARSTRING(p_mod["title"]);
    CharString mod_version = VARIANT_TO_CHARSTRING(p_mod["version"]);

    EOS_Mod_Identifier mod;
    memset(&mod, 0, sizeof(mod));
    mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;
    mod.NamespaceId = mod_namespace_id.get_data();
    mod.ItemId = mod_item_id.get_data();
    mod.ArtifactId = mod_artifact_id.get_data();
    mod.Title = mod_title.get_data();
    mod.Version = mod_version.get_data();

    EOS_Mods_UninstallModOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_MODS_UNINSTALLMOD_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.Mod = &mod;
    p_options->reference();

    EOS_Mods_UninstallMod(s_modsInterface, &options, (void *)*p_options, [](const EOS_Mods_UninstallModCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["mod"] = eosg_mods_mod_identifier_to_dict(data->Mod);
        IEOS::get_singleton()->emit_signal("mods_interface_uninstall_mod_callback", ret);
    });
}

void IEOS::mods_interface_update_mod(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_modsInterface);
    Dictionary p_mod = p_options->get("mod");
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    CharString mod_namespace_id = VARIANT_TO_CHARSTRING(p_mod["namespace_id"]);
    CharString mod_item_id = VARIANT_TO_CHARSTRING(p_mod["item_id"]);
    CharString mod_artifact_id = VARIANT_TO_CHARSTRING(p_mod["artifact_id"]);
    CharString mod_title = VARIANT_TO_CHARSTRING(p_mod["title"]);
    CharString mod_version = VARIANT_TO_CHARSTRING(p_mod["version"]);

    EOS_Mod_Identifier mod;
    memset(&mod, 0, sizeof(mod));
    mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;
    mod.NamespaceId = mod_namespace_id.get_data();
    mod.ItemId = mod_item_id.get_data();
    mod.ArtifactId = mod_artifact_id.get_data();
    mod.Title = mod_title.get_data();
    mod.Version = mod_version.get_data();

    EOS_Mods_UpdateModOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_MODS_UPDATEMOD_API_LATEST;
    options.LocalUserId = eosg_string_to_epic_account_id(local_user_id.get_data());
    options.Mod = &mod;
    p_options->reference();

    EOS_Mods_UpdateMod(s_modsInterface, &options, (void *)*p_options, [](const EOS_Mods_UpdateModCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_epic_account_id_to_string(data->LocalUserId);
        ret["mod"] = eosg_mods_mod_identifier_to_dict(data->Mod);
        IEOS::get_singleton()->emit_signal("mods_interface_update_mod_callback", ret);
    });
}

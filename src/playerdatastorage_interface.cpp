#include "ieos.h"
using namespace std;

void IEOS::playerdatastorage_interface_query_file(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));

    EOS_PlayerDataStorage_QueryFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_QUERYFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();
    p_options->reference();

    EOS_PlayerDataStorage_QueryFile(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_QueryFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_query_file_callback", ret);
    });
}

void IEOS::playerdatastorage_interface_query_file_list(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_PlayerDataStorage_QueryFileListOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_QUERYFILELIST_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_PlayerDataStorage_QueryFileList(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_QueryFileListCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["file_count"] = static_cast<int>(data->FileCount);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_query_file_list_callback", ret);
    });
}

Dictionary IEOS::playerdatastorage_interface_copy_file_metadata_by_filename(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));

    EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();

    EOS_PlayerDataStorage_FileMetadata *outMetadata = nullptr;
    EOS_EResult res = EOS_PlayerDataStorage_CopyFileMetadataByFilename(s_playerDataStorageInterface, &options, &outMetadata);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["metadata"] = eosg_playerdatastorage_file_metadata_to_dict_and_release(outMetadata);
    return ret;
}

Dictionary IEOS::playerdatastorage_interface_get_file_metadata_count(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_PlayerDataStorage_GetFileMetadataCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_GETFILEMETADATACOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    int32_t outFileMetadataCount = 0;
    EOS_EResult res = EOS_PlayerDataStorage_GetFileMetadataCount(s_playerDataStorageInterface, &options, &outFileMetadataCount);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["file_metadata_count"] = outFileMetadataCount;
    return ret;
}

Dictionary IEOS::playerdatastorage_interface_copy_file_metadata_at_index(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    int index = p_options->get("index");

    EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_COPYFILEMETADATAATINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Index = static_cast<uint32_t>(index);

    EOS_PlayerDataStorage_FileMetadata *outMetadata = nullptr;
    EOS_EResult res = EOS_PlayerDataStorage_CopyFileMetadataAtIndex(s_playerDataStorageInterface, &options, &outMetadata);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["metadata"] = eosg_playerdatastorage_file_metadata_to_dict_and_release(outMetadata);
    return ret;
}

void IEOS::playerdatastorage_interface_duplicate_file(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString source_filename = VARIANT_TO_CHARSTRING(p_options->get("source_filename"));
    CharString destination_filename = VARIANT_TO_CHARSTRING(p_options->get("destination_filename"));

    EOS_PlayerDataStorage_DuplicateFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_DUPLICATEFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.SourceFilename = source_filename.get_data();
    options.DestinationFilename = destination_filename.get_data();
    p_options->reference();

    EOS_PlayerDataStorage_DuplicateFile(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_DuplicateFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_duplicate_file_callback", ret);
    });
}

void IEOS::playerdatastorage_interface_delete_file(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));

    EOS_PlayerDataStorage_DeleteFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_DELETEFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();
    p_options->reference();

    EOS_PlayerDataStorage_DeleteFile(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_DeleteFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_delete_file_callback", ret);
    });
}

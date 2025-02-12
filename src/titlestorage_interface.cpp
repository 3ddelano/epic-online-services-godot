#include "godot_cpp/variant/packed_byte_array.hpp"
#include "ieos.h"

using namespace godot;

void IEOS::titlestorage_interface_query_file(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_titleStorageInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));

    EOS_TitleStorage_QueryFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_QUERYFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();
    p_options->reference();

    EOS_TitleStorage_QueryFile(s_titleStorageInterface, &options, (void *)*p_options, [](const EOS_TitleStorage_QueryFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("titlestorage_interface_query_file_callback", ret);
    });
}

void IEOS::titlestorage_interface_query_file_list(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL(s_titleStorageInterface);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    Array list_of_tags = p_options->get("list_of_tags");

    ERR_FAIL_COND_MSG(list_of_tags.size() == 0, "TitleStorage.QueryFileListOptions.list_of_tags must not be empty.");

    EOS_TitleStorage_QueryFileListOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_QUERYFILELIST_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.ListOfTagsCount = static_cast<uint32_t>(list_of_tags.size());
    const char **listOfTags = (const char **)memalloc(sizeof(const char *) * options.ListOfTagsCount);
    for (int i = 0; i < options.ListOfTagsCount; i++) {
        listOfTags[i] = VARIANT_TO_CHARSTRING(list_of_tags[i]).get_data();
    }
    options.ListOfTags = listOfTags;
    p_options->reference();

    EOS_TitleStorage_QueryFileList(s_titleStorageInterface, &options, (void *)*p_options, [](const EOS_TitleStorage_QueryFileListCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["file_count"] = static_cast<int>(data->FileCount);
        IEOS::get_singleton()->emit_signal("titlestorage_interface_query_file_list_callback", ret);
    });
}

Dictionary IEOS::titlestorage_interface_copy_file_metadata_by_filename(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_titleStorageInterface, {});
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));

    EOS_TitleStorage_CopyFileMetadataByFilenameOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();

    EOS_TitleStorage_FileMetadata *outMetadata = nullptr;
    EOS_EResult res = EOS_TitleStorage_CopyFileMetadataByFilename(s_titleStorageInterface, &options, &outMetadata);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["metadata"] = eosg_titlestorage_file_metadata_to_dict_and_release(outMetadata);
    return ret;
}

int IEOS::titlestorage_interface_get_file_metadata_count(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_titleStorageInterface, 0);
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_TitleStorage_GetFileMetadataCountOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_GETFILEMETADATACOUNT_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());

    return static_cast<uint32_t>(EOS_TitleStorage_GetFileMetadataCount(s_titleStorageInterface, &options));
}

Dictionary IEOS::titlestorage_interface_copy_file_metadata_at_index(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_titleStorageInterface, {});
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    int index = p_options->get("index");

    EOS_TitleStorage_CopyFileMetadataAtIndexOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_COPYFILEMETADATAATINDEX_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Index = static_cast<uint32_t>(index);

    EOS_TitleStorage_FileMetadata *outMetadata = nullptr;
    EOS_EResult res = EOS_TitleStorage_CopyFileMetadataAtIndex(s_titleStorageInterface, &options, &outMetadata);

    Dictionary ret;
    ret["result_code"] = static_cast<int>(res);
    ret["metadata"] = eosg_titlestorage_file_metadata_to_dict_and_release(outMetadata);
    return ret;
}

int IEOS::titlestorage_interface_delete_cache(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_titleStorageInterface, static_cast<int>(EOS_EResult::EOS_InvalidState));
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_TitleStorage_DeleteCacheOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_DELETECACHE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_EResult res = EOS_TitleStorage_DeleteCache(s_titleStorageInterface, &options, (void *)*p_options, [](const EOS_TitleStorage_DeleteCacheCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("titlestorage_interface_delete_cache_callback", ret);
    });

    return static_cast<int>(res);
}

Variant IEOS::titlestorage_interface_read_file(Ref<RefCounted> p_options) {
    ERR_FAIL_NULL_V(s_titleStorageInterface, Variant());
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));
    int read_chunk_length_bytes = p_options->get("read_chunk_length_bytes");

    EOS_TitleStorage_ReadFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_TITLESTORAGE_READFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();
    options.ReadChunkLengthBytes = static_cast<uint32_t>(read_chunk_length_bytes);
    options.ReadFileDataCallback = [](const EOS_TitleStorage_ReadFileDataCallbackInfo *data) {
        Dictionary ret;
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        ret["total_file_size_bytes"] = static_cast<int>(data->TotalFileSizeBytes);
        ret["is_last_chunk"] = EOSG_GET_BOOL(data->bIsLastChunk);
        ret["data_chunk_length_bytes"] = static_cast<int>(data->DataChunkLengthBytes);

        // Copy data into a PackedByteArray
        PackedByteArray data_chunk;
        data_chunk.resize(data->DataChunkLengthBytes);
        memcpy(data_chunk.ptrw(), data->DataChunk, data->DataChunkLengthBytes);
        ret["data_chunk"] = data_chunk;

        IEOS::get_singleton()->emit_signal("titlestorage_interface_read_file_data_callback", ret);
        // TODO: emit the signal on the EOSGFileTransferRequest object instead of IEOS
        return EOS_TitleStorage_EReadResult::EOS_TS_RR_ContinueReading;
    };
    options.FileTransferProgressCallback = [](const EOS_TitleStorage_FileTransferProgressCallbackInfo *data) {
        Dictionary ret;
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        ret["bytes_transferred"] = static_cast<int>(data->BytesTransferred);
        ret["total_file_size_bytes"] = static_cast<int>(data->TotalFileSizeBytes);
        IEOS::get_singleton()->emit_signal("titlestorage_interface_file_transfer_progress_callback", ret);
        // TODO: emit the signal on the EOSGFileTransferRequest object instead of IEOS
    };
    p_options->reference();

    EOS_HTitleStorageFileTransferRequest fileTranferRequest = EOS_TitleStorage_ReadFile(s_titleStorageInterface, &options, (void *)*p_options, [](const EOS_TitleStorage_ReadFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        IEOS::get_singleton()->emit_signal("titlestorage_interface_read_file_callback", ret);
    });

    return eosg_titlestorage_file_tranfer_request_to_wrapper(fileTranferRequest);
}

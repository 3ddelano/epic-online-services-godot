#include "godot_cpp/variant/packed_byte_array.hpp"
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

void IEOS::playerdatastorage_interface_delete_cache(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));

    EOS_PlayerDataStorage_DeleteCacheOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_DELETECACHE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    p_options->reference();

    EOS_PlayerDataStorage_DeleteCache(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_DeleteCacheCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_delete_cache_callback", ret);
    });
}

Variant IEOS::playerdatastorage_interface_read_file(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));
    int read_chunk_length_bytes = p_options->get("read_chunk_length_bytes");

    EOS_PlayerDataStorage_ReadFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_READFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();
    options.ReadChunkLengthBytes = static_cast<uint32_t>(read_chunk_length_bytes);
    options.ReadFileDataCallback = [](const EOS_PlayerDataStorage_ReadFileDataCallbackInfo *data) {
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

        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_read_file_data_callback", ret);
        // TODO: emit the signal on the EOSGFileTransferRequest object instead of IEOS
        return EOS_PlayerDataStorage_EReadResult::EOS_RR_ContinueReading;
    };
    options.FileTransferProgressCallback = [](const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo *data) {
        Dictionary ret;
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        ret["bytes_transferred"] = static_cast<int>(data->BytesTransferred);
        ret["total_file_size_bytes"] = static_cast<int>(data->TotalFileSizeBytes);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_file_transfer_progress_callback", ret);
        // TODO: emit the signal on the EOSGFileTransferRequest object instead of IEOS
    };
    p_options->reference();

    EOS_HPlayerDataStorageFileTransferRequest fileTranferRequest = EOS_PlayerDataStorage_ReadFile(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_ReadFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_read_file_callback", ret);
    });

    return eosg_playerdatastorage_file_tranfer_request_to_wrapper(fileTranferRequest);
}

Variant IEOS::playerdatastorage_interface_write_file(Ref<RefCounted> p_options) {
    CharString local_user_id = VARIANT_TO_CHARSTRING(p_options->get("local_user_id"));
    CharString filename = VARIANT_TO_CHARSTRING(p_options->get("filename"));
    PackedByteArray data = p_options->get("data");
    int chunk_length_bytes = p_options->get("chunk_length_bytes");

    EOS_PlayerDataStorage_WriteFileOptions options;
    memset(&options, 0, sizeof(options));
    options.ApiVersion = EOS_PLAYERDATASTORAGE_WRITEFILE_API_LATEST;
    options.LocalUserId = eosg_string_to_product_user_id(local_user_id.get_data());
    options.Filename = filename.get_data();
    options.ChunkLengthBytes = static_cast<uint32_t>(chunk_length_bytes);
    // has the signature

    options.WriteFileDataCallback = [](const EOS_PlayerDataStorage_WriteFileDataCallbackInfo *data, void *outDataBuffer, uint32_t *bytesWritten) {
        Dictionary ret;
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        ret["data_buffer_length_bytes"] = static_cast<int>(data->DataBufferLengthBytes);

        int written_buffer_length_bytes = 0;
        if (client_data->get("written_buffer_length_bytes")) {
            written_buffer_length_bytes = client_data->get("written_buffer_length_bytes");
        }

        PackedByteArray data_buffer = client_data->get("data");
        int chunk_length_bytes = client_data->get("chunk_length_bytes");

        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_write_file_data_callback", ret);

        uint32_t bytes_to_write = std::min((uint32_t)chunk_length_bytes, (uint32_t)(data_buffer.size() - written_buffer_length_bytes));
        if (bytes_to_write > 0) {
            memcpy(outDataBuffer, data_buffer.ptr() + written_buffer_length_bytes, bytes_to_write);
            *bytesWritten = static_cast<uint32_t>(bytes_to_write);
            written_buffer_length_bytes += bytes_to_write;
            client_data->set("written_buffer_length_bytes", written_buffer_length_bytes);

            if (written_buffer_length_bytes == data_buffer.size()) {
                return EOS_PlayerDataStorage_EWriteResult::EOS_WR_CompleteRequest;
            }
            return EOS_PlayerDataStorage_EWriteResult::EOS_WR_ContinueWriting;
        }
        return EOS_PlayerDataStorage_EWriteResult::EOS_WR_CompleteRequest;
    };
    options.FileTransferProgressCallback = [](const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo *data) {
        Dictionary ret;
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        ret["bytes_transferred"] = static_cast<int>(data->BytesTransferred);
        ret["total_file_size_bytes"] = static_cast<int>(data->TotalFileSizeBytes);
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_file_transfer_progress_callback", ret);
        // TODO: emit the signal on the EOSGFileTransferRequest object instead of IEOS
    };
    p_options->reference();

    EOS_HPlayerDataStorageFileTransferRequest fileTranferRequest = EOS_PlayerDataStorage_WriteFile(s_playerDataStorageInterface, &options, (void *)*p_options, [](const EOS_PlayerDataStorage_WriteFileCallbackInfo *data) {
        Dictionary ret;
        ret["result_code"] = static_cast<int>(data->ResultCode);
        Ref<RefCounted> client_data = reinterpret_cast<RefCounted *>(data->ClientData);
        client_data->unreference();
        ret["client_data"] = client_data->get("client_data");
        ret["local_user_id"] = eosg_product_user_id_to_string(data->LocalUserId);
        ret["filename"] = data->Filename;
        IEOS::get_singleton()->emit_signal("playerdatastorage_interface_write_file_callback", ret);
    });

    return eosg_playerdatastorage_file_tranfer_request_to_wrapper(fileTranferRequest);
}
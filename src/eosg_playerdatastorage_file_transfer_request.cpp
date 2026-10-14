#include "eosg_playerdatastorage_file_transfer_request.h"
#include "eos_playerdatastorage.h"
#include "utils.h"

using namespace godot;

void EOSGPlayerDataStorageFileTransferRequest::_bind_methods() {
    ClassDB::bind_method(D_METHOD("cancel_request"), &EOSGPlayerDataStorageFileTransferRequest::cancel_request);
    ClassDB::bind_method(D_METHOD("get_file_request_state"), &EOSGPlayerDataStorageFileTransferRequest::get_file_request_state);
    ClassDB::bind_method(D_METHOD("get_filename"), &EOSGPlayerDataStorageFileTransferRequest::get_filename);

    ADD_SIGNAL(MethodInfo("file_transfer_progress_callback", PropertyInfo(Variant::DICTIONARY, "callback_data")));
}

int EOSGPlayerDataStorageFileTransferRequest::get_file_request_state() {
    ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    return static_cast<int>(EOS_PlayerDataStorageFileTransferRequest_GetFileRequestState(m_internal));
}

Dictionary EOSGPlayerDataStorageFileTransferRequest::get_filename() {
    ERR_FAIL_NULL_V(m_internal, {});
    Dictionary ret;
    if (m_internal == nullptr) {
        ret["result_code"] = static_cast<int>(EOS_EResult::EOS_InvalidState);
        ret["filename"] = "";
        return ret;
    }
    char outBuffer[EOS_PLAYERDATASTORAGE_FILENAME_MAX_LENGTH_BYTES + 1] = {};
    int32_t outBufferLength = 0;
    EOS_EResult result = EOS_PlayerDataStorageFileTransferRequest_GetFilename(m_internal, sizeof(outBuffer), outBuffer, &outBufferLength);
    ret["result_code"] = static_cast<int>(result);
    ret["filename"] = EOSG_GET_STRING(outBuffer);
    return ret;
}

int EOSGPlayerDataStorageFileTransferRequest::cancel_request() {
    ERR_FAIL_NULL_V(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState));
    return static_cast<int>(EOS_PlayerDataStorageFileTransferRequest_CancelRequest(m_internal));
}
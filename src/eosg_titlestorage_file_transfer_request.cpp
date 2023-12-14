#include "eosg_titlestorage_file_transfer_request.h"
#include "eos_titlestorage.h"
#include "utils.h"

using namespace godot;

void EOSGTitleStorageFileTransferRequest::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_file_request_state"), &EOSGTitleStorageFileTransferRequest::get_file_request_state);
    ClassDB::bind_method(D_METHOD("get_filename"), &EOSGTitleStorageFileTransferRequest::get_filename);
    ClassDB::bind_method(D_METHOD("cancel_request"), &EOSGTitleStorageFileTransferRequest::cancel_request);

    ADD_SIGNAL(MethodInfo("file_transfer_progress_callback", PropertyInfo(Variant::DICTIONARY, "callback_data")));
}

int EOSGTitleStorageFileTransferRequest::get_file_request_state() {
    ERR_FAIL_NULL_V_MSG(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState), "The object has not been initialized by EOS.");
    return static_cast<int>(EOS_TitleStorageFileTransferRequest_GetFileRequestState(m_internal));
}

Dictionary EOSGTitleStorageFileTransferRequest::get_filename() {
    Dictionary ret;
    if (m_internal == nullptr) {
        ret["result_code"] = static_cast<int>(EOS_EResult::EOS_InvalidState);
        ret["filename"] = "";
        return ret;
    }
    char *outBuffer = (char *)(memalloc(EOS_TITLESTORAGE_FILENAME_MAX_LENGTH_BYTES + 1));
    int outBufferLength = EOS_TITLESTORAGE_FILENAME_MAX_LENGTH_BYTES + 1;
    EOS_EResult result = EOS_TitleStorageFileTransferRequest_GetFilename(m_internal, EOS_TITLESTORAGE_FILENAME_MAX_LENGTH_BYTES + 1, outBuffer, &outBufferLength);
    ret["result_code"] = static_cast<int>(result);
    ret["filename"] = EOSG_GET_STRING(outBuffer);
    memfree(outBuffer);
    return ret;
}

int EOSGTitleStorageFileTransferRequest::cancel_request() {
    ERR_FAIL_NULL_V_MSG(m_internal, static_cast<int>(EOS_EResult::EOS_InvalidState), "The object has not been initialized by EOS.");
    return static_cast<int>(EOS_TitleStorageFileTransferRequest_CancelRequest(m_internal));
}
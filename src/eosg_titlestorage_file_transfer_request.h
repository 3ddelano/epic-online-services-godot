#pragma once
#include "eos_titlestorage.h"
#include "eosg_file_transfer_request.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGTitleStorageFileTransferRequest : public EOSGFileTransferRequest {
    GDCLASS(EOSGTitleStorageFileTransferRequest, EOSGFileTransferRequest)

private:
    EOS_HTitleStorageFileTransferRequest m_internal = nullptr;
    static void _bind_methods();

public:
    Dictionary get_filename() override;
    int cancel_request() override;
    int get_file_request_state() override;

    EOSGTitleStorageFileTransferRequest(){};
    ~EOSGTitleStorageFileTransferRequest() {
        if (m_internal != nullptr) {
            EOS_TitleStorageFileTransferRequest_Release(m_internal);
			m_internal = nullptr;
        }
    }

    void set_internal(EOS_HTitleStorageFileTransferRequest p_internal) {
        m_internal = p_internal;
    }

    EOS_HTitleStorageFileTransferRequest get_internal() {
        return m_internal;
    }
};

} //namespace godot

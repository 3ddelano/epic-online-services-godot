#pragma once
#include "eos_playerdatastorage.h"
#include "eosg_file_transfer_request.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGPlayerDataStorageFileTransferRequest : public EOSGFileTransferRequest {
    GDCLASS(EOSGPlayerDataStorageFileTransferRequest, EOSGFileTransferRequest)

private:
    EOS_HPlayerDataStorageFileTransferRequest m_internal = nullptr;
    static void _bind_methods();

public:
    Dictionary get_filename() override;
    int cancel_request() override;
    int get_file_request_state() override;

    EOSGPlayerDataStorageFileTransferRequest(){};
    ~EOSGPlayerDataStorageFileTransferRequest() {
        if (m_internal != nullptr) {
            EOS_PlayerDataStorageFileTransferRequest_Release(m_internal);
			m_internal = nullptr;
        }
    }

    void set_internal(EOS_HPlayerDataStorageFileTransferRequest p_internal) {
        m_internal = p_internal;
    }

    EOS_HPlayerDataStorageFileTransferRequest get_internal() {
        return m_internal;
    }
};

} //namespace godot

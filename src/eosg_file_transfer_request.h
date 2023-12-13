#pragma once
#include "eos_playerdatastorage.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGFileTransferRequest : public RefCounted {
    GDCLASS(EOSGFileTransferRequest, RefCounted)

private:
    EOS_HPlayerDataStorageFileTransferRequest m_internal = nullptr;
    static void _bind_methods();

public:
    int get_file_request_state();
    Dictionary get_filename();
    int cancel_request();

    EOSGFileTransferRequest(){};
    ~EOSGFileTransferRequest() {
        if (m_internal != nullptr) {
            EOS_PlayerDataStorageFileTransferRequest_Release(m_internal);
        }
    };

    void set_internal(EOS_HPlayerDataStorageFileTransferRequest p_internal) {
        m_internal = p_internal;
    }

    EOS_HPlayerDataStorageFileTransferRequest get_internal() {
        return m_internal;
    }
};
} // namespace godot

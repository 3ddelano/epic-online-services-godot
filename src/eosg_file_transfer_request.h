#pragma once
#include "eosg_file_transfer_request.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGFileTransferRequest : public RefCounted {
    GDCLASS(EOSGFileTransferRequest, RefCounted)

private:
    static void _bind_methods() {
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, get_file_request_state);
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, get_filename);
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, cancel_request);
    };

public:
    // TODO: make these methods pure virtual
    virtual int get_file_request_state() {
        return -1;
    }
    virtual Dictionary get_filename() {
        return Dictionary();
    }
    virtual int cancel_request() {
        return -1;
    }

    EOSGFileTransferRequest(){};
    ~EOSGFileTransferRequest(){};
};
} // namespace godot

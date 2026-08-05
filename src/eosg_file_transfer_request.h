#pragma once
#include "eosg_file_transfer_request.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGFileTransferRequest : public RefCounted {
    GDCLASS(EOSGFileTransferRequest, RefCounted)

private:
    static void _bind_methods() {
// godot-cpp >= 4.4 added a hash argument to BIND_VIRTUAL_METHOD.
// With older godot-cpp (or no version define) this falls back to the 2-arg form.
#if GODOT_CPP_VERSION_MAJOR > 4 || (GODOT_CPP_VERSION_MAJOR == 4 && GODOT_CPP_VERSION_MINOR >= 4)
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, get_file_request_state, 0);
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, get_filename, 0);
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, cancel_request, 0);
#else
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, get_file_request_state);
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, get_filename);
        BIND_VIRTUAL_METHOD(EOSGFileTransferRequest, cancel_request);
#endif
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

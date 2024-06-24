#pragma once
#include "eos_common.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGContinuanceToken : public RefCounted {
    GDCLASS(EOSGContinuanceToken, RefCounted)

private:
    EOS_ContinuanceToken m_internal = nullptr;
    static void _bind_methods(){};

public:
    EOSGContinuanceToken(){};
    ~EOSGContinuanceToken(){};

    void set_internal(EOS_ContinuanceToken p_internal) {
        m_internal = p_internal;
    }

    EOS_ContinuanceToken get_internal() {
        return m_internal;
    }

    String to_string() {
        int32_t outBufferLength = 0;
        EOS_ContinuanceToken_ToString(m_internal, nullptr, &outBufferLength);

        // outBufferLength is set to required length
        char *outBuffer = (char *)(memalloc(outBufferLength + 1));

        EOS_EResult res = EOS_ContinuanceToken_ToString(m_internal, outBuffer, &outBufferLength);

        if (res == EOS_EResult::EOS_Success) {
            return String("ContinuanceToken[") + String(outBuffer) + String("]");
        }

        return "ContinuanceToken[to_string failed]";
    }
};
} // namespace godot

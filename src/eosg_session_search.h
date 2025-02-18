#pragma once
#include "eos_sessions.h"
#include "godot_cpp/classes/ref_counted.hpp"

namespace godot {

class EOSGSessionSearch : public RefCounted {
    GDCLASS(EOSGSessionSearch, RefCounted)

private:
    EOS_HSessionSearch m_internal = nullptr;
    static void _bind_methods();

public:
    Dictionary copy_search_result_by_index(int index);
    int get_search_result_count();
    int remove_parameter(const String &key, int comparison_op);
    int set_max_results(int max_results);
    int set_parameter(const String &key, Variant value, int comparison_op);
    int set_session_id(const String &session_id);
    int set_target_user_id(const String &target_user_id);
    void find(const String &local_user_id);

    EOSGSessionSearch(){};
    ~EOSGSessionSearch() {
        if (m_internal != nullptr) {
            EOS_SessionSearch_Release(m_internal);
			m_internal = nullptr;
        }
    };

    void set_internal(EOS_HSessionSearch p_internal) {
        m_internal = p_internal;
    }

    EOS_HSessionSearch get_internal() {
        return m_internal;
    }
};
} // namespace godot

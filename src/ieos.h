#pragma once

#include "eos_auth.h"
#include "eos_init.h"
#include "eos_logging.h"
#include "eos_sdk.h"
#include "eos_version.h"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/project_settings.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "utils.h"
#ifdef _WIN32
#include "Windows/eos_Windows.h"
#endif

namespace godot {

class IEOS : public RefCounted {
    GDCLASS(IEOS, RefCounted)

    static IEOS* singleton;

   protected:
    static void _bind_methods();
    EOS_HPlatform s_platformInterface = nullptr;
    EOS_HAuth s_authInterface = nullptr;

   public:
    static IEOS*
    get_singleton();

    IEOS();
    ~IEOS();

    static void tick();
    int shutdown();

    // Platform Interface
    bool platform_interface_create(Ref<RefCounted> options);
    Dictionary platform_interface_get_active_country_code(const String& user_id);
    Dictionary platform_interface_get_active_locale_code(const String& user_id);
    Dictionary platform_interface_get_override_country_code();
    Dictionary platform_interface_get_override_locale_code();
    int platform_interface_set_override_country_code(const String& country_code);
    int platform_interface_set_override_locale_code(const String& locale_code);
    int platform_interface_check_for_launcher_and_restart();
    int platform_interface_initialize(Ref<RefCounted> options);
    Dictionary platform_interface_get_desktop_crossplay_status();
    int platform_interface_set_application_status(int status);
    int platform_interface_get_application_status();
    int platform_interface_set_network_status(int status);
    int platform_interface_get_network_status();
    void platform_interface_release();
    int platform_interface_shutdown();

    // Version Interface
    String version_interface_get_version();
    Dictionary version_interface_get_constants();
};

}  // namespace godot

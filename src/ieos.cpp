#include "ieos.h"

using namespace godot;

#define IEOS_BIND_METHOD(m_name) ClassDB::bind_method(D_METHOD(#m_name), &IEOS::m_name)

void IEOS::_bind_methods() {
    ClassDB::bind_static_method("IEOS", D_METHOD("tick"), &IEOS::tick);

    IEOS_BIND_METHOD(shutdown);

    // Platform Interface
    IEOS_BIND_METHOD(platform_interface_create);
    IEOS_BIND_METHOD(platform_interface_get_active_country_code);
    IEOS_BIND_METHOD(platform_interface_get_active_locale_code);
    IEOS_BIND_METHOD(platform_interface_get_override_country_code);
    IEOS_BIND_METHOD(platform_interface_get_override_locale_code);
    IEOS_BIND_METHOD(platform_interface_set_override_country_code);
    IEOS_BIND_METHOD(platform_interface_set_override_locale_code);
    IEOS_BIND_METHOD(platform_interface_check_for_launcher_and_restart);
    IEOS_BIND_METHOD(platform_interface_initialize);
    IEOS_BIND_METHOD(platform_interface_get_desktop_crossplay_status);
    IEOS_BIND_METHOD(platform_interface_set_application_status);
    IEOS_BIND_METHOD(platform_interface_get_application_status);
    IEOS_BIND_METHOD(platform_interface_set_network_status);
    IEOS_BIND_METHOD(platform_interface_get_network_status);
    IEOS_BIND_METHOD(platform_interface_release);
    IEOS_BIND_METHOD(platform_interface_shutdown);

    // Version Interface
    IEOS_BIND_METHOD(version_interface_get_version);
    IEOS_BIND_METHOD(version_interface_get_constants);

    ADD_SIGNAL(MethodInfo("logging_interface_callback",
                          PropertyInfo(Variant::DICTIONARY, "log_message")));
}

int IEOS::shutdown() {
    EOS_EResult result = EOS_Shutdown();
    return static_cast<int>(result);
}

// ----------

IEOS *IEOS::singleton = nullptr;

IEOS *IEOS::get_singleton() { return singleton; }

IEOS::IEOS() {
    // Initialize any variables here
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;
}

IEOS::~IEOS() {
    // Add your cleanup here
    ERR_FAIL_COND(singleton != this);
    if (s_platformInterface != nullptr) {
        EOS_Platform_Release(s_platformInterface);
    }
    singleton = nullptr;
}

void IEOS::tick() {
    if (IEOS::get_singleton()->s_platformInterface != nullptr) {
        EOS_Platform_Tick(IEOS::get_singleton()->s_platformInterface);
    }
}
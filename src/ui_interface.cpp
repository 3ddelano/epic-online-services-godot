#include "ieos.h"
using namespace godot;

int IEOS::ui_interface_acknowledge_event_id(Ref<RefCounted> options) {
    return 1;
}

bool IEOS::ui_interface_get_friends_visible(Ref<RefCounted> options) {
    return false;
}

int IEOS::ui_interface_get_notification_location_preference() {
    return 1;
}

int IEOS::ui_interface_get_toggle_friends_key(Ref<RefCounted> options) {
    return 1;
}

void IEOS::ui_interface_hide_friends(Ref<RefCounted> options) {
    return;
}

bool IEOS::ui_interface_is_valid_key_combination(int key_combination) {
    return false;
}

int IEOS::ui_interface_set_display_preference(Ref<RefCounted> options) {
    return 1;
}

int IEOS::ui_interface_set_toggle_friends_key(Ref<RefCounted> options) {
    return 1;
}

void IEOS::ui_interface_show_friends(Ref<RefCounted> options) {
    return;
}

#include "ieos.h"
using namespace std;

Dictionary IEOS::presence_interface_copy_presence(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::presence_interface_create_presence_modification(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::presence_interface_get_join_info(Ref<RefCounted> options) {
    return Dictionary();
}

bool IEOS::presence_interface_has_presence(Ref<RefCounted> options) {
    return true;
}

void IEOS::presence_interface_query_presence(Ref<RefCounted> options) {
    return;
}

void IEOS::presence_interface_set_presence(Ref<RefCounted> options) {
    return;
}

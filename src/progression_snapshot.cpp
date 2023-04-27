#include "ieos.h"
using namespace std;

Dictionary IEOS::progression_snapshot_interface_begin_snapshot(Ref<RefCounted> options) {
    return Dictionary();
}

int IEOS::progression_snapshot_interface_add_progression(Ref<RefCounted> options) {
    return 1;
}

void IEOS::progression_snapshot_interface_submit_snapshot(Ref<RefCounted> options) {
    return;
}

void IEOS::progression_snapshot_interface_delete_snapshot(Ref<RefCounted> options) {
    return;
}

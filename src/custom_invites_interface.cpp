#include "ieos.h"
using namespace std;

int IEOS::custom_invites_interface_finalize_invite(Ref<RefCounted> options) {
    return 1;
}

void IEOS::custom_invites_interface_send_custom_invite(Ref<RefCounted> options) {
    return;
}

int IEOS::custom_invites_interface_set_custom_invite(Ref<RefCounted> options) {
    return 1;
}

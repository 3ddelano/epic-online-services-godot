#include "ieos.h"
using namespace std;

void IEOS::friends_interface_accept_invite(Ref<RefCounted> options) {
    return;
}

String IEOS::friends_interface_get_friend_at_index(Ref<RefCounted> options) {
    return "a";
}

int IEOS::friends_interface_get_friends_count(Ref<RefCounted> options) {
    return 1;
}

int IEOS::friends_interface_get_status(Ref<RefCounted> options) {
    return 1;
}

void IEOS::friends_interface_query_friends(Ref<RefCounted> options) {
    return;
}

void IEOS::friends_interface_reject_invite(Ref<RefCounted> options) {
    return;
}

void IEOS::friends_interface_send_invite(Ref<RefCounted> options) {
    return;
}

#include "ieos.h"
using namespace std;

int IEOS::metrics_interface_begin_player_session(Ref<RefCounted> options) {
    return 1;
}

int IEOS::metrics_interface_end_player_session(Ref<RefCounted> options) {
    return 1;
}

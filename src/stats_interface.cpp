#include "ieos.h"
using namespace std;

Dictionary IEOS::stats_interface_copy_stat_by_index(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::stats_interface_copy_stat_by_name(Ref<RefCounted> options) {
    return Dictionary();
}

int IEOS::stats_interface_get_stats_count(Ref<RefCounted> options) {
    return 1;
}

void IEOS::stats_interface_ingest_stat(Ref<RefCounted> options) {
    return;
}

void IEOS::stats_interface_query_stats(Ref<RefCounted> options) {
    return;
}

#include "ieos.h"
using namespace godot;

int IEOS::logging_interface_set_log_level(int category, int level) {
    EOS_EResult result = EOS_Logging_SetLogLevel(static_cast<EOS_ELogCategory>(category), static_cast<EOS_ELogLevel>(level));
    return static_cast<int>(result);
}

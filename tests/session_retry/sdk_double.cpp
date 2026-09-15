// Deterministic SDK callback delivery; never contacts Epic services.
#include "eos_sdk.h"
#include "eos_sessions.h"
#include <cstdio>
#include <cstdlib>

static EOS_Sessions_OnUpdateSessionCallback callback;
static void *client_data;
static int tick_index;
static int platform_token, sessions_token, modification_token;

EOS_HPlatform EOS_CALL EOS_Platform_Create(const EOS_Platform_Options *) {
    return reinterpret_cast<EOS_HPlatform>(&platform_token);
}
EOS_HSessions EOS_CALL EOS_Platform_GetSessionsInterface(EOS_HPlatform) {
    return reinterpret_cast<EOS_HSessions>(&sessions_token);
}
EOS_EResult EOS_CALL EOS_Sessions_CreateSessionModification(EOS_HSessions,
        const EOS_Sessions_CreateSessionModificationOptions *, EOS_HSessionModification *out) {
    *out = reinterpret_cast<EOS_HSessionModification>(&modification_token);
    return EOS_EResult::EOS_Success;
}
void EOS_CALL EOS_Sessions_UpdateSession(EOS_HSessions,
        const EOS_Sessions_UpdateSessionOptions *, void *data,
        EOS_Sessions_OnUpdateSessionCallback completion) {
    client_data = data;
    callback = completion;
    tick_index = 0;
}
void EOS_CALL EOS_Platform_Tick(EOS_HPlatform) {
    if (!callback) {
        return;
    }
    const int retries = std::atoi(std::getenv("EOSG_TEST_RETRIES"));
    const bool complete = tick_index++ >= retries;
    const EOS_EResult result = complete ? static_cast<EOS_EResult>(
                                                  std::atoi(std::getenv("EOSG_TEST_RESULT")))
                                        : EOS_EResult::EOS_OperationWillRetry;
    EOS_Sessions_UpdateSessionCallbackInfo info{};
    info.ResultCode = result;
    info.ClientData = client_data;
    info.SessionName = "Issue31";
    info.SessionId = result == EOS_EResult::EOS_Success ? "test-session-id" : nullptr;
    std::printf("SDK double: callback %d result=%d\n", tick_index, static_cast<int>(result));
    std::fflush(stdout);
    const auto invoke = callback;
    if (complete) {
        callback = nullptr;
    }
    invoke(&info);
}

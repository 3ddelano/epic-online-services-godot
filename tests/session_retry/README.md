# Session update retry regression (#31)

This test runs the **compiled EOSG extension inside a real Godot process**.
A test-only replacement SDK delivers session-update callbacks deterministically
from `EOS_Platform_Tick`. It exercises the public native update method, its actual
C++ callback, Godot reference counting, and the emitted GDScript signal.

It does **not** reproduce an Epic backend outage or HTTP 503. Platform and session
setup are test doubles; no credentials or network calls are needed. The real SDK
is re-exported for utility functions, including `EOS_EResult_IsOperationComplete`.
Only a temporary copy of the extension framework receives the SDK replacement.

## Run (macOS)

Requires the EOS SDK, Xcode command-line tools, Python 3, and a built extension:

```sh
scons dev_build=yes
python3 tests/session_retry/run.py --godot /Applications/godot43.app/Contents/MacOS/Godot
```

The script drops its strong reference to the update options before the first
callback. Each retry must preserve the options and deliver result 19 with the
original client data. The final callback must release the options, preserve the
payload, and emit exactly once. In headless mode, exit status is nonzero on a failed assertion,
engine error, missing PASS marker, crash, or timeout.

Additional cases:

```sh
# Retry then terminal failure (EOS_NoConnection).
python3 tests/session_retry/run.py --godot /Applications/godot43.app/Contents/MacOS/Godot --result 1
# Direct success, direct failure, and cancellation.
python3 tests/session_retry/run.py --godot /Applications/godot43.app/Contents/MacOS/Godot --retries 0
python3 tests/session_retry/run.py --godot /Applications/godot43.app/Contents/MacOS/Godot --retries 0 --result 1
python3 tests/session_retry/run.py --godot /Applications/godot43.app/Contents/MacOS/Godot --result 17
# Keep the actual test output visible in a Godot window for a screenshot.
python3 tests/session_retry/run.py --godot /Applications/godot43.app/Contents/MacOS/Godot --show
```

For debugging the original crash, `--unsafe-replay` skips the early lifetime
assertion and lets the next callback access the released options. Run that mode
only in this disposable test project. The normal regression fails cleanly before
accessing freed memory.

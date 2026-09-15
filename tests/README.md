# CharString compatibility regression (#76)

The failure is a **source compatibility** issue. Recent godot-cpp removed
`CharString`'s implicit conversion to `const char *`. Changing the Godot executable
alone does not reproduce a compiler error in an already-built extension.

## Compile the affected call sites

With the EOS SDK installed as described in the repository instructions:

```sh
./tests/check_charstring_compatibility.sh
```

For the regression to detect #76, use godot-cpp without the implicit conversion.
The tested revision is `6cceaf6a5f8b0d78ac5d71c139fd7fabba43b918` (API 4.7).
In a disposable checkout, fetch and select that revision in `godot-cpp`.
That revision requires `api_version` and renamed the API JSON files. This
repository's SConstruct still reads `extension_api.json`, so provide a local
symlink for the test (do not commit a submodule update):

```sh
git -C godot-cpp fetch https://github.com/godotengine/godot-cpp.git master
git -C godot-cpp checkout --detach 6cceaf6a5f8b0d78ac5d71c139fd7fabba43b918
ln -s extension_api-4-7.json godot-cpp/gdextension/extension_api.json
./tests/check_charstring_compatibility.sh api_version=4.7
scons dev_build=yes api_version=4.7
```

The focused check compiles the actual anti-cheat and P2P translation units,
including the socket constructor. It fails on the original code and passes with
the explicit conversions. Existing unrelated compiler warnings may remain.

The helper in `utils.h` continues to accept `const char *`. Callers extract the
buffer explicitly; the catalog path retains its owning `CharString` throughout
the EOS call instead of storing a pointer into a destroyed temporary.

## Runtime smoke test

Build normally with the repository's pinned godot-cpp 4.2 revision:

```sh
scons dev_build=yes
python3 tests/run_runtime_smoke.py /Applications/godot42.app/Contents/MacOS/Godot
python3 tests/run_runtime_smoke.py /Applications/godot47.app/Contents/MacOS/Godot
```

The runner creates a temporary project containing only the extension descriptor,
a link to the built binaries, and the smoke script. It does not load the sample
project or credentials. It checks native singleton/class registration, peer
construction, and real EOS SDK initialization/shutdown. A nonzero exit, failed
check, or engine error fails the run. Pass `--show` to leave the actual test
results visible in a Godot window for a screenshot; close the window afterward.

These are offline loading/initialization checks, not online login, anti-cheat
service, or multiplayer sessions. The compiler regression covers the changed
call sites. Use the 4.2-built binary for the 4.2–4.7 engine matrix; use the
API-4.7-built binary only with Godot 4.7.

## Session update retry regression (#31)

See [session_retry/README.md](session_retry/README.md) for the deterministic
Godot integration test and reproduction instructions.

## Inferred declaration warnings (#69 follow-up)

Build the extension, then run the addon parser and startup check:

```sh
scons dev_build=yes
python3 tests/check_inferred_declarations.py /Applications/godot47.app/Contents/MacOS/Godot
```

The runner copies tracked addon files from the working tree to a temporary
project, enables `debug/gdscript/warnings/inferred_declaration=2`, and includes
addon scripts in warning checks. It imports the scripts, then starts a minimal
scene and verifies that all ten plugin autoloads are present. It never loads the
sample project or credentials. A nonzero process exit, engine/script error, or
missing startup success marker fails the check.

The test disables `native_method_override` because the existing
`Dataclass.get_class()` override independently triggers that warning when addons
are included. This check targets inferred static declarations; dynamic
(untyped) declarations have a separate warning setting.

Godot 4.2 receives an initial editor pass to populate its global script class
cache. On the installed 4.2 and 4.4 builds, pre-existing resource-cleanup and
editor-progress errors respectively still cause the full check to fail even
when script parsing and autoload startup succeed. These diagnostics are printed
and retained in the verdict rather than silently ignored.

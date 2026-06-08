# EOSG — Epic Online Services Godot

# Task completion requirement

1. `scons dev_build=yes` should pass

## Project structure

```
src/                              # C++ GDExtension (wraps EOS C SDK)
  ieos.h / ieos.cpp               #   IEOS singleton — core bridge to EOS C SDK
  register_types.cpp              #   GDExtension entrypoint, registers IEOS + other classes
  *interface.cpp                  #   One file per EOS interface (auth, lobby, p2p, etc.)
  eosg_multiplayer_peer.cpp       #   MultiplayerPeer extension for P2P
  eosg_packet_peer_mediator.cpp   #   Internal mediator for P2P packet routing
sample/
  addons/epic-online-services-godot/  # Godot plugin directory
    eos.gd                        #   Low-level EOS.* API (typed Options classes → IEOS calls)
    base_class.gd / dataclass.gd  #   Options boilerplate (Dataclass pattern)
    runtime.gd                    #   EOSGRuntime autoload — calls IEOS.tick() each frame
    heos/                         #   High-level H* API (HPlatform, HAuth, etc.)
    eosg.gdextension              #   GDExtension config (library paths per platform)
    export_plugin.gd              #   Packs EOS SDK shared libs on export
    plugin.gd                     #   EditorPlugin — registers all autoloads
  project.godot                   # Sample Godot project
docs/                             # Docusaurus website (separate from plugin)
thirdparty/eos-sdk/               # EOS C SDK submodule (private mirror)
godot-cpp/                        # godot-cpp submodule (branch 4.2)
```

## Build

- **Build system**: SCons. Run from repo root.
- **Dev build command**:
  ```
  scons dev_build=yes
  ```
- **iOS** uses a dedicated script: `./build-ios.sh` (builds device + simulator for debug + release)
- **Output**: binaries go to `sample/addons/epic-online-services-godot/bin/<platform>/`
- **CI** (`.github/workflows/build.yml`): builds all platforms via GitHub Actions. Uses a private EOS SDK mirror with a PAT secret (`EOS_SDK_MIRROR_PAT`). Android needs NDK r23c.

## Code style

- **C++**: `.clang-format` — LLVM-based with custom indentation. Pre-commit hook runs `clang-format` v16.
- **GDScript**: tabs for indentation, `#region` / `#endregion` section markers (see `script_template.gd`). No trailing whitespace trim.
- **EditorConfig**: tabs in `*.{h,hpp,cpp,gd}`, spaces elsewhere.

## Architecture notes

- **Two API layers**:
  - **Low-level**: `EOS.Auth.AuthInterface.login(...)` etc. — ~1:1 mapping to EOS C SDK, uses typed Option classes (Dataclass pattern). Results are Dictionaries with `result_code` key and optional data.
  - **High-level** (`heos/`): `HAuth.login_devtool_async(...)` — simpler methods and signals, recommended for beginners.
- **Singletons** (registered via `plugin.gd` as Godot autoloads): EOSGRuntime, HPlatform, HAuth, HAchievements, HFriends, HStats, HLeaderboards, HLobbies, HP2P, HSessions. Also `EOSGRuntime` calls `IEOS.tick()` every frame.
- **C++ singleton** `IEOS` is registered via GDExtension `register_types.cpp`. All GDScript EOS calls go through `IEOS.*`.
- **Async pattern**: EOS SDK uses callbacks. The C++ layer emits Godot signals when callbacks fire. High-level wrappers use `await` / signals.
- **P2P**: `EOSGMultiplayerPeer` extends Godot's `MultiplayerPeer` for EOS P2P. `EOSGPacketPeerMediator` handles routing.
- **Dataclass pattern** (`dataclass.gd` / `base_class.gd`): all Options classes inherit from `Dataclass` (via `BaseClass`), providing `to_dict()`, `from_dict()`, and pretty-printing.

## Testing

No test framework is set up in this repo. The sample project (`sample/`) serves as manual integration testing.

## Common gotchas

- EOS C SDK must be manually extracted to `thirdparty/eos-sdk/SDK/`. It is NOT in the submodule (the submodule is an empty mirror for CI).
- `.env` in `sample/` contains real-looking credentials — they are sample/dev credentials but keep them out of commits.
- macOS debugging needs code-signing: `codesign --entitlements debug-entitlements.plist -f -s - /Applications/Godot.app/Contents/MacOS/Godot`
- The `eosg.gdextension` file references `godot-cpp` xcframeworks on iOS (built by `build-ios.sh`), not in the main repo.
- When adding a new EOS interface, you need to touch: C++ interface file in `src/`, registration in `register_types.cpp`, GDScript options in `eos.gd`, and optionally a high-level wrapper in `heos/`.

#!/usr/bin/env python3
"""Parse every addon script with INFERRED_DECLARATION promoted to an error."""

import argparse
import json
from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("godot", help="Path to the Godot executable")
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    relative_addon = Path("sample/addons/epic-online-services-godot")
    addon = repo / relative_addon
    tracked = subprocess.check_output(
        ["git", "ls-files", "-z", str(relative_addon)], cwd=repo
    ).decode().split("\0")
    autoloads = ["EOSGRuntime", "HPlatform", "HAuth", "HAchievements", "HFriends",
                 "HStats", "HLeaderboards", "HLobbies", "HP2P", "HSessions"]
    with tempfile.TemporaryDirectory(prefix="eosg-inferred-declarations-") as directory:
        project = Path(directory)
        destination = project / "addons/epic-online-services-godot"
        for name in filter(None, tracked):
            relative = Path(name).relative_to(relative_addon)
            if relative.parts[0] == "bin":
                continue
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(repo / name, target)
        (destination / "bin").symlink_to(addon / "bin", target_is_directory=True)
        config = (
            'config_version=5\n[application]\nconfig/name="EOSG inferred declarations test"\n'
            'run/main_scene="res://startup.tscn"\n[autoload]\n'
        )
        for name in autoloads:
            path = "runtime.gd" if name == "EOSGRuntime" else "heos/" + name.lower() + ".gd"
            config += f'{name}="*res://addons/epic-online-services-godot/{path}"\n'
        config += (
            '[editor_plugins]\nenabled=PackedStringArray("res://addons/epic-online-services-godot/plugin.cfg")\n'
            '[rendering]\nrenderer/rendering_method="gl_compatibility"\n'
            '[debug]\ngdscript/warnings/inferred_declaration=2\n'
            'gdscript/warnings/exclude_addons=false\n'
            'gdscript/warnings/directory_rules={"res://addons": 1}\n'
            # Dataclass.get_class() already overrides Object.get_class(). This
            # regression isolates inferred declarations from that separate warning.
            'gdscript/warnings/native_method_override=0\n'
        )
        (project / "project.godot").write_text(config)
        (project / ".godot").mkdir()
        (project / ".godot/extension_list.cfg").write_text(
            "res://addons/epic-online-services-godot/eosg.gdextension\n"
        )
        (project / "startup.tscn").write_text(
            '[gd_scene load_steps=2 format=3]\n'
            '[ext_resource type="Script" path="res://startup.gd" id="1"]\n'
            '[node name="Startup" type="Node"]\nscript = ExtResource("1")\n'
        )
        (project / "startup.gd").write_text(
            'extends Node\nfunc _ready() -> void:\n'
            f'\tfor singleton_name: String in {json.dumps(autoloads)}:\n'
            '\t\tvar singleton: Node = get_tree().root.get_node_or_null(singleton_name)\n'
            '\t\tif singleton == null or singleton.get_script() == null:\n'
            '\t\t\tpush_error("Missing autoload: " + singleton_name)\n'
            '\t\t\tget_tree().quit(1)\n\t\t\treturn\n'
            '\tprint("ADDON_STARTUP=PASS")\n\tget_tree().quit(0)\n'
        )
        version = subprocess.check_output([args.godot, "--version"], text=True).strip()
        command = [args.godot, "--headless", "--path", str(project), "--editor"]
        command += ["--quit-after", "120"] if version.startswith("4.2.") else ["--import"]
        # Godot 4.2 initially compiles autoloads before discovering global script
        # classes. Let the editor build its class cache before checking diagnostics.
        if version.startswith("4.2."):
            subprocess.run(command, capture_output=True, text=True, timeout=90)
        result = subprocess.run(command, capture_output=True, text=True, timeout=90)
        output = result.stdout + result.stderr
        print(output, end="")
        startup = subprocess.run(
            [args.godot, "--headless", "--path", str(project), "--quit-after", "30"],
            capture_output=True, text=True, timeout=60
        )
        startup_output = startup.stdout + startup.stderr
        print(startup_output, end="")
        passed = (result.returncode == 0 and startup.returncode == 0
                  and "ERROR:" not in output + startup_output
                  and "ADDON_STARTUP=PASS" in startup_output)
        print(f"INFERRED_DECLARATIONS={'PASS' if passed else 'FAIL'} ({version})")
        return 0 if passed else 1


if __name__ == "__main__":
    raise SystemExit(main())

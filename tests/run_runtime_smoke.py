#!/usr/bin/env python3
"""Load the built extension in an isolated, credential-free Godot project."""

import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("godot", help="Path to the Godot executable")
    parser.add_argument("--show", action="store_true", help="Keep a live results window open")
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    addon = repo / "sample/addons/epic-online-services-godot"
    with tempfile.TemporaryDirectory(prefix="eosg-runtime-smoke-") as directory:
        project = Path(directory)
        (project / "project.godot").write_text(
            'config_version=5\n[application]\nconfig/name="EOSG compatibility test"\n'
            '[rendering]\nrenderer/rendering_method="gl_compatibility"\n'
        )
        shutil.copy2(addon / "eosg.gdextension", project / "eosg.gdextension")
        (project / "bin").symlink_to(addon / "bin", target_is_directory=True)
        shutil.copy2(repo / "tests/runtime_smoke.gd", project / "runtime_smoke.gd")
        (project / ".godot").mkdir()
        (project / ".godot/extension_list.cfg").write_text("res://eosg.gdextension\n")
        command = [args.godot, "--path", str(project), "--script", "runtime_smoke.gd"]
        command += ["--", "--show"] if args.show else ["--headless"]
        if args.show:
            return subprocess.call(command)
        result = subprocess.run(command, capture_output=True, text=True, timeout=60)
        print(result.stdout, end="")
        print(result.stderr, end="")
        return 0 if (
            result.returncode == 0
            and "SMOKE_RESULT=PASS" in result.stdout
            and "ERROR:" not in result.stderr
        ) else 1


if __name__ == "__main__":
    raise SystemExit(main())

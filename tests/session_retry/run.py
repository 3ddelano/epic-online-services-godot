#!/usr/bin/env python3
"""Run the real EOSG binding in Godot with deterministic SDK callbacks (macOS)."""
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--godot', required=True, type=Path)
parser.add_argument('--retries', type=int, default=2)
parser.add_argument('--result', type=int, default=0, choices=[0, 1, 17])
parser.add_argument('--show', action='store_true', help='Keep actual results visible in a Godot window')
parser.add_argument('--unsafe-replay', action='store_true', help='Continue after premature destruction to capture the original native crash')
args = parser.parse_args()
if args.retries < 0:
    parser.error('--retries must be nonnegative')
framework = ROOT / 'sample/addons/epic-online-services-godot/bin/macos/libeosg.macos.template_debug.framework'
sdk = ROOT / 'thirdparty/eos-sdk/SDK'
with tempfile.TemporaryDirectory(prefix='eosg-issue31-') as directory:
    project = Path(directory)
    copied = project / framework.name
    shutil.copytree(framework, copied)
    # Only the temporary copy gets a replacement SDK. The real SDK is re-exported
    # for pure helpers, including EOS_EResult_IsOperationComplete.
    real_sdk = project / 'libEOSSDK-real.dylib'
    shutil.copy2(sdk / 'Bin/libEOSSDK-Mac-Shipping.dylib', real_sdk)
    subprocess.run(['install_name_tool', '-id', str(real_sdk), str(real_sdk)], check=True)
    subprocess.run(['codesign', '--force', '--sign', '-', str(real_sdk)], check=True, capture_output=True)
    # Match the installed SDK's declarations so every stub has the exact ABI.
    declarations = {}
    for header in (sdk / 'Include').glob('*.h'):
        for result_type, name, parameters in re.findall(
                r'EOS_DECLARE_FUNC\(([^)]+)\)\s+(EOS_\w+)\(([^;]*?)\);', header.read_text()):
            declarations[name] = (result_type, parameters)
    names = {name for name in declarations if (
        name.startswith('EOS_Platform_Get') and name.endswith('Interface')
        and name != 'EOS_Platform_GetSessionsInterface'
    ) or name.startswith('EOS_Sessions_AddNotify')}
    names.update(['EOS_Platform_Release', 'EOS_SessionModification_Release',
                  'EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer',
                  'EOS_RTC_GetAudioInterface', 'EOS_RTC_GetDataInterface'])
    stubs = project / 'setup_stubs.cpp'
    definitions = ['#include "eos_sdk.h"', '#include "eos_sessions.h"',
                   '#include "eos_rtc.h"', '#include "eos_integratedplatform.h"']
    for name in sorted(names):
        result_type, parameters = declarations[name]
        body = '' if result_type == 'void' else 'return {};'
        definitions.append(f'{result_type} EOS_CALL {name}({parameters}) {{ {body} }}')
    stubs.write_text('\n'.join(definitions) + '\n')
    replacement = copied / 'libEOSSDK-Mac-Shipping.dylib'
    subprocess.run(['clang++', '-dynamiclib', '-std=c++17', '-I' + str(sdk / 'Include'),
                    str(HERE / 'sdk_double.cpp'), str(stubs),
                    '-Wl,-reexport_library,' + str(real_sdk), '-o', str(replacement)], check=True)
    subprocess.run(['codesign', '--force', '--sign', '-', str(replacement)], check=True, capture_output=True)
    (project / 'project.godot').write_text('config_version=5\n[application]\nconfig/name="Issue 31 regression"\n[rendering]\nrenderer/rendering_method="gl_compatibility"\n')
    (project / 'eosg.gdextension').write_text('[configuration]\nentry_symbol="eosg_library_init"\ncompatibility_minimum="4.2"\n[libraries]\nmacos.debug="res://' + framework.name + '"\n')
    (project / '.godot').mkdir()
    (project / '.godot/extension_list.cfg').write_text('res://eosg.gdextension\n')
    shutil.copy2(HERE / 'test.gd', project / 'test.gd')
    env = dict(os.environ, EOSG_TEST_RETRIES=str(args.retries), EOSG_TEST_RESULT=str(args.result),
               EOSG_TEST_UNSAFE_REPLAY='1' if args.unsafe_replay else '0',
               EOSG_TEST_SHOW='1' if args.show else '0')
    command = [str(args.godot), '--path', str(project), '--script', 'test.gd']
    if not args.show:
        command.append('--headless')
    print('COMMAND:', ' '.join(command), flush=True)
    if args.show:
        raise SystemExit(subprocess.call(command, env=env))
    result = subprocess.run(command, env=env, timeout=30, capture_output=True, text=True)
    print(result.stdout, end='')
    print(result.stderr, end='')
    print('PROCESS EXIT:', result.returncode, flush=True)
    passed = result.returncode == 0 and 'PASS:' in result.stdout and not any(
        marker in result.stdout + result.stderr for marker in ['FAIL:', 'ERROR:'])
    raise SystemExit(0 if passed else 1)

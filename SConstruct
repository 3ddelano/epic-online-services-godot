#!/usr/bin/env python
import os
import sys
import shutil

env = SConscript("godot-cpp/SConstruct")
lib_name = "libeosg"
plugin_bin_folder = "sample/addons/epic-online-services-godot/bin"

eos_sdk_folder = "thirdparty/eos-sdk/SDK/"

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Add source files
env.Append(CPPPATH=["src/", eos_sdk_folder + "Include/"])
sources = Glob("src/*.cpp")


env.Append(LIBPATH=[eos_sdk_folder + "Lib/"])
env.Append(LIBPATH=[eos_sdk_folder + "Bin/"])
if env["platform"] == "windows":
    # TODO: dont ignore this warning
    # this disables LINK : error LNK1218: warning treated as error;
    # so that it can build in github action with scons cache
    env.Append(LINKFLAGS=["/ignore:4099"])
    
    env.Append(LIBS=["EOSSDK-Win64-Shipping"])
    
elif env["platform"] == "linux":
    env.Append(LIBS=["EOSSDK-Linux-Shipping.so"])
    
elif env["platform"] == "macos":
    env.Append(LIBS=["EOSSDK-Mac-Shipping.dylib"])

elif env["platform"] == "android":
    eos_android_arch = "arm64-v8a"
    if env["arch"] == "x86_64":
        eos_android_arch = "x86_64"
    
    env.Append(LIBPATH=[eos_sdk_folder + "Bin/Android/static-stdc++/libs/" + eos_android_arch + "/"]) 
    env.Append(LIBS=["EOSSDK"])


if env["platform"] == "macos":
    library = env.SharedLibrary(
        f"{plugin_bin_folder}/macos/{lib_name}.{env['platform']}.{env['target']}.framework/{lib_name}.{env['platform']}.{env['target']}",
        source=sources,)
else:
    library = env.SharedLibrary(
        f"{plugin_bin_folder}/{env['platform']}/{lib_name}{env['suffix']}{env['SHLIBSUFFIX']}",
        source=sources,
    )

def copy_file(from_path, to_path):
    if not os.path.exists(os.path.dirname(to_path)):
        os.makedirs(os.path.dirname(to_path))
    shutil.copyfile(from_path, to_path)

def on_complete(target, source, env):
    shutil.rmtree(plugin_bin_folder + "/windows/x64", ignore_errors=True)
    shutil.copytree(eos_sdk_folder + "Bin/x64", plugin_bin_folder + "/windows/x64")
    copy_file(eos_sdk_folder + "Bin/EOSSDK-Win64-Shipping.dll", plugin_bin_folder + "/windows/EOSSDK-Win64-Shipping.dll")

# Disable scons cache for source files
NoCache(sources)

if env["platform"] == "windows":
    complete_command = Command('complete', library, on_complete)
    Depends(complete_command, library)
    Default(complete_command)
else:
    Default(library)
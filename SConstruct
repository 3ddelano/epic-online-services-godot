#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
lib_name = "libeosg"
bin_folder = "sample/addons/epic-online-services-godot/bin"

eos_sdk_folder = "thirdparty/eos-sdk/"

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
if env["platform"] == "windows":
    env.Append(LIBS=["EOSSDK-Win64-Shipping"])
# elif env["platform"] == "linux":
#     env.Append(LIBS=["EOSSDK-Linux-Shipping.so"])
# elif env["platform"] == "macos":
#     env.Append(LIBS=["EOSSDK-Mac-Shipping.dylib"])


if env["platform"] == "macos":
    library = env.SharedLibrary(
        f"{bin_folder}/{lib_name}.{env['platform']}.{env['target']}.framework/{lib_name}.{env['platform']}.{env['target']}",
        source=sources,)
else:
    library = env.SharedLibrary(
        f"{bin_folder}/{lib_name}{env['suffix']}{env['SHLIBSUFFIX']}",
        source=sources,
    )

Default(library)

#!/usr/bin/env python
import os
import shutil

env = SConscript("godot-cpp/SConstruct")
lib_name = "libeosg"
plugin_bin_folder = "sample/addons/epic-online-services-godot/bin"

eos_sdk_folder = "thirdparty/eos-sdk/SDK/"


# Add source files
env.Append(CPPPATH=["src/", eos_sdk_folder + "Include/"])
sources = Glob("src/*.cpp")
platform = env["platform"]
build_target = env["target"]


def copy_file(from_path, to_path):
	if not os.path.exists(os.path.dirname(to_path)):
		os.makedirs(os.path.dirname(to_path))
	shutil.copyfile(from_path, to_path)

def copy_folder(from_path, to_path):
	if not os.path.exists(to_path):
		os.makedirs(to_path)
	shutil.rmtree(to_path, ignore_errors=True)
	shutil.copytree(from_path, to_path)

def extract_eos_android_libraries():
	libs_path = eos_sdk_folder + "Bin/Android/static-stdc++/libs/"
	aar_file = eos_sdk_folder + "Bin/Android/static-stdc++/aar/eossdk-StaticSTDC-release.aar"
	zip_file = libs_path + "eos-sdk.zip"
	
	# Delete libs folder if exists
	shutil.rmtree(libs_path, ignore_errors=True)
	
	# Copy aar to zip file
	copy_file(aar_file, zip_file)
	
	# Copy aar to plugin bin/android
	copy_file(aar_file, plugin_bin_folder + "/android/eossdk-StaticSTDC-release.aar")
	
	# Create folder if doesnt exist
	if not os.path.exists(libs_path + "extracted"):
		os.makedirs(libs_path + "extracted")

	# Unzip the file
	shutil.unpack_archive(zip_file, libs_path + "extracted")

	# Copy all folders from libs_path+"extracted/jni" to libs_path
	for folder in os.listdir(libs_path + "extracted/jni"):
		shutil.copytree(libs_path + "extracted/jni/" + folder, libs_path + folder)
	
	# Delete extracted folder
	shutil.rmtree(libs_path + "extracted", ignore_errors=True)
	
	# Delete zip file
	os.remove(zip_file)


genv = env
def on_complete(target, source, env):
	if platform == "windows":
		shutil.rmtree(plugin_bin_folder + "/windows/x64", ignore_errors=True)
		shutil.copytree(eos_sdk_folder + "Bin/x64", plugin_bin_folder + "/windows/x64")
		copy_file(eos_sdk_folder + "Bin/EOSSDK-Win64-Shipping.dll", plugin_bin_folder + "/windows/EOSSDK-Win64-Shipping.dll")
	
	elif platform == "linux":
		so_variant = "LinuxArm64" if genv["arch"] == "arm64" else "Linux"
		copy_file(eos_sdk_folder + f"Bin/libEOSSDK-{so_variant}-Shipping.so", plugin_bin_folder + "/linux/libEOSSDK-{so_variant}-Shipping.so")
	
	elif platform == "macos":
		framework_folder = plugin_bin_folder + f"/macos/{lib_name}.{platform}.{build_target}.framework"
		# Copies EOS dylib inside framework folder
		copy_file(eos_sdk_folder + "Bin/libEOSSDK-Mac-Shipping.dylib", framework_folder + f"/libEOSSDK-Mac-Shipping.dylib")
		lib_path = f"{framework_folder}/{lib_name}.{platform}.{build_target}"
		print(f"Updating libEOSSDK-Mac-Shipping.dylib path in {lib_path}")
		os.system(f"install_name_tool -change @rpath/libEOSSDK-Mac-Shipping.dylib @loader_path/libEOSSDK-Mac-Shipping.dylib {lib_path}")



# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags


env.Append(LIBPATH=[eos_sdk_folder + "Lib/"])
env.Append(LIBPATH=[eos_sdk_folder + "Bin/"])
if env["platform"] == "windows":
	# TODO: dont ignore this warning
	# this disables LINK : error LNK1218: warning treated as error;
	# so that it can build in github action with scons cache
	env.Append(LINKFLAGS=["/ignore:4099"])
	
	env.Append(LIBS=["EOSSDK-Win64-Shipping"])

elif env["platform"] == "linux":
	if env["arch"] == "arm64":
		env.Append(LIBS=["EOSSDK-LinuxArm64-Shipping"])
	else:
		env.Append(LIBS=["EOSSDK-Linux-Shipping"])

elif env["platform"] == "macos":
	env.Append(LIBS=["EOSSDK-Mac-Shipping"])

elif env["platform"] == "ios":
	if env["arch"] != "arm64":
		raise Exception("Only arm64 is supported on iOS.")
	copy_folder(eos_sdk_folder + "Bin/IOS/EOSSDK.xcframework", plugin_bin_folder + "/ios/EOSSDK.xcframework")
	
	env.Append(LINKFLAGS=[
		"-F", plugin_bin_folder + f"/ios/EOSSDK.xcframework/ios-arm64{"-simulator" if env["ios_simulator"] else ""}",
		'-framework', 'AuthenticationServices',
		'-framework', 'EOSSDK',
	])
	

elif env["platform"] == "android":
	eos_android_arch = "arm64-v8a"
	if env["arch"] == "x86_64":
		eos_android_arch = "x86_64"
		
	extract_eos_android_libraries()
	
	env.Append(LIBPATH=[eos_sdk_folder + "Bin/Android/static-stdc++/libs/" + eos_android_arch + "/"]) 
	env.Append(LIBS=["EOSSDK"])


if env["platform"] == "macos":
	library = env.SharedLibrary(
		f"{plugin_bin_folder}/macos/{lib_name}.{platform}.{build_target}.framework/{lib_name}.{platform}.{build_target}",
		source=sources,)
else:
	library = env.SharedLibrary(
		f"{plugin_bin_folder}/{platform}/{lib_name}{env['suffix']}{env['SHLIBSUFFIX']}",
		source=sources,
	)


# Disable scons cache for source files
# NoCache(sources)

complete_command = Command('complete', library, on_complete)
Depends(complete_command, library)
Default(complete_command)
import shutil

eos_bin_folder = "thirdparty/eos-sdk/SDK/Bin/"
plugin_bin_folder = "sample/addons/epic-online-services-godot/bin/"

def copy_to_bin(from_name, to_name):
	shutil.copyfile(eos_bin_folder + from_name, plugin_bin_folder + to_name)

def copy_folder_to_bin(folder_name):
	shutil.rmtree(plugin_bin_folder + folder_name, ignore_errors=True)
	shutil.copytree(eos_bin_folder + folder_name, plugin_bin_folder + folder_name)

copy_to_bin("EOSSDK-Win64-Shipping.dll", "EOSSDK-Win64-Shipping.dll")
copy_to_bin("libEOSSDK-Linux-Shipping.so", "EOSSDK-Linux-Shipping.so")
copy_to_bin("libEOSSDK-Mac-Shipping.dylib", "EOSSDK-Mac-Shipping.dylib")
copy_folder_to_bin("x64")
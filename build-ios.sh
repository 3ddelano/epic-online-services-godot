#!/bin/sh

build=yes
if [ "$1" = "build=n" ] || [ "$1" = "build=N" ] || [ "$1" = "build=no" ] || [ "$1" = "build=0" ]; then
    build=no
fi

dev_build=no
dev_build_ext=""
if [ "$2" = "dev_build=y" ] || [ "$2" = "dev_build=Y" ] || [ "$2" = "dev_build=yes" ] || [ "$2" = "dev_build=1" ]; then
	dev_build=yes
	dev_build_ext=".dev"
fi

if [ "$build" = "yes" ]; then
	echo "Building for iOS targets"
    # Debug builds
    scons arch=arm64 ios_simulator=yes platform=ios target=template_debug dev_build=${dev_build}
    scons arch=arm64 ios_simulator=no platform=ios target=template_debug dev_build=${dev_build}

    # Release builds
    scons arch=arm64 ios_simulator=yes platform=ios target=template_release dev_build=no
    scons arch=arm64 ios_simulator=no platform=ios target=template_release dev_build=no
fi

eosg_ios_bin_dir=./sample/addons/epic-online-services-godot/bin/ios
godotcpp_bin_dir=./godot-cpp/bin

# Delete existing libgodot-cpp xcframework if any
rm -rf ${eosg_ios_bin_dir}/libgodot-cpp.ios.template*

# Create libgodot-cpp xcframework
xcodebuild -create-xcframework \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_debug${dev_build_ext}.arm64.a \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_debug${dev_build_ext}.arm64.simulator.a \
-output ${eosg_ios_bin_dir}/libgodot-cpp.ios.template_debug.xcframework

xcodebuild -create-xcframework \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_release.arm64.a \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_release.arm64.simulator.a \
-output ${eosg_ios_bin_dir}/libgodot-cpp.ios.template_release.xcframework

# Create libeosg xcframework
xcodebuild -create-xcframework \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_debug${dev_build_ext}.arm64.dylib \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_debug${dev_build_ext}.arm64.simulator.dylib \
-output ${eosg_ios_bin_dir}/libeosg.ios.template_debug.xcframework

xcodebuild -create-xcframework \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_release.arm64.dylib \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_release.arm64.simulator.dylib \
-output ${eosg_ios_bin_dir}/libeosg.ios.template_release.xcframework


# Delete all .dylib from eosg ios bin dir
rm -rf ${eosg_ios_bin_dir}/*.dylib
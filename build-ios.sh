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


echo "Config: dev_build=${dev_build}, build=${build}"


if [ "$build" = "yes" ]; then
    echo "\nBuilding for iOS simulator template_debug target"
    scons arch=arm64 ios_simulator=yes platform=ios target=template_debug dev_build=${dev_build}

    echo "\nBuilding for iOS device template_debug target"
    scons arch=arm64 ios_simulator=no platform=ios target=template_debug dev_build=${dev_build}

    echo "\nBuilding for iOS simulator template_release target"
    scons arch=arm64 ios_simulator=yes platform=ios target=template_release dev_build=no
    
    echo "\nBuilding for iOS device template_release target"
    scons arch=arm64 ios_simulator=no platform=ios target=template_release dev_build=no
fi

eosg_ios_bin_dir=./sample/addons/epic-online-services-godot/bin/ios
godotcpp_bin_dir=./godot-cpp/bin

echo "\nDeleting existing libgodot-cpp xcframework(s) if any"
rm -rf ${eosg_ios_bin_dir}/libgodot-cpp.ios.template*

echo "\nCreating libgodot-cpp xcframework for template_debug"
xcodebuild -create-xcframework \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_debug${dev_build_ext}.arm64.a \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_debug${dev_build_ext}.arm64.simulator.a \
-output ${eosg_ios_bin_dir}/libgodot-cpp.ios.template_debug.xcframework

echo "\nCreating libgodot-cpp xcframework for template_release"
xcodebuild -create-xcframework \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_release.arm64.a \
-library ${godotcpp_bin_dir}/libgodot-cpp.ios.template_release.arm64.simulator.a \
-output ${eosg_ios_bin_dir}/libgodot-cpp.ios.template_release.xcframework

echo "\nCreating libeosg xcframework for template_debug"
xcodebuild -create-xcframework \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_debug${dev_build_ext}.arm64.dylib \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_debug${dev_build_ext}.arm64.simulator.dylib \
-output ${eosg_ios_bin_dir}/libeosg.ios.template_debug.xcframework

echo "\nCreating libeosg xcframework for template_release"
xcodebuild -create-xcframework \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_release.arm64.dylib \
-library ${eosg_ios_bin_dir}/libeosg.ios.template_release.arm64.simulator.dylib \
-output ${eosg_ios_bin_dir}/libeosg.ios.template_release.xcframework


echo "\nDeleting all .dylib files from eosg ios bin dir"
rm -rf ${eosg_ios_bin_dir}/*.dylib
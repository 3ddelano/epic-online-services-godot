#!/bin/sh
# Compile the real call sites that relied on CharString's implicit conversion.
# Use a godot-cpp revision without operator const char * (see tests/README.md).
set -eu
cd "$(dirname "$0")/.."
scons dev_build=yes "$@" \
    src/anticheat_client_interface.os \
    src/anticheat_server_interface.os \
    src/eosg_multiplayer_peer.os \
    src/eosg_packet_peer_mediator.os

#pragma once
#include "godot_cpp/variant/char_string.hpp"

using namespace godot;

#define VARIANT_TO_CHARSTRING(str) ((String)str).utf8()
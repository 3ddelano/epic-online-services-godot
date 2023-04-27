#include "ieos.h"
using namespace godot;

String IEOS::version_interface_get_version() {
    return String(EOS_GetVersion());
}

Dictionary IEOS::version_interface_get_constants() {
    Dictionary constants = Dictionary();
    constants["company_name"] = EOS_COMPANY_NAME;
    constants["copyright_string"] = EOS_COPYRIGHT_STRING;
    constants["product_name"] = EOS_PRODUCT_NAME;
    constants["product_identifier"] = EOS_PRODUCT_IDENTIFIER;
    constants["major_version"] = EOS_MAJOR_VERSION;
    constants["minor_version"] = EOS_MINOR_VERSION;
    constants["patch_version"] = EOS_PATCH_VERSION;

    return constants;
}
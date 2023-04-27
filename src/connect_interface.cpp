#include "ieos.h"
using namespace godot;

void IEOS::connect_interface_login(Ref<RefCounted> options) {
    return;
}

Dictionary IEOS::connect_interface_copy_id_token(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::connect_interface_copy_product_user_external_account_by_account_id(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::connect_interface_copy_product_user_external_account_by_account_type(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::connect_interface_copy_product_user_external_account_by_index(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::connect_interface_copy_product_user_info(Ref<RefCounted> options) {
    return Dictionary();
}

void IEOS::connect_interface_create_device_id(Ref<RefCounted> options) {
    return;
}

void IEOS::connect_interface_delete_device_id(Ref<RefCounted> options) {
    return;
}

void IEOS::connect_interface_create_user(Ref<RefCounted> options) {
    return;
}

String IEOS::connect_interface_get_external_account_mapping(Ref<RefCounted> options) {
    return "";
}

String IEOS::connect_interface_get_logged_in_user_by_index(int index) {
    return "";
}

int IEOS::connect_interface_get_logged_in_users_count() {
    return 1;
}

int IEOS::connect_interface_get_login_status(const String& local_user_id) {
    return 1;
}

int IEOS::connect_interface_get_product_user_external_account_count(Ref<RefCounted> options) {
    return 1;
}

Dictionary IEOS::connect_interface_get_product_user_id_mapping(Ref<RefCounted> options) {
    return Dictionary();
}

void IEOS::connect_interface_link_account(Ref<RefCounted> options) {
    return;
}

void IEOS::connect_interface_verify_id_token(Ref<RefCounted> options) {
    return;
}

void IEOS::connect_interface_transfer_device_id_account(Ref<RefCounted> options) {
    return;
}

void IEOS::connect_interface_unlink_account(Ref<RefCounted> options) {
    return;
}

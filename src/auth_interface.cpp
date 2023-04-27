#include "ieos.h"
using namespace godot;

void IEOS::auth_interface_login(Ref<RefCounted> options) {
    return;
}

void IEOS::auth_interface_logout(Ref<RefCounted> options) {
    return;
}

Dictionary IEOS::auth_interface_copy_id_token(Ref<RefCounted> options) {
    return Dictionary();
}

Dictionary IEOS::auth_interface_copy_user_auth_token(Ref<RefCounted> options, const String& local_user_id) {
    return Dictionary();
}

void IEOS::auth_interface_delete_persistent_auth(Ref<RefCounted> options) {
    return;
}

String IEOS::auth_interface_get_logged_in_account_by_index(int index) {
    return "";
}

int IEOS::auth_interface_get_logged_in_accounts_count() {
    return 1;
}

int IEOS::auth_interface_get_login_status(const String& local_user_id) {
    return 1;
}

String IEOS::auth_interface_get_merged_account_by_index(const String& local_user_id, int index) {
    return "";
}

int IEOS::auth_interface_get_merged_accounts_count(const String& local_user_id) {
    return 1;
}

Dictionary IEOS::auth_interface_get_selected_account_id(const String& local_user_id) {
    return Dictionary();
}

void IEOS::auth_interface_query_id_token(Ref<RefCounted> options) {
    return;
}

void IEOS::auth_interface_verify_id_token(Ref<RefCounted> options) {
    return;
}

void IEOS::auth_interface_link_account(Ref<RefCounted> options) {
    return;
}

void IEOS::auth_interface_verify_user_auth(Ref<RefCounted> _options) {
    return;
}

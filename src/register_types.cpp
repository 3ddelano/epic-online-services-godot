#include "register_types.h"

#include "eosg_active_session.h"
#include "eosg_continuance_token.h"
#include "eosg_file_transfer_request.h"
#include "eosg_lobby_details.h"
#include "eosg_lobby_modification.h"
#include "eosg_lobby_search.h"
#include "eosg_multiplayer_peer.h"
#include "eosg_packet_peer_mediator.h"
#include "eosg_presence_modification.h"
#include "eosg_session_details.h"
#include "eosg_session_modification.h"
#include "eosg_session_search.h"
#include "eosg_transaction.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/godot.hpp"
#include "ieos.h"

using namespace godot;

static IEOS *_ieos;
static EOSGPacketPeerMediator *_mediator;

void initialize_eosg_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<IEOS>();
    _ieos = memnew(IEOS);
    Engine::get_singleton()->register_singleton("IEOS", IEOS::get_singleton());

    ClassDB::register_class<godot::EOSGPacketPeerMediator>();
    _mediator = memnew(EOSGPacketPeerMediator);
    Engine::get_singleton()->register_singleton("EOSGPacketPeerMediator", EOSGPacketPeerMediator::get_singleton());

    ClassDB::register_abstract_class<godot::EOSGFileTransferRequest>();
    ClassDB::register_class<godot::EOSGPlayerDataStorageFileTransferRequest>();
    ClassDB::register_class<godot::EOSGTitleStorageFileTransferRequest>();

    ClassDB::register_class<godot::EOSGActiveSession>();
    ClassDB::register_class<godot::EOSGContinuanceToken>();
    ClassDB::register_class<godot::EOSGLobbyDetails>();
    ClassDB::register_class<godot::EOSGLobbyModification>();
    ClassDB::register_class<godot::EOSGLobbySearch>();
    ClassDB::register_class<godot::EOSGMultiplayerPeer>();
    ClassDB::register_class<godot::EOSGPresenceModification>();
    ClassDB::register_class<godot::EOSGSessionDetails>();
    ClassDB::register_class<godot::EOSGSessionModification>();
    ClassDB::register_class<godot::EOSGSessionSearch>();
    ClassDB::register_class<godot::EOSGTransaction>();
}

void uninitialize_eosg_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    Engine::get_singleton()->unregister_singleton("IEOS");
    memdelete(_ieos);

    Engine::get_singleton()->unregister_singleton("EOSGPacketPeerMediator");
    memdelete(_mediator);
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT eosg_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_eosg_module);
    init_obj.register_terminator(uninitialize_eosg_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
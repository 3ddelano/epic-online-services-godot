#include "register_types.h"

#include "continuance_token.h"
#include "eosg_file_transfer_request.h"
#include "eosg_multiplayer_peer.h"
#include "eosg_packet_peer_mediator.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/godot.hpp"
#include "ieos.h"
#include "lobby_details.h"
#include "lobby_modification.h"
#include "lobby_search.h"
#include "presence_modification.h"
#include "transaction.h"

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

    ClassDB::register_class<godot::EOSGMultiplayerPeer>();
    ClassDB::register_class<godot::ContinuanceTokenEOSG>();
    ClassDB::register_class<godot::TransactionEOSG>();
    ClassDB::register_class<godot::PresenceModificationEOSG>();
    ClassDB::register_class<godot::LobbyModificationEOSG>();
    ClassDB::register_class<godot::LobbySearchEOSG>();
    ClassDB::register_class<godot::LobbyDetailsEOSG>();
    ClassDB::register_abstract_class<godot::EOSGFileTransferRequest>();
    ClassDB::register_class<godot::EOSGPlayerDataStorageFileTransferRequest>();
    ClassDB::register_class<godot::EOSGTitleStorageFileTransferRequest>();
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
extends Control


@onready var PRODUCT_NAME: String = Env.get_var("PRODUCT_NAME")
@onready var PRODUCT_VERSION: String = Env.get_var("PRODUCT_VERSION")
@onready var PRODUCT_ID: String = Env.get_var("PRODUCT_ID")
@onready var SANDBOX_ID: String = Env.get_var("SANDBOX_ID")
@onready var DEPLOYMENT_ID: String = Env.get_var("DEPLOYMENT_ID")
@onready var ENCRYPTION_KEY: String = Env.get_var("ENCRYPTION_KEY")

@onready var SERVER_CLIENT_ID: String = Env.get_var("SERVER_CLIENT_ID")
@onready var SERVER_CLIENT_SECRET: String = Env.get_var("SERVER_CLIENT_SECRET")


const PORT = 5412
var peer = ENetMultiplayerPeer.new()


func _log(msg: String):
	print_rich("[color=cyan]Server: " + msg + "[/color]")


func _ready() -> void:
	var err = peer.create_server(PORT)
	if err != OK:
		_log("Failed to create ENet server %s (%d)" % [error_string(err), err])
	multiplayer.multiplayer_peer = peer
	
	var credentials = HCredentials.new()
	credentials.product_name = PRODUCT_NAME
	credentials.product_version = PRODUCT_VERSION
	credentials.product_id = PRODUCT_ID
	credentials.sandbox_id = SANDBOX_ID
	credentials.deployment_id = DEPLOYMENT_ID
	credentials.client_id = SERVER_CLIENT_ID
	credentials.client_secret = SERVER_CLIENT_SECRET
	credentials.encryption_key = ENCRYPTION_KEY
	
	print("Setting up EOS for dedicated server...")
	HPlatform.cache_directory = ProjectSettings.globalize_path("user://eosg-server-cache")
	HPlatform.is_server = true
	HPlatform.flags = EOS.Platform.PlatformFlags.DisableOverlay
	
	var setup_success := await HPlatform.setup_eos_async(credentials)
	if not setup_success:
		_log("Failed to setup EOS. See logs for error details")
		return
	
	var opts = EOS.Sessions.CreateSessionModificationOptions.new()
	opts.bucket_id = "party-bucket"
	opts.max_players = 4
	opts.session_name = "Party"
	
	var session := HSessions.create_session(opts)
	if not session:
		return
	
	_log("Created session")
	
	session.add_attribute("level", "margao", EOS.Sessions.SessionAttributeAdvertisementType.Advertise)
	session.set_permission_level(EOS.Sessions.OnlineSessionPermissionLevel.PublicAdvertised)
	session.set_host_address("localhost:" + str(PORT))
	var update_success := await HSessions.update_session_async(session)
	if not update_success:
		return
	
	_log("Updated session")
	
	var active_sess := HSessions.get_active_session("Party")
	if not active_sess:
		return
	
	_log("sess is %s" % active_sess.copy_info())



# This is just added as a sample in your game you can register a player in
# a different way also
@rpc("any_peer", "reliable")
func c_s_register_player(puid: String):
	var peerid = multiplayer.get_remote_sender_id()
	
	var opts = EOS.Sessions.RegisterPlayersOptions.new()
	opts.session_name = "Party"
	opts.players_to_register = [puid]
	EOS.Sessions.SessionsInterface.register_players(opts)
	
	var ret: Dictionary = await IEOS.sessions_interface_register_players_callback
	if not EOS.is_success(ret):
		_log("Failed to register player: puid=%s, result_code=%s" % [puid, EOS.result_str(ret)])
		s_c_register_player_completed.rpc_id(peerid, false)
		return
	
	_log("Registered player: puid")
	
	s_c_register_player_completed.rpc_id(peerid, true)


@rpc("reliable")
func s_c_register_player_completed(is_success: bool):
	pass

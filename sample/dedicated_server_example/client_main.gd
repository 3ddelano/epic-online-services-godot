extends Control

@onready var PRODUCT_NAME: String = Env.get_var("PRODUCT_NAME")
@onready var PRODUCT_VERSION: String = Env.get_var("PRODUCT_VERSION")
@onready var PRODUCT_ID: String = Env.get_var("PRODUCT_ID")
@onready var SANDBOX_ID: String = Env.get_var("SANDBOX_ID")
@onready var DEPLOYMENT_ID: String = Env.get_var("DEPLOYMENT_ID")
@onready var ENCRYPTION_KEY: String = Env.get_var("ENCRYPTION_KEY")

@onready var CLIENT_ID: String = Env.get_var("CLIENT_ID")
@onready var CLIENT_SECRET: String = Env.get_var("CLIENT_SECRET")

func _log(msg: String):
	print_rich("[color=WEB_GREEN]Client " + HAuth.product_user_id + ": " + msg + "[/color]")


func _ready() -> void:
	var credentials = HCredentials.new()
	credentials.product_name = PRODUCT_NAME
	credentials.product_version = PRODUCT_VERSION
	credentials.product_id = PRODUCT_ID
	credentials.sandbox_id = SANDBOX_ID
	credentials.deployment_id = DEPLOYMENT_ID
	credentials.client_id = CLIENT_ID
	credentials.client_secret = CLIENT_SECRET
	credentials.encryption_key = ENCRYPTION_KEY
	
	_log("Setting up EOS for dedicated client...")
	var setup_success := await HPlatform.setup_eos_async(credentials)
	if not setup_success:
		_log("Failed to setup EOS. See logs for error details")
		return
	
	var args = OS.get_cmdline_user_args()
	for arg in args:
		if arg.begins_with("--devuser="):
			var username = arg.replace("--devuser=", "")
			_login_as_client(username)


func _login_as_client(devusername: String):
	_log("Logging in as client... %s" % devusername)
	var success := await HAuth.login_devtool_async("localhost:4545", devusername)
	if not success:
		return
	
	_log("Client logged in")
	
	# Wait so that the server has time to create the session
	# This is just for the sample project!
	# In the real game this timer isnt needed
	await get_tree().create_timer(5).timeout
	
	var sessions = await HSessions.search_by_attribute_async({key = "level", value = "margao"})
	if not sessions:
		return
	
	var session_details = sessions[-1]
	var session_info = session_details.copy_info().info
	_log("Joining session: %s" % session_info)
	
	#HLog.log_level = HLog.LogLevel.DEBUG
	
	# Join the session
	var join_success := await HSessions.join_async(session_details, "Party")
	if not join_success:
		return

	var peer = ENetMultiplayerPeer.new()
	var addr_parts = session_info.host_address.split(":")
	var ip = addr_parts[0]
	var port = int(addr_parts[1])
	peer.create_client(ip, port)
	multiplayer.multiplayer_peer = peer
	
	await multiplayer.connected_to_server
	c_s_register_player.rpc_id(0, HAuth.product_user_id)

	
@rpc("any_peer", "reliable")
func c_s_register_player(puid: String):
	pass


@rpc("reliable")
func s_c_register_player_completed(is_success: bool):
	_log("Got register player completed response from server: register_success=%s" % str(is_success))

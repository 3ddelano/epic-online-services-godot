class_name AntiCheatServerMain
extends Node

@onready var PRODUCT_NAME: String = Env.get_var("PRODUCT_NAME")
@onready var PRODUCT_VERSION: String = Env.get_var("PRODUCT_VERSION")
@onready var PRODUCT_ID: String = Env.get_var("PRODUCT_ID")
@onready var SANDBOX_ID: String = Env.get_var("SANDBOX_ID")
@onready var DEPLOYMENT_ID: String = Env.get_var("DEPLOYMENT_ID")
@onready var CLIENT_ID: String = Env.get_var("CLIENT_ID")
@onready var CLIENT_SECRET: String = Env.get_var("CLIENT_SECRET")
@onready var ENCRYPTION_KEY: String = Env.get_var("ENCRYPTION_KEY")

const PORT = 12345

func _ready() -> void:
	print("AntiCheatServer _ready")

	var init_success = await _load_and_init_sdk()
	if not init_success:
		return

	_begin_session()
	_start_server()

func _load_and_init_sdk() -> bool:
	# -----
	# EOS Setup
	# -----

	# Initialize the SDK
	var init_opts = EOS.Platform.InitializeOptions.new()
	init_opts.product_name = PRODUCT_NAME
	init_opts.product_version = PRODUCT_VERSION
	var init_res := EOS.Platform.PlatformInterface.initialize(init_opts)
	var init_retry_count = 10
	while not EOS.is_success(init_res) and init_retry_count > 0:
		init_res = EOS.Platform.PlatformInterface.initialize(init_opts)
		init_retry_count -= 1
		await get_tree().create_timer(0.2).timeout

	if not EOS.is_success(init_res):
		print("Failed to initialize EOS SDK: ", EOS.result_str(init_res))
		return false
	print("Initialized EOS platform")

	# Create platform
	var create_opts = EOS.Platform.CreateOptions.new()
	create_opts.product_id = PRODUCT_ID
	create_opts.sandbox_id = SANDBOX_ID
	create_opts.deployment_id = DEPLOYMENT_ID
	create_opts.client_id = CLIENT_ID
	create_opts.client_secret = CLIENT_SECRET
	create_opts.encryption_key = ENCRYPTION_KEY
	create_opts.is_server = true
	create_opts.flags = EOS.Platform.PlatformFlags.DisableOverlay
	var create_success: bool = EOS.Platform.PlatformInterface.create(create_opts)
	var create_retry_count = 10
	while not create_success&&create_retry_count > 0:
		create_success = EOS.Platform.PlatformInterface.create(create_opts)
		create_retry_count -= 1
		await get_tree().create_timer(0.2).timeout
	if not create_success:
		print("Failed to create EOS Platform")
		return false
	print("Created EOS platform")

	return true

func _begin_session():
	IEOS.anticheatserver_interface_message_to_client_callback.connect(func(data):
		print("--- AntiCheatServer: message_to_client_callback: ", data)
	)

	IEOS.anticheatserver_interface_client_action_required_callback.connect(func(data):
		print("--- AntiCheatServer: client_action_required_callback: ", data)
	)

	IEOS.anticheatserver_interface_client_auth_status_changed_callback.connect(func(data):
		print("--- AntiCheatServer: client_auth_status_changed_callback: ", data)
	)

	var begin_sess_opts = EOS.AntiCheatServer.BeginSessionOptions.new()
	begin_sess_opts.register_timeout_seconds = 60
	begin_sess_opts.server_name = "Godot 4.3 server"
	begin_sess_opts.enable_gameplay_data = false

	print("--- AntiCheatServer: begin_session: ", EOS.AntiCheatServer.AntiCheatServerInterface.begin_session(begin_sess_opts))

func _verify_id_token(_peer_id: int, product_user_id: String, connect_id_jwt: String):
	var id_token = EOS.Connect.IdToken.new()
	id_token.json_web_token = connect_id_jwt
	id_token.product_user_id = product_user_id

	var verify_token_opts = EOS.Connect.VerifyIdTokenOptions.new()
	verify_token_opts.id_token = id_token

	EOS.Connect.ConnectInterface.verify_id_token(verify_token_opts)
	print("--- AntiCheatServer: verify_id_token: ", await IEOS.connect_interface_verify_id_token_callback)

func _start_server():
	multiplayer.peer_connected.connect(func(id: int):
		print("--- AntiCheatServer:: peer_connected: ", id)
	)
	multiplayer.peer_disconnected.connect(func(id: int):
		print("--- AntiCheatServer:: peer_disconnected: ", id)
	)

	var peer = ENetMultiplayerPeer.new()
	peer.create_server(PORT)
	multiplayer.multiplayer_peer = peer
	print("Listening for clients on port: ", PORT)

func on_client_message_receive(peer_id: int, type: String, data: Dictionary):
	if type == "register":
		_verify_id_token(peer_id, data.local_user_id, data.jwt)

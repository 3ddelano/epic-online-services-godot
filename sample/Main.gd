extends Control

@onready var PRODUCT_NAME: String = Env.get_var("PRODUCT_NAME") # Paste your own
@onready var PRODUCT_VERSION: String = Env.get_var("PRODUCT_VERSION") # Paste your own
@onready var PRODUCT_ID: String = Env.get_var("PRODUCT_ID") # Paste your own
@onready var SANDBOX_ID: String = Env.get_var("SANDBOX_ID") # Paste your own
@onready var DEPLOYMENT_ID: String = Env.get_var("DEPLOYMENT_ID") # Paste your own
@onready var CLIENT_ID: String = Env.get_var("CLIENT_ID") # Paste your own
@onready var CLIENT_SECRET: String = Env.get_var("CLIENT_SECRET") # Paste your own
@onready var ENCRYPTION_KEY: String = Env.get_var("ENCRYPTION_KEY") # Paste your own

@export var _views_path: NodePath
@onready var views = get_node(_views_path) as VBoxContainer


func _ready() -> void:
	print("Ready!")
	Store._main_node = self

	# -----
	# EOS Setup
	# -----

	var init_opts = EOS.Platform.InitializeOptions.new()
	init_opts.product_name = PRODUCT_NAME
	init_opts.product_version = PRODUCT_VERSION

	var create_opts = EOS.Platform.CreateOptions.new()
	create_opts.product_id = PRODUCT_ID
	create_opts.sandbox_id = SANDBOX_ID
	create_opts.deployment_id = DEPLOYMENT_ID
	create_opts.client_id = CLIENT_ID
	create_opts.client_secret = CLIENT_SECRET
	create_opts.encryption_key = ENCRYPTION_KEY
	if OS.get_name() == "Windows":
		create_opts.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl
	
	# Initialize the SDK
	var init_res := await HPlatform.initialize_async(init_opts)
	if not EOS.is_success(init_res):
		printerr("Failed to initialize EOS SDK: ", EOS.result_str(init_res))
		return
	
	# Create platform
	var create_success := await HPlatform.create_platform_async(create_opts)
	if not create_success:
		printerr("Failed to create EOS Platform")
		return
	var sdk_constants := EOS.Version.VersionInterface.get_constants()
	print("EOS SDK Version: %s (%s)" % [EOS.Version.VersionInterface.get_version(), sdk_constants.copyright_string])

	# See LoginView.gd for the user login flow
	_parse_cmdline_user_args()


func get_view_manager():
	return views


func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		print("Shutting down EOS...")
		EOS.Platform.PlatformInterface.release()
		var res := EOS.Platform.PlatformInterface.shutdown()
		if not EOS.is_success(res):
			printerr("Failed to shutdown EOS: ", EOS.result_str(res))


func _parse_cmdline_user_args():
	var args = OS.get_cmdline_user_args()


	for arg in args:
		if arg.begins_with("--devuser="):
			var login_view = Store.get_view("Login")
			var username = arg.replace("--devuser=", "")
			login_view._set_login_status("Logging in with devtool using cli arg...")
			login_view._set_login_state(LoginView.States.Pending)
			HAuth.login_devtool_async("localhost:4545", username)
		
		if arg.begins_with("--screenpos="):
			var rows = 2
			var cols = 2

			var pos := int(arg.replace("--screenpos=", ""))
			var screen_size = DisplayServer.screen_get_size()

			var scale_x = screen_size.x / cols
			var scale_y = screen_size.y / rows

			var x = (pos - 1) % cols
			@warning_ignore("integer_division")
			var y = (pos - 1) / rows

			get_window().position = Vector2(x * scale_x, y * scale_y)
			get_window().size = Vector2(screen_size.x / cols, screen_size.y / rows)

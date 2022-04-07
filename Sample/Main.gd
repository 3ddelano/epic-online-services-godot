extends Control

export (NodePath) var _views_path
onready var views = get_node(_views_path) as VBoxContainer


func _ready() -> void:
	State._main_node = self

	# -----
	# EOS Setup
	# -----

	# Initialize the SDK
	var init_options = EOS.Platform.InitializeOptions.new()
	init_options.product_name = Env.get_var("PRODUCT_NAME")
	init_options.product_version = Env.get_var("PRODUCT_VERSION")
	var init_result: int = EOS.Platform.PlatformInterface.initialize(init_options)
	if init_result != EOS.Result.Success:
		print("Failed to initialize EOS SDK: ", EOS.print_result(init_result))
		return

	# Create platform
	var create_options = EOS.Platform.CreateOptions.new()
	create_options.product_id = Env.get_var("PRODUCT_ID") # Paste your own here instead
	create_options.sandbox_id = Env.get_var("SANDBOX_ID") # Paste your own here instead
	create_options.deployment_id = Env.get_var("DEPLOYMENT_ID") # Paste your own here instead
	create_options.client_id = Env.get_var("CLIENT_ID") # Paste your own here instead
	create_options.client_secret = Env.get_var("CLIENT_SECRET") # Paste your own here instead
	create_options.encryption_key = Env.get_var("ENCRYPTION_KEY") # Paste your own here instead
	if OS.get_name() == "Windows":
		create_options.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl
	#create_options.is_server = true
	#create_options.tick_budget_in_milliseconds = 500

	var create_success: bool = EOS.Platform.PlatformInterface.create(create_options)
	if not create_success:
		print("Failed to create EOS Platform")
		return

	State.emit_signal("platform_create")


func get_view_manager():
	return views

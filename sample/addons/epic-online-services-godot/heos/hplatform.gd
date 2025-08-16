extends Node

#region Signals

## Emitted when the EOS platform was initialized successfully
signal platform_initialized

## Emitted when the EOS platform was created successfully
signal platform_created

## Emitted when the EOS SDK produces a log message
## Use this signal to handle the log however you want
signal log_msg(msg: EOS.Logging.LogMessage)

#endregion


#region Constants

## Number of times to retry initializing the EOS SDK
const INITIALIZE_RETRY_COUNT = 10
## Delay between retries initializing the EOS SDK in seconds
const INITIALIZE_RETRY_DElAY_SEC = 0.2

## Number of times to retry creating the EOS platform
const CREATE_RETRY_COUNT = 10
## Delay between retries creating the EOS platform in seconds
const CREATE_RETRY_DElAY_SEC = 0.2

#endregion


#region Public vars

## Absolute path to the folder that is going to be used for caching temporary data
var cache_directory := ProjectSettings.globalize_path("user://eosg-cache")
## Platform creation flags. This is a bitwise (union) of [enum EOS.Platform.PlatformFlags]
var flags: int = 0
## Set to true if its a dedicated game server
var is_server: bool
## Override country code for the logged-in user
var override_country_code: String
## Override local code for the logged-in user
var override_locale_code: String
## Budget, measured in milliseconds, for EOS_Platform_Tick to do its work. Set it according to your desired FPS.
var tick_budget_in_milliseconds: int
## Number of seconds for a task to wait for the network to become available before timing out with an EOS_TimedOut error
var task_network_timeout_seconds = null # float
## Configures RTC behavior upon entering to any background application statuses See [enum EOS.Platform.RTCBackgroundMode]
var rtc_options_background_mode = null

#endregion


#region Private vars 

var _log = HLog.logger("HPlatform")

#endregion


#region Built-in methods

func _ready() -> void:
	IEOS.logging_interface_callback.connect(_on_logging_callback)

#endregion


#region Public methods


## Easy setup for EOS. Returns true if EOS setup is success.
func setup_eos_async(p_creds: HCredentials) -> bool:
	_log.debug("Setting up EOS")
	
	if not p_creds.product_name:
		_log.error("HCredentials.product_name cannot be empty")
		return false
	
	var init_opts = EOS.Platform.InitializeOptions.new()
	init_opts.product_name = p_creds.product_name
	init_opts.product_version = p_creds.product_version
	
	var init_ret := await initialize_async(init_opts)
	if not EOS.is_success(init_ret):
		return false

	var create_opts = EOS.Platform.CreateOptions.new()
	create_opts.product_id = p_creds.product_id
	create_opts.sandbox_id = p_creds.sandbox_id
	create_opts.deployment_id = p_creds.deployment_id
	create_opts.client_id = p_creds.client_id
	create_opts.client_secret = p_creds.client_secret
	create_opts.encryption_key = p_creds.encryption_key
	
	create_opts.cache_directory = cache_directory
	if not flags:
		_log.debug("Using default flags for creating platform")
		if OS.get_name() == "Windows":
			create_opts.flags = EOS.Platform.PlatformFlags.WindowsEnableOverlayOpengl
	else:
		create_opts.flags = flags
	create_opts.is_server = is_server
	create_opts.override_country_code = override_country_code
	create_opts.override_locale_code = override_locale_code
	if not tick_budget_in_milliseconds:
		var max_fps = max(Engine.get_max_fps()*1.0, 60.0)
		var budget_ms = floori((0.3 * 1000) / max_fps)
		budget_ms = max(budget_ms, 2) # at least 2ms
		_log.debug("Using default tick_budget_in_milliseconds as %s" % budget_ms)
		create_opts.tick_budget_in_milliseconds = budget_ms
	else:
		create_opts.tick_budget_in_milliseconds = tick_budget_in_milliseconds
	create_opts.task_network_timeout_seconds = task_network_timeout_seconds
	create_opts.rtc_options.background_mode = rtc_options_background_mode
	
	var is_success := await create_platform_async(create_opts)
	if not is_success:
		return false
	
	return true


## Initialize the EOS SDK
func initialize_async(opts: EOS.Platform.InitializeOptions) -> EOS.Result:
	_log.debug("Initializing EOS SDK")
	var res := EOS.Platform.PlatformInterface.initialize(opts)
	
	var retry_count = INITIALIZE_RETRY_COUNT
	while not EOS.is_success(res) and retry_count > 0:
		if not EOS.is_success(res) and retry_count > 0:
			_log.debug("Failed to initialize EOS SDK: result_code=%s, retry_count=%s" % [EOS.result_str(res), INITIALIZE_RETRY_COUNT - retry_count + 1])
		
		res = EOS.Platform.PlatformInterface.initialize(opts)
		retry_count -= 1
		await get_tree().create_timer(INITIALIZE_RETRY_DElAY_SEC).timeout
		
	
	if not (EOS.is_success(res) or res == EOS.Result.AlreadyConfigured):
		_log.error("Failed to initialize EOS SDK: %s" % EOS.result_str(res))
		return res

	_log.debug("EOS SDK initialized: result_code=%s" % EOS.result_str(res))
	if EOS.is_success(res):
		platform_initialized.emit()

	return res


## Set the logging level for the EOS SDK
func set_eos_log_level(log_category: EOS.Logging.LogCategory, log_level: EOS.Logging.LogLevel) -> EOS.Result:
	var log_cat_str = EOS.Logging.LogCategory.find_key(log_category)
	_log.verbose("Setting log level: log_category=%s, log_level=%s" % [log_cat_str, log_level])
	var res := EOS.Logging.set_log_level(log_category, log_level)
	if not EOS.is_success(res):
		_log.error("Failed to set log level: %s" % EOS.result_str(res))
	
	return res


## Create the EOS Platform
func create_platform_async(opts: EOS.Platform.CreateOptions) -> bool:
	_log.debug("Creating EOS Platform")
	var res := EOS.Platform.PlatformInterface.create(opts)
	
	var retry_count = CREATE_RETRY_COUNT
	while not EOS.is_success(res) and retry_count > 0:
		if not EOS.is_success(res) and retry_count > 0:
			_log.debug("Failed to create EOS Platform: result_code=%s, retry_count=%s" % [EOS.result_str(res), CREATE_RETRY_COUNT - retry_count + 1])
		
		res = EOS.Platform.PlatformInterface.create(opts)
		retry_count -= 1
		await get_tree().create_timer(CREATE_RETRY_DElAY_SEC).timeout
		
	
	if not EOS.is_success(res):
		_log.error("Failed to create EOS Platform: %s" % EOS.result_str(res))
		return res
	
	platform_created.emit()
	
	return res

#endregion


#region Private methods

func _on_logging_callback(p_msg: Dictionary):
	var msg = EOS.Logging.LogMessage.from(p_msg) as EOS.Logging.LogMessage
	log_msg.emit(msg)

#endregion

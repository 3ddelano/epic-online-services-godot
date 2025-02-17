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


#region Private vars 

var _log = HLog.logger("HPlatform")

#endregion


#region Built-in methods

func _ready() -> void:
	IEOS.logging_interface_callback.connect(_on_logging_callback)

#endregion


#region Public methods

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
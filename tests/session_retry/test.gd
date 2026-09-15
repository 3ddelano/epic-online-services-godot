extends SceneTree

class Options extends RefCounted:
	var values: Dictionary
	func _init(data: Dictionary = {}):
		values = data
	func _get(key: StringName):
		return values.get(key)

var eos = Engine.get_singleton("IEOS")
var options_ref: WeakRef
var received: Array = []
var output: Array[String] = []
var failed = false

func report(message: String):
	output.append(message)
	print(message)

func finish(code: int):
	if not OS.get_environment("EOSG_TEST_SHOW") == "1":
		quit(code)
		return
	root.title = "EOSG issue 31 — actual regression test"
	root.size = Vector2i(1100, 680)
	var background = ColorRect.new()
	background.color = Color("17202e")
	background.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	root.add_child(background)
	var label = Label.new()
	label.position = Vector2(32, 32)
	label.add_theme_font_size_override("font_size", 22)
	label.text = "REAL GODOT / COMPILED EOSG — SDK CALLBACK TEST DOUBLE\n\n" + "\n".join(output)
	root.add_child(label)

func check(condition: bool, message: String) -> bool:
	if not condition:
		failed = true
		report("FAIL: " + message)
		finish(1)
	return condition

func _initialize():
	call_deferred("run")

func run():
	report("ISSUE 31 | Godot " + Engine.get_version_info().string)
	var platform = Options.new({
		"product_id": "", "sandbox_id": "", "deployment_id": "",
		"encryption_key": "", "flags": 0, "tick_budget_in_milliseconds": 0,
		"client_id": "", "client_secret": "", "override_country_code": "",
		"override_locale_code": "", "cache_directory": "", "is_server": true,
		"rtc_options": Options.new(),
	})
	if not check(eos.platform_interface_create(platform), "platform setup"): return
	var modification = eos.sessions_interface_create_session_modification(Options.new({
		"session_name": "Issue31", "bucket_id": "test", "max_players": 2,
		"local_user_id": "", "presence_enabled": false, "session_id": "",
		"sanctions_enabled": false, "allowed_platform_ids": [],
	}))
	if not check(modification.result_code == 0, "session setup"): return
	eos.sessions_interface_update_session_callback.connect(on_result)
	var options = Options.new({
		"session_modification": modification.session_modification,
		"client_data": {"marker": "issue31"},
	})
	options_ref = weakref(options)
	eos.sessions_interface_update_session(options)
	options = null # Only the native asynchronous operation now owns the options.
	var retries = int(OS.get_environment("EOSG_TEST_RETRIES"))
	var result = int(OS.get_environment("EOSG_TEST_RESULT"))
	var unsafe_replay = OS.get_environment("EOSG_TEST_UNSAFE_REPLAY") == "1"
	for index in range(retries):
		eos.tick()
		report("After retry %d: options alive=%s" % [index + 1, options_ref.get_ref() != null])
		if not unsafe_replay:
			if not check(options_ref.get_ref() != null, "options freed while SDK will retry"): return
			if not check(received.size() == index + 1 and received[-1] == 19, "retry signal delivery"): return
	eos.tick()
	if not check(received.size() == retries + 1 and received[-1] == result, "terminal signal delivery"): return
	if not check(options_ref.get_ref() == null, "options leaked after completion"): return
	eos.tick()
	if not check(received.size() == retries + 1, "extra completion"): return
	if failed: return
	report("PASS: %d retries -> %d; payload preserved" % [retries, result])
	report("Options released on completion; no extra callback")
	eos.platform_interface_release()
	finish(0)

func on_result(data: Dictionary):
	received.append(data.result_code)
	check(data.client_data == {"marker": "issue31"}, "client_data preserved")
	check(data.session_name == "Issue31", "session name preserved")
	check(data.session_id == ("test-session-id" if data.result_code == 0 else ""), "session id preserved")
	report("Godot signal: result=%d client_data=%s" % [data.result_code, data.client_data])

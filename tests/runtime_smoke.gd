extends SceneTree

class InitializeOptions extends RefCounted:
	var product_name: String = "EOSG compatibility test"
	var product_version: String = "1.0"

var failures: int = 0
var results: PackedStringArray = []

func check(condition: bool, description: String) -> void:
	var line: String = ("PASS: " if condition else "FAIL: ") + description
	results.append(line)
	print(line)
	if not condition:
		failures += 1

func _initialize() -> void:
	call_deferred("run")

func run() -> void:
	print("Engine: ", Engine.get_version_info()["string"])
	check(Engine.has_singleton("IEOS"), "IEOS native singleton loaded")
	check(Engine.has_singleton("EOSGPacketPeerMediator"), "P2P mediator registered")
	check(ClassDB.can_instantiate("EOSGMultiplayerPeer"), "Multiplayer peer registered")
	if ClassDB.can_instantiate("EOSGMultiplayerPeer"):
		var peer = ClassDB.instantiate("EOSGMultiplayerPeer")
		check(peer.get_connection_status() == MultiplayerPeer.CONNECTION_DISCONNECTED,
			"Native multiplayer peer starts disconnected")
	if Engine.has_singleton("IEOS"):
		var eos = Engine.get_singleton("IEOS")
		var result: int = eos.platform_interface_initialize(InitializeOptions.new())
		check(result == 0, "EOS SDK initialize returns EOS_Success (%d)" % result)
		if result == 0:
			result = eos.platform_interface_shutdown()
			check(result == 0, "EOS SDK shutdown returns EOS_Success (%d)" % result)
	print("SMOKE_RESULT=", "PASS" if failures == 0 else "FAIL")
	if "--show" in OS.get_cmdline_user_args():
		set_auto_accept_quit(false)
		root.close_requested.connect(func(): quit(0 if failures == 0 else 1))
		root.title = "EOSG #76 — live runtime compatibility test"
		root.size = Vector2i(960, 560)
		var label := Label.new()
		label.position = Vector2(40, 40)
		label.add_theme_font_size_override("font_size", 24)
		label.text = "EOSG #76 | Godot " + Engine.get_version_info()["string"]
		label.text += "\n\n" + "\n".join(results)
		label.text += "\n\n" + ("ALL CHECKS PASSED" if failures == 0 else "CHECKS FAILED")
		label.text += "\nNative extension + real EOS SDK; no login or credentials."
		root.add_child(label)
	else:
		quit(0 if failures == 0 else 1)

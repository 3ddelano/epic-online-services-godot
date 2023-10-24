extends RefCounted 
class_name EOSGMultiplayerPeer

static var __singleton : IEOSGMultiplayerPeer

static func _static_init():
	__singleton = IEOSGMultiplayerPeer.new()
	print("EOSGMultiplayerPeer singleton initialized")

static func get_singleton():
	return __singleton


static func set_local_user_id(user_id : String):
	IEOSGMultiplayerPeer.set_local_user_id(user_id)

static func get_local_user_id() -> String:
	return IEOSGMultiplayerPeer.get_local_user_id()
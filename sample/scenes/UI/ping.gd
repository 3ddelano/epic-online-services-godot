extends Label


const COUNT = 5


var _pings = []
var _ping_in_progress = false
var _last_ping_time = 0


func _ready() -> void:
	hide()


func _on_timer_timeout() -> void:
	if _ping_in_progress and Time.get_ticks_msec() - _last_ping_time > 2000:
		_ping_in_progress = false

	if not _ping_in_progress and multiplayer.multiplayer_peer and \
	not multiplayer.is_server() and \
	multiplayer.multiplayer_peer.get_connection_status() == MultiplayerPeer.ConnectionStatus.CONNECTION_CONNECTED:
		_ping_in_progress = true
		_last_ping_time = Time.get_ticks_msec()
		ping.rpc_id(1)


@rpc("any_peer")
func ping():
	pong.rpc_id(multiplayer.get_remote_sender_id())


@rpc
func pong():
	var rtt = Time.get_ticks_msec() - _last_ping_time
	_pings.append(rtt)
	if _pings.size() > COUNT:
		_pings.pop_front()
	
	
	var avg = _pings.reduce(func (acc, p): return acc + p, 0.0) / _pings.size()
	
	
	text = "Ping: %s ms" % ceil(avg)
	show()
	_ping_in_progress = false

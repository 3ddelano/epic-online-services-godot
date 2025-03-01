extends Node


#region Public vars


#endregion


#region Private vars 

var _log = HLog.logger("HP2P")

#endregion


#region Public methods


## Returns the current user's NAT type
func get_nat_type_async() -> EOS.P2P.NATType:
	_log.debug("Getting nat type...")
	EOS.P2P.P2PInterface.query_nat_type()
	
	var ret = await IEOS.p2p_interface_query_nat_type_callback
	if not EOS.is_success(ret):
		_log.error("Failed to get nat type: %s" % EOS.result_str(ret))
	else:
		_log.debug("Got nat type: %s" % ret.nat_type)

	return ret.nat_type


## Get the current relay control setting
func get_relay_control() -> EOS.P2P.RelayControl:
	var ret = EOS.P2P.P2PInterface.get_relay_control()
	if not EOS.is_success(ret):
		_log.error("Failed to get relay control: %s" % EOS.result_str(ret))

	return ret.relay_control


## Set how relay servers are to be used
func set_relay_control(relay_control: EOS.P2P.RelayControl) -> EOS.Result:
	return EOS.P2P.P2PInterface.set_relay_control(relay_control)


## Get the current chosen port and the amount of other ports to try above the chosen port if the chosen port is unavailable.
## Returns a [Dictionary] with the following keys:
##   - result_code: EOS.Result
##   - port: int
##   - max_additional_ports_to_try: int
func get_port_range() -> Dictionary:
	return EOS.P2P.P2PInterface.get_port_range()


## Set configuration options related to network ports.
func set_port_range(port: int, max_additional_ports_to_try: int) -> EOS.Result:
	var opts = EOS.P2P.SetPortRangeOptions.new()
	opts.port = port
	opts.max_additional_ports_to_try = max_additional_ports_to_try
	return EOS.P2P.P2PInterface.set_port_range(opts)


## Gets the current cached information related to the incoming and outgoing packet queues
## Returns a [Dictionary] with the following keys:
##   - result_code: EOS.Result
##   - incoming_packet_queue_current_packet_count: int
##   - incoming_packet_queue_current_size_bytes: int
##   - incoming_packet_queue_max_size_bytes: int
##   - outgoing_packet_queue_current_packet_count: int
##   - outgoing_packet_queue_current_size_bytes: int
##   - outgoing_packet_queue_max_size_bytes: int
func get_packet_queue_info():
	return EOS.P2P.P2PInterface.get_packet_queue_info()


## Sets the maximum packet queue sizes that packets waiting to be sent or received can use.
func set_packet_queue_size(incoming_packet_queue_max_size_bytes: int, outgoing_packet_queue_max_size_bytes: int) -> EOS.Result:
	var opts = EOS.P2P.SetPacketQueueSizeOptions.new()
	opts.incoming_packet_queue_max_size_bytes = incoming_packet_queue_max_size_bytes
	opts.outgoing_packet_queue_max_size_bytes = outgoing_packet_queue_max_size_bytes
	return EOS.P2P.P2PInterface.set_packet_queue_size(opts)

#endregion

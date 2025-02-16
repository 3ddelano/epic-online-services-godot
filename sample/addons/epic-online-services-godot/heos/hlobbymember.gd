## A high-level lobby member wrapper for EOSG
class_name HLobbyMember
extends BaseClass


#region Signals


## Emitted when the member's rtc state changes
signal rtc_state_updated

#endregion

#region Public vars

var product_user_id: String
var epic_account_id: String
var display_name: String

## [Array] of [Dictionary] having {"key": String, "value": Variant, "visibility": [enum EOS.Lobby.LobbyAttributeVisibility] }
var attributes = []

var rtc_state = {
	is_in_rtc_room = false,
	is_talking = false,
	is_locally_muted = false,
	is_hard_muted = false,
	is_audio_output_disabled = false,
}

#endregion


#region Private vars

var _attributes_to_add = []

var _lobby: HLobby

var _log = HLog.logger("HLobbyMember")

#endregion


#region Built-in methods

func _init(p_lobby: HLobby) -> void:
	super._init("HLobbyMember")

	_lobby = p_lobby

#endregion


#region Public methods

## Returns the member's attribute as [Dictionary] or empty [Dictionary] if not found
func get_attribute(key: String):
	for attr in attributes:
		if attr.key == key:
			return attr
	return {}


## Returns true if this member is muted
func is_muted() -> bool:
	if HAuth.product_user_id == product_user_id:
		return rtc_state.is_audio_output_disabled
	else:
		return rtc_state.is_locally_muted


## Mute/Unmute this member.
## If the member is the current user, it will mute/unmute the audio output.
## If the member is a remote user, it will locally mute/unmute the remote user's audio.
func mute_member_async() -> bool:
	var _is_muted = is_muted()
	var action = "Unmuting" if _is_muted else "Muting"

	if not _lobby or not _lobby.rtc_room_name:
		return false
	
	if HAuth.product_user_id == product_user_id:
		# Toggle our mute/unmute status
		_log.debug("%s ourself" % action)
		
		var opts := EOS.RTCAudio.UpdateSendingOptions.new()
		opts.room_name = _lobby.rtc_room_name
		opts.audio_status = EOS.RTCAudio.AudioStatus.Enabled if _is_muted else EOS.RTCAudio.AudioStatus.Disabled
		
		EOS.RTCAudio.RTCAudioInterface.update_sending(opts)
		var ret = await IEOS.rtc_audio_interface_update_sending_callback
		if not EOS.is_success(ret):
			_log.error("Failed to update audio sending: result_code=%s" % EOS.result_str(ret))
			return false
		
		var did_update = HLobby._check_diff_and_set_dict(rtc_state, "is_audio_output_disabled", ret.audio_status == EOS.RTCAudio.AudioStatus.Disabled, "member:" + product_user_id)
		if did_update:
			rtc_state_updated.emit()
			_lobby.lobby_updated.emit()

		return true
	else:
		_log.debug("%s member: product_user_id=%s" % [action, product_user_id])
		# Locally mute the remote user's audio
		var opts := EOS.RTCAudio.UpdateReceivingOptions.new()
		opts.room_name = _lobby.rtc_room_name
		opts.participant_id = product_user_id
		opts.audio_enabled = _is_muted

		EOS.RTCAudio.RTCAudioInterface.update_receiving(opts)
		var ret = await IEOS.rtc_audio_interface_update_receiving_callback
		if not EOS.is_success(ret):
			_log.error("Failed to update audio receiving: result_code=%s" % EOS.result_str(ret))
			return false
		
		var did_update = HLobby._check_diff_and_set_dict(rtc_state, "is_locally_muted", ret.audio_enabled == false, "member:" + product_user_id)
		if did_update:
			rtc_state_updated.emit()
			_lobby.lobby_updated.emit()
	
	return true


## Kick this member from the lobby. Only lobby owner can kick a member.
func kick_member_async() -> bool:
	if not _lobby.is_owner():
		return false
	
	_log.debug("Kicking member: product_user_id=%s" % product_user_id)

	var opts = EOS.Lobby.KickMemberOptions.new()
	opts.target_user_id = product_user_id

	EOS.Lobby.LobbyInterface.kick_member(opts)
	var ret = await IEOS.lobby_interface_kick_member_callback
	if not EOS.is_success(ret):
		_log.error("Failed to kick member: result_code=%s" % EOS.result_str(ret))
		return false
	
	_log.debug("Kick member was successful: product_user_id=%s" % product_user_id)

	return true


## Returns true if the member is the current logged in user
func is_self() -> bool:
	return product_user_id == HAuth.product_user_id

#endregion
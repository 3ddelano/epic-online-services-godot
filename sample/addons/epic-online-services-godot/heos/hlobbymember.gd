class_name HLobbyMember
extends BaseClass

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
	mute_action_in_progress = false,
	hard_mute_action_in_progress = false,
}

#endregion


#region Built-in methods

func _init() -> void:
	super._init("HLobbyMember")

#endregion


#region Public methods

func get_attribute(key: String):
	for attr in attributes:
		if attr.key == key:
			return attr
	return null


func add_attribute(key: String, value: Variant, visibility = EOS.Lobby.LobbyAttributeVisibility.Public) -> Dictionary:
	var attr = HLobby.make_attribute(key, value, visibility)
	attributes.append(attr)
	return attr

#endregion


#region Private methods

#endregion

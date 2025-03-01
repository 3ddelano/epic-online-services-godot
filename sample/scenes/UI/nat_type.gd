extends RichTextLabel


#region built-in methods

func _ready() -> void:
	HAuth.logged_in.connect(_on_logged_in)
	HAuth.logged_out.connect(_on_logged_out)

#endregion


#region private methods

func _on_logged_in():
	var nat_type := await HP2P.get_nat_type_async()
	
	match nat_type:
		EOS.P2P.NATType.Open:
			text = "[right]NAT: [color=#00ff00]Open[/color][/right]"
		EOS.P2P.NATType.Moderate:
			text = "[right]NAT: [color=#ffff00]Moderate[/color][/right]"
		EOS.P2P.NATType.Strict:
			text = "[right]NAT: [color=#ff0000]Strict[/color][/right]"
		_:
			text = "[right]NAT: [color=#515151]Unknown[/color][/right]"

	show()


func _on_logged_out():
	text = ""
	hide()
#endregion

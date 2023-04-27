class_name NotificationsView
extends MarginContainer

@onready var vb = $SC/PC/MC/VB

func add_notification(node: Node):
	vb.add_child(node)
	node.fade_in()

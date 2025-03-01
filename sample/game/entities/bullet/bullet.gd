class_name Bullet
extends Area2D


const SPEED = 2000.0


var dmg_amt = 25
var owner_peer_id: int
var owner_puid: String


func _physics_process(delta: float) -> void:
	position += transform.x * SPEED * delta


func _on_visible_on_screen_notifier_2d_screen_exited() -> void:
	if is_multiplayer_authority():
		remove_bullet.rpc()


@rpc("call_local", "reliable")
func remove_bullet():
	queue_free()


func _on_body_entered(body: Node2D) -> void:
	if not is_multiplayer_authority():
		return
	
	if body is Player:
		body.take_damage.rpc_id(body.peer_id, dmg_amt, owner_peer_id)
	
	remove_bullet.rpc()

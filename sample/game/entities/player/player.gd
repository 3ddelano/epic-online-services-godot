class_name Player
extends CharacterBody2D


signal health_changed
signal died(player, killing_peer_id: int)


const SPEED = 180.0
const SHOOT_COOLDOWN = 0.3
const BULLET = preload("res://game/entities/bullet/bullet.tscn")


var peer_id: int
## Remote product user id
var puid: String

var score = 0
var health = 100
var shoot_cooldown = SHOOT_COOLDOWN

@onready var _gun_container: Node2D = %GunContainer
@onready var _muzzle: Node2D = %Muzzle
@onready var _panel: Panel = $Panel

#region Built-in methods

func _enter_tree() -> void:
	set_multiplayer_authority(peer_id)


func _ready() -> void:
	Store.player_score_changed.emit()
	if not is_multiplayer_authority():
		_panel.self_modulate = Color("#ff0000")


func _physics_process(delta: float) -> void:
	if not is_multiplayer_authority(): return
	
	_handle_move()
	_handle_gun_rotation()
	
	if shoot_cooldown > -0.1:
		shoot_cooldown -= delta
	
	if not Store.is_mobile:
		if Input.is_action_pressed(&"shoot") && shoot_cooldown <= 0:
			_handle_shoot()
	else:
		if Store.shoot_joystick.get_deadzoned_vector().length_squared() > 0.1 && shoot_cooldown <= 0:
			_handle_shoot()

#endregion


#region Public methods

@rpc("any_peer", "call_local", "reliable")
func take_damage(amt: int, owner_peer_id: int):
	health = max(0, health - amt)
	health_changed.emit()
	if health == 0:
		died.emit(self, owner_peer_id)
		Store.network.add_score.rpc(owner_peer_id, 10)


@rpc("call_local", "reliable")
func shoot(owner_peer_id: int, owner_puid: String, bname: String):
	var bullet: Bullet = BULLET.instantiate()
	bullet.owner_peer_id = owner_peer_id
	bullet.owner_puid = owner_puid
	bullet.name = bname
	bullet.set_multiplayer_authority(owner_peer_id)
	get_parent().add_child(bullet)
	bullet.transform = _muzzle.global_transform

#endregion


#region Private methods

func _handle_move():
	var direction := Input.get_vector("move_left", "move_right", "move_up", "move_down")
	velocity = direction * SPEED
	move_and_slide()


func _handle_gun_rotation():
	if not Store.is_mobile:
		_gun_container.look_at(get_global_mouse_position())
	else:
		print(Store.shoot_joystick.get_deadzoned_vector())
		_gun_container.look_at(global_position + Store.shoot_joystick.get_deadzoned_vector())


func _handle_shoot():
	shoot_cooldown = SHOOT_COOLDOWN
	var bname = str(peer_id) + "_" + str(randi_range(1000, 9999))
	shoot.rpc(peer_id, puid, bname)

#endregion

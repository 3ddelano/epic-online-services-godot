## Credentials required by Epic Online Services. Get these values from the [url=https://dev.epicgames.com/portal/en-US/]Dev Portal[/url]
class_name HCredentials
extends BaseClass


func _init() -> void:
	super._init("HCredentials")


## Game name (max 64 characters)[br]Eg. "My Awesome Game"
var product_name: String

## Game version[br]Eg. "1.0"
var product_version: String

## Product ID of the game, provided by Epic Games
var product_id: String

## Sandbox ID of the game, provided by Epic Games
var sandbox_id: String

## Deployment ID of the game, provided by Epic Games
var deployment_id: String

## Client ID to use for the game, provided by Epic Games
var client_id: String

## Client Secret to use for the game, provided by Epic Games
var client_secret: String

## (Optional) 256-bit Encryption Key for file encryption in hexadecimal format (64 hex characters)
var encryption_key: String

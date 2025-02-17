extends Node


#region Public vars

## The maximum number of lobbies to return in search calls.
## Except for create_saerch and search_async
var max_search_results = 25

## Whether to show the lobby in the user's presence
var presence_enabled = true

## Local RTC options for the lobby
## (Optional) Set this to a [Dictionary] to override the defaults.[br]
## A [Dictionary] with keys: [br]
## - flags: A bitwise-or union of [enum EOS.RTC.JoinRoomFlags],[br]
## - use_manual_audio_input: [bool],[br]
## - use_manual_audio_output: [bool],[br]
## - local_audio_device_input_starts_muted: [bool]
var local_rtc_options = {
	flags = EOS.RTC.JoinRoomFlags.EnableDataChannel
}

#endregion


#region Private vars 

var _log = HLog.logger("HLobbies")

#endregion


#region Public methods

## Create a new lobby. Returns [HLobby] or [null]
func create_lobby_async(opts: EOS.Lobby.CreateLobbyOptions) -> HLobby:
	_log.debug("Creating lobby...")
	EOS.Lobby.LobbyInterface.create_lobby(opts)

	var ret = await IEOS.lobby_interface_create_lobby_callback
	if not EOS.is_success(ret):
		_log.error("Failed to create lobby: %s" % EOS.result_str(ret))
		return null
	
	_log.debug("Lobby created: lobby_id=%s" % ret.lobby_id)

	var lobby = HLobby.new()
	lobby.init_from_id(ret.lobby_id)
	return lobby


## Join an existing lobby by id. It should only be used if the lobby has had Join-by-ID enabled.
## Additionally, Join-by-ID should only be enabled to support native invites on an integrated platform.
## Returns [HLobby] or [null]
func join_by_id_async(lobby_id: String) -> HLobby:
	_log.debug("Joining lobby by id: lobby_id=%s" % lobby_id)

	var opts = EOS.Lobby.JoinLobbyByIdOptions.new()
	opts.lobby_id = lobby_id
	opts.presence_enabled = presence_enabled
	opts.local_rtc_options = local_rtc_options
	EOS.Lobby.LobbyInterface.join_lobby_by_id(opts)

	var ret = await IEOS.lobby_interface_join_lobby_by_id_callback
	if not EOS.is_success(ret):
		_log.error("Failed to join lobby: result_code=%s" % EOS.result_str(ret))
		return null

	_log.debug("Lobby joined: lobby_id=%s" % lobby_id)

	var lobby = HLobby.new()
	lobby.init_from_id(lobby_id)
	return lobby


## Join an existing lobby ex one returned from search. 
## Returns [HLobby] or [null]
func join_async(lobby: HLobby):
	if not lobby:
		return null
	_log.debug("Joining lobby ...")

	var opts = EOS.Lobby.JoinLobbyOptions.new()
	opts.lobby_details = lobby._lobby_details
	opts.presence_enabled = presence_enabled
	opts.local_rtc_options = local_rtc_options
	EOS.Lobby.LobbyInterface.join_lobby(opts)

	var ret = await IEOS.lobby_interface_join_lobby_callback
	if not EOS.is_success(ret):
		_log.error("Failed to join lobby: result_code=%s" % EOS.result_str(ret))
		return null
	
	_log.debug("Lobby joined: lobby_id=%s" % ret.lobby_id)

	var new_lobby = HLobby.new()
	new_lobby.init_from_id(ret.lobby_id)
	return new_lobby


## Search for public lobbies that a user is in.
## Returns [Array] of [HLobby] or null
func search_by_product_user_id_async(product_user_id: String):
	_log.debug("Searching for lobbies by product user id: product_user_id=%s" % product_user_id)
	var opts = EOS.Lobby.CreateLobbySearchOptions.new()
	opts.max_results = max_search_results
	
	var search: EOSGLobbySearch = create_search(opts)
	if not search:
		return null
	
	search.set_target_user_id(product_user_id)

	return await search_async(search)



## Search for public lobbies based on an attributes.
## The input argument can either be a [Dictionary] of an [Array] of [Dictionary].
## (Note there is an implicit AND operation if multiple search attributes are provided)
## Each search attribute is a Dictionary having the keys:
##  key: String - the key of the attribute
##  value: String - the value of the attribute
##  comparison: EOS.ComparisonOp - Type of comparison to make (default is EOS.ComparisonOp.Equal)
## Returns [Array] of [HLobby] or null
func search_by_attribute_async(attributes):
	if typeof(attributes) == TYPE_DICTIONARY:
		attributes = [attributes]
	_log.debug("Searching for lobbies by attributes: attributes=%s" % attributes)
	var opts = EOS.Lobby.CreateLobbySearchOptions.new()
	opts.max_results = max_search_results
	
	var search: EOSGLobbySearch = create_search(opts)
	if not search:
		return null
	
	for attr in attributes:
		if attr.has("comparison"):
			search.set_parameter(attr.key, attr.value, attr.comparison)
		else:
			search.set_parameter(attr.key, attr.value, EOS.ComparisonOp.Equal)

	return await search_async(search)


## Search for lobby by id. At most will return one lobby.
## Returns [Array] of [HLobby] or null
func search_by_lobby_id_async(lobby_id: String):
	_log.debug("Searching for lobbies by lobby id: lobby_id=%s" % lobby_id)
	var opts = EOS.Lobby.CreateLobbySearchOptions.new()
	opts.max_results = max_search_results
	
	var search: EOSGLobbySearch = create_search(opts)
	if not search:
		return null
	
	search.set_lobby_id(lobby_id)

	return await search_async(search)


## Search for lobby by bucket id.
## Returns [Array] of [HLobby] or null
func search_by_bucket_id_async(bucket_id: String):
	_log.debug("Searching for lobbies by bucket id: bucket_id=%s" % bucket_id)
	return await HLobbies.search_by_attribute_async({
		key = EOS.Lobby.SEARCH_BUCKET_ID,
		value = bucket_id,
		comparison = EOS.ComparisonOp.Equal
	})


## (Advanced) Create a new lobby search. Returns [EOSGLobbySearch] or null. Use [search_async] to perform the search.
func create_search(opts: EOS.Lobby.CreateLobbySearchOptions):
	_log.debug("Creating lobby search...")
	
	var create_ret = EOS.Lobby.LobbyInterface.create_lobby_search(opts)
	if not EOS.is_success(create_ret):
		_log.error("Failed to create lobby search: result_code=%s" % EOS.result_str(create_ret))
		return null
	
	return create_ret.lobby_search


## (Advanced) Perform the lobby search. Returns [Array] of [HLobby] or null
func search_async(lobby_search: EOSGLobbySearch):
	_log.debug("Searching for lobbies...")

	lobby_search.find(HAuth.product_user_id)

	var search_ret = await IEOS.lobby_search_find_callback
	if not EOS.is_success(search_ret):
		_log.error("Failed to search for lobbies: result_code=%s" % EOS.result_str(search_ret))
		return null
	
	var count = lobby_search.get_search_result_count()
	_log.debug("Found lobbies: count=%s" % count)
	
	var lobbies: Array[HLobby] = []
	for search_idx in count:
		var copy_ret = lobby_search.copy_search_result_by_index(search_idx)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy lobby search result by index: result_code=%s" % EOS.result_str(copy_ret))
			continue
	
		var lobby = HLobby.new()
		var details = copy_ret.lobby_details
		lobby._init_from_details(details)
		lobbies.append(lobby)
	
	return lobbies

#endregion

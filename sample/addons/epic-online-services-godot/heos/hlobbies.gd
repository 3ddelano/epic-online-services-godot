extends Node

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


## Join an existing lobby. Returns [HLobby] or [null]
func join_by_id_async(lobby_id: String) -> HLobby:
	_log.debug("Joining lobby: lobby_id=%s" % lobby_id)

	var opts = EOS.Lobby.JoinLobbyByIdOptions.new()
	opts.lobby_id = lobby_id
	EOS.Lobby.LobbyInterface.join_lobby_by_id(opts)

	var join_ret = await IEOS.lobby_interface_join_lobby_by_id_callback
	if not EOS.is_success(join_ret):
		_log.error("Failed to join lobby: result_code=%s" % EOS.result_str(join_ret))
		return null

	_log.debug("Lobby joined: lobby_id=%s" % lobby_id)

	var lobby = HLobby.new()
	lobby.init_from_id(lobby_id)
	return lobby


## Create a new lobby search. Returns [EOSGLobbySearch] or null. Use [search_async] to perform the search.
func create_search(opts: EOS.Lobby.CreateLobbySearchOptions):
	_log.debug("Creating lobby search...")
	
	var create_ret = EOS.Lobby.LobbyInterface.create_lobby_search(opts)
	if not EOS.is_success(create_ret):
		_log.error("Failed to create lobby search: result_code=%s" % EOS.result_str(create_ret))
		return null
	
	return create_ret.lobby_search


## Perform the lobby search. Returns [Array] of [HLobby] or null
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
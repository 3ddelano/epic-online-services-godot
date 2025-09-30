extends Node


#region Public vars

var max_search_results = 25

#endregion


#region Private vars 

var _log = HLog.logger("HSessions")

#endregion


#region Public methods

func create_session(opts: EOS.Sessions.CreateSessionModificationOptions) -> EOSGSessionModification:
	_log.debug("Creating session...")
	var ret = EOS.Sessions.SessionsInterface.create_session_modification(opts)
	if not EOS.is_success(ret):
		_log.error("Failed to create session: %s" % EOS.result_str(ret))
		return null
	
	var session_mod = ret.session_modification
	
	_log.debug("Session created session_name=%s, session_id=%s" % [opts.session_name, opts.session_id])
	
	return session_mod


func update_session_async(p_session: EOSGSessionModification) -> bool:
	_log.debug("Updating session...")
	
	var opts := EOS.Sessions.UpdateSessionOptions.new()
	opts.session_modification = p_session
	EOS.Sessions.SessionsInterface.update_session(opts)
	var ret = await IEOS.sessions_interface_update_session_callback
	if not EOS.is_success(ret):
		_log.error("Failed to update session: %s" % EOS.result_str(ret))
		return false
	
	_log.debug("Session updated session_name=%s, session_id=%s" % [ret.session_name, ret.session_id])
	return true


func get_active_session(p_session_name: String) -> EOSGActiveSession:
	_log.debug("Getting active session... session_name=%s" % p_session_name)
	
	var opts = EOS.Sessions.CopyActiveSessionDetailsOptions.new()
	opts.session_name = p_session_name
	var ret = EOS.Sessions.SessionsInterface.copy_active_session_details(opts)
	if not EOS.is_success(ret):
		_log.error("Failed to get active session: %s" % EOS.result_str(ret))
		return null
	
	_log.debug("Got active session session_name=%s" % p_session_name)
	return ret.active_session


func join_async(session: EOSGSessionDetails, session_name: String, presence_enabled = false) -> bool:
	if not session:
		return false
	var session_id = session.copy_info().info.session_id
	_log.debug("Joining session... session_name=%s, session_id=%s" % [session_name, session_id])

	var opts = EOS.Sessions.JoinSessionOptions.new()
	opts.session_details = session
	opts.presence_enabled = presence_enabled
	opts.session_name = session_name
	EOS.Sessions.SessionsInterface.join_session(opts)

	var ret = await IEOS.sessions_interface_join_session_callback
	if not EOS.is_success(ret):
		_log.error("Failed to join session session_id=%s: result_code=%s" % [session_id, EOS.result_str(ret)])
		return false

	_log.debug("Session joined: session_id=%s" % session_id)
	return true


func search_by_attribute_async(attributes):
	if typeof(attributes) == TYPE_DICTIONARY:
		attributes = [attributes]
	
	_log.debug("Searching for session by attributes: attributes=%s" % str(attributes))
	var opts = EOS.Sessions.CreateSessionSearchOptions.new()
	opts.max_search_results = max_search_results
	
	var search := create_search(opts)
	if not search:
		return null
	
	for attr in attributes:
		if attr.has("comparison"):
			search.set_parameter(attr.key, attr.value, attr.comparison)
		else:
			search.set_parameter(attr.key, attr.value, EOS.ComparisonOp.Equal)

	return await search_async(search)


## (Advanced)
func create_search(opts: EOS.Sessions.CreateSessionSearchOptions) -> EOSGSessionSearch:
	_log.debug("Creating session search...")
	
	var create_ret = EOS.Sessions.SessionsInterface.create_session_search(opts)
	if not EOS.is_success(create_ret):
		_log.error("Failed to create session search: result_code=%s" % EOS.result_str(create_ret))
		return null
	
	return create_ret.session_search


## (Advanced)
func search_async(session_search: EOSGSessionSearch):
	_log.debug("Searching for sessions...")

	session_search.find(HAuth.product_user_id)

	var search_ret = await IEOS.session_search_find_callback
	if not EOS.is_success(search_ret):
		_log.error("Failed to search for sessions: result_code=%s" % EOS.result_str(search_ret))
		return null
	
	var count = session_search.get_search_result_count()
	_log.debug("Found sessions: count=%s" % count)
	
	var sessions: Array = []
	for search_idx in count:
		var copy_ret = session_search.copy_search_result_by_index(search_idx)
		if not EOS.is_success(copy_ret):
			_log.error("Failed to copy session search result by index: result_code=%s" % EOS.result_str(copy_ret))
			continue
	
		var details = copy_ret.session_details
		sessions.append(details)
	
	return sessions

#endregion

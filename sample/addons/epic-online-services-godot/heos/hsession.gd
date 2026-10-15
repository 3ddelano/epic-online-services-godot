### A high-level session wrapper for EOSG
#class_name HSession
#extends BaseClass
#
#
##region Signals
#
#signal session_updated
#
##endregion
#
#
##region Public vars
#
#var session_name: String
#var session_id: String
#var host_address: String
#var num_open_public_connections: int
#var owner_user_id: String
#var owner_server_client_id: String
#
## Settings
#var bucket_id: String
#var num_public_connections: int
#var allow_join_in_progress: bool
#var permission_level: EOS.Sessions.OnlineSessionPermissionLevel
#var invites_allowed: bool
#var sanctions_enabled: bool
#var allowed_platform_ids = []
#var max_players: int
#
#var attributes = []
#var presence_enabled: bool
#
##endregion
#
#
##region Private vars
#
#var _log = HLog.logger("HSession")
#
#var _attributes_to_add = []
#var _session_details: EOSGSessionDetails
#
##endregion
#
#
##region Built-in methods
#
#func _init() -> void:
	#super._init("HSession")
#
#
#func _notification(what: int) -> void:
	#if what == NOTIFICATION_PREDELETE:
		#_session_details = null
#
##endregion
#
#
##region Public methods
#
### Check whether the session is valid
#func is_valid() -> bool:
	#return session_name != ""
#
#
### Check whether a user is the session owner
### By default it will check the current logged in user
#func is_owner(product_user_id = HAuth.product_user_id) -> bool:
	#return owner_user_id == product_user_id
#
#
### Intialize the session from a session_name
#func init_from_session_name(p_session_name: String):
	#_log.debug("Initializing from session name: %s" % p_session_name)
	#session_name = p_session_name
#
	#_copy_session_data()
#
#
### Returns [Dictionary] or empty [Dictionary] if not found.
#func get_attribute(key: String):
	#for attr in attributes:
		#if attr.key == key:
			#return attr
	#return {}
#
#
### Add an attribute to this session. Make sure to call update_async to actually update the session. Returns true if success.
#func add_attribute(key: String, value: Variant, advertisement_type = EOS.Sessions.SessionAttributeAdvertisementType.Advertise) -> bool:
	#var attr = make_attribute(key, value, advertisement_type)
	#_attributes_to_add.append(attr)
	#return true
#
#
### Helper function to create an attribute with key, value and advertisement type
#static func make_attribute(key: String, value: Variant, advertisement_type = EOS.Sessions.SessionAttributeAdvertisementType.Advertise) -> Dictionary:
	#return {
		#key = key,
		#value = value,
		#advertisement_type = advertisement_type
	#}
#
#
### Update the session, if any changes are pending
#func update_async() -> bool:
	#_log.debug("Updating session: session_name=%s" % session_name)
	#if not is_valid():
		#_log.error("Cannot update invalid session")
		#return false
#
	#var opts = EOS.Sessions.UpdateSessionModificationOptions.new()
	#opts.session_name = session_name
#
	#var sess_mod_ret = EOS.Sessions.SessionsInterface.update_session_modification(opts)
	#if not EOS.is_success(sess_mod_ret):
		#_log.error("Failed to update session modification: result_code=%s" % EOS.result_str(sess_mod_ret))
		#return false
	#
	#var session_mod: EOSGSessionModification = sess_mod_ret.session_modification
	#return await _init_from_session_mod(session_mod)
#
#
#func _init_from_session_mod(session_mod: EOSGSessionModification):
	#if is_owner(HAuth.product_user_id):
		#_log.debug("Updating session as owner...")
		#if allowed_platform_ids.size() > 0:
			#var set_allowed_platform_ids_ret = session_mod.set_allowed_platform_ids(allowed_platform_ids)
			#if not EOS.is_success(set_allowed_platform_ids_ret):
				#_log.error("Failed to set allowed platform ids on session modification: result_code=%s" % EOS.result_str(set_allowed_platform_ids_ret))
				#return false
#
		#if bucket_id:
			#var set_bucket_ret = session_mod.set_bucket_id(bucket_id)
			#if not EOS.is_success(set_bucket_ret):
				#_log.error("Failed to set bucket id on session modification: result_code=%s" % EOS.result_str(set_bucket_ret))
				#return false
##
		#var set_invites_allowed_ret = session_mod.set_invites_allowed(invites_allowed)
		#if not EOS.is_success(set_invites_allowed_ret):
			#_log.error("Failed to set invites allowed on session modification: result_code=%s" % EOS.result_str(set_invites_allowed_ret))
			#return false
##
		#var set_max_players_ret = session_mod.set_max_players(max_players)
		#if not EOS.is_success(set_max_players_ret):
			#_log.error("Failed to set max players on session modification: result_code=%s" % EOS.result_str(set_max_players_ret))
			#return false
		#
		#var set_perm_ret = session_mod.set_permission_level(permission_level)
		#if not EOS.is_success(set_perm_ret):
			#_log.error("Failed to set permission level on session modification: result_code=%s" % EOS.result_str(set_perm_ret))
			#return false
		#
		#if host_address:
			#var set_host_addr_ret = session_mod.set_host_address(host_address)
			#if not EOS.is_success(set_host_addr_ret):
				#_log.error("Failed to set host address on session modification: result_code=%s" % EOS.result_str(set_host_addr_ret))
				#return false
		#
		#var set_join_in_progress_allowed_ret = session_mod.set_join_in_progress_allowed(allow_join_in_progress)
		#if not EOS.is_success(set_join_in_progress_allowed_ret):
			#_log.error("Failed to set join in progress allowed on session modification: result_code=%s" % EOS.result_str(set_join_in_progress_allowed_ret))
			#return false
##
		#var new_attrs = attributes.duplicate(true)
		#new_attrs.append_array(_attributes_to_add)
		#for attr in new_attrs:
			#var add_ret = session_mod.add_attribute(attr.key, attr.value, attr.advertisement_type)
			#if not EOS.is_success(add_ret):
				#_log.error("Failed to add attribute on session modification. Skipping this attribute: result_code=%s, key=%s, value=%s, advertisement_type=%s" % [EOS.result_str(add_ret), attr.key, attr.value, attr.advertisement_type])
		#_attributes_to_add = []
	#
	#else:
		#_log.debug("Updating session as user...")
#
	#var update_opts = EOS.Sessions.UpdateSessionOptions.new()
	#update_opts.session_modification = session_mod
	#EOS.Sessions.SessionsInterface.update_session(update_opts)
	#
	#var update_ret = await IEOS.sessions_interface_update_session_callback
	#if not EOS.is_success(update_ret):
		#_log.error("Failed to update session: result_code=%s. session_name=%s" % [EOS.result_str(update_ret), session_name])
		#return false
	#
	#_log.debug("Session updated successfully: session_name=%s" % session_name)
#
	#_copy_session_data()
	#session_updated.emit()
#
	#return true
	#
#func destroy_async() -> bool:
	#return false
#
#
##endregion
#
#
##region Private methods
#
#func _copy_session_data():
	#var copy_opts = EOS.Sessions.CopyActiveSessionDetailsOptions.new()
	#copy_opts.session_name = session_name
	#var copy_ret = EOS.Sessions.SessionsInterface.copy_active_session_details(copy_opts)
#
	#if not EOS.is_success(copy_ret):
		#_log.error("Failed to copy session details: result_code=%s" % EOS.result_str(copy_ret))
		#return
#
	#_init_from_active_session(copy_ret.active_session)
#
#
#func _init_from_active_session(active_session: EOSGActiveSession):
	#_log.debug("Initializing from EOSGActiveSession")
	#var copy_ret = active_session.copy_info()
	#if not EOS.is_success(copy_ret):
		#_log.error("Failed to copy session info: result_code=%s" % EOS.result_str(copy_ret))
		#return
	#
	#var details: Dictionary = copy_ret.info
	#
#
#func _init_from_details(session_details: EOSGSessionDetails):
	#_log.debug("Initializing from EOSGSessionDetails")
#
	#_session_details = session_details
	#var copy_ret = session_details.copy_info()
	#if not EOS.is_success(copy_ret):
		#_log.error("Failed to copy session info: result_code=%s" % EOS.result_str(copy_ret))
		#return
#
	#var details: Dictionary = copy_ret.info
#
#func _init_from_session_details(details: Dictionary):
	#HLog._check_diff_and_set(self, "session_id", details.session_id)
	#HLog._check_diff_and_set(self, "host_address", details.host_address)
	#HLog._check_diff_and_set(self, "num_open_public_connections", details.num_open_public_connections)
	#HLog._check_diff_and_set(self, "owner_user_id", details.owner_user_id)
	#HLog._check_diff_and_set(self, "owner_server_client_id", details.owner_server_client_id)
	#HLog._check_diff_and_set(self, "bucket_id", details.settings.bucket_id)
	#HLog._check_diff_and_set(self, "num_public_connections", details.settings.num_public_connections)
	#HLog._check_diff_and_set(self, "allow_join_in_progress", details.settings.allow_join_in_progress)
	#HLog._check_diff_and_set(self, "permission_level", details.settings.permission_level)
	#HLog._check_diff_and_set(self, "invites_allowed", details.settings.invites_allowed)
	#HLog._check_diff_and_set(self, "sanctions_enabled", details.settings.sanctions_enabled)
	#HLog._check_diff_and_set(self, "allowed_platform_ids", details.settings.allowed_platform_ids)
#
	## Get attributes
	#var attr_count = session_details.get_session_attribute_count()
	#var orig_attributes = attributes.duplicate(true)
	#attributes = []
	#for attr_idx in attr_count:
		#var copy_attr_ret = session_details.copy_session_attribute_by_index(attr_idx)
		#if not EOS.is_success(copy_attr_ret):
			#_log.error("Failed to copy EOSGSessionDetails attribute by index: result_code=%s" % EOS.result_str(copy_attr_ret))
			#continue
		#var attr = copy_attr_ret.attribute
		#attr = make_attribute(attr.data.key, attr.data.value, attr.advertisement_type)
		#attributes.append(attr)
	#
	#HLog._check_attr_diff(orig_attributes, attributes, "session")
#
##endregion

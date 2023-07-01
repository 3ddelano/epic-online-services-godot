class_name FriendsView
extends VBoxContainer

# Local cache of friends data
var friends = []

@onready var loginwithepic = $VB/LoginWithEpic
@onready var friend_richtextlabel = $VB/FriendsRichTextLabel


func _ready() -> void:
	var _c
	_c = Store.login_success.connect(_on_login_success)
	_c = Store.logout_success.connect(_on_logout_success)
	_c = EOS.get_instance().friends_interface_query_friends_callback.connect(_on_query_friends_callback)
	_c = EOS.get_instance().user_info_interface_query_user_info_callback.connect(_on_query_user_info_callback)


func _on_login_success():

	if Store.epic_account_id == "":
		loginwithepic.visible = true
		return

	loginwithepic.visible = false

	var query_friends_options = EOS.Friends.QueryFriendsOptions.new()
	query_friends_options.local_user_id = Store.epic_account_id
	EOS.Friends.FriendsInterface.query_friends(query_friends_options)


func _on_logout_success():
	friends = []
	_update_friends_list()
	loginwithepic.visible = true


func _on_query_friends_callback(data: Dictionary):
	print("--- Friends: query_friends_callback: ", EOS.result_str(data))

	var get_friends_count_options = EOS.Friends.GetFriendsCountOptions.new()
	get_friends_count_options.local_user_id = Store.epic_account_id
	var friends_count = EOS.Friends.FriendsInterface.get_friends_count(get_friends_count_options)
	print("Friends Count: ", friends_count)

	friends = []
	for i in friends_count:
		var get_friend_options = EOS.Friends.GetFriendAtIndexOptions.new()
		get_friend_options.local_user_id = Store.epic_account_id
		get_friend_options.index = i

		var friend_id = EOS.Friends.FriendsInterface.get_friend_at_index(get_friend_options)
		var query_user_info_options = EOS.UserInfo.QueryUserInfoOptions.new()
		query_user_info_options.local_user_id = Store.epic_account_id
		query_user_info_options.target_user_id = friend_id
		query_user_info_options.client_data = "friends_list"
		EOS.UserInfo.UserInfoInterface.query_user_info(query_user_info_options)


func _on_query_user_info_callback(data: Dictionary):
	print("--- Friends: UserInfo: query_user_info_callback: ", EOS.result_str(data))
	if data.client_data != "friends_list":
		# Not the callback for the FriendsView
		return

	var copy_user_info_options = EOS.UserInfo.CopyUserInfoOptions.new()
	copy_user_info_options.local_user_id = Store.epic_account_id
	copy_user_info_options.target_user_id = data.target_user_id

	var user_info_data = EOS.UserInfo.UserInfoInterface.copy_user_info(copy_user_info_options)
	if user_info_data.result_code != EOS.Result.Success:
		print("--- Friends: user info data: result: ", EOS.result_str(user_info_data))
	else:
		friends.append(user_info_data.user_info)
		_update_friends_list()


func _update_friends_list():
	var base_bbcode = """[b]Friends List (%s)[/b]""" % str(friends.size())

	var rows_bbcode = ""
	for friend in friends:
		rows_bbcode += "\n"
		if friend.display_name != null:
			rows_bbcode += friend.display_name
		if friend.country != null:
			rows_bbcode += " (country=%s)" % friend.country
	friend_richtextlabel.text = base_bbcode + rows_bbcode


class_name FriendsView
extends VBoxContainer

# Local cache of friends data
var friends = []

@onready var list_title_rich_text_label = %ListTitleRichTextLabel
@onready var friend_richtextlabel = %FriendsRichTextLabel

func _ready() -> void:
	visibility_changed.connect(_query_friends)
	HAuth.logged_out.connect(_on_logged_out)
	IEOS.friends_interface_query_friends_callback.connect(_on_query_friends_callback)
	IEOS.user_info_interface_query_user_info_callback.connect(_on_query_user_info_callback)

func _query_friends():
	if not visible: return

	if Store.epic_account_id == "":
		print("Unable to query friends. Needs to login with Epic Games account.")
		return
	var query_opts = EOS.Friends.QueryFriendsOptions.new()
	query_opts.local_user_id = Store.epic_account_id
	query_opts.client_data = "friends_list"
	EOS.Friends.FriendsInterface.query_friends(query_opts)

func _on_logged_out():
	friends = []
	_rebuild_ui()

func _on_query_friends_callback(data: Dictionary):
	if data.client_data != "friends_list": return
	print("--- Friends: query_friends_callback: ", EOS.result_str(data))

	_copy_all_friends()

func _copy_all_friends():
	var get_count_opts = EOS.Friends.GetFriendsCountOptions.new()
	get_count_opts.local_user_id = Store.epic_account_id
	var friends_count: int = EOS.Friends.FriendsInterface.get_friends_count(get_count_opts)

	friends = []
	for i in friends_count:
		var get_friend_opts = EOS.Friends.GetFriendAtIndexOptions.new()
		get_friend_opts.local_user_id = Store.epic_account_id
		get_friend_opts.index = i

		var friend_epic_account_id = EOS.Friends.FriendsInterface.get_friend_at_index(get_friend_opts)
		var query_user_opts = EOS.UserInfo.QueryUserInfoOptions.new()
		query_user_opts.local_user_id = Store.epic_account_id
		query_user_opts.target_user_id = friend_epic_account_id
		query_user_opts.client_data = "friends_list"
		EOS.UserInfo.UserInfoInterface.query_user_info(query_user_opts)

func _on_query_user_info_callback(data: Dictionary):
	if data.client_data != "friends_list": return
	print("--- Friends: UserInfo: query_user_info_callback: ", EOS.result_str(data))

	var copy_user_opts = EOS.UserInfo.CopyUserInfoOptions.new()
	copy_user_opts.local_user_id = Store.epic_account_id
	copy_user_opts.target_user_id = data.target_user_id

	var user_info_data = EOS.UserInfo.UserInfoInterface.copy_user_info(copy_user_opts)
	if user_info_data.result_code != EOS.Result.Success:
		print("--- Friends: UserInfo: copy_user_info Error: ", EOS.result_str(user_info_data))
	else:
		friends.append(user_info_data.user_info)
		_rebuild_ui()

func _rebuild_ui():
	list_title_rich_text_label.text = "[b]Friends List (%s)[/b]" % str(friends.size())

	var table_bbcode = """[table=4]
[cell][b]Display Name[/b][/cell]
[cell][b]User Id[/b][/cell]
[cell][b]Nickname[/b][/cell]
[cell][b]Country[/b][/cell]
%s
[/table]
"""
	var rows_bbcode = ""
	for friend in friends:
		var nickname = friend.nickname if friend.nickname != "" else "-"
		var country = friend.country if friend.country != "" else "-"
		rows_bbcode += "[cell]%s[/cell][cell]%s[/cell][cell]%s[/cell][cell]%s[/cell]" % \
			 [friend.display_name_sanitized, friend.user_id, nickname, country]
	friend_richtextlabel.text = table_bbcode % rows_bbcode

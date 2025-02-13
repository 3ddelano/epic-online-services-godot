class_name FriendsView
extends VBoxContainer

# Local cache of friends data
var friends = []

@onready var list_title_rich_text_label = %ListTitleRichTextLabel
@onready var friend_richtextlabel = %FriendsRichTextLabel

func _ready() -> void:
	visibility_changed.connect(_query_friends)
	HAuth.logged_out.connect(_on_logged_out)

func _query_friends():
	if not visible: return

	if HAuth.epic_account_id == "":
		print("Unable to query friends. Needs to login with Epic Games account.")
		return
	
	friends = await HFriends.get_friends_async()
	_rebuild_ui()
	

func _on_logged_out():
	friends = []
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

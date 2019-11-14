
local MainScene = class("MainScene", cc.load("mvc").ViewBase)
local yimInstance = cc.YIM:create()
yimInstance.OnLogin = function (errorcode,youmeID)
	print("errorcode111:" .. errorcode .. " youmeID:" .. youmeID)
	--yimInstance:sendAudioMessage("winnie",1)
	--yimInstance:sendTextMessage("winnie",1,"11111")
	yimInstance:getContact()
end

yimInstance.OnLogout = function ()
	print("11: OnLogout")
end

yimInstance.OnDownload = function ( errorcode, strSavepath, bodytype,chattype, serial,recvid,senderid,content,params,duration, createtime )
	print("OnDownload：" .. serial .. " errorcode:" .. errorcode .. " path:" .. strSavepath)
end


--消息回掉. 普通消息发送回掉接口
yimInstance.OnSendMessageStatus = function(serial,errorcode, sendTime, isForbidRoom, reasonType, endTime)
	print("OnSendMessageStatus" .. serial .. " errorcode:" .. errorcode .. " sendTime:" .. sendTime)
end

yimInstance.OnStartSendAudioMessage = function(serial,errorcode,content,localpath,duration )

	print("OnStartSendAudioMessage" .. serial .. " errorcode:" .. errorcode .. " content:" .. content .. " localpath:" .. localpath .. " duration:" .. duration)
end

yimInstance.OnSendAudioMessageStatus = function(serial,errorcode,content,localpath,duration, sendTime, isForbidRoom, reasonType, endTime)
	print("OnSendAudioMessageStatus" .. serial .. " errorcode:" .. errorcode .. " content:" .. content .. " localpath:" .. localpath .. " duration:" .. duration .. " sendTime:" .. sendTime)
end

yimInstance.OnRecvMessage = function(bodytype,chattype, serial,recvid,senderid,content,params,duration,createtime, giftID, giftCount, anchor )

	print("OnRecvMessage" .. bodytype .. " chattype:" .. chattype .. " serial:" .. serial .. " recvid:" .. recvid .. " senderid:" .. senderid .. " content:" .. content .. " param:" .. params .. " duration: " .. duration)
	if bodytype == 5 then
		-- 音频文件可以下载
		yimInstance:downloadAudioFile(serial,"C:\\Users\\joexie\\Desktop\\data\\11.wav")
	
	end
end

--给多人发送消息
--yimInstance:multiSendTextMessage({"1223","32423","34535sdf"},"tadfafaf")

yimInstance.OnJoinChatroom = function(chatroomid ,errorcode)

	print("OnJoinChatroom id：" .. chatroomid .. "errorcode:" .. errorcode)
end

yimInstance.OnSpeechStatus=function(errorcode,requestid,downloadurl,duration,filesize,localpath,text)


end
yimInstance.OnQueryHistory = function(errorcode,targetid,remaincoutn,msglist)
	for i=1,#msglist do
		local chatType = msglist[i]["ChatType"]
		local duration = msglist[i]["Duration"]
		local param = msglist[i]["Param"]
		local recid = msglist[i]["ReceiveID"]
		local senderid = msglist[i]["SenderID"]
		local serial = msglist[i]["Serial"]
		local text = msglist[i]["Text"]
		local msgType = msglist[i]["MessageType"]
		local localpath = msglist[i]["LocalPath"]
		local createtime = msglist[i]["CreateTime"]
	end
end

yimInstance.OnGetRecentContacts =function(errorcode,contactLists)
	print("查询最近联系人回调")
    for i=1,#contactLists do
        local contactID = contactLists[i]["ContactID"]
        local messageType = contactLists[i]["MessageType"]
        local messageContent = contactLists[i]["MessageContent"]
        local createTime = contactLists[i]["CreateTime"]

        print("contactID:"..contactID.." messageType:"..messageType.." messageContent:"..messageContent.." createTime:"..createTime)
    end
end

yimInstance.OnRevMessageNotify=function ( chattype, targetID )


end

yimInstance.OnGetUserInfo = function ( errorcode, strUserID, strUserInfo )
	if errorcode == 0 and string.len(strUserInfo) ~= 0 then
		local info = json.decode( strUserInfo )

		print(strUserID .. "," .. info.nickname ..",".. info.server_area .. ",".. info.location .."," ..info.level .."," ..info.vip_level..","..info.extra)
	end

end

yimInstance.OnGetUserStatus = function ( errorcode, strUserID, status )
	print("获取玩家登录状态,userID:"..strUserID.."_status:"..status)
end

yimInstance.OnPlayCompletion = function ( errorcode , path )
	print("播放结束")
end 

yimInstance.OnTranslateTextComplete = function (errorcode, requestID, text, srcLangCode, destLangCode)
	print("onTranslateTextComplete errorcode:"..errorcode.." requestID:"..requestID.." text:"..text.." srcLangCode:"..srcLangCode.." destLangCode:"..destLangCode)
end

yimInstance.OnUpdateLocation = function (errorcode, districtCode, country, province, city, districtCounty, street, longitude, latitude)
	print("onUpdateLocation errorcode:"..errorcode.." districtCode:"..districtCode.." country:"..country.." province:"..province.." city:"..city.." districtCounty:"..districtCounty.." longitude:"..longitude.." latitude:"..latitude)
end

yimInstance.OnGetNearbyObjects = function (errorcode, startDistance, endDistance, nearbyList)
	print("onGetNearbyObjects errorcode:"..errorcode.." startDistance:"..startDistance.." endDistance:"..endDistance)
	
	for i=1,#nearbyList do
		local userID = nearbyList[i]["UserID"]
		local distance = nearbyList[i]["Distance"]
		local longitude = nearbyList[i]["Longitude"]
		local latitude = nearbyList[i]["Latitude"]
		local country = nearbyList[i]["Country"]
		local province = nearbyList[i]["Province"]
		local city = nearbyList[i]["City"]
		local districtCounty = nearbyList[i]["DistrictCounty"]
		local street = nearbyList[i]["Street"]
		
		print("userID:"..userID.." distance:"..distance.." longitude:"..longitude.." latitude:"..latitude.." country:"..country.." province:"..province.." city:"..city)
	end
end

yimInstance.OnDownloadByUrl = function ( errorcode, fromUrl , savePath)
	print("下载文件回调:");
    print("from:%s", fromUrl )
    print("savePath:%s", savePath )
    if errorcode == 0  then
        print("下载文件成功")
    else
        print("下载文件 %s 失败, error = %d  ", strSavepath, errorcode);
    end
end

yimInstance.OnUserJoinChatroom = function (chatroomid, userID)
	print("onUserJoinChatroom chatroomid:" .. chatroomid .. " userID:" .. userID)
end

yimInstance.OnLeaveJoinChatroom = function (chatroomid, userID)
	print("onLeaveJoinChatroom chatroomid:" .. chatroomid .. " userID:" .. userID)
end

yimInstance.OnGetMicrophoneStatus = function (status)
	print("onGetMicrophoneStatus status:" .. status)
end

yimInstance.OnAccusationResultNotify = function (result, userID, accusationTime)
	print("onAccusationResultNotify result:" .. result .. " userID:" .. userID .. " accusationTime:" .. accusationTime)
end

yimInstance.OnRecvNotice = function (notice)
	local noticeID = notice["NoticeID"]
	local channelID = notice["ChannelID"]
	local noticeType = notice["NoticeType"]
	local noticeContent = notice["NoticeContent"]
	local linkText = notice["LinkText"]
	local linkAddress = notice["LinkAddress"]
	local beginTime = notice["BeginTime"]
	local endTime = notice["EndTime"]
	
	print("noticeID:"..noticeID.." channelID:"..channelID.." noticeType:"..noticeType.." noticeContent:"..noticeContent.." linkText:"..linkText.." linkAddress:"..linkAddress)
end

yimInstance.OnCancelNotice = function (noticeID, channelID)
	print("onCancelNotice noticeID" .. noticeID .. " channelID:" .. channelID)
end

yimInstance.OnGetForbiddenSpeakInfo =  function ( errorcode,  arrForbiddenInfos )
	print("收到禁言数据:"..errorcode..",count:"..#arrForbiddenInfos)
    for i = 1, #arrForbiddenInfos do
	local info = arrForbiddenInfos[i];
	print("禁言:channel:"..info["channelID"]..","..","..info["reasonType"]..","..info["endTime"]);
	print(info["isForbidRoom"])
    end
end

yimInstance.OnGetRoomMemberCount = function(errorcode, roomID, count)
	print("onGetRoomMemberCount errorcode:" .. errorcode .. " roomID:" .. roomID .. " count:" .. count)
end

yimInstance.OnBlockUser = function(errorcode, userID, block)
	print("onBlockUser errorcode:" .. errorcode .. " userID:" .. userID .. " block:" .. tostring(block))
end

yimInstance.OnUnBlockAllUser = function(errorcode)
	print("onUnBlockAllUser errorcode:" .. errorcode)
end

yimInstance.OnGetBlockUsers = function(errorcode, userList)
	print("onGetBlockUsers errorcode:" .. errorcode)
	for i=1,#userList do
		print("userID:" .. userList[i])
	end
end

yimInstance.OnGetRecognizeSpeechText = function(serial, errorcode, text)
print("OnGetRecognizeSpeechText errorcode:" .. errorcode .. " serial: " .. serial .. " text:" .. text)
end

yimInstance.OnStartReconnect = function ()
	print("OnStartReconnect")
end

yimInstance.OnRecvReconnectResult = function (result)
	print("OnRecvReconnectResult result:" .. result)
end

yimInstance.OnRecordVolume = function (volume)
	print("OnRecordVolume volume:" .. volume)
end

yimInstance.OnLeaveAllChatRooms = function (errorcode)
	print("OnLeaveAllChatRooms errorcode:" .. errorcode)
end

yimInstance.onGetDistance = function(errorcode, userID, distance)
	print("onGetDistance errorcode:" .. errorcode .. " userID:" .. userID .. " distance:" .. distance)
end

yimInstance.onQueryRoomHistoryMessageFromServer = function(errorcode, roomID, remain, messageList)
	print("onQueryRoomHistoryMessageFromServer errorcode:" .. errorcode .. " roomID:" .. roomID .. " remain:" .. remain)
	
	for i=1,#messageList do
		local messageID = messageList[i]["MessageID"]
		local chatType = messageList[i]["ChatType"]
		local messageType = messageList[i]["MessageType"]
		local receiveID = messageList[i]["ReceiveID"]
		local senderID = messageList[i]["SenderID"]
		local content = messageList[i]["Content"]
		local extraParam = messageList[i]["ExtraParam"]
		local createTime = messageList[i]["CreateTime"]
		
		print("messageID:"..messageID.." chatType:"..chatType.." receiveID:"..receiveID.." senderID:"..senderID.." content:"..content.." extraParam:"..extraParam)
		
		if messageType == 1 then		--文本
			local audioDuration = messageList[i]["AudioDuration"]
		elseif messageType == 2 then	--自定义
		
		elseif messageType == 5 then	--语音
			local audioDuration = messageList[i]["AudioDuration"]
		elseif messageType == 7 then	--文件
			local fileType = messageList[i]["FileType"]
			local fileSize = messageList[i]["FileSize"]
			local fileName = messageList[i]["FileName"]
			local fileExtension = messageList[i]["FileExtension"]
		elseif messageType == 8 then	--礼物
			local giftID = messageList[i]["GiftID"]
			local giftCount = messageList[i]["GiftCount"]
			local anchor = messageList[i]["Anchor"]
		end
	end
end

yimInstance.OnQueryUserInfo = function(errorcode,userID,photoUrl,onlineState,beAddPermission,foundPermission,nickName,sex,signature,country,province,city,extraInfo)
	print("OnQueryUserInfo errorcode: "..errorcode.. " userID: " ..userID.. " onlineState：" ..onlineState.. " photoURL：" ..photoUrl.. " beAddPermission：" ..beAddPermission.. " foundPermission：" ..foundPermission.. " nickName：" ..nickName.. " sex：" ..sex.. " signature：" ..signature.. " country：" ..country.. " province：" ..province.. " city：" ..city.. " extraInfo：" ..extraInfo)
	
	end
	
	
	yimInstance.OnSetUserInfo = function(errorcode)
	print("OnSetUserInfo errorcode: " ..errorcode)
	end
	
	
	yimInstance.OnSwitchUserOnlineState = function(errorcode)
	print("OnSwitchUserOnlineState errorcode: " ..errorcode)
	end
	
	yimInstance.OnSetPhotoUrl = function(errorcode,photoUrl)
	print("OnSetPhotoUrl errorcode: "..errorcode.. " photoUrl：" ..photoUrl)
	end
	
	
	yimInstance.OnUserInfoChangeNotify = function(userID)
	print("OnUserInfoChangeNotify userID: " ..userID)
	end
	
	
	yimInstance.OnFindUser = function(errorcode,userList)
	print("OnFindUser errorcode: ".. errorcode)
	
	for i=1,#userList do
	local userID = userList[i]["UserID"]
	local nickName = userList[i]["Nickname"]
	local status = userList[i]["Status"]
	
	print(" userID:"..userID.." nickName:"..nickName.." onlineStatus:"..status)	
	end
		
	end
	
	
	yimInstance.OnRequestAddFriend = function(errorcode,userID)
	print("OnRequestAddFriend errorcode: "..errorcode.. " userID：" ..userID)	
	end
	
	
	yimInstance.OnBeRequestAddFriendNotify = function(userID,comments)
	print("OnBeRequestAddFriendNotify userID: "..userID.. " comments：" ..comments)
	local ymErrorcode = cc.exports.youmeim:dealAddFriend( cc.exports.friendID, 0)
	if ymErrorcode == 0 then
	print("dealAddFriend success")
	else
	print("dealAddFriend fail!")
	end
	
	end
	
	
	yimInstance.OnBeAddFriendNotify = function(userID,comments)
	print("OnBeAddFriendNotify userID: "..userID.. " comments：" ..comments)
	
	local ymErrorcode = cc.exports.youmeim:queryFriends( 0, 0, 5 )
	if ymErrorcode == 0 then
	print("queryFriends success.")
	else
	print("queryFriends fail!")
	end
	
	end	
	
	yimInstance.OnDealBeRequestAddFriend = function(errorcode,userID,comments,dealResult)
	print("OnDealBeRequestAddFriend errorcode" ..errorcode.." userID: "..userID.. " comments：" ..comments.." dealResult："..dealResult)	
	end
		
	cc.exports.youmeim.OnRequestAddFriendResultNotify = function(userID,comments,dealResult)
	print("OnRequestAddFriendResultNotify userID: "..userID.. " comments：" ..comments.." dealResult："..dealResult)
	if dealResult == 0 then
	local ymErrorcode = cc.exports.youmeim:queryFriends( 0, 0, 5 )
	if ymErrorcode == 0 then
	print("queryFriends success.")
	else
	print("queryFriends fail!")
	end	
	end	
	end
	
	
	yimInstance.OnDeleteFriend = function(errorcode,userID)
	print("OnDeleteFriend errorcode: "..errorcode.. " userID：" ..userID)
	local ymErrorcode = cc.exports.youmeim:queryFriends( 0, 0, 5 )
	if ymErrorcode == 0 then
	print("queryFriends success.")
	else
	print("queryFriends fail!")
	end
	
	end	
	
	yimInstance.OnBeDeleteFriendNotify = function(userID)
	print("OnBeDeleteFriendNotify userID：" ..userID)
	local ymErrorcode = cc.exports.youmeim:queryFriends( 0, 0, 5 )
	if ymErrorcode == 0 then
	print("queryFriends success.")
	else
	print("queryFriends fail!")
	end
	
	end
	
	yimInstance.OnBlackFriend = function(errorcode,type,userID)
	print("OnBlackFriend errorcode: "..errorcode.. " type:" ..type.. " userID：" ..userID)
	local ymErrorcode = cc.exports.youmeim:queryFriends( 1, 0, 5 )
	if ymErrorcode == 0 then
	print("query my blacked friends success.")
	else
	print("query my blacked friends fail!")
	end
	
	end	
	
	yimInstance.OnQueryFriends = function(errorcode,type,startIndex,firendList)
	print("OnQueryFriends errorcode: "..errorcode.. " type:" ..type.. " startIndex：" ..startIndex)
	for i=1,#firendList do
	local userID = firendList[i]["UserID"]
	local nickName = firendList[i]["Nickname"]
	local status = firendList[i]["Status"]
	
	print(" userID:"..userID.." nickName:"..nickName.." onlineState:"..status)
	end
	end
	
	
	yimInstance.OnQueryFriendRequestList = function(errorcode,startIndex,requestList)
	print("OnQueryFriendRequestList errorcode: "..errorcode.. " startIndex：" ..startIndex)
	
	for i=1,#requestList do
	local askerID = requestList[i]["AskerID"]
	local askerNickname = requestList[i]["AskerNickname"]
	local inviteeID = requestList[i]["InviteeID"]
	local inviteeNickname = requestList[i]["InviteeNickname"]
	local validateInfo = requestList[i]["ValidateInfo"]
	local status = requestList[i]["Status"]
	local createTime = requestList[i]["CreateTime"]
	
	print(" askerID:"..askerID.." askerNickname:"..askerNickname.." inviteeID:"..inviteeID.." inviteeNickname:"..inviteeNickname.." validateInfo:"..validateInfo.." status:"..status.." createTime:"..createTime)
	end
	
	end

--批量注册回掉
yimInstance.registerScriptHandler(yimInstance,
	yimInstance.OnLogin,
	yimInstance.OnLogout,
	yimInstance.OnDownload,
	yimInstance.OnSendMessageStatus,
	yimInstance.OnSendAudioMessageStatus,
	yimInstance.OnRecvMessage,
	yimInstance.OnJoinChatroom,
	yimInstance.OnSpeechStatus,
	yimInstance.OnQueryHistory,
	yimInstance.OnGetRecentContacts,
	yimInstance.OnRevMessageNotify,
	yimInstance.OnGetUserInfo,
	yimInstance.OnGetUserStatus,
	yimInstance.OnPlayCompletion,
	yimInstance.OnStartSendAudioMessage,
	yimInstance.OnTranslateTextComplete,
	yimInstance.OnUpdateLocation,
	yimInstance.OnGetNearbyObjects,
	yimInstance.OnDownloadByUrl,
	yimInstance.OnUserJoinChatroom,
	yimInstance.OnLeaveJoinChatroom,
	yimInstance.OnGetMicrophoneStatus,
	yimInstance.OnAccusationResultNotify,
	yimInstance.OnRecvNotice,
	yimInstance.OnCancelNotice,
	yimInstance.OnGetForbiddenSpeakInfo,
	yimInstance.OnGetRoomMemberCount,
	yimInstance.OnBlockUser,
	yimInstance.OnUnBlockAllUser,
	yimInstance.OnGetBlockUsers
	);

--单独注册回调
yimInstance.OnTestCallback = function (errorcode)
end

yimInstance.bindScriptHandler(yimInstance,0,yimInstance.OnTestCallback)

--注册只识别语音文本的回调
yimInstance.bindScriptHandler(yimInstance,1,yimInstance.OnGetRecognizeSpeechText)
--注册开始重连回调
yimInstance.bindScriptHandler(yimInstance,2,yimInstance.OnStartReconnect)
--注册重连结果回调
yimInstance.bindScriptHandler(yimInstance,3,yimInstance.OnRecvReconnectResult)
--注册音量回调
yimInstance.bindScriptHandler(yimInstance,4,yimInstance.OnRecordVolume)
--注册离开所有房间回调
yimInstance.bindScriptHandler(yimInstance,5,yimInstance.OnLeaveAllChatRooms)
--注册获取指定用户距离回调
yimInstance.bindScriptHandler(yimInstance,6,yimInstance.onGetDistance)
--注册从服务器查询房间历史消息回调
yimInstance.bindScriptHandler(yimInstance,7,yimInstance.onQueryRoomHistoryMessageFromServer)

--注册用户资料变更通知
yimInstance.bindScriptHandler(yimInstance,8,yimInstance.OnUserInfoChangeNotify)
--注册设置用户基本资料回调
yimInstance.bindScriptHandler(yimInstance,9,yimInstance.OnSetUserInfo)
--注册查询用户基本资料回调
yimInstance.bindScriptHandler(yimInstance,10,yimInstance.OnQueryUserInfo)
--注册设置用户头像回调
yimInstance.bindScriptHandler(yimInstance,11,yimInstance.OnSetPhotoUrl)
--注册切换用户在线状态回调
yimInstance.bindScriptHandler(yimInstance,12,yimInstance.OnSwitchUserOnlineState)
--注册查找用户回调
yimInstance.bindScriptHandler(yimInstance,13,yimInstance.OnFindUser)
--注册请求添加好友回调
yimInstance.bindScriptHandler(yimInstance,14,yimInstance.OnRequestAddFriend)
--注册被添加为好友通知
yimInstance.bindScriptHandler(yimInstance,15,yimInstance.OnBeAddFriendNotify)
--注册被请求添加为好友通知
yimInstance.bindScriptHandler(yimInstance,16,yimInstance.OnBeRequestAddFriendNotify)
--注册处理被请求添加为好友回调
yimInstance.bindScriptHandler(yimInstance,17,yimInstance.OnDealBeRequestAddFriend)
--注册请求添加好友结果通知
yimInstance.bindScriptHandler(yimInstance,18,yimInstance.OnRequestAddFriendResultNotify)
--注册删除好友回调
yimInstance.bindScriptHandler(yimInstance,19,yimInstance.OnDeleteFriend)
--注册被好友删除通知
yimInstance.bindScriptHandler(yimInstance,20,yimInstance.OnBeDeleteFriendNotify)
--注册拉黑好友回调
yimInstance.bindScriptHandler(yimInstance,21,yimInstance.OnBlackFriend)
--注册查询我的好友回调
yimInstance.bindScriptHandler(yimInstance,22,yimInstance.OnQueryFriends)
--注册查询好友请求列表回调
yimInstance.bindScriptHandler(yimInstance,23,yimInstance.OnQueryFriendRequestList)

function MainScene:onCreate()
    -- add background image
    display.newSprite("HelloWorld.png")
        :move(display.center)
        :addTo(self)

    -- add HelloWorld label
    cc.Label:createWithSystemFont("Hello World", "Arial", 40)
        :move(display.cx, display.cy + 200)
        :addTo(self)

	local btn =ccui.Button:create("HelloWorld.png")
	btn:move(display.cx, display.cy + 400)
    btn:addTo(self)
	
	
	 local function callback(ref, type)
		--yimInstance:stopAudioMessage("param")
        --yimInstance:sendTextMessage("1111",2,"聊天室消息")
		--yimInstance:sendTextMessage("winnie",1,"个人消息")
		--yimInstance:cancleAudioMessage()
		--yimInstance:leaveChatRoom("1111")
		yimInstance:logout()
    end
    btn:addClickEventListener(callback)
	
	local ierrorcode = yimInstance:init("YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0","r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=")
	print("初始化状态:" .. ierrorcode)
	ierrorcode = yimInstance:login("joexie","123456","");
	print("登陆:" .. ierrorcode)	
end

return MainScene

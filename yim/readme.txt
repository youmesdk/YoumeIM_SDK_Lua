

--[[
MainScene.lua  是一个调用例子文档


1) 创建一个luaim实例，只需要创建一个就可以
local yimInstance = cc.YIM:create()

2) 设置各种回掉
yimInstance.OnLogin = function (errorcode,youmeID)
	print("errorcode:" .. errorcode .. " youmeID:" .. youmeID)
	yimInstance:sendAudioMessage("winnie",1)
	--yimInstance:sendTextMessage("winnie",1,"11111")
	yimInstance:joinChatRoom("1111")
end

yimInstance.OnLogout = function ()
	print("11: OnLogout")
end

yimInstance.OnDownload = function (serial,errorcode,strSavepath)
	print("OnDownload：" .. serial .. " errorcode:" .. errorcode .. " path:" .. strSavepath)
end


--消息回掉. 普通消息发送回掉接口
yimInstance.OnSendMessageStatus = function(serial,errorcode)

	print("OnSendMessageStatus" .. serial .. " errorcode:" .. errorcode)
end


yimInstance.OnStartSendAudioMessage = function(serial,errorcode,content,localpath,duration )

	print("OnStartSendAudioMessage" .. serial .. " errorcode:" .. errorcode .. " content:" .. content .. " localpath:" .. localpath .. " duration:" .. duration)
end

yimInstance.OnSendAudioMessageStatus = function(serial,errorcode,content,localpath,duration )

	print("OnSendAudioMessageStatus" .. serial .. " errorcode:" .. errorcode .. " content:" .. content .. " localpath:" .. localpath .. " duration:" .. duration)
end

yimInstance.OnRecvMessage = function(bodytype,chattype, serial,recvid,senderid,content,params,duration,createtime, giftID, giftCount, anchor )

	print("OnRecvMessage" .. bodytype .. " chattype:" .. chattype .. " serial:" .. serial .. " recvid:" .. recvid .. " senderid:" .. senderid .. " content:" .. content .. " param:" .. params .. " duration: " .. duration)
	if bodytype == 5 then
		-- 音频文件可以下载
		yimInstance:downloadAudioFile(serial,"C:\\Users\\joexie\\Desktop\\data\\11.wav")
	
	end
end

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
	end
end

yimInstance.OnGetRecentContacts =function(contactLists)
	for i=1,#contactLists do
	print("OnGetRecentContacts " .. contactLists[i])
	
	end
end

yimInstance.OnRevMessageNotify=function ( chattype, targetID )


end

yimInstance.OnGetUserInfo = function ( errorcode, strUserInfo )
	if errorcode == 0 and string.len(strUserInfo) ~= 0 then
		local info = json.decode( strUserInfo )

		print( info.nickname ..",".. info.server_area .. ",".. info.location .."," ..info.level .."," ..info.vip_level..","..info.extra)
	end
end

yimInstance.OnGetUserStatus = function ( errorcode, strUserID, status )
	print("获取玩家登录状态,userID:"..strUserID.."_status:"..status)
end

yimInstance.OnPlayComplete = function ( errorcode , path )
	print("播放结束")
end 

3） 注册回掉
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
	yimInstance.OnStartSendAudioMessage
	);



4) 提供的接口，参数可以
	tolua_function(tolua_S, "init", tolua_Cocos2d_YIM_init);
	tolua_function(tolua_S, "unInit", tolua_Cocos2d_YIM_unInit);
	tolua_function(tolua_S, "sendTextMessage", tolua_Cocos2d_YIM_sendTextMessage);
	tolua_function(tolua_S, "sendAudioMessage", tolua_Cocos2d_YIM_sendAudioMessage);
	tolua_function(tolua_S, "sendOnlyAudioMessage", tolua_Cocos2d_YIM_sendOnlyAudioMessage);
	tolua_function(tolua_S, "stopAudioMessage", tolua_Cocos2d_YIM_stopAudioMessage);
	tolua_function(tolua_S, "cancleAudioMessage", tolua_Cocos2d_YIM_cancleAudioMessage);
	tolua_function(tolua_S, "joinChatRoom", tolua_Cocos2d_YIM_joinChatRoom);
	tolua_function(tolua_S, "leaveChatRoom", tolua_Cocos2d_YIM_leaveChatRoom);
	tolua_function(tolua_S, "downloadAudioFile", tolua_Cocos2d_YIM_downloadAudioFile);
	
	tolua_function(tolua_S, "login", tolua_Cocos2d_YIM_login);
	tolua_function(tolua_S, "logout", tolua_Cocos2d_YIM_logout);

	//一些全局的函数
	tolua_function(tolua_S, "getFilterText", tolua_Cocos2d_YIM_getFilterText);
	tolua_function(tolua_S, "setServerZone", tolua_Cocos2d_YIM_setServerZone);
	tolua_function(tolua_S, "setAudioCacheDir", tolua_Cocos2d_YIM_setAudioCacheDir);
	//新加的
	
	tolua_function(tolua_S, "startAudioSpeech", tolua_Cocos2d_YIM_startAudioSpeech);
	tolua_function(tolua_S, "stopAudioSpeech", tolua_Cocos2d_YIM_stopAudioSpeech);
	tolua_function(tolua_S, "queryHistoryMessage", tolua_Cocos2d_YIM_queryHistoryMessage);
	tolua_function(tolua_S, "deleteHistoryMessage", tolua_Cocos2d_YIM_deleteHistoryMessage);
	tolua_function(tolua_S, "deleteHistoryMessageByID", tolua_Cocos2d_YIM_deleteHistoryMessageByID);
	tolua_function(tolua_S, "queryRoomHistoryMessageFromServer", tolua_Cocos2d_YIM_queryRoomHistoryMessageFromServer);
	tolua_function(tolua_S, "convertAMRToWav", tolua_Cocos2d_YIM_convertAMRToWav);

	tolua_function(tolua_S, "onResume", tolua_Cocos2d_YIM_onResume);
	tolua_function(tolua_S, "onPause", tolua_Cocos2d_YIM_onPause);


	tolua_function(tolua_S, "sendGift", tolua_Cocos2d_YIM_sendGift);
	tolua_function(tolua_S, "multiSendTextMessage", tolua_Cocos2d_YIM_multiSendTextMessage);
	tolua_function(tolua_S, "getContact", tolua_Cocos2d_YIM_getContact);
	tolua_function(tolua_S, "setReceiveMessageSwitch", tolua_Cocos2d_YIM_setReceiveMessageSwitch);
	tolua_function(tolua_S, "getNewMessage", tolua_Cocos2d_YIM_getNewMessage);
    
    tolua_function(tolua_S, "setUserInfo", tolua_Cocos2d_YIM_setUserInfo);
    tolua_function(tolua_S, "getUserInfo", tolua_Cocos2d_YIM_getUserInfo);

    tolua_function(tolua_S, "queryUserStatus", tolua_Cocos2d_YIM_QueryUserStatus);
    tolua_function(tolua_S, "setVolume", tolua_Cocos2d_YIM_SetVolume);
    tolua_function(tolua_S, "startPlayAudio", tolua_Cocos2d_YIM_StartPlayAudio);
    tolua_function(tolua_S, "stopPlayAudio", tolua_Cocos2d_YIM_StopPlayAudio);
    tolua_function(tolua_S, "isPlaying", tolua_Cocos2d_YIM_IsPlaying );
    tolua_function(tolua_S, "getAudioCachePath", tolua_Cocos2d_YIM_GetAudioCachePath);
    tolua_function(tolua_S, "clearAudioCachePath", tolua_Cocos2d_YIM_ClearAudioCachePath);
    tolua_function(tolua_S, "setRoomHistoryMessageSwitch", tolua_Cocos2d_YIM_SetRoomHistoryMessageSwitch);



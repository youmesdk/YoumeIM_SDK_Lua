#include "LuaYIM.h"
#include "json/document.h"
#include <YIMPlatformDefine.h>
#include <YIMCInterface.h>
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/CCLuaStack.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

#include "json/writer.h"
#include "json/stringbuffer.h"
#include "cocos2d.h"

USING_NS_CC;
//C:\testlua\testlua\src\app\views\MainScene.lua
std::string yim_to_string(XUINT64 i)
{
	char out[32];
#if defined(WIN32)
	_ui64toa(i, out, 10);
#else
	sprintf(out, "%llu", (long long unsigned int)i);
#endif
	return out;
}


XUINT64 yim_to_xuint64(const char* s)
{
#if defined(WIN32)
	return _atoi64(s);
#else
	long long unsigned int out = 0;
	sscanf(s, "%llu", &out);
	return (XUINT64)out;
#endif
}

#ifdef WIN32

std::wstring P_UTF8_to_Unicode(const std::string& in)
{
	int len = in.length();
	wchar_t* pBuf = new wchar_t[len + 1];
	if (NULL == pBuf)
	{
		return __XT("");
	}
	size_t out_len = (len + 1) * sizeof(wchar_t);
	memset(pBuf, 0, (len + 1) * sizeof(wchar_t));
	wchar_t* pResult = pBuf;
	out_len = ::MultiByteToWideChar(CP_UTF8, 0, in.c_str(), len, pBuf, len * sizeof(wchar_t));
	std::wstring out;
	out.assign(pResult, out_len);


	delete[] pResult;
	pResult = NULL;
	return out;
}

std::string P_Unicode_to_UTF8(const std::wstring& in)
{
	std::string out;
	int len = in.length();
	size_t out_len = len * 3 + 1;
	char* pBuf = new char[out_len];
	if (NULL == pBuf)
	{
		return "";
	}
	char* pResult = pBuf;
	memset(pBuf, 0, out_len);


	out_len = ::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), len, pBuf, len * 3, NULL, NULL);
	out.assign(pResult, out_len);
	delete[] pResult;
	pResult = NULL;
	return out;
}
#define  UTF8TOPlatString(str) P_UTF8_to_Unicode(str)
#define  PlatStringToUTF8(str) P_Unicode_to_UTF8(str)
#else
#define  UTF8TOPlatString(str) str
#define  PlatStringToUTF8(str) str
#endif // WIN32
enum YouMeIMCommand
{
	CMD_UNKNOW = 0,
	CMD_LOGIN = 1,
	CMD_HEARTBEAT = 2,
	CMD_LOGOUT = 3,
	CMD_ENTER_ROOM = 4,
	CMD_LEAVE_ROOM = 5,
	CMD_SND_TEXT_MSG = 6,
	CMD_SND_VOICE_MSG = 7,
	CMD_SND_FILE_MSG = 8,
	CMD_GET_MSG = 9,
	CMD_GET_UPLOAD_TOKEN = 10,
	CMD_KICK_OFF = 11,
	CMD_SND_BIN_MSG = 12,
	CMD_RELOGIN = 13,
	CMD_CHECK_ONLINE = 14,
	CMD_SND_GIFT_MSG = 15,
	CMD_GET_ROOM_HISTORY_MSG = 16,
	CMD_GET_USR_INFO = 17,
	CMD_UPDATE_USER_INFO = 18,
	CMD_SND_TIPOFF_MSG = 19,
	CMD_GET_TIPOFF_MSG = 20,
	CMD_GET_DISTRICT = 21,
	CMD_GET_PEOPLE_NEARBY = 22,
	CMD_QUERY_NOTICE = 23,
	CMD_SET_MASK_USER_MSG = 24,
	CMD_GET_MASK_USER_MSG = 25,
	CMD_CLEAN_MASK_USER_MSG = 26,
	CMD_GET_ROOM_INFO = 27,
	CMD_LEAVE_ALL_ROOM = 28,        // 离开所有房间
	CMD_GET_FORBID_RECORD = 31,

	//关系链管理增加
	CMD_REGISTER_USER_PROFILE = 36,
	CMD_QUERY_USER_PROFILE = 37,
	CMD_UPDATE_USER_PROFILE = 38,
	CMD_UPDATE_ONLINE_STATUS = 39,
	CMD_FIND_FRIEND_BY_ID = 40,				// 按ID查找好友
	CMD_FIND_FRIEND_BY_NICKNAME = 41,		// 按昵称查找好友
	CMD_REQUEST_ADD_FRIEND = 42,			// 请求添加好友
	CMD_FRIND_NOTIFY = 44,					// 好友请求通知
	CMD_DELETE_FRIEND = 45,					// 删除好友
	CMD_BLACK_FRIEND = 46,					// 拉黑好友
	CMD_UNBLACK_FRIEND = 47,				// 解除黑名单
	CMD_DEAL_ADD_FRIEND = 48,				// 好友验证
	CMD_QUERY_FRIEND_LIST = 49,				// 获取好友列表
	CMD_QUERY_BLACK_FRIEND_LIST = 50,		// 获取黑名单列表
	CMD_QUERY_FRIEND_REQUEST_LIST = 51,		// 获取好友验证消息列表
	CMD_UPDATE_FRIEND_RQUEST_STATUS = 52,	// 更新好友请求状态
	CMD_RELATION_CHAIN_HEARTBEAT = 53,		// 关系链心跳

	CMD_HXR_USER_INFO_CHANGE_NOTIFY = 74,	// 用户信息变更通知

	//服务器通知
	NOTIFY_LOGIN = 10001,
	NOTIFY_PRIVATE_MSG,
	NOTIFY_ROOM_MSG,

	//客户端(C接口使用)
	CMD_DOWNLOAD = 20001,
	CMD_SEND_MESSAGE_STATUS,
	CMD_RECV_MESSAGE,
	CMD_STOP_AUDIOSPEECH,
	CMD_QUERY_HISTORY_MESSAGE,
	CMD_GET_RENCENT_CONTACTS,
	CMD_RECEIVE_MESSAGE_NITIFY,
	CMD_QUERY_USER_STATUS,
	CMD_AUDIO_PLAY_COMPLETE,
	CMD_STOP_SEND_AUDIO = 20010,
	CMD_TRANSLATE_COMPLETE,
	CMD_DOWNLOAD_URL,
	CMD_GET_MICROPHONE_STATUS,
	CMD_USER_ENTER_ROOM,
	CMD_USER_LEAVE_ROOM,
	CMD_RECV_NOTICE,
	CMD_CANCEL_NOTICE,
	CMD_GET_SPEECH_TEXT,  // 仅需要语音的文字识别内容
	CMD_GET_RECONNECT_RESULT, //重连结果
	CMD_START_RECONNECT = 20020,  // 开始重连
	CMD_RECORD_VOLUME,    // 音量
	CMD_GET_DISTANCE,
	CMD_REQUEST_ADD_FRIEND_NOTIFY = 20023,	// 请求添加好友通知
	CMD_ADD_FRIENT_RESULT_NOTIFY = 20024,	// 添加好友结果通知
	CMD_BE_ADD_FRIENT = 20025,				// 被好友添加通知
	CMD_BE_DELETE_FRIEND_NOTIFY = 20026,	// 被删除好友通知
	CMD_BE_BLACK_FRIEND_NOTIFY = 20027,		// 被拉黑好友通知
	CMD_GET_USER_PROFILE = 20028,			//关系链-查询用户信息
	CMD_SET_USER_PROFILE = 20029,			//关系链-设置用户信息
	CMD_SET_USER_PHOTO = 20030,				//关系链-设置头像
	CMD_SWITCH_USER_STATE = 20031			//关系链-切换用户在线状态
};

namespace {
	ScriptHandlerMgr::HandlerType EVENT_YIM_Login;
	ScriptHandlerMgr::HandlerType EVENT_YIM_Logout;
	ScriptHandlerMgr::HandlerType EVENT_YIM_Download;
	ScriptHandlerMgr::HandlerType EVENT_YIM_SendMessageStatus;
	ScriptHandlerMgr::HandlerType EVENT_YIM_SendAudioMessageStatus;
	ScriptHandlerMgr::HandlerType EVENT_YIM_RecvMessage;
	ScriptHandlerMgr::HandlerType EVENT_YIM_ChatroomMessage;
	ScriptHandlerMgr::HandlerType EVENT_YIM_SpeechStatus;
	ScriptHandlerMgr::HandlerType EVENT_YIM_QueryHistory;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetRecentContacts;
	ScriptHandlerMgr::HandlerType EVENT_YIM_RecvMsgNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetUserInfo;
	ScriptHandlerMgr::HandlerType EVENT_YIM_QueryUserStatus;
	ScriptHandlerMgr::HandlerType EVENT_YIM_PlayComplete;
	ScriptHandlerMgr::HandlerType EVENT_YIM_StartSendAudioMessage;
	ScriptHandlerMgr::HandlerType EVENT_YIM_TranslateText;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetCurrentLocation;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetNearbyObjects;
	ScriptHandlerMgr::HandlerType EVENT_YIM_DownloadByUrl;
	ScriptHandlerMgr::HandlerType EVENT_YIM_UserJoinRoom;
	ScriptHandlerMgr::HandlerType EVENT_YIM_UserLeaveRoom;
	ScriptHandlerMgr::HandlerType EVENT_YIM_MicrophoneStatus;
	ScriptHandlerMgr::HandlerType EVENT_YIM_AccusationResultNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_RecvNotice;
	ScriptHandlerMgr::HandlerType EVENT_YIM_CancelNotice;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetForbiddenSpeakInfo;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetRoomMemberCount;
	ScriptHandlerMgr::HandlerType EVENT_YIM_BlockUser;
	ScriptHandlerMgr::HandlerType EVENT_YIM_UnBlockAllUser;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetBlockUsers;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetRecognizeSpeechText;
	ScriptHandlerMgr::HandlerType EVENT_YIM_StartReconnect;
	ScriptHandlerMgr::HandlerType EVENT_YIM_RecvReconnectResult;
	ScriptHandlerMgr::HandlerType EVENT_YIM_RecvRecordVolume;


	ScriptHandlerMgr::HandlerType EVENT_YIM_Test; //对应的单独注册函数序号 0
	ScriptHandlerMgr::HandlerType EVENT_YIM_LeaveAllChatRooms;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetDistance;
	ScriptHandlerMgr::HandlerType EVENT_YIM_QueryRoomHistoryFromServer;

	ScriptHandlerMgr::HandlerType EVENT_YIM_UserProfileInfoChangeNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_SetUserProfile;
	ScriptHandlerMgr::HandlerType EVENT_YIM_GetUserProfile;
	ScriptHandlerMgr::HandlerType EVENT_YIM_SetUserPhotoUrl;
	ScriptHandlerMgr::HandlerType EVENT_YIM_SwitchUserOnlineState;

	ScriptHandlerMgr::HandlerType EVENT_YIM_FindFriend;
	ScriptHandlerMgr::HandlerType EVENT_YIM_RequestAddFriend;
	ScriptHandlerMgr::HandlerType EVENT_YIM_BeAddFriendNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_BeAddFriendRequestNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_DealBeAddFriend;
	ScriptHandlerMgr::HandlerType EVENT_YIM_AddFriendResultNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_DeleteFriend;
	ScriptHandlerMgr::HandlerType EVENT_YIM_BeDeleteNotify;
	ScriptHandlerMgr::HandlerType EVENT_YIM_BlackFriend;
	ScriptHandlerMgr::HandlerType EVENT_YIM_QueryFriendList;
	ScriptHandlerMgr::HandlerType EVENT_YIM_QueryFriendRequestList;
}

//最近联系人信息
struct ContactsMsgInfo
{
	std::string strContactID;
	YIMMessageBodyType iMessageType;
	std::string strMessageContent;
	unsigned int iCreateTime;
	unsigned int iNotReadMsgNum;

	ContactsMsgInfo() : iMessageType(MessageBodyType_Unknow), iCreateTime(0), iNotReadMsgNum(0) {}
};

struct MessageInfo
{
	int chatType;
	int messageType;
	int createTime;
	std::string messageID;
	std::string receiveID;
	std::string senderID;
	std::string content;
	std::string extraParam;
	int audioDuration;
	std::string localPath;
	int fileType;
	unsigned int fileSize;
	std::string fileName;
	std::string fileExtension;
	int giftID;
	int giftCount;
	std::string anchor;

	MessageInfo()
	{
		createTime = 0;
		audioDuration = 0;
		fileType = 0;
		fileSize = 0;
		giftID = 0;
		giftCount = 0;
	}
};

struct NearbyLocation
{
	unsigned int distance;
	double longitude;
	double latitude;
	std::string userID;
	std::string country;
	std::string province;
	std::string city;
	std::string districtCounty;
	std::string street;

	NearbyLocation() : distance(0), longitude(0), latitude(0){}
};

struct InnerForbidInfo{
	std::string channelID;
	bool isForbidRoom;
	int  reasonType;
	std::string endTime;
};

struct UserBriefInfo{
	std::string userID;
	std::string nickName;
	int onlineStatus;
};

struct FriendRequestInfo{
	std::string askerID;
	std::string askerNickName;
	std::string inviteeID;
	std::string inviteeNickName;
	std::string validateInfo;
	int addStatus;
	unsigned int createTime;
};

std::string  getArrayJsonString(lua_State* tolua_S, int index){
	int iCount = luaL_getn(tolua_S, index);
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kArrayType);
	for (int i = 1; i <= iCount; i++)
	{
		lua_rawgeti(tolua_S, index, i);
		const char* pszItem = lua_tostring(tolua_S, -1);
		rapidjson::Value item;
		item.SetString(pszItem, allocator);
		root.PushBack(item, allocator);

		lua_pop(tolua_S, 1);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	root.Accept(writer);
	std::string reststring = buffer.GetString();

	return reststring;
}

void getArrayNumber(lua_State* tolua_S, int index, std::vector<XUINT64> &vec){
	int iCount = luaL_getn(tolua_S, index);
	vec.clear();
	for (int i = 1; i <= iCount; i++)
	{
		lua_rawgeti(tolua_S, index, i);
		const char* pszItem = lua_tostring(tolua_S, -1);
		vec.push_back(yim_to_xuint64(std::string(pszItem).c_str()));

		lua_pop(tolua_S, 1);
	}
}

void ParseJsonMsgThead()
{
	typedef rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> Document;
	typedef rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::CrtAllocator> JSValue;

	while (true)
	{
		const XCHAR* pszJsonMsg = IM_GetMessage();
		
		if (CustomClass::getInstance()->m_bUninit)
		{
			break;
		}
		if (pszJsonMsg == NULL)
		{
			continue;
		}
		
		Document readdoc;
#ifdef WIN32
		readdoc.Parse<0>(P_Unicode_to_UTF8(pszJsonMsg).c_str());
#else
		
		readdoc.Parse<0>(pszJsonMsg);
		
#endif
		YouMeIMCommand command = (YouMeIMCommand)readdoc["Command"].GetInt();
		YIMErrorcode errorcode = (YIMErrorcode)readdoc["Errorcode"].GetInt();
	
		switch (command) {
		case CMD_RECEIVE_MESSAGE_NITIFY:
		{
			int chatType = readdoc["ChatType"].GetInt();
			std::string strTargetID = readdoc["TargetID"].GetString();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_RecvMsgNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(chatType);
					pStack->pushString(strTargetID.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_RENCENT_CONTACTS:
		{
			JSValue &contactList = readdoc["contacts"];
			std::vector<std::shared_ptr<ContactsMsgInfo> > strContactLists;
			for (int i = 0; i < contactList.Size(); i++)
			{
				std::shared_ptr<ContactsMsgInfo> cInfo(new ContactsMsgInfo);
				const JSValue& msg = contactList[i];
				cInfo->strContactID = msg["ContactID"].GetString();
				cInfo->iMessageType = (YIMMessageBodyType)msg["MessageType"].GetUint();
				cInfo->strMessageContent = msg["MessageContent"].GetString();
				cInfo->iCreateTime = msg["CreateTime"].GetUint();
				cInfo->iNotReadMsgNum = msg["NotReadMsgNum"].GetUint();

				strContactLists.push_back(cInfo);
			}
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetRecentContacts);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					cocos2d::LuaValueArray luaContactLists;
					for (std::vector<std::shared_ptr<ContactsMsgInfo> >::const_iterator itr = strContactLists.begin(); itr != strContactLists.end(); ++itr)
					{
						cocos2d::LuaValueDict luaContactMsg;
						luaContactMsg["ContactID"] = cocos2d::LuaValue::stringValue((*itr)->strContactID);
						luaContactMsg["MessageType"] = cocos2d::LuaValue::intValue((*itr)->iMessageType);
						luaContactMsg["MessageContent"] = cocos2d::LuaValue::stringValue((*itr)->strMessageContent);
						luaContactMsg["CreateTime"] = cocos2d::LuaValue::intValue((*itr)->iCreateTime);

						luaContactLists.push_back(cocos2d::LuaValue::dictValue(luaContactMsg));
					}
					pStack->pushLuaValueArray(luaContactLists);

					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_QUERY_HISTORY_MESSAGE:
		{
			int iRemainCount = readdoc["Remain"].GetInt();
			std::string strTargetID = readdoc["TargetID"].GetString();
			JSValue &msgList = readdoc["messageList"];
			std::vector<std::shared_ptr<MessageInfo> >hisoryLists;
			for (int i = 0; i < msgList.Size(); i++)
			{
				std::shared_ptr<MessageInfo> pMsg(new MessageInfo);
				const JSValue& msg = msgList[i];
				pMsg->messageType = msg["MessageType"].GetInt();
				pMsg->chatType = msg["ChatType"].GetInt();
				pMsg->receiveID = msg["ReceiveID"].GetString();
				pMsg->senderID = msg["SenderID"].GetString();
				pMsg->messageID = msg["Serial"].GetString();
				pMsg->createTime = msg["CreateTime"].GetInt();
				if (pMsg->messageType == MessageBodyType_TXT)
				{
					pMsg->content = msg["Content"].GetString();
				}
				else if (pMsg->messageType == MessageBodyType_CustomMesssage)
				{
					//base4 编码的数据
					pMsg->content = msg["Content"].GetString();
				}
				else if (pMsg->messageType == MessageBodyType_Voice)
				{
					pMsg->audioDuration = msg["Duration"].GetInt();
					pMsg->extraParam = msg["Param"].GetString();
					pMsg->content = msg["Text"].GetString();
					pMsg->localPath = msg["LocalPath"].GetString();
				}

				hisoryLists.push_back(pMsg);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_QueryHistory);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strTargetID.c_str());
					pStack->pushInt(iRemainCount);

					cocos2d::LuaValueArray luaMsgLists;
					for (int i = 0; i < hisoryLists.size(); i++)
					{
						cocos2d::LuaValueDict luaMsg;
						luaMsg["ChatType"] = cocos2d::LuaValue::intValue(hisoryLists[i]->chatType);
						luaMsg["Duration"] = cocos2d::LuaValue::intValue(hisoryLists[i]->audioDuration);
						luaMsg["Param"] = cocos2d::LuaValue::stringValue(hisoryLists[i]->extraParam);
						luaMsg["ReceiveID"] = cocos2d::LuaValue::stringValue(hisoryLists[i]->receiveID);
						luaMsg["SenderID"] = cocos2d::LuaValue::stringValue(hisoryLists[i]->senderID);
						luaMsg["Serial"] = cocos2d::LuaValue::stringValue(hisoryLists[i]->messageID);
						luaMsg["Text"] = cocos2d::LuaValue::stringValue(hisoryLists[i]->content);
						luaMsg["MessageType"] = cocos2d::LuaValue::intValue(hisoryLists[i]->messageType);
						luaMsg["LocalPath"] = cocos2d::LuaValue::stringValue(hisoryLists[i]->localPath);
						luaMsg["CreateTime"] = cocos2d::LuaValue::intValue(hisoryLists[i]->createTime);

						luaMsgLists.push_back(cocos2d::LuaValue::dictValue(luaMsg));
					}

					pStack->pushLuaValueArray(luaMsgLists);


					pStack->executeFunctionByHandler(handler, 4);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_ROOM_HISTORY_MSG:
		{
			std::string roomID = readdoc["RoomID"].GetString();
			int remain = readdoc["Remain"].GetInt();

			JSValue &msgList = readdoc["MessageList"];
			std::vector<MessageInfo> messages;
			for (int i = 0; i < msgList.Size(); i++)
			{
				MessageInfo msg;
				const JSValue& message = msgList[i];
				msg.chatType = message["ChatType"].GetInt();
				msg.messageType = message["MessageType"].GetInt();
				msg.messageID = message["Serial"].GetString();
				msg.createTime = message["CreateTime"].GetInt();
				msg.receiveID = message["ReceiveID"].GetString();
				msg.senderID = message["SenderID"].GetString();
				if ((msg.messageType == MessageBodyType_TXT) || (msg.messageType == MessageBodyType_CustomMesssage))
				{
					msg.content = message["Content"].GetString();
				}
				else if (msg.messageType == MessageBodyType_Voice)
				{
					msg.content = message["Text"].GetString();
					msg.extraParam = message["Param"].GetString();
					msg.audioDuration = message["Duration"].GetInt();
				}
				else if (msg.messageType == MessageBodyType_Gift)
				{
					msg.extraParam = message["Param"].GetString();
					msg.giftID = message["GiftID"].GetInt();
					msg.giftCount = message["GiftCount"].GetInt();
					msg.anchor = message["Anchor"].GetString();
				}
				else if (msg.messageType == MessageBodyType_File)
				{
					msg.fileName = message["FileName"].GetString();
					msg.fileSize = message["FileSize"].GetInt();
					msg.fileType = message["FileType"].GetInt();
					msg.fileExtension = message["FileExtension"].GetString();
					msg.extraParam = message["ExtraParam"].GetString();
				}

				messages.push_back(msg);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_QueryRoomHistoryFromServer);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(roomID.c_str());
					pStack->pushInt(remain);

					cocos2d::LuaValueArray luaMsgLists;
					for (int i = 0; i < messages.size(); i++)
					{
						cocos2d::LuaValueDict luaMsg;
						luaMsg["MessageID"] = cocos2d::LuaValue::stringValue(messages[i].messageID);
						luaMsg["MessageType"] = cocos2d::LuaValue::intValue(messages[i].messageType);
						luaMsg["ChatType"] = cocos2d::LuaValue::intValue(messages[i].chatType);
						luaMsg["ReceiveID"] = cocos2d::LuaValue::stringValue(messages[i].receiveID);
						luaMsg["SenderID"] = cocos2d::LuaValue::stringValue(messages[i].senderID);
						luaMsg["Content"] = cocos2d::LuaValue::stringValue(messages[i].content);
						luaMsg["ExtraParam"] = cocos2d::LuaValue::stringValue(messages[i].extraParam);
						luaMsg["CreateTime"] = cocos2d::LuaValue::intValue(messages[i].createTime);
						luaMsg["AudioDuration"] = cocos2d::LuaValue::intValue(messages[i].audioDuration);
						luaMsg["FileType"] = cocos2d::LuaValue::intValue(messages[i].fileType);
						luaMsg["FileSize"] = cocos2d::LuaValue::intValue(messages[i].fileSize);
						luaMsg["FileName"] = cocos2d::LuaValue::stringValue(messages[i].fileName);
						luaMsg["FileExtension"] = cocos2d::LuaValue::stringValue(messages[i].fileExtension);
						luaMsg["GiftID"] = cocos2d::LuaValue::intValue(messages[i].giftID);
						luaMsg["GiftCount"] = cocos2d::LuaValue::intValue(messages[i].giftCount);
						luaMsg["Anchor"] = cocos2d::LuaValue::stringValue(messages[i].anchor);

						luaMsgLists.push_back(cocos2d::LuaValue::dictValue(luaMsg));
					}
					pStack->pushLuaValueArray(luaMsgLists);

					pStack->executeFunctionByHandler(handler, 4);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_STOP_AUDIOSPEECH:
		{
			std::string strDownloadURL = readdoc["DownloadURL"].GetString();
			int iDuration = readdoc["Duration"].GetInt();
			int iFileSize = readdoc["FileSize"].GetInt();
			std::string strLocalPath = readdoc["LocalPath"].GetString();
			std::string strRequestID = readdoc["RequestID"].GetString();
			std::string strText = readdoc["Text"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_SpeechStatus);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strRequestID.c_str());
					pStack->pushString(strDownloadURL.c_str());
					pStack->pushInt(iDuration);
					pStack->pushInt(iFileSize);
					pStack->pushString(strLocalPath.c_str());
					pStack->pushString(strText.c_str());


					pStack->executeFunctionByHandler(handler, 7);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_DOWNLOAD:
		{
			std::string strSavePath = readdoc["SavePath"].GetString();
			YIMMessageBodyType bodyType = (YIMMessageBodyType)readdoc["MessageType"].GetInt();
			YIMChatType ChatType = (YIMChatType)readdoc["ChatType"].GetInt();
			std::string strSerial = readdoc["Serial"].GetString();
			int iCreateTime = readdoc["CreateTime"].GetInt();
			std::string RecvID = readdoc["ReceiveID"].GetString();
			std::string SenderID = readdoc["SenderID"].GetString();
			std::string Content;
			std::string Param;

			int iDuration = 0;
			if (bodyType == MessageBodyType_Voice)
			{
				Content = readdoc["Text"].GetString();
				Param = readdoc["Param"].GetString();
				iDuration = readdoc["Duration"].GetInt();
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_Download);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushString(strSavePath.c_str());

					pStack->pushInt(bodyType);
					pStack->pushInt(ChatType);
					pStack->pushString(strSerial.c_str());
					pStack->pushString(RecvID.c_str());
					pStack->pushString(SenderID.c_str());
					pStack->pushString(Content.c_str());
					pStack->pushString(Param.c_str());
					pStack->pushInt(iDuration);
					pStack->pushInt(iCreateTime);

					pStack->executeFunctionByHandler(handler, 11);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_DOWNLOAD_URL:
		{
			std::string strFromUrl = readdoc["FromUrl"].GetString();
			std::string strSavePath = readdoc["SavePath"].GetString();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_DownloadByUrl);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushString(strFromUrl.c_str());
					pStack->pushString(strSavePath.c_str());
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_LOGIN:
		{
			std::string	YouMeID = readdoc["UserID"].GetString();
			
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_Login);
				if (0 != handler) {
					
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					
					pStack->pushInt(errorcode);
					pStack->pushString(YouMeID.c_str());
					
					pStack->executeFunctionByHandler(handler, 2);
					
					pStack->clean();
					
				}
			});
		}
		break;
		case CMD_LOGOUT:
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_Logout);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->executeFunctionByHandler(handler, 0);
					pStack->clean();
				}
			});
		}
		break;


		case CMD_SEND_MESSAGE_STATUS:
		{
			std::string strSerial = readdoc["RequestID"].GetString();
			unsigned int iSendTime = readdoc["SendTime"].GetUint();
			bool isForbidRoom = readdoc["IsForbidRoom"].GetInt() == 0 ? false : true;
			int reasonType = readdoc["reasonType"].GetInt();
			std::string strEndTime = readdoc["forbidEndTime"].GetString();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_SendMessageStatus);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strSerial.c_str());
					pStack->pushInt(errorcode);
					pStack->pushInt(iSendTime);
					pStack->pushBoolean(isForbidRoom);
					pStack->pushInt(reasonType);
					pStack->pushString(strEndTime.c_str());

					pStack->executeFunctionByHandler(handler, 6);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_SND_VOICE_MSG:
		{
			std::string strSerial = readdoc["RequestID"].GetString();
			std::string strText = readdoc["Text"].GetString();
			std::string strLocalPath = readdoc["LocalPath"].GetString();
			int iDuration = readdoc["Duration"].GetInt();
			unsigned int iSendTime = readdoc["SendTime"].GetUint();
			bool isForbidRoom = readdoc["IsForbidRoom"].GetInt() == 0 ? false : true;
			int reasonType = readdoc["reasonType"].GetInt();
			std::string strEndTime = readdoc["forbidEndTime"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_SendAudioMessageStatus);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strSerial.c_str());
					pStack->pushInt(errorcode);
					pStack->pushString(strText.c_str());
					pStack->pushString(strLocalPath.c_str());
					pStack->pushInt(iDuration);
					pStack->pushInt(iSendTime);
					pStack->pushBoolean(isForbidRoom);
					pStack->pushInt(reasonType);
					pStack->pushString(strEndTime.c_str());

					pStack->executeFunctionByHandler(handler, 9);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_RECV_MESSAGE:
		{
			YIMMessageBodyType bodyType = (YIMMessageBodyType)readdoc["MessageType"].GetInt();
			YIMChatType ChatType = (YIMChatType)readdoc["ChatType"].GetInt();
			std::string strSerial = readdoc["Serial"].GetString();
			int iCreateTime = readdoc["CreateTime"].GetInt();
			std::string RecvID = readdoc["ReceiveID"].GetString();
			std::string SenderID = readdoc["SenderID"].GetString();
			std::string Content;
			std::string Param;

			std::string iFileName;
			int iFileSize;
			int iFileType;
			std::string iFileExtension;
			std::string iExtraParam;

			int GiftID = 0;
			int GiftCount = 0;
			std::string strAnchor;

			int iDuration = 0;

			if ((bodyType == MessageBodyType_TXT) || (bodyType == MessageBodyType_CustomMesssage))
			{
				Content = readdoc["Content"].GetString();
				Param = readdoc["AttachParam"].GetString();
			}
			else if (bodyType == MessageBodyType_Voice)
			{
				Content = readdoc["Text"].GetString();
				Param = readdoc["Param"].GetString();
				iDuration = readdoc["Duration"].GetInt();
			}
			else if (bodyType == MessageBodyType_Gift)
			{
				Param = readdoc["Param"].GetString();
				GiftID = readdoc["GiftID"].GetInt();
				GiftCount = readdoc["GiftCount"].GetInt();
				strAnchor = readdoc["Anchor"].GetString();
			}
			else if (bodyType == MessageBodyType_File)
			{
				iFileName = readdoc["FileName"].GetString();
				iFileSize = readdoc["FileSize"].GetInt();
				iFileType = readdoc["FileType"].GetInt();
				iFileExtension = readdoc["FileExtension"].GetString();
				iExtraParam = readdoc["ExtraParam"].GetString();
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_RecvMessage);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(bodyType);
					pStack->pushInt(ChatType);
					pStack->pushString(strSerial.c_str());
					pStack->pushString(RecvID.c_str());
					pStack->pushString(SenderID.c_str());
					pStack->pushString(Content.c_str());
					pStack->pushString(Param.c_str());
					pStack->pushInt(iDuration);
					pStack->pushInt(iCreateTime);
					pStack->pushInt(GiftID);
					pStack->pushInt(GiftCount);
					pStack->pushString(strAnchor.c_str());

					pStack->pushString(iFileName.c_str());
					pStack->pushInt(iFileSize);
					pStack->pushInt(iFileType);
					pStack->pushString(iFileExtension.c_str());
					pStack->pushString(iExtraParam.c_str());

					pStack->executeFunctionByHandler(handler, 17);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_ENTER_ROOM:
		{
			std::string strGroupID = readdoc["GroupID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_ChatroomMessage);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strGroupID.c_str());
					pStack->pushInt(errorcode);
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_LEAVE_ALL_ROOM:
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_LeaveAllChatRooms);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_USR_INFO:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			std::string strUserInfo = readdoc["UserInfo"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				//获得一个userinfo的json串，包含nickname,server_area,location,level,vip_level,extra字段
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetUserInfo);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strUserID.c_str());
					pStack->pushString(strUserInfo.c_str());
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_QUERY_USER_STATUS:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			int status = readdoc["Status"].GetInt();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_QueryUserStatus);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strUserID.c_str());
					pStack->pushInt(status);

					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_AUDIO_PLAY_COMPLETE:
		{
			std::string strPath = readdoc["Path"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_PlayComplete);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strPath.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_STOP_SEND_AUDIO:
		{
			std::string strSerial = readdoc["RequestID"].GetString();
			std::string strText = readdoc["Text"].GetString();
			std::string strLocalPath = readdoc["LocalPath"].GetString();
			int iDuration = readdoc["Duration"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_StartSendAudioMessage);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strSerial.c_str());
					pStack->pushInt(errorcode);
					pStack->pushString(strText.c_str());
					pStack->pushString(strLocalPath.c_str());
					pStack->pushInt(iDuration);
					pStack->executeFunctionByHandler(handler, 5);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_TRANSLATE_COMPLETE:
		{
			unsigned int iRequestID = readdoc["RequestID"].GetUint();
			std::string strText = readdoc["Text"].GetString();
			LanguageCode  srcLangCode = (LanguageCode)readdoc["SrcLangCode"].GetInt();
			LanguageCode destLangCode = (LanguageCode)readdoc["DestLangCode"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_TranslateText);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushInt(iRequestID);
					pStack->pushString(strText.c_str());
					pStack->pushInt(srcLangCode);
					pStack->pushInt(destLangCode);
					pStack->executeFunctionByHandler(handler, 5);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_DISTRICT:
		{
			DistrictLevel districtCode = (DistrictLevel)readdoc["DistrictCode"].GetInt();
			std::string strCountry = readdoc["Country"].GetString();
			std::string strProvince = readdoc["Province"].GetString();
			std::string strCity = readdoc["City"].GetString();
			std::string strDistrictCounty = readdoc["DistrictCounty"].GetString();
			std::string strStreet = readdoc["Street"].GetString();
			double fLongitude = readdoc["Longitude"].GetDouble();
			double fLatitude = readdoc["Latitude"].GetDouble();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetCurrentLocation);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushInt(districtCode);
					pStack->pushString(strCountry.c_str());
					pStack->pushString(strProvince.c_str());
					pStack->pushString(strCity.c_str());
					pStack->pushString(strDistrictCounty.c_str());
					pStack->pushString(strStreet.c_str());
					pStack->pushFloat(fLongitude);
					pStack->pushFloat(fLatitude);
					pStack->executeFunctionByHandler(handler, 9);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_PEOPLE_NEARBY:
		{
			unsigned int iStartDistance = readdoc["StartDistance"].GetUint();
			unsigned int iEndDistance = readdoc["EndDistance"].GetUint();
			JSValue &neighbourList = readdoc["NeighbourList"];
			std::vector<std::shared_ptr<NearbyLocation> > nearbyPeopleList;
			for (int i = 0; i < neighbourList.Size(); i++)
			{
				std::shared_ptr<NearbyLocation> pNearbyLocation(new NearbyLocation);
				const JSValue& msg = neighbourList[i];
				pNearbyLocation->userID = msg["UserID"].GetString();
				pNearbyLocation->distance = msg["Distance"].GetUint();
				pNearbyLocation->longitude = msg["Longitude"].GetDouble();
				pNearbyLocation->latitude = msg["Latitude"].GetDouble();
				pNearbyLocation->country = msg["Country"].GetString();
				pNearbyLocation->province = msg["Province"].GetString();
				pNearbyLocation->city = msg["City"].GetString();
				pNearbyLocation->districtCounty = msg["DistrictCounty"].GetString();
				pNearbyLocation->street = msg["Street"].GetString();
				nearbyPeopleList.push_back(pNearbyLocation);
			}
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetNearbyObjects);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushInt(iStartDistance);
					pStack->pushInt(iEndDistance);

					cocos2d::LuaValueArray nearbyList;
					for (std::vector<std::shared_ptr<NearbyLocation> >::const_iterator itr = nearbyPeopleList.begin(); itr != nearbyPeopleList.end(); ++itr)
					{
						cocos2d::LuaValueDict luaLocation;
						luaLocation["UserID"] = cocos2d::LuaValue::stringValue((*itr)->userID);
						luaLocation["Distance"] = cocos2d::LuaValue::intValue((*itr)->distance);
						luaLocation["Longitude"] = cocos2d::LuaValue::floatValue((*itr)->longitude);
						luaLocation["Latitude"] = cocos2d::LuaValue::floatValue((*itr)->latitude);
						luaLocation["Country"] = cocos2d::LuaValue::stringValue((*itr)->country);
						luaLocation["Province"] = cocos2d::LuaValue::stringValue((*itr)->province);
						luaLocation["City"] = cocos2d::LuaValue::stringValue((*itr)->city);
						luaLocation["DistrictCounty"] = cocos2d::LuaValue::stringValue((*itr)->districtCounty);
						luaLocation["Street"] = cocos2d::LuaValue::stringValue((*itr)->street);

						nearbyList.push_back(cocos2d::LuaValue::dictValue(luaLocation));
					}
					pStack->pushLuaValueArray(nearbyList);

					pStack->executeFunctionByHandler(handler, 4);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_DISTANCE:
		{
			std::string userID = readdoc["UserID"].GetString();
			unsigned int distance = readdoc["Distance"].GetUint();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetDistance);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushString(userID.c_str());
					pStack->pushInt(distance);
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_USER_ENTER_ROOM:
		{
			std::string strChannelID = readdoc["ChannelID"].GetString();
			std::string strUserID = readdoc["UserID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_UserJoinRoom);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushString(strChannelID.c_str());
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_USER_LEAVE_ROOM:
		{
			std::string strChannelID = readdoc["ChannelID"].GetString();
			std::string strUserID = readdoc["UserID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_UserLeaveRoom);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushString(strChannelID.c_str());
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_MICROPHONE_STATUS:
		{
			int status = readdoc["Status"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_MicrophoneStatus);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(status);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_TIPOFF_MSG:
		{
			int result = readdoc["Result"].GetInt();
			std::string userID = readdoc["UserID"].GetString();
			unsigned int status = readdoc["AccusationTime"].GetUint();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_AccusationResultNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(result);
					pStack->pushString(userID.c_str());
					pStack->pushInt(status);
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_RECV_NOTICE:
		{
			std::string noticeID = readdoc["NoticeID"].GetString();
			std::string channelID = readdoc["ChannelID"].GetString();
			int noticeType = readdoc["NoticeType"].GetInt();
			std::string noticeContent = readdoc["NoticeContent"].GetString();
			std::string linkText = readdoc["LinkText"].GetString();
			std::string linkAddress = readdoc["LinkAddress"].GetString();
			unsigned int beginTime = readdoc["BeginTime"].GetUint();
			unsigned int endTime = readdoc["EndTime"].GetUint();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_RecvNotice);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					cocos2d::LuaValueDict luaNoticeInfo;
					luaNoticeInfo["NoticeID"] = cocos2d::LuaValue::stringValue(noticeID);
					luaNoticeInfo["ChannelID"] = cocos2d::LuaValue::stringValue(channelID);
					luaNoticeInfo["NoticeType"] = cocos2d::LuaValue::intValue(noticeType);
					luaNoticeInfo["NoticeContent"] = cocos2d::LuaValue::stringValue(noticeContent);
					luaNoticeInfo["LinkText"] = cocos2d::LuaValue::stringValue(linkText);
					luaNoticeInfo["LinkAddress"] = cocos2d::LuaValue::stringValue(linkAddress);
					luaNoticeInfo["BeginTime"] = cocos2d::LuaValue::intValue(beginTime);
					luaNoticeInfo["EndTime"] = cocos2d::LuaValue::intValue(endTime);

					pStack->pushLuaValueDict(luaNoticeInfo);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_CANCEL_NOTICE:
		{
			std::string noticeID = readdoc["NoticeID"].GetString();
			std::string channelID = readdoc["ChannelID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_AccusationResultNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushString(noticeID.c_str());
					pStack->pushString(channelID.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_FORBID_RECORD:
		{
			JSValue &forbidList = readdoc["ForbiddenSpeakList"];
			std::vector<std::shared_ptr<InnerForbidInfo> > forbidInfoList;
			for (int i = 0; i < forbidList.Size(); i++)
			{
				std::shared_ptr<InnerForbidInfo> pInfo(new InnerForbidInfo);
				const JSValue& msg = forbidList[i];
				pInfo->channelID = msg["ChannelID"].GetString();
				pInfo->isForbidRoom = msg["IsForbidRoom"].GetInt() == 0 ? false : true;
				pInfo->reasonType = msg["reasonType"].GetInt();
				pInfo->endTime = msg["forbidEndTime"].GetString();

				forbidInfoList.push_back(pInfo);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetForbiddenSpeakInfo);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);

					cocos2d::LuaValueArray arr;
					for (std::vector<std::shared_ptr<InnerForbidInfo> >::const_iterator itr = forbidInfoList.begin(); itr != forbidInfoList.end(); ++itr)
					{
						cocos2d::LuaValueDict luaforbid;
						luaforbid["channelID"] = cocos2d::LuaValue::stringValue((*itr)->channelID);
						luaforbid["isForbidRoom"] = cocos2d::LuaValue::booleanValue((*itr)->isForbidRoom);
						luaforbid["reasonType"] = cocos2d::LuaValue::intValue((*itr)->reasonType);
						luaforbid["endTime"] = cocos2d::LuaValue::stringValue((*itr)->endTime);

						arr.push_back(cocos2d::LuaValue::dictValue(luaforbid));
					}
					pStack->pushLuaValueArray(arr);

					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_SET_MASK_USER_MSG:
		{
			std::string userID = readdoc["UserID"].GetString();
			bool block = readdoc["Block"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_BlockUser);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushString(userID.c_str());
					pStack->pushBoolean(block);
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_MASK_USER_MSG:
		{
			JSValue &userList = readdoc["UserList"];
			std::vector<std::string> users;
			for (int i = 0; i < userList.Size(); i++)
			{
				std::string userID = userList[i].GetString();
				users.push_back(userID);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetBlockUsers);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);

					cocos2d::LuaValueArray arr;
					for (std::vector<std::string>::const_iterator itr = users.begin(); itr != users.end(); ++itr)
					{
						arr.push_back(cocos2d::LuaValue::stringValue(*itr));
					}
					pStack->pushLuaValueArray(arr);

					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_CLEAN_MASK_USER_MSG:
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_UnBlockAllUser);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_ROOM_INFO:
		{
			std::string roomID = readdoc["RoomID"].GetString();
			unsigned int count = readdoc["Count"].GetUint();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetRoomMemberCount);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushString(roomID.c_str());
					pStack->pushInt(count);
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_SPEECH_TEXT:
		{
			std::string strSerial = readdoc["RequestID"].GetString();
			std::string text = readdoc["Text"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetRecognizeSpeechText);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushString(strSerial.c_str());
					pStack->pushInt(errorcode);
					pStack->pushString(text.c_str());
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_RECONNECT_RESULT:
		{
			int result = readdoc["Result"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_RecvReconnectResult);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(result);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_START_RECONNECT:
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_StartReconnect);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->executeFunctionByHandler(handler, 0);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_RECORD_VOLUME:
		{
			float volume = (float)readdoc["Volume"].GetDouble();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_RecvRecordVolume);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushFloat(volume);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_HXR_USER_INFO_CHANGE_NOTIFY:
		{
			std::string strUserID = readdoc["UserID"].GetString();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_UserProfileInfoChangeNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_SET_USER_PROFILE:
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_SetUserProfile);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_GET_USER_PROFILE:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			std::string strPhotoUrl = readdoc["PhotoUrl"].GetString();
			int onlineState = readdoc["OnlineState"].GetInt();
			int beAddPermission = readdoc["BeAddPermission"].GetInt();
			int foundPermission = readdoc["FoundPermission"].GetInt();
			std::string strNickName = readdoc["NickName"].GetString();
			int sex = readdoc["Sex"].GetInt();
			std::string strSignature = readdoc["Signature"].GetString();
			std::string strCountry = readdoc["Country"].GetString();
			std::string strProvince = readdoc["Province"].GetString();
			std::string strCity = readdoc["City"].GetString();
			std::string strExtraInfo = readdoc["ExtraInfo"].GetString();

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_GetUserProfile);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);

					pStack->pushString(strUserID.c_str());
					pStack->pushString(strPhotoUrl.c_str());
					pStack->pushInt(onlineState);
					pStack->pushInt(beAddPermission);
					pStack->pushInt(foundPermission);
					pStack->pushString(strNickName.c_str());
					pStack->pushInt(sex);
					pStack->pushString(strSignature.c_str());
					pStack->pushString(strCountry.c_str());
					pStack->pushString(strProvince.c_str());
					pStack->pushString(strCity.c_str());
					pStack->pushString(strExtraInfo.c_str());

					pStack->executeFunctionByHandler(handler, 13);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_SET_USER_PHOTO:
		{
			std::string strPhotoUrl = readdoc["PhotoUrl"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_SetUserPhotoUrl);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strPhotoUrl.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_SWITCH_USER_STATE:
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_SwitchUserOnlineState);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_FIND_FRIEND_BY_ID:
		{
			JSValue &userList = readdoc["UserList"];
			std::vector<std::shared_ptr<UserBriefInfo> > userInfoList;
			for (int i = 0; i < userList.Size(); i++)
			{
				std::shared_ptr<UserBriefInfo> pInfo(new UserBriefInfo);
				const JSValue& msg = userList[i];
				pInfo->userID = msg["UserID"].GetString();
				pInfo->nickName = msg["Nickname"].GetString();
				pInfo->onlineStatus = msg["Status"].GetInt();

				userInfoList.push_back(pInfo);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_FindFriend);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);

					cocos2d::LuaValueArray arr;
					for (std::vector<std::shared_ptr<UserBriefInfo> >::const_iterator itr = userInfoList.begin(); itr != userInfoList.end(); ++itr)
					{
						cocos2d::LuaValueDict luaUserBriefInfo;
						luaUserBriefInfo["UserID"] = cocos2d::LuaValue::stringValue((*itr)->userID);
						luaUserBriefInfo["Nickname"] = cocos2d::LuaValue::stringValue((*itr)->nickName);
						luaUserBriefInfo["Status"] = cocos2d::LuaValue::intValue((*itr)->onlineStatus);

						arr.push_back(cocos2d::LuaValue::dictValue(luaUserBriefInfo));
					}
					pStack->pushLuaValueArray(arr);

					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_REQUEST_ADD_FRIEND:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_RequestAddFriend);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_REQUEST_ADD_FRIEND_NOTIFY:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			std::string strComments = readdoc["Comments"].GetString();
			std::string strReqID = readdoc["ReqID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_BeAddFriendRequestNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strUserID.c_str());
					pStack->pushString(strComments.c_str());
					pStack->pushString(strReqID.c_str());
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_BE_ADD_FRIENT:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			std::string strComments = readdoc["Comments"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_BeAddFriendNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strUserID.c_str());
					pStack->pushString(strComments.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_DEAL_ADD_FRIEND:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			std::string strComments = readdoc["Comments"].GetString();
			int dealResult = readdoc["DealResult"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_DealBeAddFriend);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strUserID.c_str());
					pStack->pushString(strComments.c_str());
					pStack->pushInt(dealResult);
					pStack->executeFunctionByHandler(handler, 4);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_ADD_FRIENT_RESULT_NOTIFY:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			std::string strComments = readdoc["Comments"].GetString();
			int dealResult = readdoc["DealResult"].GetInt();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_AddFriendResultNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strUserID.c_str());
					pStack->pushString(strComments.c_str());
					pStack->pushInt(dealResult);
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_DELETE_FRIEND:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_DeleteFriend);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 2);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_BE_DELETE_FRIEND_NOTIFY:
		{
			std::string strUserID = readdoc["UserID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_BeDeleteNotify);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 1);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_BLACK_FRIEND:
		{
			int type = readdoc["Type"].GetInt();
			std::string strUserID = readdoc["UserID"].GetString();
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_BlackFriend);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}
					pStack->pushInt(errorcode);
					pStack->pushInt(type);
					pStack->pushString(strUserID.c_str());
					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_QUERY_FRIEND_LIST:
		{
			int type = readdoc["Type"].GetInt();
			int startIndex = readdoc["StartIndex"].GetInt();
			JSValue &userList = readdoc["UserList"];
			std::vector<std::shared_ptr<UserBriefInfo> > userInfoList;
			for (int i = 0; i < userList.Size(); i++)
			{
				std::shared_ptr<UserBriefInfo> pInfo(new UserBriefInfo);
				const JSValue& msg = userList[i];
				pInfo->userID = msg["UserID"].GetString();
				pInfo->nickName = msg["Nickname"].GetString();
				pInfo->onlineStatus = msg["Status"].GetInt();

				userInfoList.push_back(pInfo);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_QueryFriendList);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushInt(type);
					pStack->pushInt(startIndex);

					cocos2d::LuaValueArray arr;
					for (std::vector<std::shared_ptr<UserBriefInfo> >::const_iterator itr = userInfoList.begin(); itr != userInfoList.end(); ++itr)
					{
						cocos2d::LuaValueDict luaUserBriefInfo;
						luaUserBriefInfo["UserID"] = cocos2d::LuaValue::stringValue((*itr)->userID);
						luaUserBriefInfo["Nickname"] = cocos2d::LuaValue::stringValue((*itr)->nickName);
						luaUserBriefInfo["Status"] = cocos2d::LuaValue::intValue((*itr)->onlineStatus);

						arr.push_back(cocos2d::LuaValue::dictValue(luaUserBriefInfo));
					}
					pStack->pushLuaValueArray(arr);

					pStack->executeFunctionByHandler(handler, 4);
					pStack->clean();
				}
			});
		}
		break;
		case CMD_QUERY_FRIEND_REQUEST_LIST:
		{
			int startIndex = readdoc["StartIndex"].GetInt();
			JSValue &userList = readdoc["UserList"];
			std::vector<std::shared_ptr<FriendRequestInfo> > requestInfoList;
			for (int i = 0; i < userList.Size(); i++)
			{
				std::shared_ptr<FriendRequestInfo> pInfo(new FriendRequestInfo);
				const JSValue& msg = userList[i];
				pInfo->askerID = msg["AskerID"].GetString();
				pInfo->askerNickName = msg["AskerNickname"].GetString();
				pInfo->inviteeID = msg["InviteeID"].GetString();
				pInfo->inviteeNickName = msg["InviteeNickname"].GetString();
				pInfo->validateInfo = msg["ValidateInfo"].GetString();
				pInfo->addStatus = msg["Status"].GetInt();
				pInfo->createTime = msg["CreateTime"].GetUint();

				requestInfoList.push_back(pInfo);
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)CustomClass::getInstance(), EVENT_YIM_QueryFriendRequestList);
				if (0 != handler) {
					LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
					if (NULL == pStack) {
						return;
					}

					lua_State *tolua_s = pStack->getLuaState();
					if (NULL == tolua_s) {
						return;
					}

					pStack->pushInt(errorcode);
					pStack->pushInt(startIndex);

					cocos2d::LuaValueArray arr;
					for (std::vector<std::shared_ptr<FriendRequestInfo> >::const_iterator itr = requestInfoList.begin(); itr != requestInfoList.end(); ++itr)
					{
						cocos2d::LuaValueDict luaRequestInfo;
						luaRequestInfo["AskerID"] = cocos2d::LuaValue::stringValue((*itr)->askerID);
						luaRequestInfo["AskerNickname"] = cocos2d::LuaValue::stringValue((*itr)->askerNickName);
						luaRequestInfo["InviteeID"] = cocos2d::LuaValue::stringValue((*itr)->inviteeID);
						luaRequestInfo["InviteeNickname"] = cocos2d::LuaValue::stringValue((*itr)->inviteeNickName);
						luaRequestInfo["ValidateInfo"] = cocos2d::LuaValue::stringValue((*itr)->validateInfo);
						luaRequestInfo["Status"] = cocos2d::LuaValue::intValue((*itr)->addStatus);
						luaRequestInfo["CreateTime"] = cocos2d::LuaValue::intValue((*itr)->createTime);

						arr.push_back(cocos2d::LuaValue::dictValue(luaRequestInfo));
					}
					pStack->pushLuaValueArray(arr);

					pStack->executeFunctionByHandler(handler, 3);
					pStack->clean();
				}
			});
		}
		break;
		default:
			break;
		}
		IM_PopMessage(pszJsonMsg);
	}
}

CustomClass::CustomClass(){

}

CustomClass::~CustomClass(){
}

CustomClass* cocos2d::CustomClass::getInstance()
{
	static CustomClass* s_instance = NULL;
	if (NULL == s_instance)
	{
		s_instance = new CustomClass;
	}
	return s_instance;
}

void cocos2d::CustomClass::Uninit()
{
	m_bUninit = true;
	IM_Uninit();
	if (m_parseJsonThread.joinable())
	{
		m_parseJsonThread.join();
	}
}

static int tolua_Cocos2d_YIM_create(lua_State* tolua_S)
{
	CustomClass *wSocket = CustomClass::getInstance();
	tolua_pushusertype(tolua_S, (void*)wSocket, "cc.YIM");
	tolua_register_gc(tolua_S, lua_gettop(tolua_S));
	return 1;
}
static int tolua_Cocos2d_YIM_downloadAudioFile(lua_State* tolua_S)
{
	CustomClass *wSocket = CustomClass::getInstance();
	const char *pszSerial = ((const char*)tolua_tostring(tolua_S, 2, 0));
	const char *pszSavePath = ((const char*)tolua_tostring(tolua_S, 3, 0));
	int iErrorcode = IM_DownloadFile(yim_to_xuint64(pszSerial), UTF8TOPlatString(std::string(pszSavePath)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_downloadFileByUrl(lua_State* tolua_S)
{
	CustomClass *wSocket = CustomClass::getInstance();
	const char *pszUrl = ((const char*)tolua_tostring(tolua_S, 2, 0));
	const char *pszSavePath = ((const char*)tolua_tostring(tolua_S, 3, 0));
	int iErrorcode = IM_DownloadFileByURL(UTF8TOPlatString(std::string(pszUrl)).c_str(), UTF8TOPlatString(std::string(pszSavePath)).c_str(),FileType_Audio);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}


static int tolua_Cocos2d_YIM_leaveChatRoom(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszChatroom = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_LeaveChatRoom(UTF8TOPlatString(std::string(pszChatroom)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_joinChatRoom(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszChatroom = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_JoinChatRoom(UTF8TOPlatString(std::string(pszChatroom)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_leaveAllChatRooms(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iErrorcode = IM_LeaveAllChatRooms();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_cancleAudioMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iErrorcode = IM_CancleAudioMessage();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}



static int tolua_Cocos2d_YIM_stopAudioMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszParam = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_StopAudioMessage(UTF8TOPlatString(std::string(pszParam)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}


static int tolua_Cocos2d_YIM_sendOnlyAudioMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRecvID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iChatType = lua_tointeger(tolua_S, 3);
	XUINT64 iRequestID = -1;
	int iErrorcode = IM_SendOnlyAudioMessage(UTF8TOPlatString(std::string(pszRecvID)).c_str(), (YIMChatType)iChatType, &iRequestID);
	if (iErrorcode == 0) {
		lua_pushstring(tolua_S, yim_to_string(iRequestID).c_str());
	}
	else
	{
		lua_pushstring(tolua_S, "-1");
	}

	return 1;
}

static int tolua_Cocos2d_YIM_sendAudioMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRecvID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iChatType = lua_tointeger(tolua_S, 3);
	XUINT64 iRequestID = -1;
	IM_SendAudioMessage(UTF8TOPlatString(std::string(pszRecvID)).c_str(), (YIMChatType)iChatType, &iRequestID);
	lua_pushstring(tolua_S, yim_to_string(iRequestID).c_str());
	return 1;
}

static int tolua_Cocos2d_YIM_sendFile(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRecvID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iChatType = lua_tointeger(tolua_S, 3);
	const char *filePath = ((const char*)tolua_tostring(tolua_S, 4, 0));
	XUINT64 iRequestID = -1;
	const char *extra = ((const char*)tolua_tostring(tolua_S, 5, 0));
	int iFileType = lua_tointeger(tolua_S, 6);

	IM_SendFile(UTF8TOPlatString(std::string(pszRecvID)).c_str(), (YIMChatType)iChatType, UTF8TOPlatString(std::string(filePath)).c_str(), UTF8TOPlatString(std::string(extra)).c_str(), (YIMFileType)iFileType, &iRequestID);
	lua_pushstring(tolua_S, yim_to_string(iRequestID).c_str());
	return 1;
}

static int tolua_Cocos2d_YIM_init(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszAppKey = ((const char*)tolua_tostring(tolua_S, 2, 0));
	const char *pszAppSec = ((const char*)tolua_tostring(tolua_S, 3, 0));
	int iErrorcode = IM_Init(UTF8TOPlatString(std::string(pszAppKey)).c_str(), UTF8TOPlatString(std::string(pszAppSec)).c_str());
	if (iErrorcode == YIMErrorcode_Success)
	{
		if (!self->m_parseJsonThread.joinable())
		{
			self->m_parseJsonThread = std::thread(ParseJsonMsgThead);
		}

	}
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}



static int tolua_Cocos2d_YIM_getFilterText(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszFileText = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int level = 0;
	XCHAR* pszDealText = IM_GetFilterText(UTF8TOPlatString(std::string(pszFileText)).c_str(), &level);
	std::string strUTFText = PlatStringToUTF8(pszDealText);
	IM_DestroyFilterText(pszDealText);
	lua_pushstring(tolua_S, strUTFText.c_str());
	lua_pushinteger(tolua_S, level);
	return 2;
}

static int tolua_Cocos2d_YIM_GetAudioCachePath(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	XCHAR* pszPath = IM_GetAudioCachePath();
	std::string strPath = PlatStringToUTF8(pszPath);
	IM_DestroyFilterText(pszPath);
	lua_pushstring(tolua_S, strPath.c_str());
	return 1;
}


static int tolua_Cocos2d_YIM_setServerZone(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iServerZone = lua_tointeger(tolua_S, 2);
	IM_SetServerZone((ServerZone)iServerZone);
	return 0;
}



static int tolua_Cocos2d_YIM_startAudioSpeech(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	bool bTrans = lua_toboolean(tolua_S, 2);
	XUINT64 ulSerial = -1;
	IM_StartAudioSpeech(&ulSerial, bTrans);
	lua_pushstring(tolua_S, yim_to_string(ulSerial).c_str());
	return 1;
}

static int tolua_Cocos2d_YIM_stopAudioSpeech(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iErrorCode = IM_StopAudioSpeech();
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}

static int tolua_Cocos2d_YIM_convertAMRToWav(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char* pszSrc = lua_tostring(tolua_S, 2);
	const char* pszDest = lua_tostring(tolua_S, 3);
	int iErrorCode = IM_ConvertAMRToWav(UTF8TOPlatString(std::string(pszSrc)).c_str(), UTF8TOPlatString(std::string(pszDest)).c_str());
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}


static int tolua_Cocos2d_YIM_queryRoomHistoryMessageFromServer(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char* pszRoomID = lua_tostring(tolua_S, 2);
	int iCount = lua_tointeger(tolua_S, 3);
	int iDirection = lua_tointeger(tolua_S, 4);
	int iErrorCode = IM_QueryRoomHistoryMessageFromServer(UTF8TOPlatString(std::string(pszRoomID)).c_str(), iCount, iDirection);
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}


static int tolua_Cocos2d_YIM_deleteHistoryMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iChatType = lua_tointeger(tolua_S, 2);
	const char* pszTime = lua_tostring(tolua_S, 3);
	int iErrorCode = IM_DeleteHistoryMessage((YIMChatType)iChatType, yim_to_xuint64(pszTime));
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}


static int tolua_Cocos2d_YIM_deleteHistoryMessageByID(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char* pszMessageID = lua_tostring(tolua_S, 2);
	int iErrorCode = IM_DeleteHistoryMessageByID(yim_to_xuint64(pszMessageID));
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}

static int tolua_Cocos2d_YIM_deleteSpecifiedHistoryMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char* pszTarget = lua_tostring(tolua_S, 2);
	int iChatType = lua_tointeger(tolua_S, 3);
	std::vector<XUINT64> msgIDNumVec;
	getArrayNumber(tolua_S, 4, msgIDNumVec);

	int iErrorCode = IM_DeleteSpecifiedHistoryMessage(UTF8TOPlatString(std::string(pszTarget)).c_str(), (YIMChatType)iChatType, (XUINT64 *)msgIDNumVec.data(), (int)msgIDNumVec.size());
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}

static int tolua_Cocos2d_YIM_queryHistoryMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char* pszTarget = lua_tostring(tolua_S, 2);
	int iChatType = lua_tointeger(tolua_S, 3);
	const char* pszSerial = lua_tostring(tolua_S, 4);
	int iCount = lua_tointeger(tolua_S, 5);
	int iDirect = lua_tointeger(tolua_S, 6);
	int iErrorCode = IM_QueryHistoryMessage(UTF8TOPlatString(std::string(pszTarget)).c_str(), iChatType, yim_to_xuint64(pszSerial), iCount, iDirect);
	lua_pushinteger(tolua_S, iErrorCode);
	return 1;
}


static int tolua_Cocos2d_YIM_setAudioCacheDir(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszAudioCacheDir = ((const char*)tolua_tostring(tolua_S, 2, 0));
	IM_SetAudioCacheDir(UTF8TOPlatString(std::string(pszAudioCacheDir)).c_str());
	return 0;
}

static int tolua_Cocos2d_YIM_login(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszUserName = ((const char*)tolua_tostring(tolua_S, 2, 0));
	const char *pszPasswd = ((const char*)tolua_tostring(tolua_S, 3, 0));
	const char *pszToken = ((const char*)tolua_tostring(tolua_S, 4, 0));
	int iErrorcode = IM_Login(UTF8TOPlatString(std::string(pszUserName)).c_str(),
		UTF8TOPlatString(std::string(pszPasswd)).c_str(),
		UTF8TOPlatString(std::string(pszToken)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

//传一个json字符串，需要包含nickname,server_area,location,level,vip_level,extra字段
static int tolua_Cocos2d_YIM_setUserInfo(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	const char *pszUserInfo = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_SetUserInfo(UTF8TOPlatString(std::string(pszUserInfo)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

//获取联系人信息
static int tolua_Cocos2d_YIM_getUserInfo(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_GetUserInfo(UTF8TOPlatString(std::string(pszUserID)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

//获取用户登录状态
static int tolua_Cocos2d_YIM_QueryUserStatus(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_QueryUserStatus(UTF8TOPlatString(std::string(pszUserID)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

//设置音量
static int tolua_Cocos2d_YIM_SetVolume(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	float volume = (float)tolua_tonumber(tolua_S, 2, 0);
	IM_SetVolume(volume);
	return 0;
}

//播放录音
static int tolua_Cocos2d_YIM_StartPlayAudio(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	const char *pszPath = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iErrorcode = IM_StartPlayAudio(UTF8TOPlatString(std::string(pszPath)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_StopPlayAudio(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	int iErrorcode = IM_StopPlayAudio();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_IsPlaying(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	bool bPlaying = IM_IsPlaying();
	lua_pushboolean(tolua_S, bPlaying);
	return 1;
}

static int tolua_Cocos2d_YIM_ClearAudioCachePath(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	bool bRes = IM_ClearAudioCachePath();
	lua_pushboolean(tolua_S, bRes);
	return 1;
}

static int tolua_Cocos2d_YIM_SetRoomHistoryMessageSwitch(lua_State* tolua_S)
{

	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	std::string strTargets = getArrayJsonString(tolua_S, 2);

	bool iSave = lua_toboolean(tolua_S, 3);

	int iErrorcode = IM_SetRoomHistoryMessageSwitch(UTF8TOPlatString(strTargets).c_str(), iSave);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_sendGift(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRecvID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	const char *pszChannel = ((const char*)tolua_tostring(tolua_S, 3, 0));
	int iGiftID = lua_tointeger(tolua_S, 4);
	int iGiftCount = lua_tointeger(tolua_S, 5);
	const char *pszExtParam = ((const char*)tolua_tostring(tolua_S, 6, 0));

	XUINT64 iRequestID = -1;
	IM_SendGift(UTF8TOPlatString(std::string(pszRecvID)).c_str(), UTF8TOPlatString(std::string(pszChannel)).c_str(),
		iGiftID, iGiftCount, pszExtParam, &iRequestID);
	lua_pushstring(tolua_S, yim_to_string(iRequestID).c_str());
	return 1;
}

static int tolua_Cocos2d_YIM_getNewMessage(lua_State* tolua_S)
{
	std::string str = getArrayJsonString(tolua_S, 2);
	int iErrorcode = IM_GetNewMessage(UTF8TOPlatString(str).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_setDownloadAudioMessageSwitch(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);

	int iDownload = lua_tointeger(tolua_S, 2);

	int iErrorcode = IM_SetDownloadAudioMessageSwitch(iDownload);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_setReceiveMessageSwitch(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	std::string strTargets = getArrayJsonString(tolua_S, 2);

	bool iAutoRecv = lua_toboolean(tolua_S, 3);

	int iErrorcode = IM_SetReceiveMessageSwitch(UTF8TOPlatString(strTargets).c_str(), iAutoRecv);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_getContact(lua_State* tolua_S)
{
	int iErrorcode = IM_GetRecentContacts();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}
static int tolua_Cocos2d_YIM_multiSendTextMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iCount = luaL_getn(tolua_S, 2);
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kArrayType);
	for (int i = 1; i <= iCount; i++)
	{
		lua_rawgeti(tolua_S, 2, i);
		const char* pszRecvID = lua_tostring(tolua_S, -1);
		rapidjson::Value item;
		item.SetString(pszRecvID, allocator);
		root.PushBack(item, allocator);

		lua_pop(tolua_S, 1);
	}
	const char *pszText = ((const char*)tolua_tostring(tolua_S, 3, 0));
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	root.Accept(writer);
	std::string reststring = buffer.GetString();

	int iErrorCode = IM_MultiSendTextMessage(reststring.c_str(), UTF8TOPlatString(std::string(pszText)).c_str());
	lua_pushinteger(tolua_S, iErrorCode);

	return 1;
}

static int tolua_Cocos2d_YIM_sendTextMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRecvID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iChatType = lua_tointeger(tolua_S, 3);
	const char *pszContent = ((const char*)tolua_tostring(tolua_S, 4, 0));
	const char *pszAttachParam = ((const char*)tolua_tostring(tolua_S, 5, 0));
	XUINT64 iRequestID = -1;
	if (pszAttachParam == NULL)
		pszAttachParam = "";
	IM_SendTextMessage(UTF8TOPlatString(std::string(pszRecvID)).c_str(), (YIMChatType)iChatType, UTF8TOPlatString(std::string(pszContent)).c_str(), UTF8TOPlatString(std::string(pszAttachParam)).c_str(), &iRequestID);

	lua_pushstring(tolua_S, yim_to_string(iRequestID).c_str());
	return 1;
}

static int tolua_Cocos2d_YIM_onResume(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	IM_OnResume();
	return 0;
}

static int tolua_Cocos2d_YIM_onPause(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iPauseReceiveMessage = lua_tointeger(tolua_S, 2);
	IM_OnPause(iPauseReceiveMessage);
	return 0;
}

static int tolua_Cocos2d_YIM_logout(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iErrorcode = IM_Logout();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_TranslateText(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszText = ((const char*)tolua_tostring(tolua_S, 2, 0));
	LanguageCode destLanguage = (LanguageCode)lua_tointeger(tolua_S, 3);
	LanguageCode srcLanguage = (LanguageCode)lua_tointeger(tolua_S, 4);
	unsigned int iRequestID = 0;
	IM_TranslateText(&iRequestID, UTF8TOPlatString(std::string(pszText)).c_str(), destLanguage, srcLanguage);
	lua_pushinteger(tolua_S, iRequestID);
	return 1;
}

static int tolua_Cocos2d_YIM_GetCurrentLocation(lua_State* tolua_S)
{
	int iErrorcode = IM_GetCurrentLocation();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_GetNearbyObjects(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iCount = lua_tointeger(tolua_S, 2);
	const char *pszServerAreaID = ((const char*)tolua_tostring(tolua_S, 3, 0));
	DistrictLevel districtLevel = (DistrictLevel)lua_tointeger(tolua_S, 4);
	bool bResetStartDistance = lua_toboolean(tolua_S, 5);
	int iErrorcode = IM_GetNearbyObjects(iCount, UTF8TOPlatString(std::string(pszServerAreaID)).c_str(), districtLevel, bResetStartDistance);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_GetDistance(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszUserID = (const char*)tolua_tostring(tolua_S, 2, 0);
	int iErrorcode = IM_GetDistance(UTF8TOPlatString(std::string(pszUserID)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_SetUpdateInterval(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iInterval = lua_tointeger(tolua_S, 2);
	IM_SetUpdateInterval(iInterval);
	return 1;
}

// 设置语音识别语言
static int tolua_Cocos2d_YIM_SetSpeechRecognizeLanguage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int language = lua_tointeger(tolua_S, 2);

	int iErrorcode = IM_SetSpeechRecognizeLanguage(language);
	lua_pushinteger(tolua_S, iErrorcode);

	return 1;
}

// 设置只识别语音文字
static int tolua_Cocos2d_YIM_SetOnlyRecognizeSpeechText(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	bool recognition = lua_toboolean(tolua_S, 2);

	int iErrorcode = IM_SetOnlyRecognizeSpeechText(recognition);
	lua_pushinteger(tolua_S, iErrorcode);

	return 1;
}

// 查询麦克风状态
static int tolua_Cocos2d_YIM_GetMicrophoneStatus(lua_State* tolua_S)
{
	int iErrorcode = IM_GetMicrophoneStatus();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

// 举报
static int tolua_Cocos2d_YIM_Accusation(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszUserID = (const char*)tolua_tostring(tolua_S, 2, 0);
	int iChatType = lua_tointeger(tolua_S, 3);
	int iReason = lua_tointeger(tolua_S, 4);
	const char *pszDescription = (const char*)tolua_tostring(tolua_S, 5, 0);
	const char *pszExtraParam = (const char*)tolua_tostring(tolua_S, 6, 0);

	int iErrorcode = IM_Accusation(UTF8TOPlatString(std::string(pszUserID)).c_str(), (YIMChatType)iChatType, iReason,
		UTF8TOPlatString(std::string(pszDescription)).c_str(),
		UTF8TOPlatString(std::string(pszExtraParam)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);

	return 1;
}

// 查询公告
static int tolua_Cocos2d_YIM_QueryNotice(lua_State* tolua_S)
{
	int iErrorcode = IM_QueryNotice();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_GetForbiddenSpeakInfo(lua_State* tolua_S)
{
	int iErrorcode = IM_GetForbiddenSpeakInfo();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_GetRoomMemberCount(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRoomId = (const char*)tolua_tostring(tolua_S, 2, 0);
	int iErrorcode = IM_GetRoomMemberCount(UTF8TOPlatString(std::string(pszRoomId)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_BlockUser(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszUserId = (const char*)tolua_tostring(tolua_S, 2, 0);
	bool block = tolua_toboolean(tolua_S, 3, 0);
	int iErrorcode = IM_BlockUser(UTF8TOPlatString(std::string(pszUserId)).c_str(), block);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_UnBlockAllUser(lua_State* tolua_S)
{
	int iErrorcode = IM_UnBlockAllUser();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_GetBlockUsers(lua_State* tolua_S)
{
	int iErrorcode = IM_GetBlockUsers();
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

// 关系链
//传一个json字符串，需要包含NickName,Sex,Signature,Country,Province,City,ExtraInfo字段
static int tolua_Cocos2d_YIM_SetUserProfileInfo(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	const char *profileInfo = (const char*)tolua_tostring(tolua_S, 2, 0);
	int iErrorcode = IM_SetUserProfileInfo(UTF8TOPlatString(std::string(profileInfo)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_SetUserProfilePhoto(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	const char *photoPath = (const char*)tolua_tostring(tolua_S, 2, 0);
	int iErrorcode = IM_SetUserProfilePhoto(UTF8TOPlatString(std::string(photoPath)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_GetUserProfileInfo(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	const char *userID = (const char*)tolua_tostring(tolua_S, 2, 0);
	int iErrorcode = IM_GetUserProfileInfo(UTF8TOPlatString(std::string(userID)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_SwitchUserStatus(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	const char *userID = (const char*)tolua_tostring(tolua_S, 2, 0);
	int userStatus = lua_tointeger(tolua_S, 3);
	int iErrorcode = IM_SwitchUserStatus(UTF8TOPlatString(std::string(userID)).c_str(), (YIMUserStatus)userStatus);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_SetAddPermission(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	bool beFound = tolua_toboolean(tolua_S, 2, 0);
	int beAddPermission = lua_tointeger(tolua_S, 3);
	int iErrorcode = IM_SetAddPermission(beFound, (IMUserBeAddPermission)beAddPermission);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_FindUser(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	int findType = lua_tointeger(tolua_S, 2);
	const char *target = (const char*)tolua_tostring(tolua_S, 3, 0);
	int iErrorcode = IM_FindUser(findType, UTF8TOPlatString(std::string(target)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_RequestAddFriend(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	std::string users = getArrayJsonString(tolua_S, 2);
	const char *comments = (const char*)tolua_tostring(tolua_S, 3, 0);
	int iErrorcode = IM_RequestAddFriend(UTF8TOPlatString(std::string(users)).c_str(), UTF8TOPlatString(std::string(comments)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_DealAddFriend(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	const char *userID = (const char*)tolua_tostring(tolua_S, 2, 0);
	int dealResult = lua_tointeger(tolua_S, 3);
	const char* strReqID = lua_tostring(tolua_S, 4);

	int iErrorcode = IM_DealAddFriend(UTF8TOPlatString(std::string(userID)).c_str(), dealResult, yim_to_xuint64(strReqID));
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_DeleteFriend(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	std::string users = getArrayJsonString(tolua_S, 2);
	int deleteType = lua_tointeger(tolua_S, 3);
	int iErrorcode = IM_DeleteFriend(UTF8TOPlatString(std::string(users)).c_str(), deleteType);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_BlackFriend(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	int type = lua_tointeger(tolua_S, 2);
	std::string users = getArrayJsonString(tolua_S, 3);
	int iErrorcode = IM_BlackFriend(type, UTF8TOPlatString(std::string(users)).c_str());
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_QueryFriends(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);
	int type = lua_tointeger(tolua_S, 2);
	int startIndex = lua_tointeger(tolua_S, 3);
	int count = lua_tointeger(tolua_S, 4);
	int iErrorcode = IM_QueryFriends(type, startIndex, count);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_QueryFriendRequestList(lua_State* tolua_S)
{
	tolua_tousertype(tolua_S, 1, 0);

	int startIndex = lua_tointeger(tolua_S, 2);
	int count = lua_tointeger(tolua_S, 3);
	int iErrorcode = IM_QueryFriendRequestList(startIndex, count);
	lua_pushinteger(tolua_S, iErrorcode);
	return 1;
}

static int tolua_Cocos2d_YIM_sendCustomMessage(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	const char *pszRecvID = ((const char*)tolua_tostring(tolua_S, 2, 0));
	int iChatType = lua_tointeger(tolua_S, 3);
	const char *pszContent = ((const char*)tolua_tostring(tolua_S, 4, 0));
	XUINT64 iRequestID = -1;
	unsigned int size = (unsigned int)lua_tointeger(tolua_S, 5);

	IM_SendCustomMessage(UTF8TOPlatString(std::string(pszRecvID)).c_str(), (YIMChatType)iChatType, pszContent, size, &iRequestID);

	lua_pushstring(tolua_S, yim_to_string(iRequestID).c_str());
	return 1;
}

/********************/

static ScriptHandlerMgr::HandlerType _bindLuaFunction(lua_State *tolua_S, int index, void *cobj)
{
	int handler = toluafix_ref_function(tolua_S, index, 0);
	return cocos2d::ScriptHandlerMgr::getInstance()->addCustomHandler(cobj, handler);
}

static int tolua_Cocos2d_YIM_bindScriptHandler(lua_State* tolua_S)
{
	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iEvtType = lua_tointeger(tolua_S, 2);
	switch (iEvtType)
	{
	case 0:
	{
		EVENT_YIM_Test = _bindLuaFunction(tolua_S, 3, self);
		//如果需要回掉先判断 EVENT_YIM_Test ==0 。 如果==0 说明用户并没有注册，就不要回掉了
	}
	break;
	case 1:
	{
		EVENT_YIM_GetRecognizeSpeechText = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 2:
	{
		EVENT_YIM_StartReconnect = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 3:
	{
		EVENT_YIM_RecvReconnectResult = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 4:
	{
		EVENT_YIM_RecvRecordVolume = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 5:
	{
		EVENT_YIM_LeaveAllChatRooms = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 6:
	{
		EVENT_YIM_GetDistance = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 7:
	{
		EVENT_YIM_QueryRoomHistoryFromServer = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 8:
	{
		EVENT_YIM_UserProfileInfoChangeNotify = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 9:
	{
		EVENT_YIM_SetUserProfile = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 10:
	{
		EVENT_YIM_GetUserProfile = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 11:
	{
		EVENT_YIM_SetUserPhotoUrl = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 12:
	{
		EVENT_YIM_SwitchUserOnlineState = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 13:
	{
		EVENT_YIM_FindFriend = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 14:
	{
		EVENT_YIM_RequestAddFriend = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 15:
	{
		EVENT_YIM_BeAddFriendNotify = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 16:
	{
		EVENT_YIM_BeAddFriendRequestNotify = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 17:
	{
		EVENT_YIM_DealBeAddFriend = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 18:
	{
		EVENT_YIM_AddFriendResultNotify = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 19:
	{
		EVENT_YIM_DeleteFriend = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 20:
	{
		EVENT_YIM_BeDeleteNotify = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 21:
	{
		EVENT_YIM_BlackFriend = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 22:
	{
		EVENT_YIM_QueryFriendList = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	case 23:
	{
		EVENT_YIM_QueryFriendRequestList = _bindLuaFunction(tolua_S, 3, self);
	}
	break;
	default:
		break;
	}

	return 0;
}


static int tolua_Cocos2d_YIM_registerScriptHandler(lua_State* tolua_S)
{

	CustomClass *self = (CustomClass*)tolua_tousertype(tolua_S, 1, 0);
	int iCallbackCount = lua_gettop(tolua_S) - 1;
//	__android_log_print(ANDROID_LOG_WARN, "YIM", "tolua_Cocos2d_YIM_registerScriptHandler %d", iCallbackCount);
	// assert(iCallbackCount == 30);
	// if (iCallbackCount != 30)
	// {
	// 	return 0;
	// }
	if(iCallbackCount >= 1)
	EVENT_YIM_Login = _bindLuaFunction(tolua_S, 2, self);
	if(iCallbackCount >= 2)
	EVENT_YIM_Logout = _bindLuaFunction(tolua_S, 3, self);
	if(iCallbackCount >= 3)
	EVENT_YIM_Download = _bindLuaFunction(tolua_S, 4, self);
	if(iCallbackCount >= 4)
	EVENT_YIM_SendMessageStatus = _bindLuaFunction(tolua_S, 5, self);
	if(iCallbackCount >= 5)
	EVENT_YIM_SendAudioMessageStatus = _bindLuaFunction(tolua_S, 6, self);
	if(iCallbackCount >= 6)
	EVENT_YIM_RecvMessage = _bindLuaFunction(tolua_S, 7, self);
	if(iCallbackCount >= 7)
	EVENT_YIM_ChatroomMessage = _bindLuaFunction(tolua_S, 8, self);
	if(iCallbackCount >= 8)
	EVENT_YIM_SpeechStatus = _bindLuaFunction(tolua_S, 9, self);
	if(iCallbackCount >= 9)
	EVENT_YIM_QueryHistory = _bindLuaFunction(tolua_S, 10, self);
	if(iCallbackCount >= 10)
	EVENT_YIM_GetRecentContacts = _bindLuaFunction(tolua_S, 11, self);
	if(iCallbackCount >= 11)
	EVENT_YIM_RecvMsgNotify = _bindLuaFunction(tolua_S, 12, self);
	if(iCallbackCount >= 12)
	EVENT_YIM_GetUserInfo = _bindLuaFunction(tolua_S, 13, self);
	if(iCallbackCount >= 13)
	EVENT_YIM_QueryUserStatus = _bindLuaFunction(tolua_S, 14, self);
	if(iCallbackCount >= 14)
	EVENT_YIM_PlayComplete = _bindLuaFunction(tolua_S, 15, self);
	if(iCallbackCount >= 15)
	EVENT_YIM_StartSendAudioMessage = _bindLuaFunction(tolua_S, 16, self);
	if(iCallbackCount >= 16)
	EVENT_YIM_TranslateText = _bindLuaFunction(tolua_S, 17, self);
	if(iCallbackCount >= 17)	
	EVENT_YIM_GetCurrentLocation = _bindLuaFunction(tolua_S, 18, self);
	if(iCallbackCount >= 18)
	EVENT_YIM_GetNearbyObjects = _bindLuaFunction(tolua_S, 19, self);
	if(iCallbackCount >= 19)
	EVENT_YIM_DownloadByUrl = _bindLuaFunction(tolua_S, 20, self);
	if(iCallbackCount >= 20)
	EVENT_YIM_UserJoinRoom = _bindLuaFunction(tolua_S, 21, self);
	if(iCallbackCount >= 21)
	EVENT_YIM_UserLeaveRoom = _bindLuaFunction(tolua_S, 22, self);
	if(iCallbackCount >= 22)
	EVENT_YIM_MicrophoneStatus = _bindLuaFunction(tolua_S, 23, self);
	if(iCallbackCount >= 23)
	EVENT_YIM_AccusationResultNotify = _bindLuaFunction(tolua_S, 24, self);
	if(iCallbackCount >= 24)
	EVENT_YIM_RecvNotice = _bindLuaFunction(tolua_S, 25, self);
	if(iCallbackCount >= 25)
	EVENT_YIM_CancelNotice = _bindLuaFunction(tolua_S, 26, self);
	if(iCallbackCount >= 26)
	EVENT_YIM_GetForbiddenSpeakInfo = _bindLuaFunction(tolua_S, 27, self);
	if(iCallbackCount >= 27)
	EVENT_YIM_GetRoomMemberCount = _bindLuaFunction(tolua_S, 28, self);
	if(iCallbackCount >= 28)
	EVENT_YIM_BlockUser = _bindLuaFunction(tolua_S, 29, self);
	if(iCallbackCount >= 29)
	EVENT_YIM_UnBlockAllUser = _bindLuaFunction(tolua_S, 30, self);
	if(iCallbackCount >= 30)
	EVENT_YIM_GetBlockUsers = _bindLuaFunction(tolua_S, 31, self);
	/*
	int handler = toluafix_ref_function(tolua_S, 2, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_Login);

	handler = toluafix_ref_function(tolua_S, 3, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_Logout);

	handler = toluafix_ref_function(tolua_S, 4, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_Download);

	handler = toluafix_ref_function(tolua_S, 5, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_SendMessageStatus);

	handler = toluafix_ref_function(tolua_S, 6, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_SendAudioMessageStatus);

	handler = toluafix_ref_function(tolua_S, 7, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_RecvMessage);

	handler = toluafix_ref_function(tolua_S, 8, 0);
	cocos2d::ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_YIM_ChatroomMessage);

	*/
	return 0;
}



static int tolua_collect_LuaYouMe(lua_State* tolua_S)
{
	return 0;
}
TOLUA_API int register_youmeim_manual(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	tolua_usertype(tolua_S, "cc.YIM");


	tolua_module(tolua_S, "cc", 0);
	tolua_beginmodule(tolua_S, "cc");
	tolua_cclass(tolua_S, "YIM", "cc.YIM", "", tolua_collect_LuaYouMe);
	tolua_beginmodule(tolua_S, "YIM");
	tolua_function(tolua_S, "create", tolua_Cocos2d_YIM_create);
	tolua_function(tolua_S, "init", tolua_Cocos2d_YIM_init);
	tolua_function(tolua_S, "sendTextMessage", tolua_Cocos2d_YIM_sendTextMessage);
	tolua_function(tolua_S, "sendAudioMessage", tolua_Cocos2d_YIM_sendAudioMessage);
	tolua_function(tolua_S, "sendOnlyAudioMessage", tolua_Cocos2d_YIM_sendOnlyAudioMessage);
	tolua_function(tolua_S, "stopAudioMessage", tolua_Cocos2d_YIM_stopAudioMessage);
	tolua_function(tolua_S, "cancleAudioMessage", tolua_Cocos2d_YIM_cancleAudioMessage);
	tolua_function(tolua_S, "joinChatRoom", tolua_Cocos2d_YIM_joinChatRoom);
	tolua_function(tolua_S, "leaveChatRoom", tolua_Cocos2d_YIM_leaveChatRoom);
	tolua_function(tolua_S, "leaveAllChatRooms", tolua_Cocos2d_YIM_leaveAllChatRooms);
	tolua_function(tolua_S, "downloadAudioFile", tolua_Cocos2d_YIM_downloadAudioFile);
	tolua_function(tolua_S, "downloadFileByUrl", tolua_Cocos2d_YIM_downloadFileByUrl);

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
	tolua_function(tolua_S, "deleteSpecifiedHistoryMessage", tolua_Cocos2d_YIM_deleteSpecifiedHistoryMessage);
	tolua_function(tolua_S, "queryRoomHistoryMessageFromServer", tolua_Cocos2d_YIM_queryRoomHistoryMessageFromServer);
	tolua_function(tolua_S, "convertAMRToWav", tolua_Cocos2d_YIM_convertAMRToWav);

	tolua_function(tolua_S, "onResume", tolua_Cocos2d_YIM_onResume);
	tolua_function(tolua_S, "onPause", tolua_Cocos2d_YIM_onPause);


	tolua_function(tolua_S, "sendGift", tolua_Cocos2d_YIM_sendGift);
	tolua_function(tolua_S, "multiSendTextMessage", tolua_Cocos2d_YIM_multiSendTextMessage);
	tolua_function(tolua_S, "getContact", tolua_Cocos2d_YIM_getContact);
	tolua_function(tolua_S, "setDownloadAudioMessageSwitch", tolua_Cocos2d_YIM_setDownloadAudioMessageSwitch);
	tolua_function(tolua_S, "setReceiveMessageSwitch", tolua_Cocos2d_YIM_setReceiveMessageSwitch);
	tolua_function(tolua_S, "getNewMessage", tolua_Cocos2d_YIM_getNewMessage);

	tolua_function(tolua_S, "setUserInfo", tolua_Cocos2d_YIM_setUserInfo);
	tolua_function(tolua_S, "getUserInfo", tolua_Cocos2d_YIM_getUserInfo);

	tolua_function(tolua_S, "queryUserStatus", tolua_Cocos2d_YIM_QueryUserStatus);
	tolua_function(tolua_S, "setVolume", tolua_Cocos2d_YIM_SetVolume);
	tolua_function(tolua_S, "startPlayAudio", tolua_Cocos2d_YIM_StartPlayAudio);
	tolua_function(tolua_S, "stopPlayAudio", tolua_Cocos2d_YIM_StopPlayAudio);
	tolua_function(tolua_S, "isPlaying", tolua_Cocos2d_YIM_IsPlaying);
	tolua_function(tolua_S, "getAudioCachePath", tolua_Cocos2d_YIM_GetAudioCachePath);
	tolua_function(tolua_S, "clearAudioCachePath", tolua_Cocos2d_YIM_ClearAudioCachePath);
	tolua_function(tolua_S, "setRoomHistoryMessageSwitch", tolua_Cocos2d_YIM_SetRoomHistoryMessageSwitch);

	tolua_function(tolua_S, "translateText", tolua_Cocos2d_YIM_TranslateText);

	tolua_function(tolua_S, "getCurrentLocation", tolua_Cocos2d_YIM_GetCurrentLocation);
	tolua_function(tolua_S, "getNearbyObjects", tolua_Cocos2d_YIM_GetNearbyObjects);
	tolua_function(tolua_S, "setUpdateInterval", tolua_Cocos2d_YIM_SetUpdateInterval);
	tolua_function(tolua_S, "getDistance", tolua_Cocos2d_YIM_GetDistance);

	tolua_function(tolua_S, "setSpeechRecognizeLanguage", tolua_Cocos2d_YIM_SetSpeechRecognizeLanguage);
	tolua_function(tolua_S, "setOnlyRecognizeSpeechText", tolua_Cocos2d_YIM_SetOnlyRecognizeSpeechText);
	tolua_function(tolua_S, "getMicrophoneStatus", tolua_Cocos2d_YIM_GetMicrophoneStatus);
	tolua_function(tolua_S, "accusation", tolua_Cocos2d_YIM_Accusation);
	tolua_function(tolua_S, "queryNotice", tolua_Cocos2d_YIM_QueryNotice);

	tolua_function(tolua_S, "getForbiddenSpeakInfo", tolua_Cocos2d_YIM_GetForbiddenSpeakInfo);

	tolua_function(tolua_S, "getRoomMemberCount", tolua_Cocos2d_YIM_GetRoomMemberCount);

	tolua_function(tolua_S, "blockUser", tolua_Cocos2d_YIM_BlockUser);
	tolua_function(tolua_S, "unBlockAllUser", tolua_Cocos2d_YIM_UnBlockAllUser);
	tolua_function(tolua_S, "getBlockUsers", tolua_Cocos2d_YIM_GetBlockUsers);

	tolua_function(tolua_S, "sendFile", tolua_Cocos2d_YIM_sendFile);

	tolua_function(tolua_S, "setUserProfileInfo", tolua_Cocos2d_YIM_SetUserProfileInfo);
	tolua_function(tolua_S, "setUserProfilePhoto", tolua_Cocos2d_YIM_SetUserProfilePhoto);
	tolua_function(tolua_S, "getUserProfileInfo", tolua_Cocos2d_YIM_GetUserProfileInfo);
	tolua_function(tolua_S, "switchUserStatus", tolua_Cocos2d_YIM_SwitchUserStatus);
	tolua_function(tolua_S, "setAddPermission", tolua_Cocos2d_YIM_SetAddPermission);
	tolua_function(tolua_S, "findUser", tolua_Cocos2d_YIM_FindUser);
	tolua_function(tolua_S, "requestAddFriend", tolua_Cocos2d_YIM_RequestAddFriend);
	tolua_function(tolua_S, "dealBeRequestAddFriend", tolua_Cocos2d_YIM_DealAddFriend);
	tolua_function(tolua_S, "deleteFriend", tolua_Cocos2d_YIM_DeleteFriend);
	tolua_function(tolua_S, "blackFriend", tolua_Cocos2d_YIM_BlackFriend);
	tolua_function(tolua_S, "queryFriends", tolua_Cocos2d_YIM_QueryFriends);
	tolua_function(tolua_S, "queryFriendRequestList", tolua_Cocos2d_YIM_QueryFriendRequestList);
	tolua_function(tolua_S, "sendCustomMessage", tolua_Cocos2d_YIM_sendCustomMessage);

	tolua_function(tolua_S, "registerScriptHandler", tolua_Cocos2d_YIM_registerScriptHandler);
	tolua_function(tolua_S, "bindScriptHandler", tolua_Cocos2d_YIM_bindScriptHandler);

	tolua_endmodule(tolua_S);
	tolua_endmodule(tolua_S);

	return 1;
}

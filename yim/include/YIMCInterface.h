//
//  IYouMeCInterface.hpp
//  youme_voice_engine
//
//  Created by YouMe.im on 15/12/10.
//  Copyright © 2015年 tencent. All rights reserved.
//

#ifndef IYouMeCInterface_hpp
#define IYouMeCInterface_hpp


//这个文件对内封装的C 接口，不需要给外部。C# 只能调用C接口的函数
//为了简单只提供单实例的封装
#include <YIMPlatformDefine.h>
#include <YIM.h>

extern "C"
{
#ifndef NO_C_Interface
	 //初始化和反初始化
	YOUMEDLL_API int IM_Init(const XCHAR* appKey, const XCHAR* appSecurity);
	YOUMEDLL_API void IM_Uninit();
    
	//登陆，登出
	YOUMEDLL_API int IM_Login(const XCHAR* userID, const XCHAR* password, const XCHAR* token);
	YOUMEDLL_API int IM_Logout();
 
    //消息接口
	YOUMEDLL_API int IM_SendTextMessage(const XCHAR* receiverID, YIMChatType chatType, const XCHAR* text, const XCHAR* attachParam, XUINT64* requestID);
	YOUMEDLL_API int IM_SendCustomMessage(const XCHAR* receiverID, YIMChatType chatType, const char* content, unsigned int size, XUINT64* requestID);
    
	//发送文件接口
	YOUMEDLL_API int IM_SendFile(const XCHAR* receiverID, YIMChatType chatType, const XCHAR* filePath, const XCHAR* extraParam, YIMFileType fileType, XUINT64* requestID);


	YOUMEDLL_API int IM_SendAudioMessage(const XCHAR* receiverID, YIMChatType chatType, XUINT64* requestID);
    //不会转文字，直接发送的音频
	YOUMEDLL_API int IM_SendOnlyAudioMessage(const XCHAR* receiverID, YIMChatType chatType, XUINT64* requestID);
	YOUMEDLL_API int IM_StopAudioMessage(const XCHAR* extraParam);
    YOUMEDLL_API int IM_CancleAudioMessage();
	YOUMEDLL_API int IM_DownloadFile(XUINT64 serial, const XCHAR* savePath);
	YOUMEDLL_API int IM_DownloadFileByURL(const XCHAR* downloadURL, const XCHAR* savePath, YIMFileType fileType);
	YOUMEDLL_API int IM_SetDownloadDir(const XCHAR* path);
	YOUMEDLL_API int IM_SendGift(const XCHAR* anchorID, const XCHAR* channel, int giftId, int giftCount, const char* extraParam, XUINT64* requestID);
	YOUMEDLL_API int IM_MultiSendTextMessage(const char* receivers, const XCHAR* text);
	
    //是否自动下载语音消息(true: 自动下载语音消息， false:不自动下载语音消息(默认))，下载的路径是默认路径，下载回调中可以得到
    YOUMEDLL_API int IM_SetDownloadAudioMessageSwitch(bool download);
    
	//是否自动接收消息(true:自动接收(默认)	false:不自动接收消息，有新消息达到时会收到CMD_RECEIVE_MESSAGE_NITIFY消息，调用方需要调用IM_GetNewMessage获取新消息)
	//targets:房间ID JSON数组 "["xx","xx","xx"]"
	YOUMEDLL_API int IM_SetReceiveMessageSwitch(const XCHAR* targets, bool receive);
	//获取新消息（只有IM_SetReceiveMessageSwitch设置为不自动接收消息，才需要在收到CMD_RECEIVE_MESSAGE_NITIFY消息时，调用该函数）
	//targets:房间ID JSON数组 "["xx","xx","xx"]"
	YOUMEDLL_API int IM_GetNewMessage(const XCHAR* targets);
	// 是否保存房间消息（默认不保存）
	YOUMEDLL_API int IM_SetRoomHistoryMessageSwitch(const XCHAR* roomIDs, bool save);

	// 设置消息是否已读
	YOUMEDLL_API int IM_SetMessageRead(XUINT64 messageID, bool read);

    // 设置指定发送用户发送的所有消息是否已读
    YOUMEDLL_API int IM_SetAllMessageRead(const XCHAR* userID, bool read);
    
    //设置语音消息为已播放,true-已播放，false-未播放
    YOUMEDLL_API int IM_SetVoiceMsgPlayed(XUINT64 messageID, bool played);
    
	YOUMEDLL_API int IM_DownloadAudioFileSync(XUINT64 serial, const XCHAR* savePath);
    //聊天室接口
	YOUMEDLL_API int IM_JoinChatRoom(const XCHAR* chatRoomID);
	YOUMEDLL_API int IM_LeaveChatRoom(const XCHAR* chatRoomID);
    YOUMEDLL_API int IM_LeaveAllChatRooms();
    YOUMEDLL_API int IM_GetRoomMemberCount(const XCHAR* chatRoomID);

	//获取最近联系人(最大100条)
	YOUMEDLL_API int IM_GetRecentContacts();
	//设置用户信息 格式为json {"nickname":"","server_area":"","location":"","level":"","vip_level":""} (以上五个必填，可以添加其他字段)
	YOUMEDLL_API int IM_SetUserInfo(const XCHAR* userInfo);
	//查询用户信息
	YOUMEDLL_API int IM_GetUserInfo(const XCHAR* userID);
	//查询用户在线状态
	YOUMEDLL_API int IM_QueryUserStatus(const XCHAR* userID);
    
	//提供一个get 接口，用来读取数据,如果没有数据会阻塞，调用登出的话，函数会返回
    YOUMEDLL_API const XCHAR* IM_GetMessage();
	YOUMEDLL_API const XCHAR* IM_GetMessage2();
	YOUMEDLL_API void IM_PopMessage(const XCHAR*pszBuffer);
    
    YOUMEDLL_API int IM_GetSDKVer();
	YOUMEDLL_API void IM_SetServerZone(ServerZone zone);
	YOUMEDLL_API void IM_SetAudioCacheDir(const XCHAR* audioCacheDir);


	//设置模式0 正式环境 1开发环境 2 测试环境 3 商务环境。 默认正式环境。所以客户不需要调用这个接口
	YOUMEDLL_API void IM_SetMode(int mode);
    YOUMEDLL_API void IM_SetLogLevel(int iLogLevel, int iConsoleLogLevel);

    
    
	YOUMEDLL_API XCHAR* IM_GetFilterText(const XCHAR* text, int* level);
	YOUMEDLL_API void  IM_DestroyFilterText(XCHAR* text);

	YOUMEDLL_API int IM_StartAudioSpeech(XUINT64* requestID, bool translate);
	YOUMEDLL_API int IM_StopAudioSpeech();
	//查询历史消息
	YOUMEDLL_API int IM_QueryHistoryMessage(const XCHAR* targetID, int chatType, XUINT64 startMessageID , int count, int direction );
	//删除历史消息
	YOUMEDLL_API int IM_DeleteHistoryMessage(YIMChatType chatType, XUINT64 time = 0);
	//删除历史消息
	YOUMEDLL_API int IM_DeleteHistoryMessageByTarget(const XCHAR* targetID, int chatType, XUINT64 startMessageID = 0, unsigned int count = 0);
	//删除指定messageID对应消息
	YOUMEDLL_API int IM_DeleteHistoryMessageByID(XUINT64 messageID);
    //清除指定用户的本地消息历史记录
    YOUMEDLL_API int IM_DeleteSpecifiedHistoryMessage(const XCHAR* targetID, YIMChatType chatType, XUINT64* messageID, int num);
	//查询房间消息
	YOUMEDLL_API int IM_QueryRoomHistoryMessageFromServer(const XCHAR* chatRoomID, int count, int direction);
	//wav 转换
	YOUMEDLL_API  int IM_ConvertAMRToWav(const XCHAR* amrFilePath, const XCHAR* wavFielPath);

	//程序切到后台运行 
	YOUMEDLL_API void IM_OnPause(bool pauseReceiveMessage);
	//程序切到前台运行
	YOUMEDLL_API void IM_OnResume();

	//获取语音缓存目录
	YOUMEDLL_API XCHAR* IM_GetAudioCachePath();
	YOUMEDLL_API void IM_DestroyAudioCachePath(XCHAR* path);
	//清理语音缓存目录(注意清空语音缓存目录后历史记录中会无法读取到音频文件，调用清理历史记录接口也会自动删除对应的音频缓存文件)
	YOUMEDLL_API bool IM_ClearAudioCachePath();

	//设置播放语音音量(volume:0.0-1.0)
	YOUMEDLL_API void IM_SetVolume(float volume);
	//播放语音
	YOUMEDLL_API int IM_StartPlayAudio(const XCHAR* path);
	//停止语音播放
	YOUMEDLL_API int IM_StopPlayAudio();
	//查询播放状态
	YOUMEDLL_API bool IM_IsPlaying();
	//获取麦克风状态
	YOUMEDLL_API void GetMicrophoneStatus();

	//语音结束后是否保持原Category(IOS)
	YOUMEDLL_API void IM_SetKeepRecordModel(bool keep);

	//文本翻译
	YOUMEDLL_API int IM_TranslateText(unsigned int* requestID, const XCHAR* text, LanguageCode destLangCode, LanguageCode srcLangCode = LANG_AUTO);
	
	// 屏蔽/解除屏蔽用户消息
	YOUMEDLL_API int IM_BlockUser(const XCHAR* userID, bool block);

	// 解除所有已屏蔽用户
	YOUMEDLL_API int IM_UnBlockAllUser();

	// 获取被屏蔽消息用户
	YOUMEDLL_API int IM_GetBlockUsers();
    
    //消息传输类型开关
    YOUMEDLL_API int IM_SwitchMsgTransType(YIMMsgTransType transType);

	// 获取当前地理位置
	YOUMEDLL_API int IM_GetCurrentLocation();
	// 获取附近的目标
	YOUMEDLL_API int IM_GetNearbyObjects(int count, const XCHAR* serverAreaID, DistrictLevel districtlevel = DISTRICT_UNKNOW, bool resetStartDistance = false);
	// 获取与指定用户距离
	YOUMEDLL_API int IM_GetDistance(const XCHAR* userID);
	// 设置位置更新间隔(单位：分钟)
	YOUMEDLL_API void IM_SetUpdateInterval(unsigned int interval);

	YOUMEDLL_API int IM_SetSpeechRecognizeLanguage(int language);
    
    //设置仅识别语音文字，不发送语音消息; false:识别语音文字并发送语音消息，true:仅识别语音文字
    YOUMEDLL_API int IM_SetOnlyRecognizeSpeechText(bool recognition);

	YOUMEDLL_API int IM_GetMicrophoneStatus();

	YOUMEDLL_API int IM_Accusation(const XCHAR* userID, YIMChatType source, int reason, const XCHAR* description, const XCHAR* extraParam);

	YOUMEDLL_API int IM_QueryNotice();
    
    YOUMEDLL_API int IM_GetForbiddenSpeakInfo();

	YOUMEDLL_API int IM_SetLoginAddress(const char* ip, unsigned short port);
    
	// 查找添加好友
	YOUMEDLL_API int IM_FindUser(int findType, const XCHAR* target);
	// 添加好友
	YOUMEDLL_API int IM_RequestAddFriend(const XCHAR* users, const XCHAR* comments);
	// 处理好友添加请求
	YOUMEDLL_API int IM_DealAddFriend(const XCHAR* userID, int dealResult, XUINT64 reqID);
	// 删除好友
	YOUMEDLL_API int IM_DeleteFriend(const XCHAR* users, int deleteType = 1);
	// 拉黑好友
	YOUMEDLL_API int IM_BlackFriend(int type, const XCHAR* users);
	// 查询我的好友
	YOUMEDLL_API int IM_QueryFriends(int type, int startIndex, int count);
	// 查询好友请求列表
	YOUMEDLL_API int IM_QueryFriendRequestList(int startIndex = 0, int count = 20);
    
    //关系链-用户信息管理
    //设置用户基本资料
    YOUMEDLL_API int IM_SetUserProfileInfo(const XCHAR* profileInfo);
    
    //设置用户头像
    YOUMEDLL_API int IM_SetUserProfilePhoto(const XCHAR* photoPath);
    
    //获取用户基本资料
    YOUMEDLL_API int IM_GetUserProfileInfo(const XCHAR* userID = __XT(""));
    
    //切换用户状态
    YOUMEDLL_API int IM_SwitchUserStatus(const XCHAR* userID, YIMUserStatus userStatus);
    
    //设置好友添加权限
    YOUMEDLL_API int IM_SetAddPermission(bool beFound, IMUserBeAddPermission beAddPermission);
    
    // 处理图片大小
    YOUMEDLL_API void IM_ResizeImage(const XCHAR* srcImagePath, const XCHAR* resizedSavePath);

#endif
}
#endif /* IYouMeCInterface_hpp */

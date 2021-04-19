# IM SDK for Cocos2d Lua 开发指引

## 概述
游密即时通讯SDK（IM SDK）为玩家提供完整的游戏内互动服务，游戏开发者无需关注IM通讯复杂的内部工作流程，只需调用IM SDK提供的接口，即可快速实现世界聊天、公会聊天、组队聊天、文字、表情、语音等多项功能。

## IM SDK接口调用顺序

![IM SDK接口调用顺序](https://youme.im/doc/images/im_sdk_interface_call_order.png)


## 四步集成IM SDK
### 第一步 注册账号
在[游密官网](https://console.youme.im/user/register)注册游密账号。

### 第二步 添加游戏，获取Appkey
在控制台添加游戏，获得接入需要的Appkey、Appsecret。

### 第三步 下载IM SDK包体
[下载地址](https://www.youme.im/download.php?type=IM)

### 第四步 开发环境配置
[开发环境配置](#快速接入)

## 快速接入
### 1. 导入IM SDK
#### 针对Android导入

- 将`yim`复制到Cocos2d-x开发工具生成的游戏目录根目录下。

- 对`proj.android/jni/Android.mk`文件进行如下修改，**仅需修改标注部分，其余部分无需处理。**

    ``` mk

    LOCAL_PATH := $(call my-dir)

    include $(CLEAR_VARS)

    LOCAL_MODULE := cocos2dlua_shared

    LOCAL_MODULE_FILENAME := libcocos2dlua

    LOCAL_SRC_FILES := \
    ../../Classes/AppDelegate.cpp \
    ../../Classes/ide-support/SimpleConfigParser.cpp \
    ../../Classes/ide-support/RuntimeLuaImpl.cpp \
    ../../Classes/ide-support/lua_debugger.c \
    hellolua/main.cpp \
    # #########添加如下代码.########
    ../../../../yim/LuaYIM.cpp
    # #########添加如下代码########

    LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes  \
    # #########添加如下代码,添加头文件路径.注意上一行结尾处添加换行符\########
    $(LOCAL_PATH)/../../../../yim/include \
    $(LOCAL_PATH)/../../../../yim/
    # ###############################################################

    # _COCOS_HEADER_ANDROID_BEGIN
    # _COCOS_HEADER_ANDROID_END

    LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
    LOCAL_STATIC_LIBRARIES += cocos2d_simulator_static
    # #############添加如下代码,链接动态库##############################
    LOCAL_SHARED_LIBRARIES := YIM
    # ###############################################################

    #_COCOS_LIB_ANDROID_BEGIN
    #_COCOS_LIB_ANDROID_END

    include $(BUILD_SHARED_LIBRARY)
    # ###################### 包含YMSDK子配置 #########################
    include $(LOCAL_PATH)/../../../../yim/lib/android/Android.mk
    # ###############################################################

    $(call import-module,scripting/lua-bindings/proj.android)
    $(call import-module,tools/simulator/libsimulator/proj.android)

    # _COCOS_LIB_IMPORT_ANDROID_BEGIN
    # _COCOS_LIB_IMPORT_ANDROID_END
    ```

- 若不需要语音转文字功能，需要对`yim/lib/android/Android.mk`文件进行如下修改：

    ``` mk
    LOCAL_PATH := $(call my-dir)

    include $(CLEAR_VARS)

    LOCAL_MODULE := YIM
    LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libyim.so \

    include $(PREBUILT_SHARED_LIBRARY)

    # ############# 不使用翻译可以删除下面内容 ##############
    
    include $(CLEAR_VARS)
    LOCAL_MODULE := YIMNLS
    LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libnlscppsdk.so \                       
                       
    include $(PREBUILT_SHARED_LIBRARY)
    
    include $(CLEAR_VARS)
    LOCAL_MODULE := YIMUSC
    LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libuscasr.so \
                       
    include $(PREBUILT_SHARED_LIBRARY)
    
    include $(CLEAR_VARS)
    LOCAL_MODULE := YIMU
    LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)+/libuuid.so \
                                             
    include $(PREBUILT_SHARED_LIBRARY)
    # ###################################################
    ```

- 将 `yim/lib/android/` 目录中如下文件复制到`proj.android/libs/`目录下。
>`yim.jar`
>`alisr.jar`
>`usc.jar`
>`android-support-v4.jar`


-  修改`proj.android/AndroidManifest.xml`文件，确保声明了如下权限：

    ``` xml
     <!-- 添加到application节点内 -->
     <application xxxx>
         <receiver android:name="com.youme.im.NetworkStatusReceiver" android:label="NetworkConnection" >
            <intent-filter>
                <action android:name="android.net.conn.CONNECTIVITY_CHANGE" />
            </intent-filter>
      </receiver>
     </application>
     <!-- 添加到跟application平级 -->
     <uses-permission android:name="android.permission.INTERNET" />
     <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
     <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
     <uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
     <uses-permission android:name="android.permission.CHANGE_NETWORK_STATE" />
     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
     <uses-permission android:name="android.permission.READ_PHONE_STATE" />
     <uses-permission android:name="android.permission.RECORD_AUDIO" />
     <!-- 获取地理位置的权限，可选 -->
     <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
     <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    ```

- 打开Eclipse，导入上一步的Android工程，在项目中第一个启动的`AppActivity`中的`onCreate`中增加以下Java代码:
    `注意必须把游密的初始化加到第一行，否则可能出现部分机型加载so不正常`

    ``` java
    @Override
    protected void onCreate(Bundle bundle)
    {
       //下面的两个调用顺序不能错
        com.youme.im.IMEngine.init(this);
        super.onCreate(bundle);              //super.onCreate是默认已经有的，不用再复制
    }
    ```

-  如果打包或生成APK时需要混淆，则需要在`proguard.cfg`文件中添加如下代码：

  ```

     -keep class com.youme.** {*;}
     -keep class com.iflytek.**
     -keepattributes Signature  
  ```

#### 针对iOS导入
- 将`yim`复制到Cocos2d-x开发工具生成的游戏目录**根目录下**
- 在`Build Settings -> Search Paths -> Header Search Paths`中添加头文件路径：`$(SRCROOT)/../../../yim/include/`
- 在`Build Settings -> Search Paths -> Framework Search Paths`中添加框架文件路径：`$(SRCROOT)/../../../yim/lib/ios/`
- 在`Build Settings -> Search Paths -> Library Search Paths`中添加库文件路径：`$(SRCROOT)/../../../yim/lib/ios/`
- 在`Build Phases  -> Link Binary With Libraries`添加如下依赖库：
 >`libc++.1.tbd`
 >`libsqlite3.0.tbd`
 >`libresolv.9.tbd`
 >`SystemConfiguration.framework`
 >`libYouMeCommon.a`
 >`libyim.a`
 >`libz.tbd`
 >`CoreTelephony.framework`
 >`AliyunNlsSdk.framework`  //如果是带语音转文字的sdk需要添加，动态库，需要在`General -> Embedded Binaries`也进行添加
 >`USCModule.framework`     //如果是带语音转文字的sdk需要添加
 >`AVFoundation.framework`
 >`AudioToolbox.framework`
 >`CoreLocation.framework`  
 >`libBoringSSL.a`      //如果是带google语音转文字的sdk需要
 >`libgoogleapis.a`//如果是带google语音转文字的sdk需要
 >`libgRPC-Core.a`//如果是带google语音转文字的sdk需要
 >`libgRPC-ProtoRPC.a`//如果是带google语音转文字的sdk需要
 >`libgRPC-RxLibrary.a`//如果是带google语音转文字的sdk需要
 >`libgRPC.a`//如果是带google语音转文字的sdk需要
 >`libnanopb.a`//如果是带google语音转文字的sdk需要
 >`libProtobuf.a`//如果是带google语音转文字的sdk需要
 
 - 如果接入带google语音识别的版本，需在`Build Phases  ->Copy Bundle Resources` 里加入 `gRPCCertificates.bundle`
 
- **需要为iOS10以上版本添加录音权限配置**
iOS 10 使用录音权限，需要在 `info` 新加`Privacy - Microphone Usage Description` 键，值为字符串，比如“语音聊天需要录音权限”。
首次录音时会向用户申请权限。配置方式如图(选择Private-Microphone Usage Description)。
![iOS10录音权限配置](https://www.youme.im/doc/images/im_iOS_record_config.jpg)

- **LBS添加获取地理位置权限**
若使用LBS，需要在 ` info` 新加 ` Privacy - Location Usage Description ` 键，值为字符串，比如“查看附近的玩家需要获取地理位置权限”。首次使用定位时会向用户申请权限，配置方式如上图录音权限。

#### 针对Windows导入
- 将`yim`复制到Cocos2d-x开发工具生成的游戏目录<b>根目录下</b>。

- 拷贝yim下的两个文件：LuaYIM.h，LuaYIM.cpp 到Classes目录,并加入项目。

- 拷贝yim/lib/x86 目录下的`  yim.dll ，  yim.lib， libusc.dll， jsoncpp-0.y.z.dll， libeay32.dll， nlscppsdk.dll， pthreadVC2.dll， ssleay32.dll `拷贝到执行目录(如果是64位机器，使用yim/lib/x64)

- 在项目 -> 属性 -> C/C++ -> 常规 -> 附加包含目录 中添加
  `$(ProjectDir)..\..\..\yim\include`

- 在项目->属性->链接器->输入->附加依赖库 中添加 ` yim.lib `（该库是release模式下的  如果运行的时候是debug模式请添加yim_Debug.lib）

### 2. 初始化
成功导入SDK后，应用启动的第一时间需要调用初始化接口。

#### 注册Lua接口
- 引入C++头文件

  ``` c
      #include <LuaYIM.h>
  ```

- 注册Lua方法
![luabind.jpg](https://youme.im/doc/images/im_sdk_lua-3.jpg)

#### 获取SDK管理器
**为了方便说明如何调用接口，我们全文中都把youmeim作为YIM的实例对象使用**

- 接口和示例：

  ``` lua
      local youmeim = cc.YIM:create()
  ```
 
#### 初始化IM SDK

- **调用示例：**

  ``` lua
      local errorcode = youmeim:init("strAppKey","strAppSecret")
  ```
  
- **参数：**    
  - `strAppKey`：用户游戏产品区别于其它游戏产品的标识，可以在[游密官网](https://account.youme.im/login)获取、查看
  - `strAppSecret`：用户游戏产品的密钥，可以在[游密官网](https://account.youme.im/login)获取、查看
    
- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	
- **备注：**
  此接口本是异步操作，未给出异步回调，一般返回的错误码是Success即表示初始化成功。
  
### 3. 设置回调监听  
IM引擎底层对于耗时操作都采用异步回调的方式，函数调用会立即返回，操作结果会同步回调。因此，用户必须实现相关接口并在初始化完成以后进行注册。常用功能需要登录回调，聊天室回调，消息回调，如需其它功能查看API接口。

- **调用示例：**

  ``` lua
  --登录回调
  youmeim.OnLogin = function (errorcode,youmeID)
      print("errorcode:" .. errorcode .. " youmeID:" .. youmeID)
  end

  --登出回调
  youmeim.OnLogout = function ()
        print("11: OnLogout")
  end 
  
  --下载语音文件回调
  youmeim.OnDownload = function ( errorcode, strSavepath, bodytype,chattype, serial,recvid,senderid,content,params,duration, createtime )     
      print("OnDownload：" .. serial .. " errorcode:" .. errorcode .. " path:" .. strSavepath)
  end

  --文本和自定义消息发送状态回调
  youmeim.OnSendMessageStatus = function(serial, errorcode, sendTime,  isForbidRoom, reasonType, endTime)
        print("OnSendMessageStatus" .. serial .. " errorcode:" .. errorcode)
  end 
  
  --自己的语音消息发送状态回调
  youmeim.OnSendAudioMessageStatus = function(serial, errorcode, content, localpath, duration, sendTime, isForbidRoom, reasonType, endTime)
        print("OnSendAudioMessageStatus" .. serial .. " errorcode:" .. errorcode .. " content:" .. content .. " localpath:" .. localpath .. " duration:" .. duration)
  end
  
  --接收到他人的消息回调
  youmeim.OnRecvMessage = function(bodytype,chattype, serial,recvid,senderid,content,params,duration)
        print("OnRecvMessage" .. bodytype .. " chattype:" .. chattype .. " serial:" .. serial .. " recvid:" .. recvid .. " senderid:" .. senderid .. " content:" .. content .. " param:" .. params .. " duration: " .. duration)
        if bodytype == 1 then
            -- 文本消息
        end
        if bodytype == 5 then
             -- 音频文件可以下载
          youmeim:downloadAudioFile(serial,"C:\\Users\\joexie\\Desktop\\data\\11.wav")

        end
  end  
  
  --进入聊天频道的通知
  youmeim.OnJoinChatroom = function(chatroomid ,errorcode)
        print("OnJoinChatroom id：" .. chatroomid .. "errorcode:" .. errorcode)
  end 
  
  --录音播放完成回调
  youmeim.OnPlayCompletion = function ( errorcode, path )
        print("播放结束:"..path)
  end
   
  --自己的语音开始上传回调
  youmeim.OnStartSendAudioMessage = function(serial,errorcode,content,localpath,duration)
        print("OnStartSendAudioMessage" .. serial .. " errorcode:" .. errorcode .. " content:" .. content .. " localpath:" .. localpath .. " duration:" .. duration)
  end  
    
  -- 注册回调
  youmeim.registerScriptHandler(youmeim,
            youmeim.OnLogin,
            youmeim.OnLogout,
            youmeim.OnDownload,
            youmeim.OnSendMessageStatus,
            youmeim.OnSendAudioMessageStatus,
            youmeim.OnRecvMessage,
            youmeim.OnJoinChatroom,            
            youmeim.OnPlayCompletion,  
            youmeim.OnStartSendAudioMessage            
    );

  ```
	  
### 4. 登录IM系统
![登录界面介绍](https://www.youme.im/doc/images/im_login.png)

成功初始化IM后，需要使用IM功能时，先调用IM SDK登录接口。

- **调用示例：**

  ``` lua
      local errorcode = youmeim:login("userid","user_password","token")
  ```
  
- **参数：**
	- `userid`：由调⽤者分配，不可为空字符串，只可由字母或数字或下划线组成，用户首次登录会自动注册所用的用户ID和密码
	- `user_password`：⽤户密码，不可为空字符串，由调用者分配，二次登录时需与首次登录一致，否则会报UsernamePasswordError
	- `token`：用户验证token，可选，如不使用token验证传入:""
	
- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	
- **回调接口与参数：**
   登录接口是异步操作，登录成功的标识是`OnLogin`回调的错误码是Success；调用`login`同步返回值是Success才能收到`OnLogin`回调；特别的如果调用`login`多次，收到AlreadyLogin错误码也表示登录成功。 
   
   - `登录回调接口`：
     `youmeim.OnLogin = function (errorcode,youmeID)`
     
   - `youmeID`：用户ID
   - `errorcode`：错误码
   
### 5. 进入聊天频道
![频道](https://www.youme.im/doc/images/im_room.png)

登录成功后，如果有群组聊天需要，比如游戏里面的世界、工会、区域等，需要进入聊天频道，调用方为聊天频道设置一个唯一的频道ID，可以进入多个频道。

- **调用示例：**

  ``` lua
      local errorcode = youmeim:joinChatRoom("room_id")
  ```
  
- **参数**	
	- `room_id`：请求加入的频道ID，仅支持数字、字母、下划线组成的字符串，区分大小写，长度限制为255字节

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)

- **回调接口与参数：**
  进入频道接口是异步操作，进入频道成功的标识是`OnJoinChatroom`回调的错误码是Success；调用`joinChatRoom`同步返回值是Success才能收到`OnJoinChatroom`回调。
  
   - `进入频道回调接口`：
     `youmeim.OnJoinChatroom=function(channelID, errorcode)`
     
   - `errorcode`：错误码
   - `channelID`：频道ID
   
### 6. 发送文本消息
![频道](https://www.youme.im/doc/images/im_room_send_btn.png)

在成功登录IM后，即可发送IM消息。

- **调用示例：**

  ``` lua
      local requestid = youmeim:sendTextMessage("str_reciver_id",iChatType,"message_content", "attachParam");
  ```
  
- **参数：**	     
	- `str_reciver_id`：接收者ID,（⽤户ID或者频道ID）
	- `iChatType`：聊天类型,私聊传`1`，频道聊天传`2`；需要频道聊天得先成功进入频道后发文本消息，此频道内的成员才能接收到消息
	- `message_content`：聊天内容
	- `attachParam`：发送文本附加信息
	
- **返回值：**	
	- `requestid`：消息序列号，用于校验一条消息发送成功与否的标识，`-1`表示调用失败，失败后不会有回调通知。
	
- **回调接口与参数：**
  发送文本消息接口是异步操作，发送文本消息成功的标识是`OnSendMessageStatus`回调的错误码是Success；调用`sendTextMessage`同步返回值是Success才能收到`OnSendMessageStatus`回调；文本消息发送成功，接收方会收到`OnRecvMessage`回调，能从该回调中取得消息内容，消息发送者等信息。
  
   - `发送文本消息回调接口`：
     `youmeim.OnSendMessageStatus = function(serial, errorcode, sendTime,  isForbidRoom, reasonType, forbidEndTime)`
     
   - `serial`：消息序列号，用于校验一条消息发送成功与否的标识
   - `errorcode`：错误码
   - `sendTime`：消息发送时间
   - `isForbidRoom`：若发送的是频道消息，显示在此频道是否被禁言，true-被禁言，false-未被禁言
   - `reasonType`：若在频道被禁言，禁言原因类型，0-未知，1-发广告，2-侮辱，3-政治敏感，4-恐怖主义，5-反动，6-色情，7-其它
   - `forbidEndTime`：若在频道被禁言，禁言结束时间   

- **拓展功能：**
  发送消息时，可以将玩家头像、昵称、角色等级、vip等级等要素打包成json格式发送。

### 7. 发送语音消息
![发送语音消息](https://www.youme.im/doc/images/im_send_voice_message.jpg)

在成功登录IM后，可以发送语音消息
>- 按住语音按钮时，调用`sendAudioMessage`启动录音
>- 松开按钮，调用`stopAudioMessage`接口，发送语音消息
>- 按住过程若需要取消发送，调用`CancleAudioMessage`取消本次语音发送

- **调用示例：**

  ``` lua
      -- ========== 带文字识别的录音启动 ============
      local requestid = youmeim:sendAudioMessage("reciver_id",chatType)
      
      -- ========== 不带文字识别的录音启动 ============
      local requestid = youmeim.sendOnlyAudioMessage("reciver_id",chatType)
      
      -- 结束录音并发送
      local errorcode = youmeim:stopAudioMessage(extra_param)
      
      -- 如果不想发送语音，调取消本次语音，调取消后收不到消息的发送回调
      local errorcode = youmeim:cancleAudioMessage()
  ```
    
- **参数：**	
	- `reciver_id`：接收者ID（⽤户ID或者频道ID）
	- `chatType`：消息类型，表示私聊还是频道消息
	- `extra_param`：发送语音消息附加信息，主要用于调用方特别需求

- **返回值：**	
	- `requestID`：消息序列号，用于校验一条消息发送成功与否的标识，`-1`表示调用失败，失败后不会有回调通知。
	
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	
- **回调接口与参数：**
   调用`stopAudioMessage`接口后，会收到发送语音的回调，如果录音成功会收到开始发送语音的回调`OnStartSendAudioMessage`，无论录音是否成功都会收到发送语音消息结果的回调`OnSendAudioMessageStatus`，`OnStartSendAudioMessage`在接收时间上比`OnSendAudioMessageStatus`快，常用于上屏显示。
   
   - `发送语音消息回调接口`：
     `youmeim.OnStartSendAudioMessage = function(serial,errorcode,content,localpath,duration)`
     
     `youmeim.OnSendAudioMessageStatus = function(serial, errorcode, content, localpath, duration, sendTime, isForbidRoom, reasonType, forbidEndTime)`
     
   - `serial`：消息序列号，用于校验一条消息发送成功与否的标识
   - `errorcode`：错误码
   - `content`：语音转文字识别的文本内容，如果没有用带语音转文字的接口，该字段为空字符串
   - `localpath`：录音生成的wav文件的本地完整路径
   - `duration`：录音时长(单位秒)
   
   - `sendTime`：消息发送时间
   - `isForbidRoom`：若发送的是频道消息，显示在此频道是否被禁言，true-被禁言，false-未被禁言
   - `reasonType`：若在频道被禁言，禁言原因类型，0-未知，1-发广告，2-侮辱，3-政治敏感，4-恐怖主义，5-反动，6-色情，7-其它
   - `forbidEndTime`：若在频道被禁言，禁言结束时间
   
- **备注：**
  此套接口是异步操作，发送语音消息成功的标识是收到`OnStartSendAudioMessage`，或者`OnSendAudioMessageStatus`回调的错误码是Success；调用`stopAudioMessage`同步返回值是Success才能收到回调；语音消息发送成功，接收方会收到`OnRecvMessage`回调，能从该回调中下载语音文件。

### 8. 接收消息
![接收消息](https://www.youme.im/doc/images/im_receive_message.jpg)
>- 通过`OnRecvMessage`接口被动接收消息，需要开发者实现


- **相关接口与参数：**
	
``` lua
	    youmeim.OnRecvMessage = function(bodyType,chatType,serial,recvID,senderID,content,param,duration,createTime,giftID,giftCount,anchor,fileName,fileSize,fileType,fileExtension,extraParam)

	      print("OnRecvMessage" .. bodytype .. " chattype:" .. chattype .. " serial:" .. serial .. " recvid:" .. recvid .. " senderid:" .. senderid .. " content:" .. content .. " param:" .. params .. " duration: " .. duration)
	     if bodytype == 5 then
		     -- 音频文件可以下载
		     yimInstance:downloadAudioFile(serial,"C:\\Users\\joexie\\Desktop\\data\\11.wav")	
	    end
    end
```   

- **备注：**
	收到此回调，可以根据消息基类的`bodyType`获取消息类型并进行相应处理。

### 9. 接收语音消息并播放
![接收语音消息](https://www.youme.im/doc/images/im_receive_message-2.jpg)
>- 通过`bodyType`分拣出语音消息`（5）`
>- ⽤`serial`获得消息ID
>- 点击语音气泡，调用函数`downloadAudioFile`下载语音消息
>- 调用方调用函数`startPlayAudio`播放语音消息

- **调用示例：**

  ``` lua
      --下载语音
      local errorcode = youmeim:downloadAudioFile("serial","save_path")
      
      -- 可在下载语音成功的回调里调播放语音
      local errorcode = youmeim:startPlayAudio( "path" )
  ```
  
- **参数：**      
	- `serial`：消息ID,数字字符串
	- `save_path`：语音文件保存路径
	
	- `path`：待播放文件路径

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)

- **回调接口与参数：**
  下载语音接口是异步操作，下载语音成功的标识是`OnDownload`回调的错误码为Success，调用`IM_DownloadAudioFile`接口同步返回值是Success才能收到`OnDownload`回调。成功下载语音消息后即可播放语音消息。
  
  - `下载回调接口`：
    `youmeim.OnDownload = function(errorcode,savePath,bodyType,chatType,serial,recvID,senderID,content,param,audioTime,createTime)`	
    
  - `errorcode`：错误码
  - `savePath`：保存路径
  - `bodyType`：消息类型，0-未知类型，1-文本消息，2-自定义消息，3-表情，4-图片，5-语音，6-视频，7-文件，8-礼物
  - `chatType`：聊天类型，私聊/频道聊天
  - `serial`：消息ID
  - `recvID`：消息接收者ID
  - `senderID`：消息发送者ID
  - `content`：语音识别的文本内容
  - `param`：发送语音消息的附加信息
  - `audioTime`：语音时长（单位：秒）
  - `createTime`：消息发送时间  

  - `播放完成回调接口`：
    `youmeim.OnPlayCompletion = function( errorcode， path )` 
    
  - `errorcode`：错误码
  - `path`：文件路径

### 10. 登出IM系统
注销账号时，调用登出接口`logout`登出IM系统。

- **调用示例：**

  ``` lua
      local errorcode = youmeim:logout()
  ``` 
  	  
- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)  
	  
- **回调接口：**
  登出是异步操作，登出成功的标识是`OnLogout`回调的错误码为Success，调用`logout`接口同步返回值是Success才能收到`OnLogout`回调。
  
  - `登出回调接口`：
    `youmeim.OnLogout = function ()`   

## 典型场景集成方案
主要分为世界频道聊天，用户私聊，直播聊天室；集成的时都需要先初始化SDK，登录IM系统

***流程图

### 启动应用，初始化IM SDK
应用启动的第一时间需要调用初始化接口。

- **接口与参数：**
  - `init`：初始化接口
  - `strAppKey`：用户游戏产品区别于其它游戏产品的标识，可以在[游密官网](https://account.youme.im/login)获取、查看
  - `strAppSecret`：用户游戏产品的密钥，可以在[游密官网](https://account.youme.im/login)获取、查看

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	  
- **代码示例与详细说明：**
	- [Lua示例](#初始化IM SDK)

### 登录应用时，登录IM系统
![登录界面介绍](https://www.youme.im/doc/images/im_login.png)
>- 点击`登录`按钮时，调用IM SDK登录接口。

- **接口与参数：**
	- `login`：登录接口
	- `userid`：由调⽤者分配，不可为空字符串，只可由字母或数字或下划线组成，用户首次登录会自动注册所用的用户ID和密码
	- `user_password`：⽤户密码，不可为空字符串，由调用者分配，二次登录时需与首次登录一致，否则会报UsernamePasswordError
	- `token`：用户验证token，可选，如不使用token验证传入:""
	
- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)

- **代码示例与详细说明：**
	- [Lua示例](#4. 登录IM系统)

### 典型场景
#### 世界频道聊天
![频道](https://www.youme.im/doc/images/im_room.png)
>- 进入应用后，调用加入频道接口，进入世界、公会、区域等需要进入的聊天频道。
>- 应用需要为各个聊天频道设置一个唯一的频道ID。
>- 成功进入频道后，发送频道消息，文本、语音消息等。

- **接口与参数**
	- `joinChatRoom`：加入频道
	- `room_id`：请求加入的频道ID，仅支持数字、字母、下划线组成的字符串，区分大小写，长度限制为255字节

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	
- **代码示例与详细说明：**
	- [Lua示例](#5. 进入聊天频道)

#### 用户私聊
>- 成功登录IM系统后，可和其它登录IM的用户发私聊消息，文本、语音消息等；发文本消息接口的聊天类型参数使用 `1`-私聊类型
>- 调用流程查看[发送文本消息](#发送文本消息)，[发送语音消息](#发送语音消息)

#### 直播聊天室
>- 用户集成直播SDK后，导入IM SDK
>- 初始化IM SDK
>- 登录IM 系统
>- 进入指定的聊天频道
>- 发送频道消息（例如：弹幕式），调用流程查看[发送文本消息](#发送文本消息)

### 发送文本消息
![频道](https://www.youme.im/doc/images/im_room_send_btn.png)
>- 点击发送按钮，调用发消息接口，将输入框中的内容发送出去
>- 发送出的消息出现在聊天框右侧
>- 表情消息可以将表情信息打包成Json格式发送

- **相关接口与参数：**
	- `sendTextMessage`：发文字消息接口
	- `str_reciver_id`：接收者ID,（⽤户ID或者频道ID）
	- `iChatType`：聊天类型,私聊传`1`，频道聊天传`2`；需要频道聊天得先成功进入频道后发文本消息，此频道内的成员才能接收到消息
	- `message_content`：聊天内容
	- `attachParam`：发送文本附加信息

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)

- **代码示例与详细说明：**
	- [Lua示例](#6. 发送文本消息)

- **拓展功能：**
发送消息时，可以将玩家头像、昵称、角色等级、vip等级等要素打包成json格式发送。

### 发送语音消息
![发送语音消息](https://www.youme.im/doc/images/im_send_voice_message.jpg)
>- 按住语音按钮时，调用`sendAudioMessage`
>- 松开按钮，调用`stopAudioMessage`接口，发送语音消息
>- 按住过程若需要取消发送，调用`cancleAudioMessage`取消发送

- **相关接口与参数：**
	- `sendAudioMessage`：发送语音消息接口
	- `stopAudioMessage`：结束录音接口
	- `cancleAudioMessage`：取消录音接口
	
	- `reciver_id`：接收者ID（⽤户ID或者群ID）
	- `chatType`：消息类型，表示私聊还是频道消息
	- `extra_param`：发送语音消息附加信息，主要用于调用方特别需求

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)

- **代码示例与详细说明：**
	- [Lua示例](#7. 发送语音消息)

### 接收消息
![接收消息](https://www.youme.im/doc/images/im_receive_message.jpg)
>- 通过`OnRecvMessage`接口被动接收消息，需要开发者实现，接收消息进行相应的展示，如果是语音消息需要下载语音，以及下载完成后的语音播放

- **相关接口与参数：**
	- `OnRecvMessage`：收消息接口

- **代码示例与详细说明：**
	- [Lua示例](#8. 接收消息)

### 接收语音消息并播放
![接收语音消息](https://www.youme.im/doc/images/im_receive_message-2.jpg)
>- 通过`bodyType`分拣出语音消息`（5）`
>- ⽤`serial`获得消息ID
>- 点击语音气泡，调用`downloadAudioFile`接口下载语音文件
>- 调用方播放语音文件

- **相关接口与参数：**
  - `downloadAudioFile`：下载语音文件接口
  - `startPlayAudio`：播放语音文件接口
   
	- `serial`：消息ID,数字字符串
	- `save_path`：语音文件保存路径
	
	- `path`：待播放文件路径

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	
- **代码示例与详细说明：**
	- [Lua示例](#9. 接收语音消息并播放)

### 登出IM系统
![更换账号](https://www.youme.im/doc/images/im_change_account.jpg)
>- 如下两种情况需要登出IM系统：
>- 注销账号时，调用`logout`接口登出IM系统
>- 退出应用时，调用`logout`接口登出IM系统

- **相关接口：**
	- `logout`：登出接口

- **返回值：**
	- `errorcode`：错误码，详细描述见[错误码定义](IMSDKCocosLua.php#错误码定义)
	
- **代码示例与详细说明：**
	- [Lua示例](#10. 登出IM系统)


//
//  USCSpeechUnderstander.h
//  nlu&asr
//
//  Created by yunzhisheng on 14-12-1.
//  Copyright (c) 2014年 usc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "USCConstant.h"

@class USCScene,USCSpeechResult,USCSceneManage,USCIAudioSource;
#pragma mark -
#pragma mark 代理
@protocol USCSpeechUnderstanderDelegate <NSObject>
/**
 *   错误信息
 *
 *  @param type  错误类型
 *  @param error 错误
 */
- (void)onRecognizeError:(int)type error:(NSError *)error;
/**
 *  事件回调
 *
 *  @param type 事件类型:如SPPEECHSTART,RECORDINGSTART,RECORDINGSTOP
 *  @param timeMs 
 */
//- (void)onEvent:(int)type timeMs:(int)timeMs;


/**
 事件回调

 @param type   事件类型
 @param timeMs 耗时回调, 不需要返回-1;
 */
- (void)onRecognizeEvent:(int)type timeMs:(int)timeMs;

/**
 *  结果回调
 *
 *  @param type 结果类型:ASR_FIX_RESULT,ASR_ONLINE_RESULT,ASR_ONLINE_LAST_RESULT,ASR_NLU_RESULT
 *  @param jsonString 结果
 */
- (void)onRecognizeResult:(int)type jsonString:(NSString *)jsonString;

@optional

/**
 *   默认设备录音数据结束回调,手动设置setAudioSource此回调无效
 *
 *  @param recordingDatas       录音数据 pcm
 */
- (void)onRecognizeAudioSourceRecordingStop:(NSMutableData *)recordingDatas;

@end

#pragma mark -
#pragma mark 类

/**
 *  语音理解类
 *  包含  在线识别 和 离线识别 语音唤醒
 */
@interface USCSpeechUnderstander : NSObject

@property (nonatomic, assign) float threshold;  //离线分数的阈值 (接口待开放给用户)

/**
 *  代理
 */
@property (nonatomic,weak) id<USCSpeechUnderstanderDelegate> delegate;

/**
 *  初始化
 *
 *  @param context 上下文环境变量，设置nil即可
 *
 *  @return 识别对象
 */
- (id)initWithContext:(NSString *)context;

/**
 *  开始语音识别,使用默认模型
 */
- (void)startWithTag:(NSString *)grammar_tag;
/**
 *  编译唤醒词
 */
-(void)compileUserData:(NSString *)in_partial_data_path out_dir:(NSString *)out_dir cmdArray:(NSMutableArray*)cmdArray;
/**
 开始启动buffuer识别
 */
- (void)startRecognizeData;

/**
 *  结束录音，停止本次语音识别，并回调相关结果,非阻塞.先录音停止回调onEvent type=ASR_EVENT_RECORDING_STOP 再完成识别回调onEvent type=ASR_EVENT_RECOGNIZITION_END，
 */
- (void)stop;

/**
 *  取消本次语音识别,停止所有相关回调
 */
- (void)cancel;

/**
 *  设置可选项，设置识别模式 SpeechConstants.ASR_SERVICE_MODE
 *  设置识别模式 SpeechConstants.ASR_SERVICE_MODE
 *  设置在线识别带宽 SpeechConstants.ASR_BANDWIDTH
 *  如云端识别 SpeechConstants.ASR_SERVICE_MODE_NET
 *  设置在线识别带宽 SpeechConstants.ASR_BANDWIDTH
 *  设置识别领域 SpeechConstants.ASR_DOMAIN
 *  设置远近讲 SpeechConstants.ASR_VOICE_FIELD 默认为VOICE_FIELD_NEAR
 *  设置识别语言 SpeechConstants.ASR_LANGUAGE 默认LANGUAGE_MANDARIN
 *  设置VAD前端点超时 SpeechConstants.ASR_VAD_TIMEOUT_FRONTSIL 范围 int 500~3000 (ms)
 *  设置VAD后端点超时 SpeechConstants.ASR_VAD_TIMEOUT_BACKSIL 范围 int 500~3000 (ms)
 *  设置语音解析服务器 SpeechConstants.ASR_SERVER_ADDR 格式:ip:port
 *  设置同步请求语义结果 SpeechConstants.NLU_ENABLE 默认同步请求语义
 *  设置语义理解场景 SpeechConstants.NLU_SCENARIO
 *  设置语义解析服务器 SpeechConstants.NLU_SERVER_ADDR 格式:ip:port
 *  设置网络交互超时 SpeechConstants.ASR_NET_TIMEOUT 范围 int 3000~10000 (ms)
 *  (不可用)设置云平台返回的历史信息 SpeechConstants.GENERAL_HISTORY(不可用)
 *  (不可用)设置解析城市信息 SpeechConstants.GENERAL_CITY
 *  (不可用)设置语义日志ID SpeechConstants.GENERAL_VOICEID
 *
 *  @param key   键
 *  @param value 值
 */
- (void)setOption:(int)key value:(id)value;

/**
 *  获取设置选项
 *
 *  @param key 选项对应key
 *
 *  @return 选项值
 */
- (id)getOption:(int)key;

/**
 *  暂不可用
 *  获取当前状态
 *
 *  @return 状态如READY, RECOGNIZING, etc
 */
- (int)getStatus;

/**
 *  暂不可用
 *  根据json初始化ASR引擎
 *
 *  @param jsonString json
 *
 *  @return  0 表示成功， 否则返回相应错误码
 */
- (int)init:(NSString *)jsonString;

/**
 *  暂不可用
 *  将识别参数以json格式批量输入到引擎,加入到目前识别中,当postInfo和setOption设置同一参数时,以最新调用为准 可设置参数请参考setoption
 *
 *  @param jsonString json
 *
 *  @return  0 表示成功， 否则返回相应错误码
 */
- (int)postInfo:(NSString *)jsonString;

/**
 *  上传用户词表，非阻塞,上传完成后回调onEvent type=　ASR_EVENT_USERDATA_UPLOADED,用户设置命令词并上传，提高识别率,如果当前为在线识别，则上传词表
 *
 *  词表为NSDictionary 类型, 键是NSNumber或NSString类型的枚举值 USCUserDataType ,值是NSArray类型
 *
 *  @param userData 用户词表
 */
- (void)uploadUserData:(NSDictionary <NSString *, NSArray *>*)userData;

/**
 *   设置audiosource
 *
 *  @param audioSource 如果不设置默认使用内部录音设备
 *
 *  @return 0 表示成功， 否则返回相应错误码
 */
- (int)setAudioSource:(USCIAudioSource *)audioSource;

/**
 *  暂不可用
 *  编译用户词表，非阻塞 编译完成后回调onEvent type=　ASR_EVENT_GRAMMAR_COMPILED用户自定义jsgf格式标准语法文件，语音识别系统可以根据此生成标准格式的识别语法  如果当前为离线识别，则编译编译模型
 *
 *  @param jsgfPath JSGF文件路径
 *  @param desFile  目标二进制文件
 *
 *  @return 0 表示成功， 否则返回相应错误码
 */
- (int)compileGrammar:(NSString *)jsgfPath desFile:(NSString *)desFile;

/**
 *  暂不可用
 *  把二进制的grammar文件插入相应的host grammar的槽内,非阻塞插入完成后回调onEvent tyÁpe=　ASR_GRAMMAR_INSERTED
 *
 *  @param grammarName - grammar名字
 *  @param grammarPath grammar路径
 *  @param slotName slotName
 *
 *  @return 0 表示成功， 否则返回相应错误码
 */
-(int)insertGrammar:(NSString *)grammarName grammarPath:(NSString *)grammarPath slotName:(NSString *)slotName;

/**
 *  暂不可用
 *  把动态获得的用户信息插入相应的grammar内, 非阻塞 插入完成后回调onEvent type=　ASR_EVENT_VOCAB_INSERTED  执行后即可start进行识别
 *
 *  @param strList   用户信息列表 比如 联系人列表
 *  @param vocabName 词表名称比如contact,需要和grammar对应
 *
 *  @return 0 表示成功， 否则返回相应错误码
 */
- (int)insertVocab:(NSArray *)strList vocab:(NSString *)vocabName;

/**
 *  暂不可用
 *  保存当前Grammar到文件
 *
 *  @param grammarPath grammar保存路径
 *
 *  @return 0 表示成功， 否则返回相应错误码
 */
- (int)saveCurGrammar:(NSString *)grammarPath;

/**
 *  暂不可用
 *  根据type值进行卸载ASR_ENGINE,ASR_USERDICT, ASR_GRAMMAR, ASR_VOCAB
 *  dataName:null,userDictName,grammarName,vocabName
 *
 *  @param type type
 *  @param dataName dataName
 *
 *  @return 0 表示成功， 否则返回相应错误码
 */
- (int)release:(int)type dataName:(NSString *)dataName;

/*!
 *  直接识别音频文件，注意调用这个方法后，会直接开始识别，不需要在调用start方法。音频文件的格式只支持PCM或者WAV。
 *
 *  @brief  识别音频
 *
 *  @param audioFilePath 音频文件的路径
 */
- (void)recognizeAudioFile:(NSString *)audioFilePath;

/*!
 *  直接识别音频流，开始识别: -[USCSpeechUnderstander startRecognizeData], 完成后调用stop方法。音频文件的格式只支持PCM或者WAV。
 *
 *  @brief  识别音频
 *
 *  @param audioData 音频字节数据, 长度不能大于9600字节
 */
- (void)recognizeAudioFileFromBytesData:(NSData *)audioData;

/**
 释放麦克风资源
 */
- (void)releaseMacroResource;

@end

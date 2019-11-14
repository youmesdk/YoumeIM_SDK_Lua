//
//  USCOnlineTTS.h
//  tts_online_test2
//
//  Created by yunzhisheng-zy on 14-12-4.
//  Copyright (c) 2014年 usc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class USCIAudioSource;

#pragma mark -
#pragma mark protocal

/**
 *  语音合成参数
 */
typedef enum
{
    /**
     *  音高 取值 0~100 50为标准音高,只能设整数
     */
    USCSynthesizeParam_Volume,
    /**
     *  语速 取值 0~100 50为标准语速,只能设整数
     */
    USCSynthesizeParam_Speed,
    /**
     *  发音人 为说话人 @"xiaoli" 中文女生（默认） @"Joe" 英文男生，注意英文男生首字母大写
     */
    USCSynthesizeParam_VoiceName,
    /**
     *  设置音高
     */
    USCSynthesizeParam_Pitch
}USCSynthesizeParam;

// @class - 协议
// @brief - 语音合成的相关代理方法
@protocol USCSpeechSynthesizerDelegate <NSObject>
/**
 *  合成出错
 *
 *  @param type     类型
 *  @param error 错误
 */
- (void)onSynthesizeError:(int)type error:(NSError *)error;

/**
 *  在合成和播放过程中的事件的回调 包括 start，pause，play，resume， end
 *
 *  @param type 事件类型
 */
- (void)onSynthesizeEvent:(int)type;

/**
 *  合成数据回调,会有多次回调
 *
 *  @param data     音频buffuer
 */
- (void)onSynthesizePlayResultData:(NSData *)data;

@end

#pragma mark -
#pragma mark 类
/**
 TTS类语音合成播放类
 */
@interface USCSpeechSynthesizer : NSObject

@property (nonatomic,weak) id<USCSpeechSynthesizerDelegate> delegate;
/**
 *  初始化
 *
 *  @param context  预留参数  传nil即可
 *
 *  @return 返回合成实例
 */
- (id)initWithContext:(NSString *)context;

/**
 *  恢复播放
 */
- (void)resumeSpeaking;

#pragma mark -
#pragma mark new interface

/**
 *  设置需要合成的utf8的文本, 并开始合成,非阻塞
 *  开始合成回调 onEvent type=TTS_EVENT_SYNTHESIZE_START
 *  合成完成后回调onEvent type= onEvent type=TTS_EVENT_SYNTHESIZE_STOP
 *
 *  @param utfTxt 要合成的文本
 */
- (void)synthesizeText:(NSString *)utfTxt;

/**
 *  合成并播放utf8的文本,直到播放结束,非阻塞 开始合成回调onEvent type=TTS_EVENT_SYNTHESIZE_START 合成完成后回调onEvent type= TTS_EVENT_SYNTHESIZE_STOP
 *  播放完成后回调onEvent type= TTS_EVENT_PLAYING_STOP
 *
 *  @param utfTxt 要合成的文本
 *
 *  @return 正常播放返回0
 */
- (int)playText:(NSString *)utfTxt;


/**
 *  将前面合成的声音播放出来,非阻塞 播放完成后回调onEvent type= TTS_EVENT_PLAYING_STOP
 */
- (void)playSynWav;

/**
 *  停止播放,非阻塞
 */
- (void)stop;

/**
 *  暂停后恢复播放,非阻塞
 */
- (void)resume;

/**
 *  暂停播放,非阻塞
 */
- (void)pause;

/**
 *  设置合成相关参数
 *  
 *  设置离线识别
 *  设置合成语速 SpeechConstants.TTS_VOICE_SPEED 范围 0 ~ 100 int
 *  设置合成音高 SpeechConstants.TTS_VOICE_PITCH 范围 0 ~ 100 int
 *  设置合成音量 SpeechConstants.TTS_VOICE_VOLUME 范围 0 ~ 100 int
 *  设置合成领域 SpeechConstants.TTS_FIELD (暂不支持)
 *  设置服务器地址 SpeechConstants.TTS_SERVER_ADDR  (暂不支持)
 *  设置合成码率 SpeechConstants.TTS_SAMPLE_RATE  (暂不支持)
 *  设置播放开始缓冲时间 SpeechConstants.TTS_PLAY_START_TIME  (暂不支持)
 *  
 *  @param key   键
 *  @param value 值
 */
- (void)setOption:(int)key value:(id)value;

/**
 *  获取合成相关参数设置
 *  
 *  @param key   键
 */
- (id)getOption:(int)key;

/**
 *  判断当前状态  READY : 1,
                SYNTHESIZING : 2,
                PLAYING : 3,
                PAUSE : 4,
                STOPPED : 5
 */
- (int)getStatus;

#pragma mark mark - hide public method
/*!
 *  @author usc_zy, 15-01-16 14:01:33
 *
 *  @brief  设置tts的服务器地址
 *
 *  @param address 地址
 *  @param publicCloud  公有云私有云
 *
 *  @since 1.5
 */
- (void)setTTSAddress:(NSString *)address public:(BOOL)publicCloud;
@end

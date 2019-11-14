//
//  UScAudioSource.h
//  asr_nlu_tts
//
//  Created by iOSDeveloper-zy on 15-6-18.
//  Copyright (c) 2015年 usc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol USCIAudioSourceDelegate <NSObject>

/**
 *  录音开始
 *
 *  @param errorCode 录音开启失败返回错误码
 */
- (void) onRecordingStart:(int)errorCode;

/**
 *  录音停止
 *
 *  @param recordingDatas 返回的录音数据
 */
- (void) onRecordingStop:(NSMutableData *)recordingDatas;

@end


/**
 音频源类, 如果需要自定义音频源,继承本类并重写方法
 */
@interface USCIAudioSource : NSObject

@property (nonatomic, assign) id delegate;

/**
 *  打开录音设备
 *
 *  @return 0 表示成功，否则返回错误码
 */
- (int)openAudioIn;

///**
// *  打开放音设备
// *
// *  @return 0 表示成功，否则返回错误码
// */
//- (int)openAudioOut;

/**
 *  读取 size 大小的声音到buffer里,注意:当返回值<0表示录音结束.等于0表示当前没有数据可读,sdk内部会循环等待.
 *
 *  @param size size
 *
 *  @return 实际读取的字节数。
 */
- (NSData *)readDataSize:(int)size;

///**
// *  写入size大小的buffer到放音设备
// *
// *  @param buffer 数据
// *  @param size   数据大小
// *
// *  @return 实际写入的字节数
// */
//- (int)writeData:(NSData *)buffer size:(int)size;

/**
 *  关闭录音设备
 */
- (void)closeAudioIn;

///**
// *  关闭放音设备
// */
//- (void)closeAudioOut;

/**
 *  设置录音采样率
 */
- (id)initWithSampeleRate:(int)outSampeleRate;

/**
 释放资源
 */
- (void)releaseMacroResource;

/**
 音频buffuer数量

 @return 数量
 */
- (int)numOfArray;
@end

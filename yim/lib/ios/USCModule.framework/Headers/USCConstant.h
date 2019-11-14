//
//  USCConstantFile.h
//  asr&nlu&tts
//
//  Created by iOSDeveloper-zy on 15-5-5.
//  Copyright (c) 2015年 usc. All rights reserved.
//

#import <Foundation/Foundation.h>

#define AUDIOBUFFERSIZE 3200
#define kLabelH 20
#define kUSCPadding 10

/**
 stauts
 */
typedef enum {
   USC_Status_Ready = 1, // 初始化完成后状态
   USC_Status_Recognizing = 2, // 表示正在进行语音识别中...
   USC_Status_Stop,
   USC_Status_End // 表示识别结束,可能是正常结束或者中间出错结束
}USCSpeechStatus;

typedef enum {
    USCEventDownloadAudioFile = 100
}USCEventId;

/*!
 属性
 */
typedef enum
{
    USC_ASR_SERVICE_ADDRESS = 100,
    USC_NLU_URL = 1000
}USCSpeechUnderstanderProperty;
///*!
// 识别采样率
// */
typedef enum
{
    USCBankWidth_RATE_8K = 8000,
    USCBankWidth_RATE_16K = 16000,
    USCBankWidth_BANDWIDTH_AUTO,
}USCBankWidth;

/*!
 识别语言
 */
typedef enum {
    USCSpeechRecognizeLanguage_CN,// 中文
    USCSpeechRecognizeLanguage_EN,// 英文
    USCSpeechRecognizeLanguage_CO // 粤语
}USCSpeechRecognizeLanguage;

// 识别领域
static NSString * const USC_ENGINE_GENERAL = @"general";
static NSString * const USC_ENGINE_POI = @"poi";
static NSString * const USC_ENGINE_SONG = @"song";
static NSString * const USC_ENGINE_MOVIETV = @"movietv";
static NSString * const USC_ENGINE_MEDICAL = @"medical";

//离线tts模型
static NSString * const FM_KEY = @"FM";  //字典模型
static NSString * const BM_KEY = @"BM";  //发音人模型

//// 识别语言
//NSString * const USC_LANGUAGE_CHINESE = @"chinese";
//NSString * const USC_LANGUAGE_ENGLISH = @"english";
//NSString * const USC_LANGUAGE_CANTONESE = @"cantonese";

typedef enum {
    LVL_N = 0,// 不打印日志 NO
    LVL_A = 1,// 日志打印等级
    LVL_E = 2,              //Error
    LVL_D = 3,              //Debug
    LVL_I = 4,              //Info
    LVL_W = 5,              //Warn
    LVL_F = 6               //Fatal
}USCLogLevel;

//typedef enum {
//    USCLogLevel_NO,
//    USCLogLevel_Debug,
//    USCLogLevel_Info,
//    USCLogLevel_Verbose
//}USCLogLevel;

/*
 上传个性化数据支持的类型
 */
typedef enum
{
    kUSCPersonName = 1,
    kUSCAppName    = 2,
    kUSCSongName   = 3,
    kUSCSongerName = 4,
    kUSCAlbumName  = 5,
    kUSCCommandName= 6,
    kUSCPoiName    = 7
} USCUserDataType;

typedef enum
{
    USC_SERVICE_ADDRESS_PORT = 100, //设置私有识别服务器
    SAMPLE_RATE_AUTO = 400, //设置2G/3G智能切换
}USCRecognizerProperty;

/*!
 识别语言
 */
typedef enum {
    USCRecognizeLanguage_CN,// 中文
    USCRecognizeLanguage_EN,// 英文
    USCRecognizeLanguage_CO // 粤语
}USCRecognizeLanguage;

typedef enum {
    USCVPRBindTypeBindClient = 1,  // 绑定
    USCVPRBindTypeUnBindClient,    // 解绑
    USCVPRBindTypeRecognizeUser,   // 识别
    USCVPRBindTypeWhetherBind      // 绑定
}USCVPRBindType;

//static   NSString *VOICE_FIELD_FAR = @"far"; 
//static   NSString *VOICE_FIELD_NEAR = @"near";
static   NSString *SAMPLE_RATE_8K = @"8k";
static   NSString *SAMPLE_RATE_16K = @"16k";
static   NSString *SAMPLE_RATE_16kto8K = @"16kto8k";

static  NSString *ASR_RECOGNIZE_ONLINE_ONLY = @"online_only";
static  NSString *ASR_RECOGNIZE_OFFLINE_ONLY = @"offline_only";
static  NSString *ASR_RECOGNIZE_MIX = @"mixing";
static  NSString *ASR_RECOGNIZE_WAKEUP = @"recognize_wakeup";
static  NSString *ASR_WAKEUP_AND_RECOGNIZE = @"wakeup_and_recognize";



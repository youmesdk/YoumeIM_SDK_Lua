//
//  USCVoiceprintParams.h
//  asr&nlu&tts
//
//  Created by iOSDeveloper-zy on 15-4-17.
//  Copyright (c) 2015年 usc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "USCConstant.h"





/**
 声纹识别参数类
 */
@interface USCVoiceprintParams : NSObject

//MARK: 非必须参数
/**
 用户token
 当accessToken不为空时，则子系统ID也不可为空，并且此时bindMode为byUser
 */
@property (nonatomic ,  copy)NSString *accessToken;

/**
 子系统ID
 */
@property (nonatomic ,  copy)NSString *subSystemId;

/** 用户Id */
@property (nonatomic ,  copy)NSString *userId;

/**
 VPR绑定模式
 分为按用户绑定（byUser）或按设备绑定（byDevice），默认是按照设备绑定（分别最多绑定6个）
 */
@property (nonatomic ,  copy)NSString *bindMode;
@property (nonatomic ,assign)USCVPRBindType bindType;


//MARK: 必须参数

/**
 VPR host
 */
@property (nonatomic ,  copy, readonly)NSString *host;

/**
 VPR port
 */
@property (nonatomic ,assign, readonly)int port;

/**
 当前应用的KEY
 */
@property (nonatomic ,  copy)NSString *appkey;

/**
 当前设备ID
 按设备绑定或按用户绑定都需要传UDID，UDID就是userId
 */
@property (nonatomic ,  copy)NSString *udid;

/**
 输入数据类型（text,picture,audio等）
 暂时只支持audio
 */
@property (nonatomic ,  copy)NSString *inputDataType;

/**
 自定义名称
 在一个UDID（或passportId）维度中是唯一的
 */
@property (nonatomic ,  copy)NSString *customizedName;

/**
 服务类型名称
 请求vpr对应的服务名，分别是bindClient（绑定设备）、unBindClient（解除绑定）、recognizeUser（识别用户身份）、whetherBind（查询用户是否绑定）
 */
@property (nonatomic ,  copy)NSString *serviceTypeName;


/**
 构造函数 两个参数必须要设置
 host : VPR host
 port : VPR port
 */
+ (instancetype)paramWithHost:(NSString *)host port:(int)port;

@end

/* inputDataType */
extern NSString * const USC_VPR_Param_InputDataType_Audio;
extern NSString * const USC_VPR_Param_InputDataType_Text;
extern NSString * const USC_VPR_Param_InputDataType_Picture;

/* bindMode */
extern NSString * const USC_VPR_Param_ByUser;
extern NSString * const USC_VPR_Param_ByDevice;

/* serviceTypeName */
extern NSString * const USC_VPR_Param_BindClient;
extern NSString * const USC_VPR_Param_UnBindClient;
extern NSString * const USC_VPR_Param_RecognizeUser;
extern NSString * const USC_VPR_Param_WhetherBind;





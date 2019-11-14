//
//  USCPreferenceConfigure.h
//  USC_SDK
//
//  Created by zhusanbao on 2016/11/2.
//  Copyright © 2016年 zhusanbao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "USCConstant.h"

@class USCRestInterface;

/**
 *  sdk 配置类,可以获取版本号，设置日志打印等级等
 */

@interface USCPreferenceConfigure : NSObject

/**
 *  获取当前sdk版本号
 *
 *  @return 版本号
 */
+ (NSString *)getVersion;

/**
 是否开启打印日志, 默认关闭

 @param enable YES开启, NO关闭
 */
+ (void)setLogEnable:(BOOL)enable;

/**
 *  设置日志打印等级
 *
 *  @param level 日志等级 default: LVL_D
 */
+ (void)setLogLevel:(USCLogLevel)level;
/**
 *  获取当前日志等级
 */
+(int)logLvl;

/**
 * 设备中心激活，激活前，需先给kUSCRestShareInterface赋值。
 */
+ (void)active:(void (^)(id))activeHandle;

@end

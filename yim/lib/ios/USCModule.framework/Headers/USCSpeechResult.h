//
//  USCSpeechResult.h
//  nlu&asr
//
//  Created by yunzhisheng-zy on 14-12-1.
//  Copyright (c) 2014年 usc. All rights reserved.
//

#import <Foundation/Foundation.h>

// @class - 理解结果类
// @brief - 包括了要理解的结果，场景，结果，
@interface USCSpeechResult : NSObject
/**
 *  请求返回的json结果
 */
@property (nonatomic,copy) NSString  *stringResult;
/**
 *  json中的text标签中内容
 */
@property (nonatomic,copy) NSString *responseText;
/**
 *  请求理解的文本
 */
@property (nonatomic,copy) NSString *requestText;
/**
 *  场景名字
 */
@property (nonatomic,copy) NSString *scenario;

@end

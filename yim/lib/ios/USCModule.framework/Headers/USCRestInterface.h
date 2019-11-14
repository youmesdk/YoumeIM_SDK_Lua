//
//  USCRestInterface.h
//  USC_SDK
//
//  Created by zhusanbao on 2016/11/16.
//  Copyright © 2016年 zhusanbao. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kUSCRestShareInterface ([USCRestInterface shareInterface])

/**
 设备激活参数类, 必选参数传入才能正常开启识别
 */
@interface USCRestInterface : NSObject

/**
 参数类单例

 @return 返回实例
 */
+ (instancetype)shareInterface;

/**客户认可的设备唯一标识（由客户端APP设置）（可选)*/
@property (nonatomic, copy) NSString *deviceSn;

/**应用KEY        （必填）  请到官网申请: http://www.unisound.com/*/
@property (nonatomic, copy) NSString *appKey;

/**应用appSecret  （必填）  请到官网申请  http://www.unisound.com/*/
@property (nonatomic, copy) NSString *appSecret;

/**应用版本号 (可选)*/
@property (nonatomic, copy) NSString *appVersion;

/**包名（可选）*/
@property (nonatomic, copy) NSString *pkgName;

/**设备的IMEI（可选）*/
@property (nonatomic, copy) NSString *imei;

/**用户第三方唯一标识（可选）*/
@property (nonatomic, copy) NSString *userId;

/**设备MAC地址（可选项）*/
@property (nonatomic, copy) NSString *macAddress;

/**WIFI的名称（可选项）*/
@property (nonatomic, copy) NSString *wifiSsid;

/**运营商（可选项）*/
@property (nonatomic, copy) NSString *telecomOperator;

/**当前的接入点MAC地址（可选项）*/
@property (nonatomic, copy) NSString *bssId;

/**设备名称（可选项）*/
@property (nonatomic, copy) NSString *productName;

/**设备型号名称（可选项）*/
@property (nonatomic, copy) NSString *productModel;

/**制造商名称（可选项）*/
@property (nonatomic, copy) NSString *productMfr;

/**操作系统（可选项）*/
@property (nonatomic, copy) NSString *productOs;

/**操作系统版本号*/
@property (nonatomic, copy) NSString *productOsVersion;

/**厂商硬件序列号（可选项）*/
@property (nonatomic, copy) NSString *hardwareSn;

/**备注（可选项）*/
@property (nonatomic, copy) NSString *memo;

@end

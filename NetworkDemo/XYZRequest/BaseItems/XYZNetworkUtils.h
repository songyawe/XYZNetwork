//
//  XYZNetworkUtils.h
//  工具类
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XYZBaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT void XYZLog(NSString *format, ...) NS_FORMAT_FUNCTION(1,2);

@class AFHTTPSessionManager;

@interface XYZNetworkUtils : NSObject
//不备份到iCloud
+ (void)addDoNotBackupAttribute:(NSString *)path;
//md5加密
+ (NSString *)md5StringFromString:(NSString *)string;
//软件版本
+ (NSString *)appVersionString;
//获取请求编码方式
+ (NSStringEncoding)stringEncodingWithRequest:(XYZBaseRequest *)request;
//验证断点下载的数据
+ (BOOL)validateResumeData:(NSData *)data;

@end

NS_ASSUME_NONNULL_END


//
//  XYZNetworkConfig.h
//  配置文件
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class AFSecurityPolicy;


//网络配置文件
@interface XYZNetworkConfig : NSObject

// 使用类方法创建单例对象
+ (XYZNetworkConfig *)sharedConfig;

// 请求的根 URL，默认是空字符串
@property (nonatomic, strong) NSString *baseUrl;

// 同 AFNetworking 中使用的安全策略
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

// 是否记录调试信息，默认是 NO
@property (nonatomic, getter = isDebugLogEnabled) BOOL debugLogEnabled;

// 用来初始化 AFHTTPSessionManager,默认是 nil
@property (nonatomic, strong) NSURLSessionConfiguration* sessionConfiguration;
@end

NS_ASSUME_NONNULL_END

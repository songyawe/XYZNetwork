//
//  XYZNetworkManager.h
//  网络请求管理类
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class XYZBaseRequest;


@interface XYZNetworkManager : NSObject


+ (XYZNetworkManager *)sharedManager;

/// 添加请求
- (void)addRequest:(XYZBaseRequest *)request;

///  取消请求
- (void)cancelRequest:(XYZBaseRequest *)request;

///  取消所有未完成的请求
- (void)cancelAllRequests;

///  返回链接URL
- (NSString *)buildRequestUrl:(XYZBaseRequest *)request;

@end

NS_ASSUME_NONNULL_END

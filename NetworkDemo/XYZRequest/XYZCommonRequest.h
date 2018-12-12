//
//  XYZCommonRequest.h
//  NetworkDemo
//
//  Created by xyz on 2018/12/11.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZBaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

@interface XYZCommonRequest : XYZBaseRequest

/**
普通网络请求方式
 */
+ (instancetype)XYZ_requestWithUrl:(NSString *)urlString requestType:(XYZRequestType)requestType;
/**
 请求参数
 */
@property (nonatomic,copy) NSDictionary *requestParames;

@end

NS_ASSUME_NONNULL_END

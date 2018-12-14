//
//  XYZCacheCommonRequest.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/12.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZCacheCommonRequest.h"

@implementation XYZCacheCommonRequest

+ (instancetype)XYZ_requestWithUrl:(NSString *)urlString requestType:(XYZRequestType)requestType useCacheType:(XYZCacheType)cacheType{
    XYZCacheCommonRequest *commonR = [[self alloc]init];
    commonR.requestUrl = urlString;
    commonR.requestMethod = requestType;
    commonR.useCacheType = cacheType;
    commonR.requestTimeoutInterval = 10;
    return commonR;
}
//设置附加请求头
- (NSDictionary *)requestHeaderFieldValueDictionary {
    
    NSMutableDictionary *mDic = [NSMutableDictionary dictionary];
    [mDic setValue:NSLocalizedString(@"chinese", nil) forKey:@"Accept-Language"];
    
    NSString *cookie = [[NSUserDefaults standardUserDefaults] objectForKey: @"Set-Cookie"];
    if (cookie != nil && cookie.length > 0) {
        cookie = [cookie stringByReplacingOccurrencesOfString:@"," withString:@";"];
        [mDic setValue:cookie forKey:@"Cookie"];
    }
    return [mDic copy];
}


@end

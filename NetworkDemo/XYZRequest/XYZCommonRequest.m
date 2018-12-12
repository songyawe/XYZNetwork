//
//  XYZCommonRequest.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/11.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZCommonRequest.h"
@interface XYZCommonRequest(){
    /**请求url*/
    NSString *_urlString;
    /**请求方式*/
    XYZRequestType _requestType;
}

@end
@implementation XYZCommonRequest

+ (instancetype)XYZ_requestWithUrl:(NSString *)urlString requestType:(XYZRequestType)requestType{
    XYZCommonRequest *commonR = [[self alloc]init];
    commonR -> _urlString = urlString;
    commonR -> _requestType = requestType;
    return commonR;
}

- (XYZRequestType)requestMethod {
    return _requestType;
}

- (NSString *)requestUrl {
    return _urlString;
}
//超时10秒
- (NSTimeInterval)requestTimeoutInterval {
    return 10;
}

- (id)parames {
    return self.requestParames;
}

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

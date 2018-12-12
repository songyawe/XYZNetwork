//
//  XYZDownloadRequest.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/11.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZDownloadRequest.h"

@implementation XYZDownloadRequest
//设置请求路径时覆盖
- (NSString *)requestUrl {
    return @"thunder7/Thunder_dl_7.9.34.4908.exe";
}
//设置根URL时覆盖
- (NSString *)baseUrl {
    return @"http://down.sandai.net/";
}
//设置超时时覆盖
- (NSTimeInterval)requestTimeoutInterval {
    return 10;
}

- (NSString *)downloadPath{
     NSString*cachesDir = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory,NSUserDomainMask,YES) firstObject];
    NSString *path = [cachesDir stringByAppendingPathComponent:@"XYZ/Download/"];
    return path;
}

- (XYZRequestType)requestMethod {
    return XYZRequestTypeGET;
}

@end

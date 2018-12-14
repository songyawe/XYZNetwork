//
//  XYZDownloadRequest.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/11.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZDownloadRequest.h"

@implementation XYZDownloadRequest

- (instancetype)init{
    self = [super init];
    if (self) {
        self.baseUrl = @"http://down.sandai.net/";
        self.requestUrl =  @"thunder7/Thunder_dl_7.9.34.4908.exe";
        self.requestMethod = XYZRequestTypeGET;
        self.requestTimeoutInterval = 10;
    }
    return self;
}

//文件下载到本地的路径设置
- (NSString *)downloadPath{
     NSString*cachesDir = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory,NSUserDomainMask,YES) firstObject];
    NSString *path = [cachesDir stringByAppendingPathComponent:@"XYZ/Download/"];
    return path;
}

@end

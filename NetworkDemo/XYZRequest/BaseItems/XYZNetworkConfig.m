//
//  XYZNetworkConfig.m
//  配置文件
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZNetworkConfig.h"
#import <AFNetworking/AFNetworking.h>


@implementation XYZNetworkConfig

+ (XYZNetworkConfig *)sharedConfig {
    static id sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _baseUrl = @"";
        _securityPolicy = [AFSecurityPolicy defaultPolicy];
        _debugLogEnabled = NO;
    }
    return self;
}

@end

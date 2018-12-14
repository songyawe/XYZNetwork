//
//  XYZBaseRequest.m
//  网络请求基类
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZBaseRequest.h"
#import "XYZNetworkManager.h"
#import "XYZNetworkUtils.h"
#import <AFNetworking/AFNetworking.h>

@implementation XYZBaseRequest

- (instancetype)init{
    if (self = [super init]) {
        //默认参数
        _requestTimeoutInterval = 60;
        _requestMethod = XYZRequestTypeGET;
        _requestSerializerType = XYZRequestSerializerTypeHTTP;
        _responseSerializerType = XYZResponseSerializerTypeJSON;
        _allowsCellularAccess = YES;
    }
    return self;
}

#pragma mark - 设置请求成功失败block

- (void)setCompletionBlockWithSuccess:(XYZRequestCompletionBlock)success
                              failure:(XYZRequestCompletionBlock)failure {
    self.successCompletionBlock = success;
    self.failureCompletionBlock = failure;
}

- (void)clearCompletionBlock {
    self.successCompletionBlock = nil;
    self.failureCompletionBlock = nil;
}

#pragma mark - 请求类相关方法

- (void)start {
    [[XYZNetworkManager sharedManager] addRequest:self];
}

- (void)stop {
    [[XYZNetworkManager sharedManager] cancelRequest:self];
}

- (void)sendRequestWithSuccess:(XYZRequestCompletionBlock)success
                                    failure:(XYZRequestCompletionBlock)failure {
    [self setCompletionBlockWithSuccess:success failure:failure];
    [self start];
}

#pragma mark - 子类中用到需要覆盖

- (void)requestCompletePreprocessor {
}

- (void)requestCompleteFilter {
}

- (void)requestFailedPreprocessor {
}

- (void)requestFailedFilter {
}

- (id)cacheFileNameFilterForParames:(id)parames {
    return parames;
}

- (NSArray *)requestAuthorizationHeaderFieldArray {
    return nil;
}

- (NSDictionary *)requestHeaderFieldValueDictionary {
    return nil;
}

- (NSURLRequest *)buildCustomUrlRequest {
    return nil;
}

- (BOOL)statusCodeValidator {
    NSInteger statusCode = [self responseStatusCode];
    return (statusCode >= 200 && statusCode <= 299);
}



#pragma mark - Request and Response Information

- (NSHTTPURLResponse *)response {
    return (NSHTTPURLResponse *)self.requestTask.response;
}

- (NSInteger)responseStatusCode {
    return self.response.statusCode;
}

- (NSDictionary *)responseHeaders {
    return self.response.allHeaderFields;
}

- (NSURLRequest *)currentRequest {
    return self.requestTask.currentRequest;
}

- (NSURLRequest *)originalRequest {
    return self.requestTask.originalRequest;
}

- (BOOL)isCancelled {
    if (!self.requestTask) {
        return NO;
    }
    return self.requestTask.state == NSURLSessionTaskStateCanceling;
}

- (BOOL)isExecuting {
    if (!self.requestTask) {
        return NO;
    }
    return self.requestTask.state == NSURLSessionTaskStateRunning;
}

@end

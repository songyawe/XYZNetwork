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


#pragma mark - Request Configuration

- (void)setCompletionBlockWithSuccess:(XYZRequestCompletionBlock)success
                              failure:(XYZRequestCompletionBlock)failure {
    self.successCompletionBlock = success;
    self.failureCompletionBlock = failure;
}

- (void)clearCompletionBlock {
    self.successCompletionBlock = nil;
    self.failureCompletionBlock = nil;
}

#pragma mark - Request Action

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

#pragma mark - Subclass Override

- (void)requestCompletePreprocessor {
}

- (void)requestCompleteFilter {
}

- (void)requestFailedPreprocessor {
}

- (void)requestFailedFilter {
}
//设置请求路径时覆盖
- (NSString *)requestUrl {
    return @"";
}
//设置根URL时覆盖
- (NSString *)baseUrl {
    return @"";
}
//设置超时时覆盖
- (NSTimeInterval)requestTimeoutInterval {
    return 60;
}
//设置请求参数时覆盖
- (id)parames {
    return nil;
}

- (id)cacheFileNameFilterForParames:(id)parames {
    return parames;
}

- (XYZRequestType)requestMethod {
    return XYZRequestTypeGET;
}

- (XYZRequestSerializerType)requestSerializerType {
    return XYZRequestSerializerTypeHTTP;
}

- (XYZResponseSerializerType)responseSerializerType {
    return XYZResponseSerializerTypeJSON;
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

- (BOOL)allowsCellularAccess {
    return YES;
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

//
//  XYZGroupRequest.m
//  群组请求
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZGroupRequest.h"
#import "XYZNetworkUtils.h"
#import "XYZBaseRequest.h"

@implementation XYZGroupRequest

- (instancetype)initWithRequestArray:(NSArray<XYZBaseRequest *> *)requestArray {
    self = [super init];
    if (self) {
        _requestArray = [requestArray copy];
        _finishedCount = 0;
        for (XYZBaseRequest * req in _requestArray) {
            if (![req isKindOfClass:[XYZBaseRequest class]]) {
                XYZLog(@"Error, request item must be XYZBaseRequest instance.");
                return nil;
            }
        }
    }
    return self;
}

- (void)start {
    if (_finishedCount > 0) {
        XYZLog(@"Error! Batch request has already started.");
        return;
    }
    _failedRequest = nil;
    for (XYZBaseRequest * req in _requestArray) {
        [req clearCompletionBlock];
        __weak typeof(self) weakSelf = self;
        [req sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
            weakSelf.finishedCount++;
            if (weakSelf.successProcessBlock) {
                weakSelf.successProcessBlock(request,self);
            }
            if (weakSelf.finishedCount == weakSelf.requestArray.count) {
                if (weakSelf.successCompletionBlock) {
                    weakSelf.successCompletionBlock(self);
                }
                [weakSelf clearCompletionBlock];
            }

        } failure:^(__kindof XYZBaseRequest * _Nonnull request) {
            weakSelf.failedRequest = request;
            for (XYZBaseRequest *req in weakSelf.requestArray) {
                [req stop];
            }
            if (weakSelf.failureCompletionBlock) {
                weakSelf.failureCompletionBlock(self);
            }
            [weakSelf clearCompletionBlock];
        }];
    }
}

- (void)stop {
    [self clearRequest];
}

- (void)sendRequestWithProcess:(nullable void (^)(XYZBaseRequest *processRequest , XYZGroupRequest *groupRequest))process
                       success:(nullable void (^)(XYZGroupRequest *groupRequest))success
                       failure:(nullable void (^)(XYZGroupRequest *groupRequest))failure{
    [self setCompletionBlockWithPrcocess:process success:success failure:failure];
    [self start];
}

- (void)setCompletionBlockWithPrcocess:(nullable void (^)(XYZBaseRequest *processRequest, XYZGroupRequest *groupRequest))process
                               success:(nullable void (^)(XYZGroupRequest *groupRequest))success
                               failure:(nullable void (^)(XYZGroupRequest *groupRequest))failure{
    self.successProcessBlock = process;
    self.successCompletionBlock = success;
    self.failureCompletionBlock = failure;
}

- (void)clearCompletionBlock {
    self.successProcessBlock = nil;
    self.successCompletionBlock = nil;
    self.failureCompletionBlock = nil;
}

- (void)dealloc {
    [self clearRequest];
}

- (void)clearRequest {
    for (XYZBaseRequest * req in _requestArray) {
        [req stop];
    }
    [self clearCompletionBlock];
}

@end

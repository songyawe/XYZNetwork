//
//  XYZGroupRequest.h
//  群组请求
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class XYZBaseRequest;
@class XYZGroupRequest;
@protocol XYZRequestAccessory;

//请求组
@interface XYZGroupRequest : NSObject

//保存所有请求
@property (nonatomic, strong, readonly) NSArray<XYZBaseRequest *> *requestArray;

//请求组访问全部成功返回
@property (nonatomic, copy, nullable) void (^successProcessBlock)(XYZBaseRequest *,XYZGroupRequest *);

//请求组访问全部成功返回
@property (nonatomic, copy, nullable) void (^successCompletionBlock)(XYZGroupRequest *);

//请求组访问一个或多个失败返回
@property (nonatomic, copy, nullable) void (^failureCompletionBlock)(XYZGroupRequest *);

//第一个访问失败的请求
@property (nonatomic, strong, nullable) XYZBaseRequest *failedRequest;

//完成第几个
@property (nonatomic) NSInteger finishedCount;
//初始化请求组
- (instancetype)initWithRequestArray:(NSArray<XYZBaseRequest *> *)requestArray;

//设置请求组block
- (void)setCompletionBlockWithPrcocess:(nullable void (^)(XYZBaseRequest *processRequest , XYZGroupRequest *groupRequest))process
                               success:(nullable void (^)(XYZGroupRequest *groupRequest))success
                              failure:(nullable void (^)(XYZGroupRequest *groupRequest))failure;

//置空block,防止循环引用.
- (void)clearCompletionBlock;


//开启请求组访问
- (void)start;

//停止请求组访问
- (void)stop;

//开启请求组访问
- (void)sendRequestWithProcess:(nullable void (^)(XYZBaseRequest *processRequest , XYZGroupRequest *groupRequest))process
                       success:(nullable void (^)(XYZGroupRequest *groupRequest))success
                                    failure:(nullable void (^)(XYZGroupRequest *groupRequest))failure;

@end

NS_ASSUME_NONNULL_END

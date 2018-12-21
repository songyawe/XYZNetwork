// UIRefreshControl+AFNetworking.m
//
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
此类别为UIKit框架的“UIRefreshControl”类添加方法。 此类别中的方法根据会话任务的加载状态提供对自动开始和结束刷新的支持。
 */
@interface UIRefreshControl (AFNetworking)

///-----------------------------------
/// @name Refreshing for Session Tasks
///-----------------------------------

/**
 将刷新状态绑定到指定任务的状态。
  
   @param task 任务。 如果是`nil`，将禁用从任何先前指定的操作自动更新。
 */
- (void)setRefreshingWithStateOfTask:(NSURLSessionTask *)task;

@end

NS_ASSUME_NONNULL_END

#endif

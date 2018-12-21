// UIActivityIndicatorView+AFNetworking.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV

#import <UIKit/UIKit.h>

/**
此类别为UIKit框架的“UIActivityIndicatorView”类添加了方法。 此类别中的方法支持根据会话任务的加载状态自动启动和停止动画。
 */
@interface UIActivityIndicatorView (AFNetworking)

///----------------------------------
/// @name Animating for Session Tasks
///----------------------------------

/**
 将动画状态绑定到指定任务的状态。
 
   @param task 任务任务。 如果是`nil`，将禁用从任何先前指定的操作自动更新。
 */
- (void)setAnimatingWithStateOfTask:(nullable NSURLSessionTask *)task;

@end

#endif

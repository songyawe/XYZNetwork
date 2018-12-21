// UIProgressView+AFNetworking.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN


/**
此类别为UIKit框架的“UIProgressView”类添加了方法。 此类别中的方法支持将进度绑定到会话任务的上载和下载进度。
 */
@interface UIProgressView (AFNetworking)

///------------------------------------
/// @name Setting Session Task Progress
///------------------------------------

/**
 将进度绑定到指定会话任务的上载进度。
 
   @param task 任务会话任务。
   @param animated 动画`YES`如果更改应该是动画，'NO`如果更改应该立即发生。
 */
- (void)setProgressWithUploadProgressOfTask:(NSURLSessionUploadTask *)task
                                   animated:(BOOL)animated;

/**
 将进度绑定到指定会话任务的下载进度。
 
   @param task 任务会话任务。
   @param animated 动画`YES`如果更改应该是动画，'NO`如果更改应该立即发生。
 */
- (void)setProgressWithDownloadProgressOfTask:(NSURLSessionDownloadTask *)task
                                     animated:(BOOL)animated;

@end

NS_ASSUME_NONNULL_END

#endif

// UIWebView+AFNetworking.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class AFHTTPSessionManager;

/**
 此类别为UIKit框架的“UIWebView”类添加方法。 此类别中的方法可以更好地控制请求周期，包括进度监控和成功/失败处理。
 
   @discussion使用这些类别方法时，请确保为Web视图分配`delegate`，它实现`-webView：shouldStartLoadWithRequest：navigationType：`。 这允许通过AFNetworking加载tapped链接，并且可以确保`canGoBack`和`canGoForward`正确地更新它们的值。
 */
@interface UIWebView (AFNetworking)

/**
会话管理器用于下载所有请求。
 */
@property (nonatomic, strong) AFHTTPSessionManager *sessionManager;

/**
 异步加载指定的请求。
 
   @param request 一个URL请求，标识要加载的内容的位置。 这绝不是“零”。
   @param progress 监视当前下载进度的进度对象。
   @param success 请求完成加载后要执行的块对象。 此块返回要由Web视图加载的HTML字符串，并采用两个参数：响应和响应字符串。
   @param failure 数据任务未成功完成或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值并采用单个参数：发生的错误。
 */
- (void)loadRequest:(NSURLRequest *)request
           progress:(NSProgress * _Nullable __autoreleasing * _Nullable)progress
            success:(nullable NSString * (^)(NSHTTPURLResponse *response, NSString *HTML))success
            failure:(nullable void (^)(NSError *error))failure;

/**
 使用指定的MIME类型和文本编码异步加载与特定请求关联的数据。
 
  @param request 一个URL请求，标识要加载的内容的位置。这绝不是“零”。
  @param MIMEType 内容的MIME类型。如果未指定，则默认为响应的内容类型。
  @param textEncodingName IANA编码名称，如`utf-8`或`utf-16`。如果未指定，则默认为响应文本编码。
 @param progress 监视当前下载进度的进度对象。
  @param success 请求完成加载后要执行的块对象。此块返回Web视图要加载的数据，并采用两个参数：响应和下载的数据。
  @param failure 数据任务未成功完成或成功完成但在解析响应数据时遇到错误时要执行的块对象。此块没有返回值并采用单个参数：发生的错误。
 */
- (void)loadRequest:(NSURLRequest *)request
           MIMEType:(nullable NSString *)MIMEType
   textEncodingName:(nullable NSString *)textEncodingName
           progress:(NSProgress * _Nullable __autoreleasing * _Nullable)progress
            success:(nullable NSData * (^)(NSHTTPURLResponse *response, NSData *data))success
            failure:(nullable void (^)(NSError *error))failure;

@end

NS_ASSUME_NONNULL_END

#endif

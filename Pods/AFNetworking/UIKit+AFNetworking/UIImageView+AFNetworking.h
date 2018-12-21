// UIImageView+AFNetworking.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class AFImageDownloader;

/**
此类别为UIKit框架的`UIImageView`类添加方法。 此类别中的方法支持从URL异步加载远程图像。
 */
@interface UIImageView (AFNetworking)

///------------------------------------
/// @name Accessing the Image Downloader
///------------------------------------

/**
 设置用于下载图像的共享图像下载程序。
 
   @param imageDownloader 用于下载图像的共享图像下载程序。
 */
+ (void)setSharedImageDownloader:(AFImageDownloader *)imageDownloader;

/**
用于下载图像的共享图像下载程序。
 */
+ (AFImageDownloader *)sharedImageDownloader;

///--------------------
/// @name Setting Image
///--------------------

/**
 从指定的URL异步下载映像，并在请求完成后设置它。 之前对接收器的任何图像请求都将被取消。
 
   如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
   默认情况下，URL请求的“Accept”标头字段值为“image / *”，缓存策略为“NSURLCacheStorageAllowed”，超时间隔为30秒，并且设置为不处理cookie。 要以不同方式配置URL请求，请使用`setImageWithURLRequest：placeholderImage：success：failure：`
 
   @param url 用于图像请求的URL。
 */
- (void)setImageWithURL:(NSURL *)url;

/**
 从指定的URL异步下载映像，并在请求完成后设置它。 之前对接收器的任何图像请求都将被取消。
 
   如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
   默认情况下，URL请求的“Accept”标头字段值为“image / *”，缓存策略为“NSURLCacheStorageAllowed”，超时间隔为30秒，并且设置为不处理cookie。 要以不同方式配置URL请求，请使用`setImageWithURLRequest：placeholderImage：success：failure：`
 
   @param url 用于图像请求的URL。
   @param placeholderImage 最初要设置的图像，直到图像请求完成。 如果为“nil”，图像视图将不会更改其图像，直到图像请求完成。
 */
- (void)setImageWithURL:(NSURL *)url
       placeholderImage:(nullable UIImage *)placeholderImage;

/**
 从指定的URL请求异步下载映像，并在请求完成后设置它。之前对接收器的任何图像请求都将被取消。
 
  如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
  如果指定了成功块，则块的责任是在返回之前设置图像视图的图像。如果未指定成功块，则应用使用“self.image = image”设置图像的默认行为。
 
  @param urlRequest 用于图像请求的URL请求。
  @param placeholderImage 最初要设置的图像，直到图像请求完成。如果为“nil”，图像视图将不会更改其图像，直到图像请求完成。
  @param success 图像数据任务成功完成时要执行的块。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应以及从请求的响应数据创建的映像。如果图像是从缓存返回的，则响应参数将为“nil”。
  @param failure 当映像数据任务未成功完成或成功完成时要执行的块对象。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应，以及描述网络或发生的解析错误的错误对象。
 */
- (void)setImageWithURLRequest:(NSURLRequest *)urlRequest
              placeholderImage:(nullable UIImage *)placeholderImage
                       success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, UIImage *image))success
                       failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;

/**
 取消接收器的任何正在执行的图像操作（如果存在）。
 */
- (void)cancelImageDownloadTask;

@end

NS_ASSUME_NONNULL_END

#endif

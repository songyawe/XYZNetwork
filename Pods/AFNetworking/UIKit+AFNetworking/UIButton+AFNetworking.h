// UIButton+AFNetworking.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class AFImageDownloader;

/**
 此类别为UIKit框架的“UIButton”类添加了方法。 此类别中的方法支持从URL异步加载远程图像和背景图像。
 
   @warning不支持控制`state`的复合值（例如`UIControlStateHighlighted | UIControlStateDisabled`）。
 */
@interface UIButton (AFNetworking)

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
 异步下载指定URL中的图像，并在请求完成后将其设置为指定状态的图像。 之前对接收器的任何图像请求都将被取消。
 
    如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
   @param state 控制状态。
   @param url 用于图像请求的URL。
 */
- (void)setImageForState:(UIControlState)state
                 withURL:(NSURL *)url;

/**
 异步下载指定URL中的图像，并在请求完成后将其设置为指定状态的图像。 之前对接收器的任何图像请求都将被取消。
 
   如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
   @param state 控制状态。
   @param url 用于图像请求的URL。
   @param placeholderImage 最初要设置的图像，直到图像请求完成。 如果为“nil”，则在图像请求完成之前，按钮不会更改其图像。
 */
- (void)setImageForState:(UIControlState)state
                 withURL:(NSURL *)url
        placeholderImage:(nullable UIImage *)placeholderImage;

/**
 异步下载指定URL请求中的图像，并在请求完成后将其设置为指定状态的图像。之前对接收器的任何图像请求都将被取消。
 
  如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
  如果指定了成功块，则块的责任是在返回之前设置按钮的图像。如果未指定成功块，则应用使用`setImage：forState：`设置图像的默认行为。
 
  @param state 控制状态。
  @param urlRequest 用于图像请求的URL请求。
  @param placeholderImage 最初要设置的图像，直到图像请求完成。如果为“nil”，则在图像请求完成之前，按钮不会更改其图像。
  @param success 图像数据任务成功完成时要执行的块。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应以及从请求的响应数据创建的映像。如果图像是从缓存返回的，则响应参数将为“nil”。
  @param failure 当映像数据任务未成功完成或成功完成时要执行的块对象。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应，以及描述网络或发生的解析错误的错误对象。
 */
- (void)setImageForState:(UIControlState)state
          withURLRequest:(NSURLRequest *)urlRequest
        placeholderImage:(nullable UIImage *)placeholderImage
                 success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, UIImage *image))success
                 failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;


///-------------------------------
/// @name Setting Background Image
///-------------------------------

/**
 异步下载指定URL中的图像，并在请求完成后将其设置为指定状态的背景图像。 之前的任何接收器背景图像请求都将被取消。
 
   如果背景图像在本地缓存，则立即设置背景图像，否则将立即设置指定的占位符背景图像，然后在请求完成后设置远程背景图像。
 
   @param state 控制状态。
   @param url 用于背景图像请求的URL。
 */
- (void)setBackgroundImageForState:(UIControlState)state
                           withURL:(NSURL *)url;

/**
 异步下载指定URL中的图像，并在请求完成后将其设置为指定状态的背景图像。 之前对接收器的任何图像请求都将被取消。
 
   如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
   @param state 控制状态。
   @param url 用于背景图像请求的URL。
   @param placeholderImage 最初要设置的背景图像，直到背景图像请求完成。 如果为“nil”，则在背景图像请求完成之前，按钮不会更改其背景图像。
 */
- (void)setBackgroundImageForState:(UIControlState)state
                           withURL:(NSURL *)url
                  placeholderImage:(nullable UIImage *)placeholderImage;

/**
 异步下载指定URL请求中的图像，并在请求完成后将其设置为指定状态的图像。之前对接收器的任何图像请求都将被取消。
 
  如果图像在本地缓存，则立即设置图像，否则将立即设置指定的占位符图像，然后在请求完成后设置远程图像。
 
  如果指定了成功块，则块的责任是在返回之前设置按钮的图像。如果未指定成功块，则应用使用`setBackgroundImage：forState：`设置图像的默认行为。
 
  @param state 控制状态。
  @param urlRequest 用于图像请求的URL请求。
  @param placeholderImage 最初要设置的背景图像，直到背景图像请求完成。如果为“nil”，则在背景图像请求完成之前，按钮不会更改其背景图像。
  @param success 图像数据任务成功完成时要执行的块。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应以及从请求的响应数据创建的映像。如果图像是从缓存返回的，则响应参数将为“nil”。
  @param failure 当映像数据任务未成功完成或成功完成时要执行的块对象。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应，以及描述网络或发生的解析错误的错误对象。
 */
- (void)setBackgroundImageForState:(UIControlState)state
                    withURLRequest:(NSURLRequest *)urlRequest
                  placeholderImage:(nullable UIImage *)placeholderImage
                           success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, UIImage *image))success
                           failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;


///------------------------------
/// @name Canceling Image Loading
///------------------------------

/**
 取消接收器的指定控制状态的任何正在执行的图像任务（如果存在）。
 
   @param state 控制状态。
 */
- (void)cancelImageDownloadTaskForState:(UIControlState)state;

/**
 取消接收器的指定控制状态的任何正在执行的背景图像任务（如果存在）。
 
   @param state 控制状态。
 */
- (void)cancelBackgroundImageDownloadTaskForState:(UIControlState)state;

@end

NS_ASSUME_NONNULL_END

#endif

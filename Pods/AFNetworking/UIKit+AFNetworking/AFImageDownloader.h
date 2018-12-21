// AFImageDownloader.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV 

#import <Foundation/Foundation.h>
#import "AFAutoPurgingImageCache.h"
#import "AFHTTPSessionManager.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, AFImageDownloadPrioritization) {
    AFImageDownloadPrioritizationFIFO,
    AFImageDownloadPrioritizationLIFO
};

/**
`AFImageDownloadReceipt`是启动数据任务时由`AFImageDownloader`提供的对象。 它可用于取消在`AFImageDownloader`会话上运行的活动任务。 作为一般规则，应使用`AFImageDownloadReceipt`取消图像数据任务，而不是直接在`task`本身上调用`cancel`。 `AFImageDownloader`经过优化，可以处理重复的任务场景以及待处理和活动下载。
 */
@interface AFImageDownloadReceipt : NSObject

/**
由`AFImageDownloader`创建的数据任务。
 */
@property (nonatomic, strong) NSURLSessionDataTask *task;

/**
成功和失败的唯一标识符在发出重复请求时阻止。
 */
@property (nonatomic, strong) NSUUID *receiptID;
@end

/**
 `AFImageDownloader`类负责在优先级队列上并行下载图像。 传入的下载将添加到队列的前面或后面，具体取决于下载优先级。 每个下载的图像都缓存在底层的“NSURLCache”以及内存中的图像缓存中。 默认情况下，在图像缓存中具有等效缓存图像的任何下载请求将自动为缓存的图像表示提供服务。
 */
@interface AFImageDownloader : NSObject

/**
用于存储所有下载图像的图像缓存。默认情况下为“AFAutoPurgingImageCache”。
 */
@property (nonatomic, strong, nullable) id <AFImageRequestCache> imageCache;

/**
用于下载图像的`AFHTTPSessionManager`。 默认情况下，它配置了一个`AFImageResponseSerializer`，以及一个用于所有图像下载的共享`NSURLCache`。
 */
@property (nonatomic, strong) AFHTTPSessionManager *sessionManager;

/**
定义插入队列的传入下载请求的顺序优先级。 `AFImageDownloadPrioritizationFIFO`默认情况下。
 */
@property (nonatomic, assign) AFImageDownloadPrioritization downloadPrioritizaton;

/**
 使用默认值初始化`AFImageDownloader`的共享默认实例。
 */
+ (instancetype)defaultInstance;

/**
 创建具有常用用法参数值的默认`NSURLCache`。
 
   @returns默认的`NSURLCache`实例。
 */
+ (NSURLCache *)defaultURLCache;

/**
 具有常用用法参数值的默认`NSURLSessionConfiguration`。
 */
+ (NSURLSessionConfiguration *)defaultURLSessionConfiguration;

/**
 默认初始化程序
 
   @return使用默认值初始化的`AFImageDownloader`实例。
 */
- (instancetype)init;

/**
 具有特定`URLSessionConfiguration`的初始化程序
  
   @param configuration 配置要使用的`NSURLSessionConfiguration`
  
   @return使用默认值和自定义`NSURLSessionConfiguration'初始化的`AFImageDownloader`实例
 */
- (instancetype)initWithSessionConfiguration:(NSURLSessionConfiguration *)configuration;

/**
 级，最大活动下载计数和图像缓存。
 
   @param sessionManager 用于下载图像的会话管理器。
   @param downloadPrioritization 下载队列的下载优先级。
   @param maximumActiveDownloads 在任何给定时间允许的最大活动下载数。 推荐`4`。
   @param imageCache 用于存储所有下载图像的图像缓存。
 
   @return新的`AFImageDownloader`实例。
 */
- (instancetype)initWithSessionManager:(AFHTTPSessionManager *)sessionManager
                downloadPrioritization:(AFImageDownloadPrioritization)downloadPrioritization
                maximumActiveDownloads:(NSInteger)maximumActiveDownloads
                            imageCache:(nullable id <AFImageRequestCache>)imageCache;

/**
 使用指定URL请求的`sessionManager`实例创建数据任务。
 
  如果相同的数据任务已在队列中或当前正在下载，则成功和失败块为
  附加到现有任务。任务完成后，所有成功或失败块都附加到
  任务按添加顺序执行。
 
  @param request URL请求。
  @param success 图像数据任务成功完成时要执行的块。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应以及从请求的响应数据创建的映像。如果图像是从缓存返回的，则响应参数将为“nil”。
  @param failure 当映像数据任务未成功完成或成功完成时要执行的块对象。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应，以及描述网络或发生的解析错误的错误对象。
 
  @return数据任务的图像下载收据（如果有）。如果图像存储在缓存中，则为“nil”。
  缓存和URL请求缓存策略允许使用缓存。
 */
- (nullable AFImageDownloadReceipt *)downloadImageForURLRequest:(NSURLRequest *)request
                                                        success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse  * _Nullable response, UIImage *responseObject))success
                                                        failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;

/**
 使用指定URL请求的`sessionManager`实例创建数据任务。
 
  如果相同的数据任务已在队列中或当前正在下载，则成功和失败块为
  附加到现有任务。任务完成后，所有成功或失败块都附加到
  任务按添加顺序执行。
 
  @param request URL请求。
  @param receiptID 用于为此请求创建的下载收据的标识符。这必须是不代表任何其他请求的唯一标识符。
  @param success 图像数据任务成功完成时要执行的块。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应以及从请求的响应数据创建的映像。如果图像是从缓存返回的，则响应参数将为“nil”。
  @param failure 当映像数据任务未成功完成或成功完成时要执行的块对象。此块没有返回值，并且有三个参数：从客户端发送的请求，从服务器接收的响应，以及描述网络或发生的解析错误的错误对象。
 
  @return数据任务的图像下载收据（如果有）。如果图像存储在缓存中，则为“nil”。
  缓存和URL请求缓存策略允许使用缓存。
 */
- (nullable AFImageDownloadReceipt *)downloadImageForURLRequest:(NSURLRequest *)request
                                                 withReceiptID:(NSUUID *)receiptID
                                                        success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse  * _Nullable response, UIImage *responseObject))success
                                                        failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;

/**
 通过删除相应的成功和失败块并在必要时取消数据任务来取消收据中的数据任务。
 
   如果队列中的数据任务处于挂起状态，则如果没有向数据任务注册其他成功和失败块，则将取消该任务。 如果数据任务当前正在执行或已经完成，则会删除成功和失败块，并且在任务完成时不会调用。
 
   @param imageDownloadReceipt 要取消的图像下载收据。
 */
- (void)cancelTaskForImageDownloadReceipt:(AFImageDownloadReceipt *)imageDownloadReceipt;

@end

#endif

NS_ASSUME_NONNULL_END

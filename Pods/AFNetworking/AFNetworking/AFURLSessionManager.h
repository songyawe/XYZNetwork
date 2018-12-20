// AFURLSessionManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
#import <Foundation/Foundation.h>

#import "AFURLResponseSerialization.h"
#import "AFURLRequestSerialization.h"
#import "AFSecurityPolicy.h"
#import "AFCompatibilityMacros.h"
#if !TARGET_OS_WATCH
#import "AFNetworkReachabilityManager.h"
#endif

/**
 `AFURLSessionManager`基于指定的`NSURLSessionConfiguration`对象创建和管理`NSURLSession`对象，该对象符合`<NSURLSessionTaskDelegate>`，`<NSURLSessionDataDelegate>`，`<NSURLSessionDownloadDelegate>`和`<NSURLSessionDelegate>`。
 
  ## Subclassing Notes
 
  这是`AFHTTPSessionManager`的基类，它添加了特定于发出HTTP请求的功能。如果您希望专门为HTTP扩展`AFURLSessionManager`，请考虑改为继承`AFHTTPSessionManager`。
 
  ## NSURLSession和NSURLSessionTask委托方法
 
  `AFURLSessionManager`实现以下委托方法：
 
  ###`NSURLSessionDelegate`
 
   - `URLSession：didBecomeInvalidWithError：`
   - `URLSession：didReceiveChallenge：completionHandler：`
   - `URLSessionDidFinishEventsForBackgroundURLSession：`
 
  ###`NSURLSessionTaskDelegate`
 
   - `URLSession：willPerformHTTPRedirection：newRequest：completionHandler：`
   - `URLSession：task：didReceiveChallenge：completionHandler：`
   - `URLSession：task：didSendBodyData：totalBytesSent：totalBytesExpectedToSend：`
   - `URLSession：task：needNewBodyStream：`
   - `URLSession：task：didCompleteWithError：`
 
  ###`NSURLSessionDataDelegate`
 
   - `URLSession：dataTask：didReceiveResponse：completionHandler：`
   - `URLSession：dataTask：didBecomeDownloadTask：`
   - `URLSession：dataTask：didReceiveData：`
   - `URLSession：dataTask：willCacheResponse：completionHandler：`
 
  ###`NSURLSessionDownloadDelegate`
 
   - `URLSession：downloadTask：didFinishDownloadingToURL：`
   - `URLSession：downloadTask：didWriteData：totalBytesWritten：totalBytesWritten：totalBytesExpectedToWrite：`
   - `URLSession：downloadTask：didResumeAtOffset：expectedTotalBytes：`
 
  如果在子类中重写了这些方法中的任何一个，那么它们必须首先调用`super`实现。
 
  ##网络可达性监控
 
  可以通过`reachabilityManager`属性获得网络可达性状态和更改监视。应用程序可以选择监视网络可访问性条件，以防止或暂停任何出站请求。有关详细信息，请参阅`AFNetworkReachabilityManager`。
 
  ## NSCoding警告
 
   - 编码管理器不包含任何块属性。使用`-initWithCoder：`或`NSKeyedUnarchiver`时，请务必设置委托回调块。
 
  ## NSCopying警告
 
   - `-copy`和`-copyWithZone：`返回一个新的管理器，其中新的`NSURLSession`是从原始配置创建的。
   - 操作副本不包括任何委托回调块，因为它们通常强烈捕获对“self”的引用，否则在复制时会指向_original_会话管理器的非直观副作用。
 
  @warning后台会话的经理必须在使用期间拥有。这可以通过创建应用程序范围或共享单例实例来完成。
 */

NS_ASSUME_NONNULL_BEGIN

@interface AFURLSessionManager : NSObject <NSURLSessionDelegate, NSURLSessionTaskDelegate, NSURLSessionDataDelegate, NSURLSessionDownloadDelegate, NSSecureCoding, NSCopying>

/**
 托管会话
 */
@property (readonly, nonatomic, strong) NSURLSession *session;

/**
运行委托回调的操作队列。
 */
@property (readonly, nonatomic, strong) NSOperationQueue *operationQueue;

/**
 在使用`dataTaskWithRequest创建的数据任务中从服务器发送的响应：成功：失败：`并使用`GET` /`POST` / et al运行。 方便性方法由响应序列化器自动验证和序列化。 默认情况下，此属性设置为`AFJSONResponseSerializer`的实例。
 
   @warning`responseSerializer`不能是'nil`。
 */
@property (nonatomic, strong) id <AFURLResponseSerialization> responseSerializer;

///-------------------------------
/// @name Managing Security Policy
///-------------------------------

/**
 创建的会话用于评估服务器对安全连接的信任的安全策略。 除非另有说明，否则`AFURLSessionManager`使用`defaultPolicy`。
 */
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

#if !TARGET_OS_WATCH
///--------------------------------------
/// @name Monitoring Network Reachability
///--------------------------------------

/**
网络可达性管理器。 `AFURLSessionManager`默认使用`sharedManager`。
 */
@property (readwrite, nonatomic, strong) AFNetworkReachabilityManager *reachabilityManager;
#endif

///----------------------------
/// @name Getting Session Tasks
///----------------------------

/**
托管会话当前运行的数据，上载和下载任务。
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionTask *> *tasks;

/**
当前由托管会话运行的数据任务。
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionDataTask *> *dataTasks;

/**
托管会话当前运行的上载任务。
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionUploadTask *> *uploadTasks;

/**
当前由托管会话运行的下载任务。
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionDownloadTask *> *downloadTasks;

///-------------------------------
/// @name Managing Callback Queues
///-------------------------------

/**
 `completionBlock`的调度队列。 如果为“NULL”（默认值），则使用主队列。
 */
@property (nonatomic, strong, nullable) dispatch_queue_t completionQueue;

/**
“completionBlock”的调度组。 如果为“NULL”（默认），则使用私有调度组。
 */
@property (nonatomic, strong, nullable) dispatch_group_t completionGroup;

///---------------------------------
/// @name Working Around System Bugs
///---------------------------------

/**
 当初始调用返回“nil”时，是否尝试重试为后台会话创建上载任务。 “NO”默认情况下。
 
   @bug从iOS 7.0开始，有一个错误，即为后台任务创建的上传任务有时是“无”。 作为解决方法，如果此属性为“是”，AFNetworking将遵循Apple的建议再次尝试创建任务。
 */
@property (nonatomic, assign) BOOL attemptsToRecreateUploadTasksForBackgroundSessions;

///---------------------
/// @name Initialization
///---------------------

/**
 为使用指定配置创建的会话创建并返回管理器。 这是指定的初始化程序。
 
   @param configuration用于创建托管会话的配置。
 
   @return新创建的会话的经理。
 */
- (instancetype)initWithSessionConfiguration:(nullable NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

/**
 使托管会话无效，可选择取消挂起的任务。
 
   @param cancelPendingTasks是否取消挂起的任务。
 */
- (void)invalidateSessionCancelingTasks:(BOOL)cancelPendingTasks;

///-------------------------
/// @name Running Data Tasks
///-------------------------

/**
 使用指定的请求创建`NSURLSessionDataTask`。
 
   @param request请求的HTTP请求。
   @param completionHandler任务完成时要执行的块对象。 此块没有返回值，并且有三个参数：服务器响应，该序列化程序创建的响应对象以及发生的错误（如果有）。
 */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                            completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject,  NSError * _Nullable error))completionHandler DEPRECATED_ATTRIBUTE;

/**
 使用指定的请求创建`NSURLSessionDataTask`。
 
   @param request请求的HTTP请求。
   @param uploadProgressBlock更新上载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param downloadProgressBlock更新下载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param completionHandler任务完成时要执行的块对象。 此块没有返回值，并且有三个参数：服务器响应，该序列化程序创建的响应对象以及发生的错误（如果有）。
 */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                               uploadProgress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                             downloadProgress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                            completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject,  NSError * _Nullable error))completionHandler;

///---------------------------
/// @name Running Upload Tasks
///---------------------------

/**
 使用指定的本地文件请求创建`NSURLSessionUploadTask`。
 
   @param request请求的HTTP请求。
   @param fileURL要上载的本地文件的URL。
   @param uploadProgressBlock更新上载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param completionHandler任务完成时要执行的块对象。 此块没有返回值，并且有三个参数：服务器响应，该序列化程序创建的响应对象以及发生的错误（如果有）。
 */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromFile:(NSURL *)fileURL
                                         progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                                completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError  * _Nullable error))completionHandler;

/**
 使用HTTP主体的指定请求创建`NSURLSessionUploadTask`。
 
   @param request请求的HTTP请求。
   @param bodyData包含要上载的HTTP正文的数据对象。
   @param uploadProgressBlock更新上载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param completionHandler任务完成时要执行的块对象。 此块没有返回值，并且有三个参数：服务器响应，该序列化程序创建的响应对象以及发生的错误（如果有）。
 */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromData:(nullable NSData *)bodyData
                                         progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                                completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError * _Nullable error))completionHandler;

/**
 使用指定的流请求创建`NSURLSessionUploadTask`。
 
   @param request请求的HTTP请求。
   @param uploadProgressBlock更新上载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param completionHandler任务完成时要执行的块对象。 此块没有返回值，并且有三个参数：服务器响应，该序列化程序创建的响应对象以及发生的错误（如果有）。
 */
- (NSURLSessionUploadTask *)uploadTaskWithStreamedRequest:(NSURLRequest *)request
                                                 progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                                        completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError * _Nullable error))completionHandler;

///-----------------------------
/// @name Running Download Tasks
///-----------------------------

/**
 使用指定的请求创建`NSURLSessionDownloadTask`。
 
  @param request请求的HTTP请求。
  @param downloadProgressBlock更新下载进度时要执行的块对象。请注意，此块在会话队列上调用，而不是在主队列中调用。
  @param destination要执行的块对象，以确定下载文件的目标。此块接受两个参数，即目标路径和服务器响应，并返回所得到的下载所需的文件URL。下载过程中使用的临时文件将在移动到返回的URL后自动删除。
  @param completionHandler任务完成时要执行的块。此块没有返回值，并且有三个参数：服务器响应，下载文件的路径以及描述网络或发生的解析错误（如果有）的错误。
 
  @warning如果在iOS上使用后台`NSURLSessionConfiguration`，这些块将在应用程序终止时丢失。后台会话可能更喜欢使用`-setDownloadTaskDidFinishDownloadingBlock：`来指定用于保存下载文件的URL，而不是此方法的目标块。
 */
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request
                                             progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                                          destination:(nullable NSURL * (^)(NSURL *targetPath, NSURLResponse *response))destination
                                    completionHandler:(nullable void (^)(NSURLResponse *response, NSURL * _Nullable filePath, NSError * _Nullable error))completionHandler;

/**
 使用指定的恢复数据创建`NSURLSessionDownloadTask`。
 
   @param resumeData用于恢复下载的数据。
   @param downloadProgressBlock更新下载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param destination要执行的块对象，以确定下载文件的目标。 此块接受两个参数，即目标路径和服务器响应，并返回所得到的下载所需的文件URL。 下载过程中使用的临时文件将在移动到返回的URL后自动删除。
   @param completionHandler任务完成时要执行的块。 此块没有返回值，并且有三个参数：服务器响应，下载文件的路径以及描述网络或发生的解析错误（如果有）的错误。
 */
- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData
                                                progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                                             destination:(nullable NSURL * (^)(NSURL *targetPath, NSURLResponse *response))destination
                                       completionHandler:(nullable void (^)(NSURLResponse *response, NSURL * _Nullable filePath, NSError * _Nullable error))completionHandler;

///---------------------------------
/// @name Getting Progress for Tasks
///---------------------------------

/**
 返回指定任务的上载进度。
 
   @param任务会话任务。 一定不能是'nil`。
 
   @return一个`NSProgress`对象报告任务的上传进度，如果进度不可用，则为'nil`。
 */
- (nullable NSProgress *)uploadProgressForTask:(NSURLSessionTask *)task;

/**
 返回指定任务的下载进度。
 
   @param任务会话任务。 一定不能是'nil`。
 
   @return一个`NSProgress`对象报告任务的下载进度，如果进度不可用，则为'nil`。
 */
- (nullable NSProgress *)downloadProgressForTask:(NSURLSessionTask *)task;

///-----------------------------------------
/// @name Setting Session Delegate Callbacks
///-----------------------------------------

/**
 设置当托管会话变为无效时要执行的块，由`NSURLSessionDelegate`方法`URLSession：didBecomeInvalidWithError：`处理。
 
   @param block当托管会话变为无效时要执行的块对象。 该块没有返回值，并且有两个参数：会话，以及与失效原因相关的错误。
 */
- (void)setSessionDidBecomeInvalidBlock:(nullable void (^)(NSURLSession *session, NSError *error))block;

/**
 设置在发生连接级别身份验证质询时要执行的块，由`NSURLSessionDelegate`方法`URLSession：didReceiveChallenge：completionHandler：`处理。
 
   @param block发生连接级别身份验证质询时要执行的块对象。 该块返回身份验证质询的处置，并采用三个参数：会话，身份验证质询和指向应用于解决质询的凭据的指针。
 */
- (void)setSessionDidReceiveAuthenticationChallengeBlock:(nullable NSURLSessionAuthChallengeDisposition (^)(NSURLSession *session, NSURLAuthenticationChallenge *challenge, NSURLCredential * _Nullable __autoreleasing * _Nullable credential))block;

///--------------------------------------
/// @name Setting Task Delegate Callbacks
///--------------------------------------

/**
 设置当任务需要新的请求正文流发送到远程服务器时要执行的块，由`NSURLSessionTaskDelegate`方法`URLSession：task：needNewBodyStream：`处理。
 
   @param block当任务需要新的请求正文流时要执行的块对象。
 */
- (void)setTaskNeedNewBodyStreamBlock:(nullable NSInputStream * (^)(NSURLSession *session, NSURLSessionTask *task))block;

/**
 设置当HTTP请求尝试执行重定向到不同URL时要执行的块，由`NSURLSessionTaskDelegate`方法`URLSession：willPerformHTTPRedirection：newRequest：completionHandler：`处理。
 
   @param block当HTTP请求尝试执行重定向到其他URL时要执行的块对象。 该块返回重定向请求，并采用四个参数：会话，任务，重定向响应和与重定向响应相对应的请求。
 */
- (void)setTaskWillPerformHTTPRedirectionBlock:(nullable NSURLRequest * _Nullable (^)(NSURLSession *session, NSURLSessionTask *task, NSURLResponse *response, NSURLRequest *request))block;

/**
 设置当会话任务收到请求特定的身份验证质询时要执行的块，由`NSURLSessionTaskDelegate`方法`URLSession：task：didReceiveChallenge：completionHandler：`处理。
 
   @param block当会话任务收到请求特定的身份验证质询时要执行的块对象。 该块返回身份验证质询的处置，并采用四个参数：会话，任务，身份验证质询以及指向应用于解决质询的凭据的指针。
 */
- (void)setTaskDidReceiveAuthenticationChallengeBlock:(nullable NSURLSessionAuthChallengeDisposition (^)(NSURLSession *session, NSURLSessionTask *task, NSURLAuthenticationChallenge *challenge, NSURLCredential * _Nullable __autoreleasing * _Nullable credential))block;

/**
 设置一个定期执行的块来跟踪上传进度，由`NSURLSessionTaskDelegate`方法`URLSession：task：didSendBodyData：totalBytesSent：totalBytesExpectedToSend：`处理。
 
   @param block当已将未确定数量的字节上载到服务器时要调用的块对象。 该块没有返回值，并且有五个参数：会话，任务，自上次调用上载进度块以来写入的字节数，写入的总字节数，以及在请求期间预期写入的总字节数， 最初由HTTP主体的长度决定。 该块可以被多次调用，并将在主线程上执行。
 */
- (void)setTaskDidSendBodyDataBlock:(nullable void (^)(NSURLSession *session, NSURLSessionTask *task, int64_t bytesSent, int64_t totalBytesSent, int64_t totalBytesExpectedToSend))block;

/**
 将要执行的块设置为与特定任务相关的最后一条消息，由`NSURLSessionTaskDelegate`方法`URLSession：task：didCompleteWithError：`处理。
 
   @param block会话任务完成时要执行的块对象。 该块没有返回值，并且有三个参数：会话，任务以及执行任务的过程中发生的任何错误。

 */
- (void)setTaskDidCompleteBlock:(nullable void (^)(NSURLSession *session, NSURLSessionTask *task, NSError * _Nullable error))block;

///-------------------------------------------
/// @name Setting Data Task Delegate Callbacks
///-------------------------------------------

/**
 设置当数据任务收到响应时要执行的块，由`NSURLSessionDataDelegate`方法`URLSession：dataTask：didReceiveResponse：completionHandler：`处理。
 
   @param block数据任务收到响应时要执行的块对象。 该块返回会话响应的处置，并采用三个参数：会话，数据任务和接收的响应。
 */
- (void)setDataTaskDidReceiveResponseBlock:(nullable NSURLSessionResponseDisposition (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSURLResponse *response))block;

/**
 设置当数据任务成为下载任务时要执行的块，由`NSURLSessionDataDelegate`方法`URLSession：dataTask：didBecomeDownloadTask：`处理。
 
   @param block数据任务成为下载任务时要执行的块对象。 该块没有返回值，并且有三个参数：会话，数据任务和它已成为的下载任务。
 */
- (void)setDataTaskDidBecomeDownloadTaskBlock:(nullable void (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSURLSessionDownloadTask *downloadTask))block;

/**
 设置数据任务接收数据时要执行的块，由`NSURLSessionDataDelegate`方法`URLSession：dataTask：didReceiveData：`处理。
 
   @param block当从服务器下载未确定的字节数时要调用的块对象。 此块没有返回值，并且有三个参数：会话，数据任务和接收的数据。 该块可以被多次调用，并将在会话管理器操作队列上执行。
 */
- (void)setDataTaskDidReceiveDataBlock:(nullable void (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSData *data))block;

/**
 设置要执行的块以确定数据任务的缓存行为，由`NSURLSessionDataDelegate`方法`URLSession：dataTask：willCacheResponse：completionHandler：`处理。
 
   @param block要执行的块对象，以确定数据任务的缓存行为。 该块返回对缓存的响应，并采用三个参数：会话，数据任务和建议的缓存URL响应。
 */
- (void)setDataTaskWillCacheResponseBlock:(nullable NSCachedURLResponse * (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSCachedURLResponse *proposedResponse))block;

/**
 设置一个为会话排队的所有消息都要执行的块，由`NSURLSessionDataDelegate`方法`URLSessionDidFinishEventsForBackgroundURLSession：`处理。
 
   @param block一旦为会话排队的所有消息都已传递，将执行的块对象。 该块没有返回值，只接受一个参数：会话。
 */
- (void)setDidFinishEventsForBackgroundURLSessionBlock:(nullable void (^)(NSURLSession *session))block AF_API_UNAVAILABLE(macos);

///-----------------------------------------------
/// @name Setting Download Task Delegate Callbacks
///-----------------------------------------------

/**
 设置下载任务完成下载时要执行的块，由`NSURLSessionDownloadDelegate`方法`URLSession：downloadTask：didFinishDownloadingToURL：`处理。
 
   @param block下载任务完成时要执行的块对象。 该块返回应下载的URL，并采用三个参数：会话，下载任务和下载文件的临时位置。 如果文件管理器在尝试将临时文件移动到目标时遇到错误，将发布“AFURLSessionDownloadTaskDidFailToMoveFileNotification”，其中下载任务作为其对象，以及错误的用户信息。
 */
- (void)setDownloadTaskDidFinishDownloadingBlock:(nullable NSURL * _Nullable  (^)(NSURLSession *session, NSURLSessionDownloadTask *downloadTask, NSURL *location))block;

/**
 设置一个定期执行的块来跟踪下载进度，由`NSURLSessionDownloadDelegate`方法`URLSession：downloadTask：didWriteData：totalBytesWritten：totalBytesWritten：totalBytesExpectedToWrite：`处理。
 
   @param block当从服务器下载未确定的字节数时要调用的块对象。 此块没有返回值，并且有五个参数：会话，下载任务，自上次调用下载进度块以来读取的字节数，读取的总字节数以及请求期间预期要读取的总字节数 ，最初由`NSHTTPURLResponse`对象的预期内容大小确定。 该块可以被多次调用，并将在会话管理器操作队列上执行。
 */
- (void)setDownloadTaskDidWriteDataBlock:(nullable void (^)(NSURLSession *session, NSURLSessionDownloadTask *downloadTask, int64_t bytesWritten, int64_t totalBytesWritten, int64_t totalBytesExpectedToWrite))block;

/**
 设置下载任务恢复时要执行的块，由`NSURLSessionDownloadDelegate`方法`URLSession：downloadTask：didResumeAtOffset：expectedTotalBytes：`处理。
 
   @param block恢复下载任务时要执行的块对象。 该块没有返回值，并且有四个参数：会话，下载任务，恢复下载的文件偏移量以及预期要下载的总字节数。
 */
- (void)setDownloadTaskDidResumeBlock:(nullable void (^)(NSURLSession *session, NSURLSessionDownloadTask *downloadTask, int64_t fileOffset, int64_t expectedTotalBytes))block;

@end

///--------------------
/// @name Notifications
///--------------------

/**
 任务恢复时发布。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidResumeNotification;

/**
 在任务完成执行时发布。 包含userInfo字典，其中包含有关该任务的其他信息。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidCompleteNotification;

/**
 在任务暂停执行时发布。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidSuspendNotification;

/**
在会话失效时发布。
 */
FOUNDATION_EXPORT NSString * const AFURLSessionDidInvalidateNotification;

/**
在将临时下载文件移动到指定目标时，会话下载任务遇到错误时发布。
 */
FOUNDATION_EXPORT NSString * const AFURLSessionDownloadTaskDidFailToMoveFileNotification;

/**
任务的原始响应数据。 如果任务的响应数据存在，则包含在`AFNetworkingTaskDidCompleteNotification`的userInfo字典中。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidCompleteResponseDataKey;

/**
任务的序列化响应对象。 如果响应被序列化，则包含在`AFNetworkingTaskDidCompleteNotification`的userInfo字典中。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidCompleteSerializedResponseKey;

/**
 响应序列化程序用于序列化响应。 如果任务具有关联的响应序列化程序，则包含在“AFNetworkingTaskDidCompleteNotification”的userInfo字典中。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidCompleteResponseSerializerKey;

/**
 与下载任务关联的文件路径。 如果响应数据已直接存储到磁盘，则包含在`AFNetworkingTaskDidCompleteNotification`的userInfo字典中。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidCompleteAssetPathKey;

/**
与任务相关的任何错误，或响应的序列化。 如果存在错误，则包含在`AFNetworkingTaskDidCompleteNotification`的userInfo字典中。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidCompleteErrorKey;

NS_ASSUME_NONNULL_END

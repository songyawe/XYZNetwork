// AFHTTPSessionManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>
#if !TARGET_OS_WATCH
#import <SystemConfiguration/SystemConfiguration.h>
#endif
#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_WATCH || TARGET_OS_TV
#import <MobileCoreServices/MobileCoreServices.h>
#else
#import <CoreServices/CoreServices.h>
#endif

#import "AFURLSessionManager.h"

/**
 `AFHTTPSessionManager`是`AFURLSessionManager`的子类，带有用于发出HTTP请求的便捷方法。当提供`baseURL`时，使用`GET` /`POST` / et al进行请求。便利方法可以用相对路径进行。
 
  改变所有数据任务操作构造的行为，这也用于`GET` /`POST` / et al。方便的方法，覆盖`dataTaskWithRequest：uploadProgress：downloadProgress：completionHandler：`。
 
  HTTP客户端创建的请求将包含默认头和根据`requestSerializer`属性编码参数，该属性是符合`<AFURLRequestSerialization>`的对象。
 
  从服务器收到的响应由`responseSerializers`属性自动验证和序列化，该属性是符合`<AFURLResponseSerialization>的对象。
 
  对于HTTP便捷方法，请求序列化程序使用`NSURL + URLWithString：relativeToURL：`来提供相对于`-baseURL`的路径构造URL。如果`baseURL`是`nil`，`path`需要使用`NSURL + URLWithString：`解析为有效的`NSURL`对象。
 
 另外需要注意的是，任何`baseURL`都会添加一个尾部斜杠。当使用没有前导斜杠的路径构造URL时，这会导致意外行为。
 */

NS_ASSUME_NONNULL_BEGIN

@interface AFHTTPSessionManager : AFURLSessionManager <NSSecureCoding, NSCopying>

/**
根路径
 */
@property (readonly, nonatomic, strong, nullable) NSURL *baseURL;

/**
默认情况下为`AFHTTPRequestSerializer`的实例，它为`GET`，`HEAD`和`DELETE`请求序列化查询字符串参数，或以其他方式对HTTP消息体进行URL格式编码。
 */
@property (nonatomic, strong) AFHTTPRequestSerializer <AFURLRequestSerialization> * requestSerializer;

/**
默认情况下，此属性设置为`AFJSONResponseSerializer`的实例。
 */
@property (nonatomic, strong) AFHTTPResponseSerializer <AFURLResponseSerialization> * responseSerializer;

///-------------------------------
/// @name Managing Security Policy
///-------------------------------

/**
安全策略。 除非另有说明，否则`AFURLSessionManager`使用`defaultPolicy`。
 */
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

///---------------------
/// @name Initialization
///---------------------

/**
创建实例
 */
+ (instancetype)manager;

/**
初始化实例
 */
- (instancetype)initWithBaseURL:(nullable NSURL *)url;

/**
 使用指定的基本URL初始化`AFHTTPSessionManager`对象。
 
   这是指定的初始化程序。
 
   @param url HTTP客户端的基本URL。
   @param configuration 用于创建托管会话的配置。
 
   @return新初始化的HTTP客户端
 */
- (instancetype)initWithBaseURL:(nullable NSURL *)url
           sessionConfiguration:(nullable NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

///---------------------------
/// @name Making HTTP Requests
///---------------------------

/**
 使用`GET`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)GET:(NSString *)URLString
                   parameters:(nullable id)parameters
                      success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                      failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;


/**
 使用`GET`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param downloadProgress 更新下载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。

 */
- (nullable NSURLSessionDataTask *)GET:(NSString *)URLString
                            parameters:(nullable id)parameters
                              progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgress
                               success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                               failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 使用`HEAD`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，只接受一个参数：数据任务。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)HEAD:(NSString *)URLString
                    parameters:(nullable id)parameters
                       success:(nullable void (^)(NSURLSessionDataTask *task))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 使用`POST`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                    parameters:(nullable id)parameters
                       success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;

/**
 使用`POST`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param uploadProgress 更新上载进度时要执行的块对象。 请注意，此块在会话队列上调用，而不是在主队列中调用。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                             parameters:(nullable id)parameters
                               progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgress
                                success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                                failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 使用多部分`POST`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param block 一个块，它接受一个参数并将数据附加到HTTP主体。 block参数是采用`AFMultipartFormData`协议的对象。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                    parameters:(nullable id)parameters
     constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                       success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;

/**
 使用多部分`POST`请求​​创建并运行`NSURLSessionDataTask`。
 
  @param URLString 用于创建请求URL的URL字符串。
  @param parameters 参数根据客户端请求序列化程序编码的参数。
  @param block 一个块，它接受一个参数并将数据附加到HTTP主体。 block参数是采用`AFMultipartFormData`协议的对象。
  @param uploadProgress 更新上载进度时要执行的块对象。请注意，此块在会话队列上调用，而不是在主队列中调用。
  @param success 任务成功完成时要执行的块对象。此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
  @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                             parameters:(nullable id)parameters
              constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                               progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgress
                                success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                                failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 使用`PUT`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)PUT:(NSString *)URLString
                   parameters:(nullable id)parameters
                      success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                      failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 使用`PATCH`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)PATCH:(NSString *)URLString
                     parameters:(nullable id)parameters
                        success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                        failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 使用`DELETE`请求创建并运行`NSURLSessionDataTask`。
 
   @param URLString 用于创建请求URL的URL字符串。
   @param parameters 参数根据客户端请求序列化程序编码的参数。
   @param success 任务成功完成时要执行的块对象。 此块没有返回值，并且有两个参数：数据任务和客户端响应序列化程序创建的响应对象。
   @param failure 当任务完成失败或成功完成但在解析响应数据时遇到错误时要执行的块对象。 此块没有返回值，并采用两个参数：数据任务和描述网络的错误或发生的解析错误。
 */
- (nullable NSURLSessionDataTask *)DELETE:(NSString *)URLString
                      parameters:(nullable id)parameters
                         success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                         failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

@end

NS_ASSUME_NONNULL_END

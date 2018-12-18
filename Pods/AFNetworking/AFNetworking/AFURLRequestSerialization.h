// AFURLRequestSerialization.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>
#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV
#import <UIKit/UIKit.h>
#elif TARGET_OS_WATCH
#import <WatchKit/WatchKit.h>
#endif

NS_ASSUME_NONNULL_BEGIN

/**
 根据RFC 3986的规定：URL百分比编码的保留字段分为：
  1.   ':'  '#'  '['  ']'  '@'  '?'  '/'
  2.   '!'  '$'  '&'  '''  '('  ')'  '*'  '+'  ','  ';' '='
  在对查询字段百分比编码时，'?'和'/'可以不用编码，其他的都要进行编码。
 1. 字符串需要经过过滤 ，过滤法则通过 NSMutableCharacterSet 实现。添加规则后，只对规则内的因子进行编码。
 2. 为了处理类似emoji这样的字符串，rangeOfComposedCharacterSequencesForRange 使用了while循环来处理，也就是把字符串按照batchSize分割处理完再拼回。
 */
FOUNDATION_EXPORT NSString * AFPercentEscapedStringFromString(NSString *string);

/**
 该方法最终把类型为NSDictionary的参数处理为字符串类型。
 举个简单的没进行百分百编码的例子：
 如果参数是NSDictionary *info = @{@"name":@"zhangsan",@"age":20} ;
 AFQueryStringFromParameters(info)  的结果就是：name=zhangsan&age=20 (没有百分比编码)
 */
FOUNDATION_EXPORT NSString * AFQueryStringFromParameters(NSDictionary *parameters);

/**

 */
@protocol AFURLRequestSerialization <NSObject, NSSecureCoding, NSCopying>

/**

 */
- (nullable NSURLRequest *)requestBySerializingRequest:(NSURLRequest *)request
                               withParameters:(nullable id)parameters
                                        error:(NSError * _Nullable __autoreleasing *)error NS_SWIFT_NOTHROW;

@end

#pragma mark -

/**

 */
typedef NS_ENUM(NSUInteger, AFHTTPRequestQueryStringSerializationStyle) {
    AFHTTPRequestQueryStringDefaultStyle = 0,
};

@protocol AFMultipartFormData;

/**

 */
@interface AFHTTPRequestSerializer : NSObject <AFURLRequestSerialization>

/**
字符串编码，默认UTF8
 */
@property (nonatomic, assign) NSStringEncoding stringEncoding;

/**
是否允许访问蜂窝网络
 */
@property (nonatomic, assign) BOOL allowsCellularAccess;

/**
缓存策略
 NSURLRequestUseProtocolCachePolicy      这个是默认的缓存策略，缓存不存在，就请求服务器，缓存存在，会根据response中的Cache-Control字段判断下一步操作，如: Cache-Control字段为must-revalidata, 则询问服务端该数据是否有更新，无更新的话直接返回给用户缓存数据，若已更新，则请求服务端。
 NSURLRequestReloadIgnoringLocalCacheData   这个策略是不管有没有本地缓存，都请求服务器。
 NSURLRequestReloadIgnoringLocalAndRemoteCacheData   这个策略会忽略本地缓存和中间代理 直接访问源server
 NSURLRequestReturnCacheDataElseLoad    这个策略指，有缓存就是用，不管其有效性，即Cache-Control字段 ，没有就访问源server
 NSURLRequestReturnCacheDataDontLoad   这个策略只加载本地数据，不做其他操作，适用于没有网路的情况
 NSURLRequestReloadRevalidatingCacheData  这个策略标示缓存数据必须得到服务器确认才能使用，未实现。
 */
@property (nonatomic, assign) NSURLRequestCachePolicy cachePolicy;

/**

 */
@property (nonatomic, assign) BOOL HTTPShouldHandleCookies;

/**
 是否使用管线
在HTTP连接中，一般都是一个请求对应一个连接，每次简历tcp连接是需要一定时间的。管线化，允许一次发送一组请求而不必等到相应。但由于目前并不是所有的服务器都支持这项功能，因此这个属性默认是不开启的。管线化使用同一tcp连接完成任务，因此能够大大提交请求的时间。但是响应要和请求的顺序 保持一致才行。使用场景也有，比如说首页要发送很多请求，可以考虑这种技术。但前提是建立连接成功后才可以使用。
 */
@property (nonatomic, assign) BOOL HTTPShouldUsePipelining;

/**
网络服务类型
 可以通过这个值来指定当前的网络类型，系统会跟据制定的网络类型对很多方面进行优化，这个就设计到很细微的编程技巧了，可作为一个优化的点备用。
 */
@property (nonatomic, assign) NSURLRequestNetworkServiceType networkServiceType;

/**
超时默认60s
 */
@property (nonatomic, assign) NSTimeInterval timeoutInterval;

///---------------------------------------
/// @name Configuring HTTP Request Headers
///---------------------------------------

/**
HTTPHeader 信息
 */
@property (readonly, nonatomic, strong) NSDictionary <NSString *, NSString *> *HTTPRequestHeaders;

/**
 返回默认配置实例化
 */
+ (instancetype)serializer;

/**
设置HTTP header 当value为nil 就移除field
 */
- (void)setValue:(nullable NSString *)value
forHTTPHeaderField:(NSString *)field;

/**
返回field标记的内容
 */
- (nullable NSString *)valueForHTTPHeaderField:(NSString *)field;

/**
设置Authorization字段
 下边这两个方法Authorization 这个词有关，上边的那个方法是根据用户名和密码 生成一个 Authorization 和值，拼接到请求头中规则是这样的
 */
- (void)setAuthorizationHeaderFieldWithUsername:(NSString *)username
                                       password:(NSString *)password;

/**
清空
 */
- (void)clearAuthorizationHeader;

///-------------------------------------------------------
/// @name Configuring Query String Parameter Serialization
///-------------------------------------------------------

/**

 */
@property (nonatomic, strong) NSSet <NSString *> *HTTPMethodsEncodingParametersInURI;

/**

 */
- (void)setQueryStringSerializationWithStyle:(AFHTTPRequestQueryStringSerializationStyle)style;

/**

 */
- (void)setQueryStringSerializationWithBlock:(nullable NSString * (^)(NSURLRequest *request, id parameters, NSError * __autoreleasing *error))block;

///-------------------------------
/// @name Creating Request Objects
///-------------------------------

/**

 */
- (NSMutableURLRequest *)requestWithMethod:(NSString *)method
                                 URLString:(NSString *)URLString
                                parameters:(nullable id)parameters
                                     error:(NSError * _Nullable __autoreleasing *)error;

/**
 
 */
- (NSMutableURLRequest *)multipartFormRequestWithMethod:(NSString *)method
                                              URLString:(NSString *)URLString
                                             parameters:(nullable NSDictionary <NSString *, id> *)parameters
                              constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                                                  error:(NSError * _Nullable __autoreleasing *)error;

/**
 
 */
- (NSMutableURLRequest *)requestWithMultipartFormRequest:(NSURLRequest *)request
                             writingStreamContentsToFile:(NSURL *)fileURL
                                       completionHandler:(nullable void (^)(NSError * _Nullable error))handler;

@end

#pragma mark -

/**
 
 */
@protocol AFMultipartFormData

/**

 */
- (BOOL)appendPartWithFileURL:(NSURL *)fileURL
                         name:(NSString *)name
                        error:(NSError * _Nullable __autoreleasing *)error;

/**
 
 */
- (BOOL)appendPartWithFileURL:(NSURL *)fileURL
                         name:(NSString *)name
                     fileName:(NSString *)fileName
                     mimeType:(NSString *)mimeType
                        error:(NSError * _Nullable __autoreleasing *)error;

/**
 
 */
- (void)appendPartWithInputStream:(nullable NSInputStream *)inputStream
                             name:(NSString *)name
                         fileName:(NSString *)fileName
                           length:(int64_t)length
                         mimeType:(NSString *)mimeType;

/**

 */
- (void)appendPartWithFileData:(NSData *)data
                          name:(NSString *)name
                      fileName:(NSString *)fileName
                      mimeType:(NSString *)mimeType;

/**
 
 */

- (void)appendPartWithFormData:(NSData *)data
                          name:(NSString *)name;


/**
 
 */
- (void)appendPartWithHeaders:(nullable NSDictionary <NSString *, NSString *> *)headers
                         body:(NSData *)body;

/**
 
 */
- (void)throttleBandwidthWithPacketSize:(NSUInteger)numberOfBytes
                                  delay:(NSTimeInterval)delay;

@end

#pragma mark -

/**
 
 */
@interface AFJSONRequestSerializer : AFHTTPRequestSerializer

/**
 
 */
@property (nonatomic, assign) NSJSONWritingOptions writingOptions;

/**
 
 */
+ (instancetype)serializerWithWritingOptions:(NSJSONWritingOptions)writingOptions;

@end

#pragma mark -

/**
 
 */
@interface AFPropertyListRequestSerializer : AFHTTPRequestSerializer

/**
 
 */
@property (nonatomic, assign) NSPropertyListFormat format;

/**

 */
@property (nonatomic, assign) NSPropertyListWriteOptions writeOptions;

/**
 
 */
+ (instancetype)serializerWithFormat:(NSPropertyListFormat)format
                        writeOptions:(NSPropertyListWriteOptions)writeOptions;

@end

#pragma mark -

///----------------
/// @name Constants
///----------------

/**
 
 */
FOUNDATION_EXPORT NSString * const AFURLRequestSerializationErrorDomain;

/**

 */
FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLRequestErrorKey;

/**
 
 */
FOUNDATION_EXPORT NSUInteger const kAFUploadStream3GSuggestedPacketSize;
FOUNDATION_EXPORT NSTimeInterval const kAFUploadStream3GSuggestedDelay;

NS_ASSUME_NONNULL_END

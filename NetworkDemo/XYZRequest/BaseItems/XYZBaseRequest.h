//
//  XYZBaseRequest.h
//  网络请求基类
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


// HTTP 请求方式
typedef NS_ENUM(NSInteger, XYZRequestType) {
    XYZRequestTypeGET = 0,
    XYZRequestTypePOST,
    XYZRequestTypeHEAD,
    XYZRequestTypePUT,
    XYZRequestTypeDELETE,
    XYZRequestTypePATCH,
};

// 请求数据序列化的方式 HTTP 还是 JSON
typedef NS_ENUM(NSInteger, XYZRequestSerializerType) {
    XYZRequestSerializerTypeHTTP = 0,
    XYZRequestSerializerTypeJSON,
};

// 返回数据的序列化方式，决定了 responseObject 的数据类型
typedef NS_ENUM(NSInteger, XYZResponseSerializerType) {
    XYZResponseSerializerTypeHTTP,
    XYZResponseSerializerTypeJSON,
    XYZResponseSerializerTypeXMLParser,
};

// 请求的优先级
typedef NS_ENUM(NSInteger, XYZRequestPriority) {
    XYZRequestPriorityLow = -4L,
    XYZRequestPriorityDefault = 0,
    XYZRequestPriorityHigh = 4,
};

@protocol AFMultipartFormData;

typedef void (^AFConstructingBlock)(id<AFMultipartFormData> formData);
typedef void (^AFURLSessionTaskProgressBlock)(NSProgress *);

@class XYZBaseRequest;
typedef void(^XYZRequestCompletionBlock)(__kindof XYZBaseRequest *request);

//网络请求的抽象基类，它提供了许多选项用于构建请求
@interface XYZBaseRequest : NSObject


#pragma mark - 参数相关
//基地址,设置后此请求会忽略全局的基地址
@property (nonatomic,copy,readwrite, nullable)NSString *baseUrl;

//请求路径
@property (nonatomic,copy,readwrite, nullable)NSString *requestUrl;

// 设置请求超时时间，默认 60 秒.
@property(nonatomic, readwrite) NSTimeInterval requestTimeoutInterval;

// 设置请求的参数
@property(nonatomic, readwrite, nullable) id requestParames;

// 设置 HTTP 请求方式
@property(nonatomic, readwrite) XYZRequestType requestMethod;

// 设置请求数据序列化的方式
@property(nonatomic, readwrite) XYZRequestSerializerType requestSerializerType;

// 设置返回数据序列化的方式.
@property(nonatomic, readwrite) XYZResponseSerializerType responseSerializerType;

// 是否允许请求使用蜂窝网络，默认是允许
@property (nonatomic) BOOL allowsCellularAccess;

// tag 可以用来标识请求，默认是0
@property (nonatomic) NSInteger tag;

// userInfo 可以用来存储请求的附加信息，默认是 nil
@property (nonatomic, copy, nullable) NSDictionary *userInfo;

// 请求成功的回调
@property (nonatomic, copy, nullable) XYZRequestCompletionBlock successCompletionBlock;

// 请求失败的回调
@property (nonatomic, copy, nullable) XYZRequestCompletionBlock failureCompletionBlock;

// 可以用于在 POST 请求中需要时构造 HTTP body，默认是 nil
@property (nonatomic, copy, nullable) AFConstructingBlock constructingBodyBlock;

// 设置断点续传下载请求的地址，默认是 nil
// 如果带文件名，文件名需要有后缀，否则会被认为是文件夹。需要服务器支持
@property (nonatomic, copy, nullable) NSString *downloadPath;

// 捕获下载进度
@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock downloadProgressBlock;

// 设置请求优先级
@property (nonatomic) XYZRequestPriority requestPriority;

// 设置请求完成回调 block
- (void)setCompletionBlockWithSuccess:(nullable XYZRequestCompletionBlock)success
                              failure:(nullable XYZRequestCompletionBlock)failure;

// 清除请求回调 block
- (void)clearCompletionBlock;


#pragma mark - 请求类相关方法

// 将当前 self 网络请求加入请求队列，并且开始请求
- (void)start;

// 从请求队列中移除 self 网络请求，并且取消请求
- (void)stop;

// 使用带有成功失败 `block` 回调的方法开始请求(储存 block，调用 start)
- (void)sendRequestWithSuccess:(nullable XYZRequestCompletionBlock)success
                                    failure:(nullable XYZRequestCompletionBlock)failure;

#pragma mark - 子类中用到需要覆盖
//一下方法根据需求在子类中自行覆盖
// 请求成功后，在切换到主线程之前，在后台线程上调用。要注意，如果加载了缓存，则将在主线程上调用此方法。
- (void)requestCompletePreprocessor;

// 请求成功时会在主线程被调用
- (void)requestCompleteFilter;

// 请求成功后，在切换到主线程之前，在后台线程上调用。
- (void)requestFailedPreprocessor;

// 请求失败时会在主线程被调用
- (void)requestFailedFilter;

// 重写这个方法可以在缓存时过滤请求中的某些参数
- (id)cacheFileNameFilterForParames:(id)parames;

// 用来 HTTP 授权的用户名和密码，应该返回@[@"Username", @"Password"]这种格式
- (nullable NSArray<NSString *> *)requestAuthorizationHeaderFieldArray;

// 附加的 HTTP 请求头
- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;

// 用来创建完全自定义的请求，返回一个 NSURLRequest，其他参数将无效
- (nullable NSURLRequest *)buildCustomUrlRequest;

// 验证 responseStatusCode 是否是有效的，默认是 code 在200-300之间是有效的
- (BOOL)statusCodeValidator;

#pragma mark - 请求和相应信息
// NSURLSessionTask 底层相关的
@property (nonatomic, strong, readwrite) NSURLSessionTask *requestTask;
@property (nonatomic, strong, readonly) NSURLRequest *currentRequest;
@property (nonatomic, strong, readonly) NSURLRequest *originalRequest;
@property (nonatomic, strong, readonly) NSHTTPURLResponse *response;
@property (nonatomic, readonly) NSInteger responseStatusCode;
@property (nonatomic, copy, readonly, nullable) NSDictionary *responseHeaders;

// 响应的数据表现形式，请求失败则是 nil
@property (nonatomic, strong, readwrite, nullable) NSData *responseData;

// 响应的字符串表现形式，请求失败则是 nil
@property (nonatomic, copy, readwrite, nullable) NSString *responseString;

@property (nonatomic, strong, readwrite, nullable) id responseObject;

// 如果设置响应序列化方式是 XYZResponseSerializerTypeJSON，这个就是响应结果序列化后的对象
@property (nonatomic, strong, readwrite, nullable) id responseJSONObject;

// 请求序列化错误或者网络错误，默认是 nil
@property (nonatomic, strong, readwrite, nullable) NSError *error;

// 请求任务是否已经取消
@property (nonatomic, readonly, getter=isCancelled) BOOL cancelled;

// 请求任务是否在执行
@property (nonatomic, readonly, getter=isExecuting) BOOL executing;


@end

NS_ASSUME_NONNULL_END

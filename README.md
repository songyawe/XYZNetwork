# XYZNetwork
一个很实用的网络请求，可以断点下载和上传，网络请求方式可以使用缓存和网络缓存并用的方式。有Demo可以参考


###XYZBaseRequest 网络请求基类
```

// tag 可以用来标识请求，默认是0
@property (nonatomic) NSInteger tag;

// userInfo 可以用来存储请求的附加信息，默认是 nil
@property (nonatomic, strong, nullable) NSDictionary *userInfo;

// 请求成功的回调
@property (nonatomic, copy, nullable) XYZRequestCompletionBlock successCompletionBlock;

// 请求失败的回调
@property (nonatomic, copy, nullable) XYZRequestCompletionBlock failureCompletionBlock;

// 可以用于在 POST 请求中需要时构造 HTTP body，默认是 nil
@property (nonatomic, copy, nullable) AFConstructingBlock constructingBodyBlock;

// 设置断点续传下载请求的地址，默认是 nil
// 如果带文件名，文件名需要有后缀，否则会被认为是文件夹。需要服务器支持
@property (nonatomic, strong, nullable) NSString *downloadPath;

// 捕获下载进度，也可以看看 downloadPath
@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock downloadProgressBlock;

// 设置请求优先级
@property (nonatomic) XYZRequestPriority requestPriority;

// 设置请求完成回调 block
- (void)setCompletionBlockWithSuccess:(nullable XYZRequestCompletionBlock)success
                              failure:(nullable XYZRequestCompletionBlock)failure;

// 清除请求回调 block
- (void)clearCompletionBlock;


#pragma mark - Request Action

// 将当前 self 网络请求加入请求队列，并且开始请求
- (void)start;

// 从请求队列中移除 self 网络请求，并且取消请求
- (void)stop;

// 使用带有成功失败 `block` 回调的方法开始请求(储存 block，调用 start)
- (void)sendRequestWithSuccess:(nullable XYZRequestCompletionBlock)success
                                    failure:(nullable XYZRequestCompletionBlock)failure;


#pragma mark - Subclass Override
//一下方法根据需求在子类中自行覆盖
// 请求成功后，在切换到主线程之前，在后台线程上调用。要注意，如果加载了缓存，则将在主线程上调用此方法。
- (void)requestCompletePreprocessor;

// 请求成功时会在主线程被调用
- (void)requestCompleteFilter;

// 请求成功后，在切换到主线程之前，在后台线程上调用。
- (void)requestFailedPreprocessor;

// 请求失败时会在主线程被调用
- (void)requestFailedFilter;

//基地址
- (NSString *)baseUrl;

//请求路径
- (NSString *)requestUrl;

// 设置请求超时时间，默认 60 秒.
- (NSTimeInterval)requestTimeoutInterval;

// 设置请求的参数
- (nullable id)parames;

// 重写这个方法可以在缓存时过滤请求中的某些参数
- (id)cacheFileNameFilterForParames:(id)parames;

// 设置 HTTP 请求方式
- (XYZRequestType)requestMethod;

// 设置请求数据序列化的方式
- (XYZRequestSerializerType)requestSerializerType;

// 设置返回数据序列化的方式.
- (XYZResponseSerializerType)responseSerializerType;

// 用来 HTTP 授权的用户名和密码，应该返回@[@"Username", @"Password"]这种格式
- (nullable NSArray<NSString *> *)requestAuthorizationHeaderFieldArray;

// 附加的 HTTP 请求头
- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;

// 用来创建完全自定义的请求，返回一个 NSURLRequest，其他参数将无效
- (nullable NSURLRequest *)buildCustomUrlRequest;

// 是否允许请求使用蜂窝网络，默认是允许
- (BOOL)allowsCellularAccess;

// 验证 responseStatusCode 是否是有效的，默认是 code 在200-300之间是有效的
- (BOOL)statusCodeValidator;
```

###XYZNetworkManager 管理类
```

/// 添加请求
- (void)addRequest:(XYZBaseRequest *)request;

///  取消请求
- (void)cancelRequest:(XYZBaseRequest *)request;

///  取消所有未完成的请求
- (void)cancelAllRequests;

///  返回链接URL
- (NSString *)buildRequestUrl:(XYZBaseRequest *)request;
```

###XYZCacheRequest 带缓存网络请求基类
```
typedef NS_ENUM(NSInteger, XYZCacheType) {
    XYZCacheTypeNET = 0,      //只用网络数据
    XYZCacheTypeCACHE,        //有缓存用缓存，没有缓存再请求
    XYZCacheTypeCACHEANDNET   //先加载缓存，等网络请求成功再加载网络数据
};


//数据是否来自缓存
- (BOOL)isDataFromCache;

//缓存是否加载成功
- (BOOL)loadCacheWithError:(NSError * __autoreleasing *)error;

//不加载缓存直接请求网络
- (void)startWithoutCache;

//选择加载数据的方式
- (XYZCacheType)useCacheType;

//缓存需要保存的数据
- (void)saveResponseDataToCacheFile:(NSData *)data;

#pragma mark - Subclass Override

//缓存时间（秒），默认7天，在加载方式不为XYZCacheTypeNET有效，设置大于0的数值有效
- (NSInteger)cacheTimeInSeconds;

//缓存版本默认为0
- (long long)cacheVersion;

//是否异步写入缓存
- (BOOL)writeCacheAsynchronously;
//缓存基路径
- (NSString *)cacheBasePath;

```

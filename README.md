# XYZNetwork
XYZNetwork是基于AFNetworking封装实现的一个很实用的网络请求框架，包括了现实生产中实际用到的各种场景需求，可以断点下载和上传，网络请求方式可以使用缓存和网络缓存并用的方式。使用方便，使用时只需要将项目里的XYZRequest文件夹放入自己项目中即可使用。有Demo可以参考
###使用Demo
```

//普通网络请求
- (void)login {

XYZCommonRequest *request = [XYZCommonRequest XYZ_requestWithUrl:@"/app/auth/signin" requestType:XYZRequestTypePOST];
request.requestParames = @{
@"client":@"primary",
@"code":@"1614",
@"mobile":@"15001389806"
};
[request sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
ResponseModel* model = [ResponseModel mj_objectWithKeyValues:request.responseJSONObject];
NSLog(@"请求成功！");

} failure:^(__kindof XYZBaseRequest * _Nonnull request) {
NSLog(@"请求失败！");
}];
}

//带缓存的网络请求
- (void)cacheLogin{

XYZCacheCommonRequest *request = [XYZCacheCommonRequest XYZ_requestWithUrl:@"/app/auth/signin" requestType:XYZRequestTypePOST useCacheType:XYZCacheTypeCACHEANDNET];
request.requestParames = @{
@"client":@"primary",
@"code":@"1614",
@"mobile":@"15001389806"
};
[request sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
ResponseModel* model = [ResponseModel mj_objectWithKeyValues:request.responseJSONObject];
NSLog(@"请求成功！");

} failure:^(__kindof XYZBaseRequest * _Nonnull request) {
NSLog(@"请求失败！");
}];
}

//一组相互依赖的网络请求
- (void)group{
NSMutableArray *group = [NSMutableArray array];
for (int i = 0; i < 10; i ++) {
XYZCommonRequest *request = [XYZCommonRequest XYZ_requestWithUrl:@"/app/auth/signin" requestType:XYZRequestTypePOST];
request.tag = i;
request.requestParames = @{
@"client":@"primary",
@"code":@"1614",
@"mobile":@"15001389806"
};
[group addObject:request];
}

XYZGroupRequest *groupRequest = [[XYZGroupRequest alloc]initWithRequestArray:group];

[groupRequest sendRequestWithProcess:^(XYZBaseRequest * _Nonnull processRequest , XYZGroupRequest *groupRequest) {
NSLog(@"------本次成功请求的编号:---%ld--------",(long)processRequest.tag);
NSLog(@"-----完成了第几个:----%ld--------",(long)groupRequest.finishedCount);
} success:^(XYZGroupRequest * _Nonnull groupRequest) {
NSLog(@"---------请求组成功完成任务,进行其他任务--------");

} failure:^(XYZGroupRequest * _Nonnull groupRequest) {
NSLog(@"---------失败,提示用户--------");
}];

}

//断点下载

- (void)downloadAction{
[self.btn0 setEnabled:NO];
__weak typeof(self)weakSelf = self;
XYZDownloadRequest *download = [[XYZDownloadRequest alloc]init];
download.downloadProgressBlock = ^(NSProgress * _Nonnull progress) {
NSLog(@"--------%.2f------",progress.fractionCompleted);
dispatch_async(dispatch_get_main_queue(), ^{
[weakSelf.btn0 setTitle:[NSString stringWithFormat:@"下载进度:%.2f",progress.fractionCompleted] forState:(UIControlStateNormal)];
});

};
[download sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
NSLog(@"下载成功！");
[weakSelf.btn0 setEnabled:YES];
[weakSelf.btn0 setTitle:@"下载成功，重新下载" forState:(UIControlStateNormal)];
} failure:^(__kindof XYZBaseRequest * _Nonnull request) {
[weakSelf.btn0 setEnabled:YES];
NSLog(@"下载失败！");
}];
_download = download;
}
```

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

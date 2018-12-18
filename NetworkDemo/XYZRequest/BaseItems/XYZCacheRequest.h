//
//  XYZCacheRequest.h
//  带缓存的网络请求基类
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZBaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, XYZCacheType) {
    XYZCacheTypeNET = 0,      //只用网络数据
    XYZCacheTypeCACHE,        //有缓存用缓存，没有缓存再请求
    XYZCacheTypeCACHEANDNET   //先加载缓存，等网络请求成功再加载网络数据
};

FOUNDATION_EXPORT NSString *const XYZCacheRequestCacheErrorDomain;

NS_ENUM(NSInteger) {
    XYZCacheRequestCacheErrorExpired = -1,             //缓存失效
    XYZCacheRequestCacheErrorVersionMismatch = -2,     // 缓存版本验证失败
    XYZCacheRequestCacheErrorAppVersionMismatch = -3,  // 缓存app版本验证失败
    XYZCacheRequestCacheErrorInvalidCacheTime = -4,    // 未设置缓存时间
    XYZCacheRequestCacheErrorInvalidMetadata = -5,     // 加载缓存元数据失败，没有缓存数据
    XYZCacheRequestCacheErrorInvalidCacheData = -6,    // 加载缓存数据失败，没有缓存数据
};

//带缓存的网络请求
@interface XYZCacheRequest : XYZBaseRequest
#pragma mark - 参数相关

//选择加载数据的方式
@property(nonatomic, readwrite) XYZCacheType useCacheType;
//缓存版本默认为0
@property(nonatomic, readwrite) long cacheVersion;
//缓存时间（秒），默认7天，在加载方式不为XYZCacheTypeNET有效，设置大于0的数值有效
@property(nonatomic, readwrite) NSInteger cacheTimeInSeconds;
//是否异步写入缓存,默认是异步
@property(nonatomic, readwrite, getter = isWriteCacheAsynchronously) BOOL writeCacheAsynchronously;

#pragma mark - 方法相关


//数据是否来自缓存
- (BOOL)isDataFromCache;

//缓存是否加载成功
- (BOOL)loadCacheWithError:(NSError * __autoreleasing *)error;

//不加载缓存直接请求网络
- (void)startWithoutCache;

//缓存需要保存的数据
- (void)saveResponseDataToCacheFile:(NSData *)data;

//缓存基路径
- (NSString *)cacheBasePath;

@end

NS_ASSUME_NONNULL_END

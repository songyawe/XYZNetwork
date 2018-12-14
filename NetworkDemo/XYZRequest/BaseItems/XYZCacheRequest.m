//
//  XYZCacheRequest.m
//  带缓存的网络请求基类
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZCacheRequest.h"
#import "XYZNetworkConfig.h"
#import "XYZNetworkUtils.h"

#ifndef NSFoundationVersionNumber_iOS_8_0
#define NSFoundationVersionNumber_With_QoS_Available 1140.11
#else
#define NSFoundationVersionNumber_With_QoS_Available NSFoundationVersionNumber_iOS_8_0
#endif

NSString *const XYZCacheRequestCacheErrorDomain = @"kXYZCacheRequestCacheErrorDomain";

static dispatch_queue_t XYZCacheRequest_cache_writing_queue() {
    static dispatch_queue_t queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        dispatch_queue_attr_t attr = DISPATCH_QUEUE_SERIAL;
        if (NSFoundationVersionNumber >= NSFoundationVersionNumber_With_QoS_Available) {
            attr = dispatch_queue_attr_make_with_qos_class(attr, QOS_CLASS_BACKGROUND, 0);
        }
        queue = dispatch_queue_create("queue_XYZ_cache", attr);
    });

    return queue;
}

@interface XYZCacheMetadata : NSObject<NSSecureCoding>

@property (nonatomic, assign) long long version;
@property (nonatomic, assign) NSStringEncoding stringEncoding;
@property (nonatomic, strong) NSDate *creationDate;
@property (nonatomic, copy) NSString *appVersionString;

@end

@implementation XYZCacheMetadata

+ (BOOL)supportsSecureCoding {
    return YES;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
    [aCoder encodeObject:@(self.version) forKey:NSStringFromSelector(@selector(version))];
    [aCoder encodeObject:@(self.stringEncoding) forKey:NSStringFromSelector(@selector(stringEncoding))];
    [aCoder encodeObject:self.creationDate forKey:NSStringFromSelector(@selector(creationDate))];
    [aCoder encodeObject:self.appVersionString forKey:NSStringFromSelector(@selector(appVersionString))];
}

- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [self init];
    if (!self) {
        return nil;
    }

    self.version = [[aDecoder decodeObjectOfClass:[NSNumber class] forKey:NSStringFromSelector(@selector(version))] integerValue];
    self.stringEncoding = [[aDecoder decodeObjectOfClass:[NSNumber class] forKey:NSStringFromSelector(@selector(stringEncoding))] integerValue];
    self.creationDate = [aDecoder decodeObjectOfClass:[NSDate class] forKey:NSStringFromSelector(@selector(creationDate))];
    self.appVersionString = [aDecoder decodeObjectOfClass:[NSString class] forKey:NSStringFromSelector(@selector(appVersionString))];

    return self;
}

@end

@interface XYZCacheRequest()

@property (nonatomic, strong) NSData *cacheData;
@property (nonatomic, copy) NSString *cacheString;
@property (nonatomic, strong) id cacheJSON;
@property (nonatomic, strong) NSXMLParser *cacheXML;

@property (nonatomic, strong) XYZCacheMetadata *cacheMetadata;
@property (nonatomic, assign) BOOL dataFromCache;

@end

@implementation XYZCacheRequest

- (instancetype)init{
    if (self = [super init]) {
        //默认参数
        _useCacheType = XYZCacheTypeNET;
        _writeCacheAsynchronously = YES;
        _cacheTimeInSeconds = 60 * 60 * 24 * 7;
        _cacheVersion = 0;
    }
    return self;
}

- (void)start {
    if (self.useCacheType == XYZCacheTypeNET) {
        [self startWithoutCache];
        return;
    } else if (self.useCacheType == XYZCacheTypeCACHE){

        if (![self loadCacheWithError:nil]) {
            [self startWithoutCache];
            return;
        }
        
        _dataFromCache = YES;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [self requestCompletePreprocessor];
            [self requestCompleteFilter];
            XYZCacheRequest *strongSelf = self;
            if (strongSelf.successCompletionBlock) {
                strongSelf.successCompletionBlock(strongSelf);
            }
            [strongSelf clearCompletionBlock];
        });
    }else if (self.useCacheType == XYZCacheTypeCACHEANDNET){
        BOOL isLoadCache = [self loadCacheWithError:nil];
        if (isLoadCache) {
            _dataFromCache = YES;
            dispatch_async(dispatch_get_main_queue(), ^{
                [self requestCompletePreprocessor];
                [self requestCompleteFilter];
                XYZCacheRequest *strongSelf = self;
                if (strongSelf.successCompletionBlock) {
                    strongSelf.successCompletionBlock(strongSelf);
                }
            });
          }
         [self startWithoutCache];
        }
}

- (void)startWithoutCache {
    [self clearCacheVariables];
    [super start];
}

#pragma mark - Network Request Delegate

- (void)requestCompletePreprocessor {
    [super requestCompletePreprocessor];

    if (self.writeCacheAsynchronously) {
        dispatch_async(XYZCacheRequest_cache_writing_queue(), ^{
            [self saveResponseDataToCacheFile:[super responseData]];
        });
    } else {
        [self saveResponseDataToCacheFile:[super responseData]];
    }
}

#pragma mark -

- (BOOL)isDataFromCache {
    return _dataFromCache;
}

- (NSData *)responseData {
    if (_cacheData) {
        return _cacheData;
    }
    return [super responseData];
}

- (NSString *)responseString {
    if (_cacheString) {
        return _cacheString;
    }
    return [super responseString];
}

- (id)responseJSONObject {
    if (_cacheJSON) {
        return _cacheJSON;
    }
    return [super responseJSONObject];
}

- (id)responseObject {
    if (_cacheJSON) {
        return _cacheJSON;
    }
    if (_cacheXML) {
        return _cacheXML;
    }
    if (_cacheData) {
        return _cacheData;
    }
    return [super responseObject];
}

#pragma mark -

- (BOOL)loadCacheWithError:(NSError * _Nullable __autoreleasing *)error {
    // 确认设置了缓存时间
    if ([self cacheTimeInSeconds] < 0) {
        if (error) {
            *error = [NSError errorWithDomain:XYZCacheRequestCacheErrorDomain code:XYZCacheRequestCacheErrorInvalidCacheTime userInfo:@{ NSLocalizedDescriptionKey:@"Invalid cache time"}];
        }
        return NO;
    }

    // 加载元数据
    if (![self loadCacheMetadata]) {
        if (error) {
            *error = [NSError errorWithDomain:XYZCacheRequestCacheErrorDomain code:XYZCacheRequestCacheErrorInvalidMetadata userInfo:@{ NSLocalizedDescriptionKey:@"Invalid metadata. Cache may not exist"}];
        }
        return NO;
    }

    // 检查缓存是否在有效期
    if (![self validateCacheWithError:error]) {
        return NO;
    }

    if (![self loadCacheData]) {
        if (error) {
            *error = [NSError errorWithDomain:XYZCacheRequestCacheErrorDomain code:XYZCacheRequestCacheErrorInvalidCacheData userInfo:@{ NSLocalizedDescriptionKey:@"Invalid cache data"}];
        }
        return NO;
    }

    return YES;
}

- (BOOL)validateCacheWithError:(NSError * _Nullable __autoreleasing *)error {
    // 时间有效
    NSDate *creationDate = self.cacheMetadata.creationDate;
    NSTimeInterval duration = -[creationDate timeIntervalSinceNow];
    if (duration < 0 || duration > [self cacheTimeInSeconds]) {
        if (error) {
            *error = [NSError errorWithDomain:XYZCacheRequestCacheErrorDomain code:XYZCacheRequestCacheErrorExpired userInfo:@{ NSLocalizedDescriptionKey:@"Cache expired"}];
        }
        return NO;
    }
    // 缓存版本有效
    long long cacheVersionFileContent = self.cacheMetadata.version;
    if (cacheVersionFileContent != [self cacheVersion]) {
        if (error) {
            *error = [NSError errorWithDomain:XYZCacheRequestCacheErrorDomain code:XYZCacheRequestCacheErrorVersionMismatch userInfo:@{ NSLocalizedDescriptionKey:@"Cache version mismatch"}];
        }
        return NO;
    }

    // 软件版本
    NSString *appVersionString = self.cacheMetadata.appVersionString;
    NSString *currentAppVersionString = [XYZNetworkUtils appVersionString];
    if (appVersionString || currentAppVersionString) {
        if (appVersionString.length != currentAppVersionString.length || ![appVersionString isEqualToString:currentAppVersionString]) {
            if (error) {
                *error = [NSError errorWithDomain:XYZCacheRequestCacheErrorDomain code:XYZCacheRequestCacheErrorAppVersionMismatch userInfo:@{ NSLocalizedDescriptionKey:@"App version mismatch"}];
            }
            return NO;
        }
    }
    return YES;
}

- (BOOL)loadCacheMetadata {
    NSString *path = [self cacheMetadataFilePath];
    NSFileManager * fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:path isDirectory:nil]) {
        @try {
            _cacheMetadata = [NSKeyedUnarchiver unarchiveObjectWithFile:path];
            return YES;
        } @catch (NSException *exception) {
            XYZLog(@"Load cache metadata failed, reason = %@", exception.reason);
            return NO;
        }
    }
    return NO;
}

- (BOOL)loadCacheData {
    NSString *path = [self cacheFilePath];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error = nil;

    if ([fileManager fileExistsAtPath:path isDirectory:nil]) {
        NSData *data = [NSData dataWithContentsOfFile:path];
        _cacheData = data;
        _cacheString = [[NSString alloc] initWithData:_cacheData encoding:self.cacheMetadata.stringEncoding];
        switch (self.responseSerializerType) {
            case XYZResponseSerializerTypeHTTP:
                return YES;
            case XYZResponseSerializerTypeJSON:
                _cacheJSON = [NSJSONSerialization JSONObjectWithData:_cacheData options:(NSJSONReadingOptions)0 error:&error];
                return error == nil;
            case XYZResponseSerializerTypeXMLParser:
                _cacheXML = [[NSXMLParser alloc] initWithData:_cacheData];
                return YES;
        }
    }
    return NO;
}

- (void)saveResponseDataToCacheFile:(NSData *)data {
    if ([self cacheTimeInSeconds] > 0 && ![self isDataFromCache]) {
        if (data != nil) {
            @try {
                [data writeToFile:[self cacheFilePath] atomically:YES];

                XYZCacheMetadata *metadata = [[XYZCacheMetadata alloc] init];
                metadata.version = [self cacheVersion];
                metadata.stringEncoding = [XYZNetworkUtils stringEncodingWithRequest:self];
                metadata.creationDate = [NSDate date];
                metadata.appVersionString = [XYZNetworkUtils appVersionString];
                [NSKeyedArchiver archiveRootObject:metadata toFile:[self cacheMetadataFilePath]];
            } @catch (NSException *exception) {
                XYZLog(@"Save cache failed, reason = %@", exception.reason);
            }
        }
    }
}

- (void)clearCacheVariables {
    _cacheData = nil;
    _cacheXML = nil;
    _cacheJSON = nil;
    _cacheString = nil;
    _cacheMetadata = nil;
    _dataFromCache = NO;
}

#pragma mark -

- (void)createDirectoryIfNeeded:(NSString *)path {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDir;
    if (![fileManager fileExistsAtPath:path isDirectory:&isDir]) {
        [self createBaseDirectoryAtPath:path];
    } else {
        if (!isDir) {
            NSError *error = nil;
            [fileManager removeItemAtPath:path error:&error];
            [self createBaseDirectoryAtPath:path];
        }
    }
}

- (void)createBaseDirectoryAtPath:(NSString *)path {
    NSError *error = nil;
    [[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES
                                               attributes:nil error:&error];
    if (error) {
        XYZLog(@"create cache directory failed, error = %@", error);
    } else {
        //禁止icloud备份
        [XYZNetworkUtils addDoNotBackupAttribute:path];
    }
}

- (NSString *)cacheBasePath {
    NSString *pathOfLibrary = [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *path = [pathOfLibrary stringByAppendingPathComponent:@"XYZCacheRequestCache"];

    [self createDirectoryIfNeeded:path];
    return path;
}

- (NSString *)cacheFileName {
    NSString *requestUrl = [self requestUrl];
    NSString *baseUrl = [XYZNetworkConfig sharedConfig].baseUrl;
    id argument = [self cacheFileNameFilterForParames:[self requestParames]];
    NSString *requestInfo = [NSString stringWithFormat:@"Method:%ld Host:%@ Url:%@ Argument:%@",
                             (long)[self requestMethod], baseUrl, requestUrl, argument];
    NSString *cacheFileName = [XYZNetworkUtils md5StringFromString:requestInfo];
    return cacheFileName;
}

- (NSString *)cacheFilePath {
    NSString *cacheFileName = [self cacheFileName];
    NSString *path = [self cacheBasePath];
    path = [path stringByAppendingPathComponent:cacheFileName];
    return path;
}

- (NSString *)cacheMetadataFilePath {
    NSString *cacheMetadataFileName = [NSString stringWithFormat:@"%@.metadata", [self cacheFileName]];
    NSString *path = [self cacheBasePath];
    path = [path stringByAppendingPathComponent:cacheMetadataFileName];
    return path;
}

@end

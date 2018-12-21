// AFAutoPurgingImageCache.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <TargetConditionals.h>
#import <Foundation/Foundation.h>

#if TARGET_OS_IOS || TARGET_OS_TV
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
`AFImageCache`协议定义了一组API，用于同步添加，删除和从缓存中提取图像。
 */
@protocol AFImageCache <NSObject>

/**
 使用给定标识符将图像添加到缓存。
 
   @param image 要缓存的图像。
   @param identifier 缓存中图像的唯一标识符。
 */
- (void)addImage:(UIImage *)image withIdentifier:(NSString *)identifier;

/**
 从与给定标识符匹配的缓存中删除图像。
 
   @param identifier 缓存中图像的唯一标识符。
 
   @return 指示图像是否已从缓存中删除的BOOL。
 */
- (BOOL)removeImageWithIdentifier:(NSString *)identifier;

/**
 从缓存中删除所有图像。
 
   @return 指示是否已从缓存中删除所有图像的BOOL。
 */
- (BOOL)removeAllImages;

/**
 返回与给定标识符关联的缓存中的图像。
 
   @param identifier 缓存中图像的唯一标识符。
 
   @return 匹配标识符的图像，或者为nil。
 */
- (nullable UIImage *)imageWithIdentifier:(NSString *)identifier;
@end


/**
`ImageRequestCache`协议扩展了`ImageCache`协议，添加了一些方法，用于在给定“NSURLRequest”和附加标识符的情况下从缓存中添加，删除和提取图像。
 */
@protocol AFImageRequestCache <AFImageCache>

/**
 询问是否应使用从请求和附加标识符创建的标识符来缓存图像。
  
   @param image 要缓存的图像。
   @param request 标识图像资产的唯一URL请求。
   @param identifier 要应用于URL请求以标识映像的附加标识符。
  
   @return 指示是否应将图像添加到缓存的BOOL。 YES将缓存，NO将阻止缓存。
 */
- (BOOL)shouldCacheImage:(UIImage *)image forRequest:(NSURLRequest *)request withAdditionalIdentifier:(nullable NSString *)identifier;

/**
 使用从请求和附加标识符创建的标识符将映像添加到高速缓存。
 
   @param image 要缓存的图像。
   @param request 标识图像资产的唯一URL请求。
   @param identifier 要应用于URL请求以标识映像的附加标识符。
 */
- (void)addImage:(UIImage *)image forRequest:(NSURLRequest *)request withAdditionalIdentifier:(nullable NSString *)identifier;

/**
 使用从请求和附加标识符创建的标识符从高速缓存中删除映像。
 
   @param request 标识图像资产的唯一URL请求。
   @param identifier 要应用于URL请求以标识映像的附加标识符。
  
   @return 指示是否已从缓存中删除所有图像的BOOL。
 */
- (BOOL)removeImageforRequest:(NSURLRequest *)request withAdditionalIdentifier:(nullable NSString *)identifier;

/**
 返回与从请求和附加标识符创建的标识符关联的缓存中的图像。
 
   @param request 标识图像资产的唯一URL请求。
   @param identifier 要应用于URL请求以标识映像的附加标识符。
 
   @return 匹配请求和标识符的图像，或者为nil。
 */
- (nullable UIImage *)imageforRequest:(NSURLRequest *)request withAdditionalIdentifier:(nullable NSString *)identifier;

@end

/**
内存中图像缓存中的“AutoPurgingImageCache”用于存储高达给定内存容量的图像。 达到内存容量时，图像缓存按上次访问日期排序，然后连续清除最旧的图像，直到满足清除后的首选内存使用量。 每次通过缓存访问图像时，都会更新图像的内部访问日期。
 */
@interface AFAutoPurgingImageCache : NSObject <AFImageRequestCache>

/**
缓存的总内存容量，以字节为单位。
 */
@property (nonatomic, assign) UInt64 memoryCapacity;

/**
 清除后的首选内存使用量（以字节为单位）。 在清除过程中，将清除图像，直到内存容量降至此限制以下。
 */
@property (nonatomic, assign) UInt64 preferredMemoryUsageAfterPurge;

/**
存储在缓存中的当前总内存使用量（以字节为单位）。
 */
@property (nonatomic, assign, readonly) UInt64 memoryUsage;

/**
 初始化`AutoPurgingImageCache`实例，其中包含内存容量的默认值以及清除限制后的首选内存使用情况。 `memoryCapcity`默认为'100 MB`。 `preferredMemoryUsageAfterPurge`默认为`60 MB`。
 
   @return新的`AutoPurgingImageCache`实例。
 */
- (instancetype)init;

/**
 使用给定的内存容量和首选内存使用情况初始化`AutoPurgingImageCache`实例
   清除限制后。
 
   @param memoryCapacity 缓存的总内存容量，以字节为单位。
   @param preferredMemoryCapacity 清除后的首选内存使用量，以字节为单位。
 
   @return新的`AutoPurgingImageCache`实例。
 */
- (instancetype)initWithMemoryCapacity:(UInt64)memoryCapacity preferredMemoryCapacity:(UInt64)preferredMemoryCapacity;

@end

NS_ASSUME_NONNULL_END

#endif


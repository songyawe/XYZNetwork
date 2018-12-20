// AFURLResponseSerialization.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//


#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

/**
这是一个协议，只要遵守这个协议，就要实现NSSecureCoding/NSCopying这两个协议，还要实现
 */
@protocol AFURLResponseSerialization <NSObject, NSSecureCoding, NSCopying>

/**
这个方法来返回序列化后的结果。不管是下边的AFHTTPResponseSerializer，还是它的子类，都遵守这个协议，也就是在各自的实现中实现了这个协议，然后返回了属于自身的一个结果
 */
- (nullable id)responseObjectForResponse:(nullable NSURLResponse *)response
                           data:(nullable NSData *)data
                          error:(NSError * _Nullable __autoreleasing *)error NS_SWIFT_NOTHROW;

@end

#pragma mark -

/**
 `AFHTTPResponseSerializer`符合`AFURLRequestSerialization`和`AFURLResponseSerialization`协议，提供查询字符串/ URL表单编码参数序列化和默认请求标题的具体基本实现，以及响应状态代码和内容类型验证。
 
   鼓励任何处理HTTP的请求或响应序列化程序子类化`AFHTTPResponseSerializer`以确保一致的默认行为。
 */
@interface AFHTTPResponseSerializer : NSObject <AFURLResponseSerialization>

- (instancetype)init;

@property (nonatomic, assign) NSStringEncoding stringEncoding DEPRECATED_MSG_ATTRIBUTE("The string encoding is never used. AFHTTPResponseSerializer only validates status codes and content types but does not try to decode the received data in any way.");

/**
创建并返回具有默认配置的序列化程序。
 */
+ (instancetype)serializer;

///-----------------------------------------
/// @name Configuring Response Serialization
///-----------------------------------------

/**
响应的可接受HTTP状态代码。 当非“nil”时，具有未包含状态代码的响应将在验证期间导致错误。
 */
@property (nonatomic, copy, nullable) NSIndexSet *acceptableStatusCodes;

/**
响应的可接受MIME类型。 当非“nil”时，具有MIME类型的“Content-Type”的响应与该集合不相交将在验证期间导致错误。
 */
@property (nonatomic, copy, nullable) NSSet <NSString *> *acceptableContentTypes;

/**
 验证指定的响应和数据。
 
   在其基本实现中，此方法检查可接受的状态代码和内容类型。 子类可能希望添加其他特定于域的检查。
 
   @param response要验证的响应。
   @param data与响应关联的数据。
   @param error尝试验证响应时发生的错误。
 
   @return`YES`如果响应有效，否则为“NO”。
 */
- (BOOL)validateResponse:(nullable NSHTTPURLResponse *)response
                    data:(nullable NSData *)data
                   error:(NSError * _Nullable __autoreleasing *)error;

@end

#pragma mark -


/**
 `AFJSONResponseSerializer`是`AFHTTPResponseSerializer`的子类，用于验证和解码JSON响应。
 
   默认情况下，`AFJSONResponseSerializer`接受以下MIME类型，包括官方标准，`application / json`，以及其他常用类型：
 
    - `application / json`
    - `text / json`
    - `text / javascript`
 
   在RFC 7159  - 第8.1节中，它声明JSON文本需要以UTF-8，UTF-16或UTF-32编码，默认编码为UTF-8。 NSJSONSerialization支持规范中列出的所有编码，并建议使用UTF-8来提高效率。 使用不受支持的编码将导致序列化错误。 有关更多详细信息，请参阅`NSJSONSerialization`文档。
 */
@interface AFJSONResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
 Options for reading the response JSON data and creating the Foundation objects. For possible values, see the `NSJSONSerialization` documentation section "NSJSONReadingOptions". `0` by default.
 */
@property (nonatomic, assign) NSJSONReadingOptions readingOptions;

/**
 Whether to remove keys with `NSNull` values from response JSON. Defaults to `NO`.
 */
@property (nonatomic, assign) BOOL removesKeysWithNullValues;

/**
 Creates and returns a JSON serializer with specified reading and writing options.

 @param readingOptions The specified JSON reading options.
 */
+ (instancetype)serializerWithReadingOptions:(NSJSONReadingOptions)readingOptions;

@end

#pragma mark -

/**
 `AFXMLParserResponseSerializer` is a subclass of `AFHTTPResponseSerializer` that validates and decodes XML responses as an `NSXMLParser` objects.

 By default, `AFXMLParserResponseSerializer` accepts the following MIME types, which includes the official standard, `application/xml`, as well as other commonly-used types:

 - `application/xml`
 - `text/xml`
 */
@interface AFXMLParserResponseSerializer : AFHTTPResponseSerializer

@end

#pragma mark -

#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED

/**
 `AFXMLDocumentResponseSerializer` is a subclass of `AFHTTPResponseSerializer` that validates and decodes XML responses as an `NSXMLDocument` objects.

 By default, `AFXMLDocumentResponseSerializer` accepts the following MIME types, which includes the official standard, `application/xml`, as well as other commonly-used types:

 - `application/xml`
 - `text/xml`
 */
@interface AFXMLDocumentResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
 Input and output options specifically intended for `NSXMLDocument` objects. For possible values, see the `NSXMLDocument` documentation section "Input and Output Options". `0` by default.
 */
@property (nonatomic, assign) NSUInteger options;

/**
 Creates and returns an XML document serializer with the specified options.

 @param mask The XML document options.
 */
+ (instancetype)serializerWithXMLDocumentOptions:(NSUInteger)mask;

@end

#endif

#pragma mark -

/**
 `AFPropertyListResponseSerializer` is a subclass of `AFHTTPResponseSerializer` that validates and decodes XML responses as an `NSXMLDocument` objects.

 By default, `AFPropertyListResponseSerializer` accepts the following MIME types:

 - `application/x-plist`
 */
@interface AFPropertyListResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
 The property list format. Possible values are described in "NSPropertyListFormat".
 */
@property (nonatomic, assign) NSPropertyListFormat format;

/**
 The property list reading options. Possible values are described in "NSPropertyListMutabilityOptions."
 */
@property (nonatomic, assign) NSPropertyListReadOptions readOptions;

/**
 Creates and returns a property list serializer with a specified format, read options, and write options.

 @param format The property list format.
 @param readOptions The property list reading options.
 */
+ (instancetype)serializerWithFormat:(NSPropertyListFormat)format
                         readOptions:(NSPropertyListReadOptions)readOptions;

@end

#pragma mark -

/**
 `AFImageResponseSerializer` is a subclass of `AFHTTPResponseSerializer` that validates and decodes image responses.

 By default, `AFImageResponseSerializer` accepts the following MIME types, which correspond to the image formats supported by UIImage or NSImage:

 - `image/tiff`
 - `image/jpeg`
 - `image/gif`
 - `image/png`
 - `image/ico`
 - `image/x-icon`
 - `image/bmp`
 - `image/x-bmp`
 - `image/x-xbitmap`
 - `image/x-win-bitmap`
 */
@interface AFImageResponseSerializer : AFHTTPResponseSerializer

#if TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH
/**
 The scale factor used when interpreting the image data to construct `responseImage`. Specifying a scale factor of 1.0 results in an image whose size matches the pixel-based dimensions of the image. Applying a different scale factor changes the size of the image as reported by the size property. This is set to the value of scale of the main screen by default, which automatically scales images for retina displays, for instance.
 */
@property (nonatomic, assign) CGFloat imageScale;

/**
 Whether to automatically inflate response image data for compressed formats (such as PNG or JPEG). Enabling this can significantly improve drawing performance on iOS when used with `setCompletionBlockWithSuccess:failure:`, as it allows a bitmap representation to be constructed in the background rather than on the main thread. `YES` by default.
 */
@property (nonatomic, assign) BOOL automaticallyInflatesResponseImage;
#endif

@end

#pragma mark -

/**
 `AFCompoundSerializer` is a subclass of `AFHTTPResponseSerializer` that delegates the response serialization to the first `AFHTTPResponseSerializer` object that returns an object for `responseObjectForResponse:data:error:`, falling back on the default behavior of `AFHTTPResponseSerializer`. This is useful for supporting multiple potential types and structures of server responses with a single serializer.
 */
@interface AFCompoundResponseSerializer : AFHTTPResponseSerializer

/**
 The component response serializers.
 */
@property (readonly, nonatomic, copy) NSArray <id<AFURLResponseSerialization>> *responseSerializers;

/**
 Creates and returns a compound serializer comprised of the specified response serializers.

 @warning Each response serializer specified must be a subclass of `AFHTTPResponseSerializer`, and response to `-validateResponse:data:error:`.
 */
+ (instancetype)compoundSerializerWithResponseSerializers:(NSArray <id<AFURLResponseSerialization>> *)responseSerializers;

@end

///----------------
/// @name Constants
///----------------

/**
 ## Error Domains

 The following error domain is predefined.

 - `NSString * const AFURLResponseSerializationErrorDomain`

 ### Constants

 `AFURLResponseSerializationErrorDomain`
 AFURLResponseSerializer errors. Error codes for `AFURLResponseSerializationErrorDomain` correspond to codes in `NSURLErrorDomain`.
 */
FOUNDATION_EXPORT NSString * const AFURLResponseSerializationErrorDomain;

/**
 ## User info dictionary keys

 These keys may exist in the user info dictionary, in addition to those defined for NSError.

 - `NSString * const AFNetworkingOperationFailingURLResponseErrorKey`
 - `NSString * const AFNetworkingOperationFailingURLResponseDataErrorKey`

 ### Constants

 `AFNetworkingOperationFailingURLResponseErrorKey`
 The corresponding value is an `NSURLResponse` containing the response of the operation associated with an error. This key is only present in the `AFURLResponseSerializationErrorDomain`.

 `AFNetworkingOperationFailingURLResponseDataErrorKey`
 The corresponding value is an `NSData` containing the original data of the operation associated with an error. This key is only present in the `AFURLResponseSerializationErrorDomain`.
 */
FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLResponseErrorKey;

FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLResponseDataErrorKey;

NS_ASSUME_NONNULL_END

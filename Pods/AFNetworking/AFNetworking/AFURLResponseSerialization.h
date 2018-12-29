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
 
   @param response 要验证的响应。
   @param data 与响应关联的数据。
   @param error 尝试验证响应时发生的错误。
 
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
用于读取响应JSON数据和创建Foundation对象的选项。 有关可能的值，请参阅“NSJSONSerialization”文档部分“NSJSONReadingOptions”。 默认为“0”。
 */
@property (nonatomic, assign) NSJSONReadingOptions readingOptions;

/**
 是否从响应JSON中删除带有“NSNull”值的键。 默认为“NO”。
 */
@property (nonatomic, assign) BOOL removesKeysWithNullValues;

/**
 创建并返回具有指定读取和写入选项的JSON序列化程序。
 
   @param readingOptions 指定的JSON读取选项。
 */
+ (instancetype)serializerWithReadingOptions:(NSJSONReadingOptions)readingOptions;

@end

#pragma mark -

/**
 `AFXMLParserResponseSerializer`是`AFHTTPResponseSerializer`的子类，它将XML响应验证和解码为`NSXMLParser`对象。
 
   默认情况下，`AFXMLParserResponseSerializer`接受以下MIME类型，包括官方标准，`application / xml`，以及其他常用类型：
 
    - `application / xml`
    - `text / xml`
 */
@interface AFXMLParserResponseSerializer : AFHTTPResponseSerializer

@end

#pragma mark -

#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED

/**
 `AFXMLDocumentResponseSerializer`是`AFHTTPResponseSerializer`的子类，它将XML响应验证并解码为`NSXMLDocument`对象。
 
   默认情况下，`AFXMLDocumentResponseSerializer`接受以下MIME类型，包括官方标准，`application / xml`，以及其他常用类型：
 
    - `application / xml`
    - `text / xml`
 */
@interface AFXMLDocumentResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
输入和输出选项专门用于`NSXMLDocument`对象。 有关可能的值，请参阅“NSXMLDocument”文档部分“输入和输出选项”。 默认为“0”。
 */
@property (nonatomic, assign) NSUInteger options;

/**
 使用指定的选项创建并返回XML文档序列化程序。
 
   @param mask XML文档选项。
 */
+ (instancetype)serializerWithXMLDocumentOptions:(NSUInteger)mask;

@end

#endif

#pragma mark -

/**
 `AFPropertyListResponseSerializer`是`AFHTTPResponseSerializer`的子类，它将XML响应验证并解码为`NSXMLDocument`对象。
 
   默认情况下，`AFPropertyListResponseSerializer`接受以下MIME类型：
 
    - `application / x-plist`
 */
@interface AFPropertyListResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
属性列表格式。 可能的值在“NSPropertyListFormat”中描述。
 */
@property (nonatomic, assign) NSPropertyListFormat format;

/**
财产清单阅读选项。 可能的值在“NSPropertyListMutabilityOptions”中描述。
 */
@property (nonatomic, assign) NSPropertyListReadOptions readOptions;

/**
 创建并返回具有指定格式，读取选项和写入选项的属性列表序列化程序。
 
   @param format 属性列表格式。
   @param readOptions 属性列表读取选项。
 */
+ (instancetype)serializerWithFormat:(NSPropertyListFormat)format
                         readOptions:(NSPropertyListReadOptions)readOptions;

@end

#pragma mark -

/**
 `AFImageResponseSerializer`是`AFHTTPResponseSerializer`的子类，用于验证和解码图像响应。
 
   默认情况下，`AFImageResponseSerializer`接受以下MIME类型，这些类型对应于UIImage或NSImage支持的图像格式：
 
    - `image / tiff`
    - `image / jpeg`
    - `image / gif`
    - `image / png`
    - `image / ico`
    - `image / x-icon`
    - `image / bmp`
    - `image / x-bmp`
    - `image / x-xbitmap`
    - `image / x-win-bitmap`
 */
@interface AFImageResponseSerializer : AFHTTPResponseSerializer

#if TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH
/**
解释图像数据以构造`responseImage`时使用的比例因子。 指定比例因子1.0会导致图像的大小与图像的基于像素的尺寸相匹配。 应用不同的比例因子会更改size属性报告的图像大小。 默认设置为主屏幕的比例值，例如，自动缩放视网膜显示的图像。
 */
@property (nonatomic, assign) CGFloat imageScale;

/**
是否自动为压缩格式（如PNG或JPEG）充气响应图像数据。 与`setCompletionBlockWithSuccess：failure：`一起使用时，启用此功能可以显着提高iOS上的绘图性能，因为它允许在后台而不是主线程上构建位图表示。 默认为“是”。
 */
@property (nonatomic, assign) BOOL automaticallyInflatesResponseImage;
#endif

@end

#pragma mark -

/**
`AFCompoundSerializer`是`AFHTTPResponseSerializer`的子类，它将响应序列化委托给第一个`AFHTTPResponseSerializer`对象，该对象返回`responseObjectForResponse：data：error：`的对象，回退到`AFHTTPResponseSerializer`的默认行为。 这对于使用单个序列化程序支持多种潜在类型和结构的服务器响应非常有用。
 */
@interface AFCompoundResponseSerializer : AFHTTPResponseSerializer

/**
 组件响应序列化程序。
 */
@property (readonly, nonatomic, copy) NSArray <id<AFURLResponseSerialization>> *responseSerializers;

/**
 创建并返回由指定的响应序列化程序组成的复合序列化程序。
 
   @warning指定的每个响应序列化程序必须是`AFHTTPResponseSerializer`的子类，并且响应`-validateResponse：data：error：`。
 */
+ (instancetype)compoundSerializerWithResponseSerializers:(NSArray <id<AFURLResponseSerialization>> *)responseSerializers;

@end

///----------------
/// @name Constants
///----------------

/**
 ##错误域
 
   以下错误域是预定义的。
 
    - `NSString * const AFURLResponseSerializationErrorDomain`
 
   ###常数
 
  `AFURLResponseSerializationErrorDomain`
   AFURLResponseSerializer错误。 `AFURLResponseSerializationErrorDomain`的错误代码对应于`NSURLErrorDomain`中的代码。
 */
FOUNDATION_EXPORT NSString * const AFURLResponseSerializationErrorDomain;

/**
 ##用户信息字典键
 
   除了为NSError定义的密钥之外，这些密钥可能存在于用户信息字典中。
 
    - `NSString * const AFNetworkingOperationFailingURLResponseErrorKey`
    - `NSString * const AFNetworkingOperationFailingURLResponseDataErrorKey`
 
   ###常数
 
  `AFNetworkingOperationFailingURLResponseErrorKey`
   相应的值是一个`NSURLResponse`，它包含与错误相关的操作的响应。 该键仅出现在`AFURLResponseSerializationErrorDomain`中。
 
  `AFNetworkingOperationFailingURLResponseDataErrorKey`
   相应的值是一个“NSData”，包含与错误相关的操作的原始数据。 该键仅出现在`AFURLResponseSerializationErrorDomain`中。
 */
FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLResponseErrorKey;

FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLResponseDataErrorKey;

NS_ASSUME_NONNULL_END

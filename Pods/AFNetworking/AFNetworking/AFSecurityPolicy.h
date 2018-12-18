// AFSecurityPolicy.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//还是先看头文件里边有什么东西。要实现认证功能需要添加系统的<Security/Security.h>，这个是必须的。

#import <Foundation/Foundation.h>
#import <Security/Security.h>

typedef NS_ENUM(NSUInteger, AFSSLPinningMode) {
    AFSSLPinningModeNone,        //代表无条件信任服务器的证书
    AFSSLPinningModePublicKey,   //代表会对服务器返回的证书中的PublicKey进行验证，通过则通过，否则不通过
    AFSSLPinningModeCertificate,  //代表会对服务器返回的证书同本地证书全部进行校验，通过则通过，否则不通过
};

/**
  AFSecurityPolicy 用来评价通过X.509(数字证书的标准)的数字证书和公开密钥进行的安全网络连接是否值得信任。在应用内添加SSL证书能够有效的防止中间人的攻击和安全漏洞。强烈建议涉及用户敏感或隐私数据或金融信息的应用全部网络连接都采用使用SSL的HTTPS连接。
 */

NS_ASSUME_NONNULL_BEGIN

@interface AFSecurityPolicy : NSObject <NSSecureCoding, NSCopying>

/**
返回SSL Pinning的类型。默认的是AFSSLPinningModeNone。
 */
@property (readonly, nonatomic, assign) AFSSLPinningMode SSLPinningMode;

/**
 这个属性保存着所有的可用做校验的证书的集合。AFNetworking默认会搜索工程中所有.cer的证书文件。如果想制定某些证书，可使用certificatesInBundle在目标路径下加载证书，然后调用policyWithPinningMode:withPinnedCertificates创建一个本类对象。
 
 注意： 只要在证书集合中任何一个校验通过，evaluateServerTrust:forDomain: 就会返回true，即通过校验。
 */
@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

/**
允许使用无效或过期的证书，默认是不允许。
 */
@property (nonatomic, assign) BOOL allowInvalidCertificates;

/**
是否验证证书中的域名domain
 */
@property (nonatomic, assign) BOOL validatesDomainName;

///-----------------------------------------
/// @name Getting Certificates from the Bundle
///-----------------------------------------

/**
 返回指定bundle中的证书。如果使用AFNetworking的证书验证 ，就必须实现此方法，并且使用policyWithPinningMode:withPinnedCertificates 方法来创建实例对象。
 */
+ (NSSet <NSData *> *)certificatesInBundle:(NSBundle *)bundle;

///-----------------------------------------
/// @name Getting Specific Security Policies
///-----------------------------------------

/**
 默认的实例对象，默认的认证设置为：
 1. 不允许无效或过期的证书
 2. 验证domain名称
 3. 不对证书和公钥进行验证
 */
+ (instancetype)defaultPolicy;

///---------------------
/// @name Initialization
///---------------------

/** */
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode;

/**
 */
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode withPinnedCertificates:(NSSet <NSData *> *)pinnedCertificates;

///------------------------------
/// @name Evaluating Server Trust
///------------------------------

/**
核心方法：使用起来是这样的，这个方法AFNetworking在内部调用了。
 */
- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END

///----------------
/// @name Constants
///----------------

/**
 AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
  AFSecurityPolicy *securityPolicy = [[AFSecurityPolicy alloc] init];
  [securityPolicy setAllowInvalidCertificates:NO];
  [securityPolicy setSSLPinningMode:AFSSLPinningModeCertificate];
  [securityPolicy setValidatesDomainName:YES];
  [securityPolicy setValidatesCertificateChain:NO];
  manager.securityPolicy = securityPolicy;
*/

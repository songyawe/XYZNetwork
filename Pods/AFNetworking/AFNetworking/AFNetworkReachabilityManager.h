// AFNetworkReachabilityManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#if !TARGET_OS_WATCH
//网络监控的实现是依赖SystemConfiguration这个api的
#import <SystemConfiguration/SystemConfiguration.h>

//网络类型
//在这里作者是枚举了4种类型。这几种类型能够满足我们开发中大部分的功能，如果不满足，可以自行进行扩展。
typedef NS_ENUM(NSInteger, AFNetworkReachabilityStatus) {
    AFNetworkReachabilityStatusUnknown          = -1, //未知
    AFNetworkReachabilityStatusNotReachable     = 0,  //无网
    AFNetworkReachabilityStatusReachableViaWWAN = 1,  //手机网络
    AFNetworkReachabilityStatusReachableViaWiFi = 2,  //Wifi
};

/*
 NS_ASSUME_NONNULL_BEGIN
 NS_ASSUME_NONNULL_END
 这个是为了swift的可选类型配添加的，在这两个终点的内容的参数默认都是nonnull的。
 */
NS_ASSUME_NONNULL_BEGIN

/**
 网络可达性监控
 AFNetworkReachabilityManager的实例必须先使用`-startMonitoring`启动，然后才能确定可达性状态。
 */
@interface AFNetworkReachabilityManager : NSObject

/**
网络状态
 */
@property (readonly, nonatomic, assign) AFNetworkReachabilityStatus networkReachabilityStatus;

/**
 是否是可达的
 */
@property (readonly, nonatomic, assign, getter = isReachable) BOOL reachable;

/**
当前连接是否是WWAN
 */
@property (readonly, nonatomic, assign, getter = isReachableViaWWAN) BOOL reachableViaWWAN;

/**
当前连接是否是WiFi
 */
@property (readonly, nonatomic, assign, getter = isReachableViaWiFi) BOOL reachableViaWiFi;

///---------------------
/// @name Initialization 提供了5中初始化方法，能够满足大部分的需求
///---------------------
/**
 Returns the shared network reachability manager.
 */
+ (instancetype)sharedManager;

/**
 Creates and returns a network reachability manager with the default socket address.
 
 @return An initialized network reachability manager, actively monitoring the default socket address.
 */
+ (instancetype)manager;

/**
 @param 监听制定domain的网络状态
 @return
 */
+ (instancetype)managerForDomain:(NSString *)domain;

/**
 @param 监听某个socket地址的网络状态
 @return
 */
+ (instancetype)managerForAddress:(const void *)address;

/**
 @param SCNetworkReachabilityRef 这个很重要，这个类的就是基于它开发的
 @return
 */
- (instancetype)initWithReachability:(SCNetworkReachabilityRef)reachability NS_DESIGNATED_INITIALIZER;

/**
 *  Unavailable initializer
 */
+ (instancetype)new NS_UNAVAILABLE;

/**
 *  Unavailable initializer
 */
- (instancetype)init NS_UNAVAILABLE;

///--------------------------------------------------
/// @name Starting & Stopping Reachability Monitoring
///--------------------------------------------------

/**开始监听 */
- (void)startMonitoring;

/**关闭监听 */
- (void)stopMonitoring;

///-------------------------------------------------
/// @name Getting Localized Reachability Description
///-------------------------------------------------

/**返回一个网络状态的本地语言的字符串。往往我们可以根据这个字符串来告诉用户，当前网络发生了什么，当然，也可以根据状态自定义提示文字。*/
- (NSString *)localizedNetworkReachabilityStatusString;

///---------------------------------------------------
/// @name Setting Network Reachability Change Callback
///---------------------------------------------------

/**
 设置网络转态改变的回调，监听网络改变的回调有两种方式：
 1.使用下边的这个方法。
 2.监听AFNetworkingReachabilityDidChangeNotification通知。
 @param
 */
- (void)setReachabilityStatusChangeBlock:(nullable void (^)(AFNetworkReachabilityStatus status))block;

@end

///--------------------
/// @name Notifications
///--------------------

/**
 这个是与网络状态变化相关的通知。接受的通知中会有一个userinfo 是一个NSDictionary 其中key就是
 AFNetworkingReachabilityNotificationStatusItem
 
  FOUNDATION_EXPORT 和#define 都能定义常量。FOUNDATION_EXPORT 能够使用==进行判断，效率略高。而且能够隐藏定义细节(就是实现部分不在.中)
 */
FOUNDATION_EXPORT NSString * const AFNetworkingReachabilityDidChangeNotification;
FOUNDATION_EXPORT NSString * const AFNetworkingReachabilityNotificationStatusItem;

///--------------------
/// @name Functions
///--------------------

/**对函数：根据状态获取字符串  声明。*/
FOUNDATION_EXPORT NSString * AFStringFromNetworkReachabilityStatus(AFNetworkReachabilityStatus status);

NS_ASSUME_NONNULL_END
#endif

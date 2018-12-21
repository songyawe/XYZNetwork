// AFNetworkActivityIndicatorManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#import <Foundation/Foundation.h>

#import <TargetConditionals.h>

#if TARGET_OS_IOS

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 `AFNetworkActivityIndi​​catorManager`管理状态栏中网络活动指示器的状态。启用后，它将侦听指示会话任务已启动或已完成的通知，并相应地启动或停止动画指示器。活动请求的数量递增和递减很像堆栈或信号量，只要该数字大于零，活动指示符就会生成动画。
 
  应用程序完成启动时，应启用“AFNetworkActivityIndi​​catorManager”的共享实例。在`AppDelegate应用程序：didFinishLaunchingWithOptions：`中，您可以使用以下代码执行此操作：
 
     [[AFNetworkActivityIndi​​catorManager sharedManager] setEnabled：YES];
 
  通过为`sharedManager`设置`enabled`为'YES`，网络活动指示器将在请求开始和结束时自动显示和隐藏。你不应该自己调用`incrementActivityCount`或`decrementActivityCount`。
 
  有关详细信息，请参阅Apple人机界面指南部分有关网络活动指示器的信息：
  http://developer.apple.com/library/iOS/#documentation/UserExperience/Conceptual/MobileHIG/UIElementGuidelines/UIElementGuidelines.html#//apple_ref/doc/uid/TP40006556-CH13-SW44
 */
NS_EXTENSION_UNAVAILABLE_IOS("Use view controller based solutions where appropriate instead.")
@interface AFNetworkActivityIndicatorManager : NSObject

/**
 一个布尔值，指示是否启用了管理器。
 
   如果是，管理员将根据收到的网络操作通知更改状态栏网络活动指示器。 默认值为NO。
 */
@property (nonatomic, assign, getter = isEnabled) BOOL enabled;

/**
布尔值，指示网络活动指示器管理器当前是否处于活动状态。
*/
@property (readonly, nonatomic, assign, getter=isNetworkActivityIndicatorVisible) BOOL networkActivityIndicatorVisible;

/**
 一个时间间隔，指示在显示活动指示符之前应发生的最短网络活动持续时间。 默认值为1秒。 如果网络活动发生时应立即显示网络活动指示符，则此值应设置为0秒。
  
   Apple的HIG描述了以下内容：
 
   >显示网络活动指示器，以便在您的应用访问网络超过几秒钟时提供反馈。 如果操作完成的时间早于此，则不必显示网络活动指示符，因为指示器可能会在用户注意到其存在之前消失。

 */
@property (nonatomic, assign) NSTimeInterval activationDelay;

/**
一个时间间隔，指示在禁用活动指示符之前不需要网络活动的持续时间。 这允许跨多个请求连续显示网络活动指示符。 默认值为0.17秒。
 */

@property (nonatomic, assign) NSTimeInterval completionDelay;

/**
 返回系统的共享网络活动指示器管理器对象。
 
   @return 系统范围的网络活动指示器管理器。
 */
+ (instancetype)sharedManager;

/**
增加活动网络请求的数量。 如果在递增之前此数字为零，则将开始为状态栏网络活动指示器设置动画。
 */
- (void)incrementActivityCount;

/**
减少活动网络请求的数量。 如果此数字在递减后变为零，则将停止动画状态栏网络活动指示器。
 */
- (void)decrementActivityCount;

/**
 设置应隐藏/显示网络活动指示器管理器时要执行的自定义方法。 默认情况下，这是null，并且将自动管理UIApplication Network活动指示符。 如果设置了此块，则调用者负责管理网络活动指示器。
 
   @param block网络活动指示器状态更改时要执行的块。
 */
- (void)setNetworkingActivityActionWithBlock:(nullable void (^)(BOOL networkActivityIndicatorVisible))block;

@end

NS_ASSUME_NONNULL_END

#endif

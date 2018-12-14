//
//  XYZCacheCommonRequest.h
//  NetworkDemo
//
//  Created by xyz on 2018/12/12.
//  Copyright © 2018年 xyz. All rights reserved.

/**
 * 在四种情况下不会加载缓存而是直接请求网络：
 * 1.缓存类型XYZCacheType为XYZCacheTypeNET.
 * 2.缓存时间失效
 * 3.缓存版本变化
 * 4.软件版本变化
 */


#import "XYZCacheRequest.h"

NS_ASSUME_NONNULL_BEGIN

@interface XYZCacheCommonRequest : XYZCacheRequest

/**
* 普通缓存网络请求方式
*
*@param urlString   请求路径.
*@param requestType 请求方式
*@param cacheType   缓存方式
*@return return     返回实例化对象
*/
+ (instancetype)XYZ_requestWithUrl:(NSString *)urlString requestType:(XYZRequestType)requestType useCacheType:(XYZCacheType)cacheType;

@end

NS_ASSUME_NONNULL_END

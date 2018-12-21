// UIKit+AFNetworking.h
//
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )

#if TARGET_OS_IOS || TARGET_OS_TV
#import <UIKit/UIKit.h>

#ifndef _UIKIT_AFNETWORKING_
    #define _UIKIT_AFNETWORKING_

#if TARGET_OS_IOS
    #import "AFAutoPurgingImageCache.h"
    #import "AFImageDownloader.h"
    #import "AFNetworkActivityIndicatorManager.h"
    #import "UIRefreshControl+AFNetworking.h"
    #import "UIWebView+AFNetworking.h"
#endif

    #import "UIActivityIndicatorView+AFNetworking.h"
    #import "UIButton+AFNetworking.h"
    #import "UIImageView+AFNetworking.h"
    #import "UIProgressView+AFNetworking.h"
#endif /* _UIKIT_AFNETWORKING_ */
#endif

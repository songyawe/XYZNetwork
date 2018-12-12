//
//  XYZUploadRequest.h
//  NetworkDemo
//
//  Created by xyz on 2018/12/12.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZBaseRequest.h"
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface XYZUploadRequest : XYZBaseRequest

/**
 *@param data 要上传的数据.
*@param name 指定关联的名字,此参数不能为“nil”
*@param fileName 指定文件的名字,此参数不能为“nil”.
 *@param type 指定数据的MIME类型。 （例如，JPEG图像的MIME类型是image / jpeg。）有关有效MIME类型的列表，请参阅http://www.iana.org/assignments/media-types/。 此参数不能为“nil”。
 */
- (id)initWithData:(NSData *)data name:(NSString *)name fileName:(NSString *)fileName type:(NSString *)type ;

@end

NS_ASSUME_NONNULL_END

//
//  XYZUploadRequest.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/12.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "XYZUploadRequest.h"
#import <AFNetworking/AFNetworking.h>
@interface XYZUploadRequest()
@property(nonatomic,strong)NSData *data;
@property(nonatomic,strong)NSString *name;
@property(nonatomic,strong)NSString *fileName;
@property(nonatomic,strong)NSString *type;

@end
@implementation XYZUploadRequest

- (id)initWithData:(NSData *)data name:(NSString *)name fileName:(NSString *)fileName type:(NSString *)type {
    self = [super init];
    if (self) {
        _data = data;
        _name = name;
        _fileName = fileName;
        _type = type;
    }
    return self;
}

- (XYZRequestType)requestMethod{
    return XYZRequestTypePOST;
}

- (NSString *)requestUrl{
    return @"url";
}

- (AFConstructingBlock)constructingBodyBlock{
    __weak typeof(self) weakSelf = self;
    return ^(id<AFMultipartFormData> formData){
//        NSData *data = UIImageJPEGRepresentation(weakSelf.image, 0.5);
//        NSString *name = @"image";
//        NSString *fileName = @"image";
//        NSString *type = @"image/jpeg";
        [formData appendPartWithFileData:weakSelf.data name:weakSelf.name fileName:weakSelf.fileName mimeType:weakSelf.type];
    };
}

@end

//
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ResponseModel : NSObject
@property (nonatomic, strong) NSArray *errors;
@property (nonatomic, strong) NSObject *data;
@property (nonatomic, strong) id tag;
@end

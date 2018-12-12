//
//  UserModel.h
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum : NSUInteger {
    GenderMale = 1,
    GenderFemale = 2,
    GenderUnknown = 3,
} Gender;

extern NSString* genderName(Gender gender);

typedef enum : NSUInteger {
    CourseTypeTrial = 0,
    CourseTypeChild = 1,
    CourseTypeAdult = 2,
    CourseTypeMaster = 3
} CourseType;

@interface UserModel: NSObject<NSCoding, NSCopying>
@property (nonatomic, strong) NSString *userId;
@property (nonatomic, strong) NSString *teacherId;
@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *mobile;
@property (nonatomic, assign) Gender gender;
@property (nonatomic, strong) NSString *birthday;
@property (nonatomic, strong) NSString *welcomeStatus;
@property (nonatomic, assign) BOOL canAppoint;
@property (nonatomic, assign) BOOL todayCourseStatus;
@property (nonatomic, assign) CourseType course;
@property (nonatomic, strong) NSString *avatar;
@property (nonatomic, strong) NSString *thumb;

@end

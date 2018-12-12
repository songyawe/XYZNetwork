//
//  UserModel.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "UserModel.h"
#import <MJExtension.h>


@implementation UserModel
+ (NSDictionary *)mj_replacedKeyFromPropertyName{
    return @{
             @"userId": @"user_id",
             @"teacherId": @"teacher_id",
             @"welcomeStatus": @"welcome_status",
             @"canAppoint": @"can_appoint",
             @"todayCourseStatus": @"today_course_status",
             @"course": @"course_type"
             };
}
MJExtensionCodingImplementation;
//- (void)encodeWithCoder:(NSCoder *)aCoder {
//    [aCoder encodeObject:_userId forKey:@"userId"];
//    [aCoder encodeObject:_teacherId forKey:@"teacherId"];
//    [aCoder encodeObject:_name forKey:@"name"];
//    [aCoder encodeObject:_mobile forKey:@"mobile"];
//    [aCoder encodeObject:[NSNumber numberWithUnsignedInteger:_gender] forKey:@"gender"];
//    [aCoder encodeObject:_birthday forKey:@"birthday"];
//    [aCoder encodeObject:_welcomeStatus forKey:@"welcomeStatus"];
//    [aCoder encodeObject:[NSNumber numberWithBool:_canAppoint] forKey:@"canAppoint"];
//    [aCoder encodeObject:[NSNumber numberWithBool:_todayCourseStatus] forKey:@"todayCourseStatus"];
//    [aCoder encodeObject:[NSNumber numberWithUnsignedInteger:_course] forKey:@"course"];
//    [aCoder encodeObject:_avatar forKey:@"avatar"];
//    [aCoder encodeObject:_thumb forKey:@"thumb"];
//}
//
//- (instancetype)initWithCoder:(NSCoder *)aDecoder {
//    if (self = [super init]) {
//        _userId = [aDecoder decodeObjectForKey:@"userId"];
//        _teacherId = [aDecoder decodeObjectForKey:@"teacherId"];
//        _name = [aDecoder decodeObjectForKey:@"name"];
//        _mobile = [aDecoder decodeObjectForKey:@"mobile"];
//        _gender = [[aDecoder decodeObjectForKey:@"gender"] unsignedIntegerValue];
//        _birthday = [aDecoder decodeObjectForKey:@"birthday"];
//        _welcomeStatus = [aDecoder decodeObjectForKey:@"welcomeStatus"];
//        _canAppoint = [[aDecoder decodeObjectForKey:@"canAppoint"] boolValue];
//        _todayCourseStatus = [[aDecoder decodeObjectForKey:@"todayCourseStatus"] boolValue];
//        _course = [[aDecoder decodeObjectForKey:@"course"] unsignedIntegerValue];
//        _avatar = [aDecoder decodeObjectForKey:@"avatar"];
//        _thumb = [aDecoder decodeObjectForKey:@"thumb"];
//    }
//    return self;
//}

- (instancetype)copyWithZone:(NSZone *)zone {
    UserModel *copy = [[[self class] allocWithZone:zone] init];
    copy.userId = [_userId copyWithZone:zone];
    copy.name = [_name copyWithZone:zone];
    copy.mobile = [_mobile copyWithZone:zone];
    copy.gender = _gender;
    return copy;
}
@end

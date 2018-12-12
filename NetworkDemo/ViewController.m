//
//  ViewController.m
//  NetworkDemo
//
//  Created by xyz on 2018/12/10.
//  Copyright © 2018年 xyz. All rights reserved.
//

#import "ViewController.h"
#import "XYZCommonRequest.h"
#import "XYZDownloadRequest.h"
#import "MJExtension.h"
#import "ResponseModel.h"
#import "UserModel.h"
#import "XYZCacheCommonRequest.h"
#import "XYZGroupRequest.h"


@interface ViewController ()
@property (nonatomic,strong)XYZDownloadRequest *download;
@property (nonatomic,strong)UIButton *btn0;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIButton *btn0 = [UIButton buttonWithType:(UIButtonTypeCustom)];
    [btn0.titleLabel setFont:[UIFont boldSystemFontOfSize:15.f]];
    btn0.frame = CGRectMake(10, 100, 150, 50);
    [btn0 setTitle:@"开始断点下载" forState:(UIControlStateNormal)];
    [btn0 setBackgroundColor:[UIColor redColor]];
    [btn0 addTarget:self action:@selector(downloadAction) forControlEvents:(UIControlEventTouchUpInside)];
    [self.view addSubview:btn0];
    _btn0 = btn0;

    
    UIButton *btn1 = [UIButton buttonWithType:(UIButtonTypeCustom)];
    [btn1.titleLabel setFont:[UIFont boldSystemFontOfSize:15.f]];
    btn1.frame = CGRectMake(CGRectGetMaxX(btn0.frame) + 10, 100, 150, 50);
    [btn1 setTitle:@"停止下载" forState:(UIControlStateNormal)];
    [btn1 setBackgroundColor:[UIColor redColor]];
    [btn1 addTarget:self action:@selector(stopDownloadAction) forControlEvents:(UIControlEventTouchUpInside)];
    [self.view addSubview:btn1];
    
    UIButton *btn3 = [UIButton buttonWithType:(UIButtonTypeCustom)];
    [btn3.titleLabel setFont:[UIFont boldSystemFontOfSize:15.f]];
    btn3.frame = CGRectMake(10, CGRectGetMaxY(btn0.frame)+10, 150, 50);
    [btn3 setTitle:@"带缓存的网络请求" forState:(UIControlStateNormal)];
    [btn3 setBackgroundColor:[UIColor redColor]];
    [btn3 addTarget:self action:@selector(cacheLogin) forControlEvents:(UIControlEventTouchUpInside)];
    [self.view addSubview:btn3];
    
    UIButton *btn4 = [UIButton buttonWithType:(UIButtonTypeCustom)];
    [btn4.titleLabel setFont:[UIFont boldSystemFontOfSize:15.f]];
    btn4.frame = CGRectMake(CGRectGetMaxX(btn3.frame) + 10, CGRectGetMaxY(btn0.frame)+10, 150, 50);
    [btn4 setTitle:@"群组请求" forState:(UIControlStateNormal)];
    [btn4 setBackgroundColor:[UIColor redColor]];
    [btn4 addTarget:self action:@selector(group) forControlEvents:(UIControlEventTouchUpInside)];
    [self.view addSubview:btn4];
}

- (IBAction)webTest:(id)sender {
    [self login];
}
- (void)group{
    NSMutableArray *group = [NSMutableArray array];
    for (int i = 0; i < 10; i ++) {
        XYZCommonRequest *request = [XYZCommonRequest XYZ_requestWithUrl:@"/app/auth/signin" requestType:XYZRequestTypePOST];
        request.tag = i;
        request.requestParames = @{
                                    @"client":@"primary",
                                    @"code":@"1614",
                                    @"mobile":@"15001389806"
                                    };
        [group addObject:request];
    }

    XYZGroupRequest *groupRequest = [[XYZGroupRequest alloc]initWithRequestArray:group];
    
    [groupRequest sendRequestWithProcess:^(XYZBaseRequest * _Nonnull processRequest , XYZGroupRequest *groupRequest) {
        NSLog(@"------本次成功请求的编号:---%ld--------",(long)processRequest.tag);
        NSLog(@"-----完成了第几个:----%ld--------",(long)groupRequest.finishedCount);
    } success:^(XYZGroupRequest * _Nonnull groupRequest) {
        NSLog(@"---------请求组成功完成任务,进行其他任务--------");
        
    } failure:^(XYZGroupRequest * _Nonnull groupRequest) {
        NSLog(@"---------失败,提示用户--------");
    }];
    
}
//带缓存的网络请求
- (void)cacheLogin{
    
    XYZCacheCommonRequest *request = [XYZCacheCommonRequest XYZ_requestWithUrl:@"/app/auth/signin" requestType:XYZRequestTypePOST];
    request.requestParames = @{
                               @"client":@"primary",
                               @"code":@"1614",
                               @"mobile":@"15001389806"
                               };
    [request sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
        ResponseModel* model = [ResponseModel mj_objectWithKeyValues:request.responseJSONObject];
        model.data = [UserModel mj_objectWithKeyValues:request.responseJSONObject[@"user"]];
        
        NSDictionary *headers = request.responseHeaders;
        NSString *cookie = headers[@"Set-Cookie"];
        [[NSUserDefaults standardUserDefaults] setObject:cookie forKey:@"Set-Cookie"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        NSLog(@"请求成功！");
        
    } failure:^(__kindof XYZBaseRequest * _Nonnull request) {
        NSLog(@"请求失败！");
    }];
}
//普通网络请求
- (void)login {
    
    XYZCommonRequest *request = [XYZCommonRequest XYZ_requestWithUrl:@"/app/auth/signin" requestType:XYZRequestTypePOST];
    request.requestParames = @{
                        @"client":@"primary",
                        @"code":@"1614",
                        @"mobile":@"15001389806"
                        };
    [request sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
        ResponseModel* model = [ResponseModel mj_objectWithKeyValues:request.responseJSONObject];
        model.data = [UserModel mj_objectWithKeyValues:request.responseJSONObject[@"user"]];

        NSDictionary *headers = request.responseHeaders;
        NSString *cookie = headers[@"Set-Cookie"];
        [[NSUserDefaults standardUserDefaults] setObject:cookie forKey:@"Set-Cookie"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        NSLog(@"请求成功！");

    } failure:^(__kindof XYZBaseRequest * _Nonnull request) {
         NSLog(@"请求失败！");
    }];
}

- (void)stopDownloadAction{
    [self.btn0 setEnabled:YES];
    [_download stop];
}

- (void)downloadAction{
    [self.btn0 setEnabled:NO];
    __weak typeof(self)weakSelf = self;
    XYZDownloadRequest *download = [[XYZDownloadRequest alloc]init];
    download.downloadProgressBlock = ^(NSProgress * _Nonnull progress) {
        NSLog(@"--------%.2f------",progress.fractionCompleted);
        dispatch_async(dispatch_get_main_queue(), ^{
            [weakSelf.btn0 setTitle:[NSString stringWithFormat:@"下载进度:%.2f",progress.fractionCompleted] forState:(UIControlStateNormal)];
        });
        
    };
    [download sendRequestWithSuccess:^(__kindof XYZBaseRequest * _Nonnull request) {
        NSLog(@"下载成功！");
        [weakSelf.btn0 setEnabled:YES];
        [weakSelf.btn0 setTitle:@"下载成功，重新下载" forState:(UIControlStateNormal)];
    } failure:^(__kindof XYZBaseRequest * _Nonnull request) {
        [weakSelf.btn0 setEnabled:YES];
        NSLog(@"下载失败！");
    }];
    _download = download;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
//    NSString *cookie = [[NSUserDefaults standardUserDefaults] objectForKey: @"Set-Cookie"];
//    NSLog(@"存储的Cookie:%@",cookie);

    

}

@end

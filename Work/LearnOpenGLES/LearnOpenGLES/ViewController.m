//
//  ViewController.m
//  LearnOpenGLES
//
//  Created by Wicky on 2018/10/24.
//  Copyright © 2018 Wicky. All rights reserved.
//

#import "ViewController.h"
#import <GLKit/GLKit.h>


@interface ViewController ()<GLKViewDelegate>
@property (nonatomic ,strong) EAGLContext * mContext;

@property (nonatomic ,strong) GLKView * view;

@end

@implementation ViewController

-(void)loadView {
    [super loadView];
    self.view = [[GLKView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.view.delegate = self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self configGLContext];
}

-(void)configGLContext {
    self.mContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2]; //2.0，还有1.0和3.0
    self.view.context = self.mContext;
    self.view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;  //颜色缓冲区格式
    [EAGLContext setCurrentContext:self.mContext];
}

-(void)doRenderTask {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

#pragma mark --- glkView Delegate ---
-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [self doRenderTask];
}


@end

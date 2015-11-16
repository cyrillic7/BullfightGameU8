//
//  FMUIWebViewBridge.h
//  BullfightGame
//
//  Created by 张 恒 on 15/6/25.
//
//

#import <Foundation/Foundation.h>

#import <CoreLocation/CoreLocation.h>

#import <UIKit/UIKit.h>


#import "FMLayerWebView.h"


@interface FMUIWebViewBridge : NSObject<UIWebViewDelegate,UIAlertViewDelegate>{
    
    FMLayerWebView * mLayerWebView;
    
    UIView    *mView;
    
    UIWebView *mWebView;
    
    UIToolbar *mToolbar;
    
    UIBarButtonItem *mBackButton;
    
    UIActivityIndicatorView *activityView;
}


-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString;

-(void) backClicked:(id)sender;

@end

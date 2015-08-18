//
//  FMUIWebViewBridge.m
//  BullfightGame
//
//  Created by 张 恒 on 15/6/25.
//
//
#import "FMUIWebViewBridge.h"

#import "EAGLView.h"

@implementation FMUIWebViewBridge

- (id)init{
    
    self = [super init];
    
    if (self) {
        
        // init code here.
        
    }
    
    return self;
    
}

- (void)dealloc{
    
    [mBackButton release];
    
    [mToolbar release];
    
    [mWebView release];
    
    [mView release];
    
    [super dealloc];
    
}

-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString{
    
    mLayerWebView = iLayerWebView;
    
    CGRect cRect=[[UIScreen mainScreen] bounds];
    
    cocos2d::CCSize size;
    size.width=cRect.size.width;
    size.height=cRect.size.height;
    //cocos2d::CCSize size = mLayerWebView-> getContentSize();
    cocos2d::CCPoint pos = mLayerWebView->getPosition();
    
    
    
    mView = [[UIView alloc] initWithFrame:CGRectMake(pos.x, pos.y, size.width , size.height)];
    // create webView
    
    //Bottom size
    
    int wBottomMargin = size.height*0.10;
    
    int wWebViewHeight = size.height - wBottomMargin;
    
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, wBottomMargin, size.width, wWebViewHeight)];
   
    
    //[mWebView touchesBegan:<#(NSSet *)#> withEvent:<#(UIEvent *)#>]
    //[mWebView setExclusiveTouch:NO];
    mWebView.delegate = self;
    
    NSString *urlBase = [NSString stringWithCString:urlString encoding:NSUTF8StringEncoding];
    
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase ]]];
    
    [mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are
    
    //create a tool bar for the bottom of the screen to hold the back button
    
    mToolbar = [UIToolbar new];
    
    [mToolbar setFrame:CGRectMake(0, 0, size.width, wBottomMargin)];
    //NSLog(@"%d,%d",wWebViewHeight,wBottomMargin);
    
    mToolbar.barStyle = UIBarStyleBlackOpaque;
    
    //Create a button
    
    mBackButton = [[UIBarButtonItem alloc] initWithTitle:@"返回"
                   
                                                   style: UIBarButtonItemStyleDone
                   
                                                  target: self
                   
                                                  action:@selector(backClicked:)];
    
    
    //[mBackButton setBounds:CGRectMake(0.0, 0.0, 95.0, 34.0)];
    
    [mToolbar setItems:[NSArray arrayWithObjects:mBackButton,nil] animated:YES];
    
    [mView addSubview:mToolbar];
    
    //[mToolbar release];
    
    
    // add the webView to the view
    
    [mView addSubview:mWebView];
    
    [[EAGLView sharedEGLView] addSubview:mView];
    
}

-(NSString*) TruncateUrlPage:(NSString *)strURL{
    NSString *url=nil;
    NSArray *arrSplit =[strURL componentsSeparatedByString:@"?"];
    if(arrSplit.count>1){
        if(arrSplit[1]!=nil){
            url=[[[NSString alloc] initWithFormat:@"%@",arrSplit[1]] autorelease];
        }
    }
    return url;
}
-(NSDictionary *)URLRequest:(NSString*)URL{
    NSMutableDictionary *dic=[[[NSMutableDictionary alloc] init] autorelease];
    NSString *strUrlParam=[self TruncateUrlPage:URL];
    if (strUrlParam!=nil) {
        NSArray *arrSplit=[strUrlParam componentsSeparatedByString:@"&"];
        for (int i=0; i<arrSplit.count;i++ ) {
            NSString *strSplit=[[[NSString alloc] initWithFormat:@"%@",arrSplit[i]] autorelease];
            NSArray *arrSpliEqual=[strSplit componentsSeparatedByString:@"="];
            if(arrSpliEqual.count>1){
                NSString *strValue=[[[NSString alloc] initWithFormat:@"%@",arrSpliEqual[1]] autorelease];
                NSString *strKey=[[[NSString alloc] initWithFormat:@"%@",arrSpliEqual[0]] autorelease];
                [dic setValue:strValue forKey:strKey];
            }
        }
    }
    return dic;
}
- (void)webViewDidStartLoad:(UIWebView *)thisWebView {
    
}
- (void)webViewDidFinishLoad:(UIWebView *)thisWebView{
    [mWebView setUserInteractionEnabled:YES];
    mLayerWebView->webViewDidFinishLoad();
    
    NSString *strUrl=[[[thisWebView request] URL] absoluteString];
    NSDictionary *dix=[self URLRequest:strUrl];
    
    NSString *strId=[dix objectForKey:@"Id"];
    NSString *strPwd=[dix objectForKey:@"pwd"];
    if (strId) {
        
       // [mWebView setUserInteractionEnabled:NO];
        //[mWebView stopLoading];
        mLayerWebView->logonQQ([strId cStringUsingEncoding:NSUTF8StringEncoding], [strPwd cStringUsingEncoding:NSUTF8StringEncoding]);
        
        [self backClicked:nil];
        //return;
    }
    
   
    
    
}

- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error {
    
    if ([error code] != -999 && error != NULL) {
        //error -999 happens when the user clicks on something before it's done loading.
        
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error"
                                                        message:@"Unable to load the page. Please keep network connection."
                              
                                                       delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alert show];
        
        [alert release];
        
        
    }
    
}


-(void) backClicked:(id)sender {
    if([mWebView canGoBack]&&mLayerWebView->iCurAction==201){
        [mWebView goBack];
    }else
    {
        mWebView.delegate = nil; //keep the webview from firing off any extra messages
        
        //remove items from the Superview...just to make sure they're gone
        
        [mToolbar removeFromSuperview];
        
        [mWebView removeFromSuperview];
        
        [mView removeFromSuperview];
        
        mLayerWebView->onBackbuttonClick();
    }
    
    
}

@end
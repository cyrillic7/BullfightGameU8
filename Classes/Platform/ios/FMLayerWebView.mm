//
//  FMLayerWebView.m
//  BullfightGame
//
//  Created by 张 恒 on 15/6/25.
//
//

#include "FMLayerWebView.h"

#include "FMUIWebViewBridge.h"

static FMUIWebViewBridge *g_FMUIWebViewBridge=nil;

FMLayerWebView::FMLayerWebView(){
    
}

FMLayerWebView::~FMLayerWebView(){
    
    [g_FMUIWebViewBridge release];
    
}

void FMLayerWebView::webViewDidFinishLoad(){
    
}

void FMLayerWebView::onBackbuttonClick(){
    
    this->removeFromParentAndCleanup(true);
    
}

bool FMLayerWebView::init(){
    
    if ( !CCLayer::init() ){
        
        return false;
        
    }
    
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    
    
    
    return true;
    
}
void FMLayerWebView::setUrlWithOpen(std::string url){
   // NSString *strUrl=[[[NSString alloc] initWithFormat:@"%s",url.c_str()] autorelease];
    [g_FMUIWebViewBridge setLayerWebView : this URLString:url.c_str()];
}

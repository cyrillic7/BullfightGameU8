//
//  FMLayerWebView.m
//  BullfightGame
//
//  Created by 张 恒 on 15/6/25.
//
//

#include "FMLayerWebView.h"

#include "FMUIWebViewBridge.h"
#include "../../LogonScene/LogonScene.h"
#include "../../GameLobby/GameLobbyScene.h"
static FMUIWebViewBridge *g_FMUIWebViewBridge=nil;

FMLayerWebView::FMLayerWebView(){
    
}

FMLayerWebView::~FMLayerWebView(){
    
    [g_FMUIWebViewBridge release];
    
}

void FMLayerWebView::webViewDidFinishLoad(){
    
}

void FMLayerWebView::onBackbuttonClick(){
    switch (iCurAction) {
        case 200:
        {
            LogonScene::pLScene->closeWebView();
        }
            break;
        case 201:
        {
            
            GameLobbyScene::lobbyScene->closeWebView();
        }
            break;
        default:
            break;
    }
    
    this->removeFromParentAndCleanup(true);
    
}
void FMLayerWebView::logonQQ(const char*id,const char*pwd){
    LogonScene::pLScene->logonQQ(id, pwd);
    //this->removeFromParentAndCleanup(true);
}

bool FMLayerWebView::init(){
    
    if ( !CCLayer::init() ){
        
        return false;
        
    }
    
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    return true;
}
void FMLayerWebView::setUrlWithOpen(std::string url){
    [g_FMUIWebViewBridge setLayerWebView : this URLString:url.c_str()];
}

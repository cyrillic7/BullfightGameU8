//
//  FMLayerWebView.h
//  BullfightGame
//
//  Created by 张 恒 on 15/6/25.
//
//

#ifndef WebViewDemo_FMLayerWebView_h

#define WebViewDemo_FMLayerWebView_h


//#include "CCCommon.h"

#include "cocos2d.h"

USING_NS_CC;

class FMLayerWebView : public CCLayer{

public:
    
    FMLayerWebView();
    
    ~FMLayerWebView();
    
    virtual bool init();
    CREATE_FUNC(FMLayerWebView);
    
    //LAYER_NODE_FUNC(FMLayerWebView);
    
    void webViewDidFinishLoad();
    
    void onBackbuttonClick();
    
    void setUrlWithOpen(std::string url);
    
    void logonQQ(const char*id,const char*pwd);
private:
    
    int mWebViewLoadCounter;
    
};

#endif

//
//  LogonScene.cpp
//  
//
//  Created by on 15/3/16.
//
//

#include "LogonScene.h"
#include "Tools.h"
#include "MainScene.h"
LogonScene::LogonScene(){
}
LogonScene::~LogonScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
}
CCScene* LogonScene::scene()
{
    CCScene *scene = CCScene::create();
    LogonScene *layer = LogonScene::create();
    scene->addChild(layer);
    return scene;
}
void LogonScene::onEnter(){
	CCLayer::onEnter();
	Tools::setTransitionAnimation(0,0,MainScene::scene());
}
void LogonScene::onExit(){
	CCLayer::onExit();
}

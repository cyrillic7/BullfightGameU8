//
//  LogoScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "LogoScene.h"
#include "Tools.h"
#include "GameLobbyScene.h"
LogoScene::LogoScene(){
    scheduleOnce(SEL_SCHEDULE(&LogoScene::updateToMainScene), 0.5);
}
LogoScene::~LogoScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
}
CCScene* LogoScene::scene()
{
    CCScene *scene = CCScene::create();
    LogoScene *layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}
void LogoScene::updateToMainScene(float dt){
	Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene());
}
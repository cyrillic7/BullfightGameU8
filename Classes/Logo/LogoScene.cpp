//
//  LogoScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#include "LogoScene.h"
#include "Tools/Tools.h"
#include "GameLobby/GameLobbyScene.h"
LogoScene::LogoScene(){
	//0.5秒后进入MainScene
	scheduleOnce(SEL_SCHEDULE(&LogoScene::updateToMainScene), 0.5);
}
LogoScene::~LogoScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
}
//场景初始化
CCScene* LogoScene::scene()
{
    CCScene *scene = CCScene::create();
    LogoScene *layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}
//进入主游戏场景
void LogoScene::updateToMainScene(float dt){
	Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene());
}
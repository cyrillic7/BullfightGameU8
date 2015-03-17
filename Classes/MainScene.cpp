//
//  MainScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "MainScene.h"
#include "GameConfig.h"
MainScene::MainScene(){
}
MainScene::~MainScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
}
CCScene* MainScene::scene()
{
    CCScene *scene = CCScene::create();
    MainScene *layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}
void MainScene::onEnter(){
	CCLayer::onEnter();
	initHUD();
}
void MainScene::onExit(){
	CCLayer::onExit();
}
void MainScene::initHUD(){
	gameHUD = GameHUD::create();
	this->addChild(gameHUD, K_Z_ORDER_HUD);
}
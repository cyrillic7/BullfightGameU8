//
//  MainSceneBase.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//
#include "MainSceneBase.h"
MainSceneBase::MainSceneBase()
:gameState(STATE_OBSERVER)
{
}
MainSceneBase::~MainSceneBase(){
}
void MainSceneBase::onEnter(){
	CCLayer::onEnter();
	//addBg();
	//initCardLayer();
	initPlayerLayer();
	//initGameControl();
}
void MainSceneBase::onExit(){
	CCLayer::onExit();
}
void MainSceneBase::initPlayerLayer(){
	playerLayer = PlayerLayer::create();
	this->addChild(playerLayer);
}
//收到准备完成回调
void MainSceneBase::onEventReadyFnish(){
	CCLog("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}

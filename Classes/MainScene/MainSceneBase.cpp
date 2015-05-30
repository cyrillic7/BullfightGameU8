//
//  MainSceneBase.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//
#include "MainSceneBase.h"
#include "../PopDialogBox/PopDialogBoxLoading.h"
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
//加载Loading
void MainSceneBase::addLoadingLayer(){
	PopDialogBox *pPopBox = PopDialogBoxLoading::create();
	this->addChild(pPopBox, 10, TAG_LOADING);
}
//移除Loading
void MainSceneBase::removeLoadingLayer(){
	if (this->getChildByTag(TAG_LOADING))
	{
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	}
}
//
//  MainSceneBase.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//
#include "MainSceneBase.h"
#include "../PopDialogBox/PopDialogBoxLoading.h"
#include "../Tools/DataModel.h"
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
	this->addChild(playerLayer,K_Z_GI_PLAYER);
}
//收到准备完成回调
void MainSceneBase::onEventReadyFnish(){
	CCLOG("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}
//加载Loading
void MainSceneBase::addLoadingLayer(std::string socketName){
	PopDialogBox *pPopBox = PopDialogBoxLoading::create();
	this->addChild(pPopBox, K_Z_ORDER_POP, TAG_LOADING);
	pPopBox->setSocketName(socketName);
}
//移除Loading
void MainSceneBase::removeLoadingLayer(){
	if (this->getChildByTag(TAG_LOADING))
	{
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	}
}
//加载标题
void MainSceneBase::addTitle(){
	CCSpriteFrame *pSF = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("u_gi_lobby_type0.png");
	CCSprite *pTitleName = CCSprite::createWithSpriteFrame(pSF);
	this->addChild(pTitleName, 0);
	pTitleName->setPosition(ccp(DataModel::sharedDataModel()->deviceSize.width / 2, DataModel::sharedDataModel()->deviceSize.height / 2 + 150));
}
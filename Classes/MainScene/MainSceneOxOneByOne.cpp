//
//  MainScene.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainSceneOxOneByOne.h"
#include "Tools/GameConfig.h"
#include "Tools/DataModel.h"
#include "Tools/BaseAttributes.h"
//#include "TCPSocket.h"
//#include "CMD_LogonServer.h"
#include "Network/TCPSocket/TCPSocketControl.h"
#include "Play/CardLayer/CardLayerTwo.h"
//#include <tchar.h>
//#include "MD5.h"

//#include <thread>
//#include <iostream>
MainSceneOxOneByOne::MainSceneOxOneByOne()
:gameState(STATE_OBSERVER)
{
}
MainSceneOxOneByOne::~MainSceneOxOneByOne(){
	CCLog("~ <<%s>>", __FUNCTION__);
	TCPSocketControl::sharedTCPSocketControl()->stopSocket();

	GUIReader::purge();
	CCArmatureDataManager::purge();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();

	
	BaseAttributes *b = BaseAttributes::sharedAttributes();
	CC_SAFE_RELEASE_NULL(b);

}
/*bool MainScene::init(){
	if (!CCLayer::init())
	{
		return false;
	}
	return true;
}*/
CCScene* MainSceneOxOneByOne::scene()
{
    CCScene *scene = CCScene::create();
    MainSceneOxOneByOne *layer = MainSceneOxOneByOne::create();
    scene->addChild(layer);
	DataModel::sharedDataModel()->setMainSceneOxOneByOne(layer);
    return scene;
}
void MainSceneOxOneByOne::onEnter(){
	//CCLayer::onEnter();
	addBg();
	initCardLayer();
	//initPlayerLayer();
	MainSceneBase::onEnter();
	initGameControl();
}
void MainSceneOxOneByOne::onExit(){
	MainSceneBase::onExit();
	//CCLayer::onExit();
}
void MainSceneOxOneByOne::addBg(){
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/room1_n.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
}
void MainSceneOxOneByOne::initGameControl(){
	gameControl = GameControlOxOneByOne::create();
	this->addChild(gameControl, K_Z_ORDER_HUD);
}
//
void MainSceneOxOneByOne::initCardLayer(){
	cardLayer = CardLayerTwo::create();
	this->addChild(cardLayer);
}

//收到准备完成回调
void MainSceneOxOneByOne::onEventReadyFnish(){
	CCLog("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}
//收到发牌完成回调
void MainSceneOxOneByOne::onEventSendCardFnish(){
	
}
/*void MainScene::testLogic(){
	BYTE tempCard[5] = { 2, 2, 3, 8, 6 };
	bool ox = GetOxCard(tempCard, 5);
	CCLog("ox:%d", ox);
	BYTE tempCard[5] = {2,2,3,8,6};
	BYTE tempFristCard[5] = { 2, 2, 2, 4, 5 };
	BYTE tempNextCard[5] = { 2, 2, 2, 4, 6 };
	GameLogic *logic = new GameLogic();
	bool ox=logic->GetOxCard(tempCard, 5);

	for (int i = 0; i < 5; i++)
	{
		CCLog("--==:%d", tempCard[i]);
	}
	CCLog("%d", ox);

	bool compare=logic->CompareCard(tempFristCard, tempNextCard, 5);
	CCLog("--:%d", compare);

	CC_SAFE_DELETE(logic);
}*/
void MainSceneOxOneByOne::setGameStateWithUpdate(GameState gameState){
	setGameState(gameState);
	updateGameState();
}
void MainSceneOxOneByOne::setServerStateWithUpdate(GameState serverState){
	setServerState(serverState);
	updateServerState();
}
//更新状态
void MainSceneOxOneByOne::updateGameState(){
	//操作层更新状态
	gameControl->updateState();
	
	//玩家信息层更新状态
	playerLayer->updateState();
	//更新扑克层状态
	cardLayer->updateGameState();
}
void MainSceneOxOneByOne::updateServerState(){
	//更新扑克层状态
	cardLayer->updateServerState();
}

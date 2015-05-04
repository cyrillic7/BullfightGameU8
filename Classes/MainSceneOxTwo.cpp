//
//  MainScene.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainSceneOxTwo.h"
#include "GameConfig.h"
#include "DataModel.h"
#include "BaseAttributes.h"
//#include "TCPSocket.h"
//#include "CMD_LogonServer.h"
#include "TCPSocketControl.h"
//#include <tchar.h>
//#include "MD5.h"

//#include <thread>
//#include <iostream>
MainSceneOxTwo::MainSceneOxTwo()
:gameState(STATE_OBSERVER)
{
}
MainSceneOxTwo::~MainSceneOxTwo(){
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
CCScene* MainSceneOxTwo::scene()
{
    CCScene *scene = CCScene::create();
    MainSceneOxTwo *layer = MainSceneOxTwo::create();
    scene->addChild(layer);
	DataModel::sharedDataModel()->setMainSceneOxTwo(layer);
    return scene;
}
void MainSceneOxTwo::onEnter(){
	CCLayer::onEnter();
	addBg();
	initCardLayer();
	initPlayerLayer();
	initGameControl();
}
void MainSceneOxTwo::onExit(){
	CCLayer::onExit();
}
void MainSceneOxTwo::addBg(){
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/room1_n.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
}
void MainSceneOxTwo::initGameControl(){
	gameControl = GameControlOxTwo::create();
	this->addChild(gameControl, K_Z_ORDER_HUD);
}
//
void MainSceneOxTwo::initCardLayer(){
	cardLayer = CardLayer::create();
	this->addChild(cardLayer);
}
void MainSceneOxTwo::initPlayerLayer(){
	playerLayer = PlayerLayer::create();
	this->addChild(playerLayer);
}
//收到准备完成回调
void MainSceneOxTwo::onEventReadyFnish(){
	CCLog("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}
//收到发牌完成回调
void MainSceneOxTwo::onEventSendCardFnish(){
	
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
void MainSceneOxTwo::setGameStateWithUpdate(GameState gameState){
	setGameState(gameState);
	updateGameState();
}
void MainSceneOxTwo::setServerStateWithUpdate(GameState serverState){
	setServerState(serverState);
	updateServerState();
}
//更新状态
void MainSceneOxTwo::updateGameState(){
	//操作层更新状态
	gameControl->updateState();
	
	//玩家信息层更新状态
	playerLayer->updateState();
	//更新扑克层状态
	cardLayer->updateGameState();
}
void MainSceneOxTwo::updateServerState(){
	//更新扑克层状态
	cardLayer->updateServerState();
}

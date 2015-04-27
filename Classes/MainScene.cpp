//
//  MainScene.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainScene.h"
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
MainScene::MainScene()
:gameState(STATE_OBSERVER)
{
}
MainScene::~MainScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	TCPSocketControl::sharedTCPSocketControl()->stopSocket();

	GUIReader::purge();
	CCArmatureDataManager::purge();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();

	
	BaseAttributes *b = BaseAttributes::sharedAttributes();
	CC_SAFE_RELEASE_NULL(b);

}
CCScene* MainScene::scene()
{
    CCScene *scene = CCScene::create();
    MainScene *layer = MainScene::create();
    scene->addChild(layer);
	DataModel::sharedDataModel()->setMainScene(layer);
    return scene;
}
void MainScene::onEnter(){
	CCLayer::onEnter();
	addBg();
	initCardLayer();
	initPlayerLayer();
	initGameControl();
}
void MainScene::onExit(){
	CCLayer::onExit();
}
void MainScene::addBg(){
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/room1_n.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
}
void MainScene::initGameControl(){
	gameControl = GameControlOxTwo::create();
	this->addChild(gameControl, K_Z_ORDER_HUD);
}
//
void MainScene::initCardLayer(){
	cardLayer = CardLayer::create();
	this->addChild(cardLayer);
}
void MainScene::initPlayerLayer(){
	playerLayer = PlayerLayer::create();
	this->addChild(playerLayer);
}
//收到准备完成回调
void MainScene::onEventReadyFnish(){
	CCLog("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}
//收到发牌完成回调
void MainScene::onEventSendCardFnish(){
	
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
void MainScene::setGameStateWithUpdate(GameState gameState){
	setGameState(gameState);
	updateGameState();
}
void MainScene::setServerStateWithUpdate(GameState serverState){
	setServerState(serverState);
	updateServerState();
}
//更新状态
void MainScene::updateGameState(){
	//操作层更新状态
	gameControl->updateState();
	
	//玩家信息层更新状态
	playerLayer->updateState();
	//更新扑克层状态
	cardLayer->updateGameState();
}
void MainScene::updateServerState(){
	//更新扑克层状态
	cardLayer->updateServerState();
}

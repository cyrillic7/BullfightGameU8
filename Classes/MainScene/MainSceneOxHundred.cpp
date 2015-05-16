//
//  MainSceneOxHundred.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainSceneOxHundred.h"
#include "../Tools/GameConfig.h"
#include "../Tools/DataModel.h"
#include "../Tools/BaseAttributes.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "../Play/CardLayer/CardLayerHundred.h"
MainSceneOxHundred::MainSceneOxHundred()
:gameState(STATE_GAME_FREE)
{
}
MainSceneOxHundred::~MainSceneOxHundred(){
	CCLog("~ <<%s>>", __FUNCTION__);
	//TCPSocketControl::sharedTCPSocketControl()->stopSocket();

	GUIReader::purge();
	CCArmatureDataManager::purge();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();

	BaseAttributes *b = BaseAttributes::sharedAttributes();
	CC_SAFE_RELEASE_NULL(b);
}
CCScene* MainSceneOxHundred::scene()
{
    CCScene *scene = CCScene::create();
    MainSceneOxHundred *layer = MainSceneOxHundred::create();
    scene->addChild(layer);
	DataModel::sharedDataModel()->setMainSceneOxHundred(layer);
    return scene;
}
void MainSceneOxHundred::onEnter(){
	CCLayer::onEnter();
	addBg();
	initCardLayer();
	//initPlayerLayer();
	initGameControl();
}
void MainSceneOxHundred::onExit(){
	CCLayer::onExit();
}
void MainSceneOxHundred::addBg(){
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/room2.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scaleX=deviceSize.width/spriteBg->getContentSize().width;
	float scaleY=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScaleX(scaleX);
	spriteBg->setScaleY(scaleY);
}
void MainSceneOxHundred::initGameControl(){
	gameControl = GameControlOxHundred::create();
	this->addChild(gameControl, K_Z_ORDER_HUD);
}
//
void MainSceneOxHundred::initCardLayer(){
	cardLayer = CardLayerHundred::create();
	this->addChild(cardLayer,K_Z_ORDER_CARD);
}
void MainSceneOxHundred::initPlayerLayer(){
	playerLayer = PlayerLayer::create();
	this->addChild(playerLayer);
}
void MainSceneOxHundred::setGameStateWithUpdate(GameState gameState){
	setGameState(gameState);
	updateGameState();
}
void MainSceneOxHundred::setServerStateWithUpdate(GameState serverState){
	setServerState(serverState);
	updateServerState();
}
//更新状态
void MainSceneOxHundred::updateGameState(){
	//操作层更新状态
	gameControl->updateState();
	
	//玩家信息层更新状态
	//playerLayer->updateState();
	//更新扑克层状态
	cardLayer->updateGameState();
}
void MainSceneOxHundred::updateServerState(){
	//更新扑克层状态
	cardLayer->updateServerState();
}

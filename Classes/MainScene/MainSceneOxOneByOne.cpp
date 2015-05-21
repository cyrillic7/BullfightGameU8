//
//  MainScene.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainSceneOxOneByOne.h"
#include "../Tools/GameConfig.h"
#include "../Tools/DataModel.h"
#include "../Tools/BaseAttributes.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "../Play/CardLayer/CardLayerOneByOne.h"
#include "../Play/PlayerLayer/PlayerLayerOneByOne.h"
MainSceneOxOneByOne::MainSceneOxOneByOne()
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
	cardLayer = CardLayerOneByOne::create();
	this->addChild(cardLayer);
}
void MainSceneOxOneByOne::initPlayerLayer(){
	playerLayer = PlayerLayerOneByOne::create();
	this->addChild(playerLayer);
}
//收到准备完成回调
void MainSceneOxOneByOne::onEventReadyFnish(){
	CCLog("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}
//收到发牌完成回调
void MainSceneOxOneByOne::onEventSendCardFnish(){
	
}
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

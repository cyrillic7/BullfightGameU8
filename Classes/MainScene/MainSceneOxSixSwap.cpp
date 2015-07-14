//
//  MainScene.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainSceneOxSixSwap.h"
#include "../Tools/GameConfig.h"
#include "../Tools/DataModel.h"
#include "../Tools/BaseAttributes.h"
#include "../Play/CardLayer/CardLayerSixSwap.h"
#include "../Play/PlayerLayer/PlayerLayerSixSwap.h"
MainSceneOxSixSwap::MainSceneOxSixSwap()
{
}
MainSceneOxSixSwap::~MainSceneOxSixSwap(){
	CCLog("~ <<%s>>", __FUNCTION__);
	//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
	GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.Destroy(true);

}
CCScene* MainSceneOxSixSwap::scene()
{
    CCScene *scene = CCScene::create();
    MainSceneOxSixSwap *layer = MainSceneOxSixSwap::create();
    scene->addChild(layer);
	//DataModel::sharedDataModel()->setMainSceneOxOneByOne(layer);



    return scene;
}
void MainSceneOxSixSwap::onEnter(){
	//CCLayer::onEnter();
	addBg();
	initCardLayer();
	//initPlayerLayer();
	MainSceneBase::onEnter();
	initGameControl();
}
void MainSceneOxSixSwap::onExit(){
    
    GUIReader::purge();
    CCArmatureDataManager::purge();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    
    BaseAttributes *b = BaseAttributes::sharedAttributes();
    CC_SAFE_RELEASE_NULL(b);
    
	MainSceneBase::onExit();
	//CCLayer::onExit();
}
void MainSceneOxSixSwap::addBg(){
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/room_six_swap.jpg");
	this->addChild(spriteBg,-1);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScaleY(scale);
}
void MainSceneOxSixSwap::initGameControl(){
	gameControl = GameControlOxSixSwap::create();
	this->addChild(gameControl, K_Z_ORDER_HUD);
}
//
void MainSceneOxSixSwap::initCardLayer(){
	cardLayer = CardLayerSixSwap::create();
	this->addChild(cardLayer,K_Z_GI_CORD);
}
void MainSceneOxSixSwap::initPlayerLayer(){
	playerLayer = PlayerLayerSixSwap::create();
	this->addChild(playerLayer,K_Z_GI_PLAYER);
}
//收到准备完成回调
void MainSceneOxSixSwap::onEventReadyFnish(){
	CCLog("准备完成等待服务端响应.");
	setServerStateWithUpdate(STATE_WAIT);
}

//收到发牌完成回调
void MainSceneOxSixSwap::onEventSendCardFnish(){
	
}
void MainSceneOxSixSwap::setGameStateWithUpdate(GameState gameState){
	setGameState(gameState);
	updateGameState();
}
void MainSceneOxSixSwap::setServerStateWithUpdate(GameState serverState){
	setServerState(serverState);
	updateServerState();
}
//更新状态  
void MainSceneOxSixSwap::updateGameState(){
	//操作层更新状态
	gameControl->updateState();
	
	//玩家信息层更新状态
	playerLayer->updateState();
	//更新扑克层状态
	cardLayer->updateGameState();
}
void MainSceneOxSixSwap::updateServerState(){
	//更新扑克层状态
	cardLayer->updateServerState();
}

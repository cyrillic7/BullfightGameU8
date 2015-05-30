//
//  MainScene.cpp
//  BullfightGame
//
//  Created by on 15/3/16.
//
//

#include "MainSceneOxTwo.h"
#include "../Tools/GameConfig.h"
#include "../Tools/DataModel.h"
#include "../Tools/BaseAttributes.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "../Play/CardLayer/CardLayerTwo.h"
MainSceneOxTwo::MainSceneOxTwo()
{
}
MainSceneOxTwo::~MainSceneOxTwo(){
	CCLog("~ <<%s>>", __FUNCTION__);
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);

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
	//CCLayer::onEnter();
	addBg();
	initCardLayer();
	MainSceneBase::onEnter();
	//initPlayerLayer();
	initGameControl();
}
void MainSceneOxTwo::onExit(){
	MainSceneBase::onExit();
	//CCLayer::onExit();
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
	cardLayer = CardLayerTwo::create();
	this->addChild(cardLayer);
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

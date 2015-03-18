//
//  MainScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "MainScene.h"
#include "GameConfig.h"
#include "DataModel.h"
#include "BaseAttributes.h"
#include "GameLogic.h"
#include "TCPSocket.h"

MainScene::MainScene(){
}
MainScene::~MainScene(){
	CCLog("~ <<%s>>", __FUNCTION__);

	DataModel *m = DataModel::sharedDataModel();
	CC_SAFE_RELEASE_NULL(m);
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
	initHUD();
	initCard();
	testLogic();
}
void MainScene::onExit(){
	CCLayer::onExit();
}
void MainScene::addBg(){
	CCSprite *bg = CCSprite::create("res/room1_n.jpg");
	this->addChild(bg);
	bg->setPosition(ccp(SCENE_SIZE.width/2,SCENE_SIZE.height/2));
}
void MainScene::initHUD(){
	gameHUD = GameHUD::create();
	this->addChild(gameHUD, K_Z_ORDER_HUD);
}
//初始化扑克动画
void MainScene::initCard(){
	TCPSocket *tcp = new TCPSocket();
   /* ODSocket cdSocket;
    cdSocket.Init();
    bool isok=cdSocket.Create(AF_INET,SOCK_STREAM,0);
    bool iscon=cdSocket.Connect("127.0.0.1",8888);*/
	//lp= sevser;
    TCPSocket tc;
    tc.Init();
    tc.Create(AF_INET, SOCK_STREAM,0);
    bool isConnect=tc.Connect("125.88.145.41", 8100);
	//bool isConnect=tcp->Connect("125.88.145.41", 8100);
    CCLog("connect:%d",isConnect);
	CC_SAFE_DELETE(tcp);
}
void MainScene::testLogic(){
	BYTE tempCard[5] = {2,2,2,4,5};
	BYTE tempFristCard[5] = { 2, 2, 2, 4, 5 };
	BYTE tempNextCard[5] = { 2, 2, 2, 4, 6 };
	GameLogic *logic = new GameLogic();
	bool ox=logic->GetOxCard(tempCard, 5);
	CCLog("%d", ox);

	bool compare=logic->CompareCard(tempFristCard, tempNextCard, 5);
	CCLog("--:%d", compare);

	CC_SAFE_DELETE(logic);
}
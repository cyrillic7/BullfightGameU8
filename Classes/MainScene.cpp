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
#include "TCPSocket.h"
#include "StructLogon.h"
#include "CardLayer.h"
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
	initCardLayer();
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
void MainScene::initCardLayer(){
	CardLayer *cardLayer = CardLayer::create();
	this->addChild(cardLayer);
	/*//TCPSocket *tcp = new TCPSocket();
	//lp= sevser;
    TCPSocket tcp;
    tcp.Init();
    tcp.Create(AF_INET, SOCK_STREAM,0);
    bool isConnect=tcp.Connect("125.88.145.41", 8100);
	//bool isConnect = tcp.Connect("127.0.0.1", 7880);
	//bool isConnect=tcp->Connect("125.88.145.41", 8100);
    CCLog("connect:%d",isConnect);

	//tc.Send("123", strlen("123")+1, 0);
	//bool isSend=tc.SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS);
	//tc.SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, , );
	CMD_MB_LogonAccounts logonAccounts;
	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = 2.1;

	memset(logonAccounts.szAccounts, '/0', LEN_ACCOUNTS);
	memset(logonAccounts.szMachineID, '/0', LEN_MACHINE_ID);
	memset(logonAccounts.szMobilePhone, '/0', LEN_MOBILE_PHONE);
	memset(logonAccounts.szPassPortID, '/0', LEN_PASS_PORT_ID);
	memset(logonAccounts.szPassword, '/0', LEN_MD5);
	memset(logonAccounts.szPhoneVerifyID, '/0', LEN_PHONE_VERIFY_ID);

	//logonAccounts.szAccounts = _T(zh);

	logonAccounts.wModuleID = 0;

	bool isSend=tcp.SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("send:%d", isSend);
	bool isRead=tcp.OnSocketNotifyRead(0,0);
	CCLog("read:%d", isRead);
	
	//CC_SAFE_DELETE(tcp);*/
}
void MainScene::testLogic(){
	/*BYTE tempCard[5] = {2,2,3,8,6};
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

	CC_SAFE_DELETE(logic);*/
}
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
#include <tchar.h>
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
	testTcpSocket();
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
}
void MainScene::testTcpSocket(){
	TCPSocket tcp;
	tcp.Init();
	tcp.Create(AF_INET, SOCK_STREAM, 0);
	bool isConnect = tcp.Connect("125.88.145.41", 8100);
	//bool isConnect = tcp.Connect("192.168.0.104", 8100);
	CCLog("connect:%d", isConnect);

	CMD_MB_LogonAccounts logonAccounts;
	memset(&logonAccounts, 0, sizeof(CMD_MB_LogonAccounts));
	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = 2;

	int n = sizeof(CMD_MB_LogonAccounts);

	//CWHEncrypt::MD5Encrypt(szTempPassword, szPassword);
	//CWHEncrypt::MD5Encrypt("1234533", );
	 

    _tcscpy(logonAccounts.szPassword, _TEXT("12333333"));
	_tcscpy(logonAccounts.szAccounts, _TEXT("test"));
	_tcscpy(logonAccounts.szMachineID, _TEXT("ie3289423jjkj"));
	_tcscpy(logonAccounts.szMobilePhone, _TEXT("13623456543"));
	_tcscpy(logonAccounts.szPassPortID, _TEXT("12345678900987"));
	
	_tcscpy(logonAccounts.szPhoneVerifyID, _TEXT("18667653456"));
	 
	//unsigned char* output;
	//CCCrypto::MD5("ds", 2, output);
	 
	//CCCrypto::MD5(logonAccounts.szPassword, sizeof(logonAccounts.szPassword), output);
	logonAccounts.wModuleID = 0;


	bool isSend = tcp.SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("send:%d", isSend);
	bool isRead = tcp.OnSocketNotifyRead(0, 0);
	CCLog("read:%d", isRead);
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
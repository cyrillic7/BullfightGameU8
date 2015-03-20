//
//  MainScene.cpp
//  BullfightGame
//
//  Created by 寮�鎭�on 15/3/16.
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
#include "MMD5.h"
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
//鍒濆鍖栨墤鍏嬪姩鐢�
void MainScene::initCardLayer(){
	CardLayer *cardLayer = CardLayer::create();
	this->addChild(cardLayer);
}
void MainScene::testTcpSocket(){
	Init();
	Create(AF_INET, SOCK_STREAM, 0);
	bool isConnect=Connect("125.88.145.41", 8100);
	//bool isConnect=Connect("192.168.0.104", 8100);
	CCLog("Connect:%d", isConnect);

	CMD_MB_LogonAccounts logonAccounts;
	memset(&logonAccounts, 0, sizeof(CMD_MB_LogonAccounts));
	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = 17235969;

	//_tcscpy(logonAccounts.szPassword, _TEXT("123456"));
	_tcscpy(logonAccounts.szAccounts, _TEXT("z40144322"));
	_tcscpy(logonAccounts.szMachineID, _TEXT("ie3289423jjkj"));
	_tcscpy(logonAccounts.szMobilePhone, _TEXT("13623456543"));
	_tcscpy(logonAccounts.szPassPortID, _TEXT("12345678900987"));
	_tcscpy(logonAccounts.szPhoneVerifyID, _TEXT("1234567"));

	logonAccounts.wModuleID = 0;

	MMD5 m;
	MMD5::char8 str[] = "z12345678";
	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();

	_tcscpy(logonAccounts.szPassword, _TEXT(md5PassWord.c_str()));

	bool isSend = SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("send:%d", isSend);
	bool isRead = OnSocketNotifyRead(0, 0);
	CCLog("read:%d", isRead);

	//isRead = OnSocketNotifyRead(0, 0);
	//CCLog("2read:%d", isRead);

	//isRead = OnSocketNotifyRead(0, 0);
	//CCLog("3read:%d", isRead);
}
void MainScene::testLogic(){
	BYTE tempCard[5] = { 2, 2, 3, 8, 6 };
	bool ox = GetOxCard(tempCard, 5);
	CCLog("ox:%d", ox);
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
bool MainScene::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, void * pDataBuffer, WORD wDataSize){
	if (Command.wMainCmdID == MDM_GP_LOGON)
	{
		if (Command.wSubCmdID == SUB_GP_UPDATE_NOTIFY)
		{
			CCLog("升级提示");
		}
	}
	//登录失败
	if (Command.wMainCmdID == MDM_MB_LOGON)
	{
		if (Command.wSubCmdID == SUB_GP_LOGON_FAILURE)
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_MB_LogonSuccess));
			if (wDataSize < sizeof(CMD_MB_LogonSuccess)) return false;

			CMD_MB_LogonFailure *lf = (CMD_MB_LogonFailure*)pDataBuffer;
			LONG code = lf->lResultCode;
			TCHAR *describeStr = lf->szDescribeString;
			CCLog("%s", describeStr);
		}
	}
	//登录成功
	if (Command.wMainCmdID == MDM_MB_LOGON)
	{
		if (Command.wSubCmdID == SUB_GP_LOGON_SUCCESS)
		{
			//效验参数
			if (wDataSize != sizeof(CMD_MB_LogonSuccess)) return false;
			
			CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)pDataBuffer;
			CCLog("登录成功");
			//读取列表
			TCPSocket::OnSocketNotifyRead(0, 0);
		}
	}
	//获取列表
	if (Command.wMainCmdID == MDM_MB_SERVER_LIST)
	{
		if (Command.wSubCmdID == SUB_MB_LIST_FINISH)
		{
			//tagGameServer *gs = (tagGameServer*)pDataBuffer;
		}
	}
	return 1;
}

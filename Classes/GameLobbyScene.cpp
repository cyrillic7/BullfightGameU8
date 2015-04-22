//
//  GameLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameLobbyScene.h"
#include "Tools.h"
#include "GameConfig.h"
#include "PopDialogBoxUserInfo.h"
#include "PopDialogBoxLoading.h"
#include "ClassicLobbyScene.h"
#include "DataModel.h"
#include "LogonListerner.h"
#include "PopDialogBoxSetUp.h"
//#include <stdio.h>
#include "QueueData.h"
#include "MD5.h"
#include "TCPSocketControl.h"
#include "SEvent.h"
GameLobbyScene::GameLobbyScene()
:deleteSocket(false){
	scheduleUpdate();
}
GameLobbyScene::~GameLobbyScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	unscheduleUpdate();
	
	//移除对象
	this->removeAllChildrenWithCleanup(true);
	//清理数据
	GUIReader::purge();
	CCArmatureDataManager::purge();
	//清理纹理
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
CCScene* GameLobbyScene::scene()
{
    CCScene *scene = CCScene::create();
    GameLobbyScene *layer = GameLobbyScene::create();
    scene->addChild(layer);
    return scene;
}
void GameLobbyScene::onEnter(){
	BaseLobbyScene::onEnter();
	/*//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/main_bg.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);*/
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);

	/*UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuResetUser));

	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonSetUp"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuSetUp));*/
	UIButton* button=NULL;
	for (int i = 0; i < 3;i++)
	{
		button = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("buttonMode%d",i+1)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuSelectMode));
	}
	scroll=static_cast<UIScrollView*>(m_pWidget->getWidgetByName("ScrollView"));
	scroll->setInnerContainerSize(scroll->getContentSize());
	/*//用户名
	userName=static_cast<UILabel*>(m_pWidget->getWidgetByName("labelUserName"));
	//金币
	pLabelGoldCount=static_cast<UILabel*>(m_pWidget->getWidgetByName("LabelGoldCount"));*/
	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameLobbyScene::callbackData),S_L_LOGON,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameLobbyScene::onOpen),S_L_OPEN,NULL);
	initTCPLogon();
}

void GameLobbyScene::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_LOGON); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_OPEN); 

	BaseLobbyScene::onExit();
}
void GameLobbyScene::onOpen(CCObject *obj){
	CMD_MB_LogonAccounts logonAccounts;

	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = 17235969;

	strcpy(logonAccounts.szAccounts,"z40144322");
	//strcpy(logonAccounts.szAccounts,"zhangh189");

	strcpy(logonAccounts.szMachineID,"12");
	strcpy(logonAccounts.szMobilePhone,"32");
	strcpy(logonAccounts.szPassPortID,"12");
	strcpy(logonAccounts.szPhoneVerifyID,"1");
	

	logonAccounts.wModuleID = 210; //210为二人牛牛标示


	MD5 m;
	MD5::char8 str[] = "z12345678";

	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();

	strcpy(logonAccounts.szPassword,md5PassWord.c_str());

	bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("send:%d", isSend);
}
void GameLobbyScene::callbackData(CCObject *obj){
    QueueData *pData=(QueueData*)obj;
    CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)pData->sendData.sSendData;
    
	
	//CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)qData->pDataBuffer;
	//CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)obj;
	
	PopDialogBoxLoading *pdb = (PopDialogBoxLoading*)this->getChildByTag(189);
	pdb->removeFromParentAndCleanup(true);
	//////////////////////////////////////////////////////////////////////////
	//设置用户名
	char *name=DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName;
//	userName->setText(UTEXT(name));
	
	deleteSocket=true;

}
void GameLobbyScene::update(float dt){
	char *name=DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName;
	if(strcmp(userName->getStringValue(),"游客")==0&&strcmp(name, "") != 0)
	{
		userName->setText(Tools::GBKToUTF8(name));
	}
	if (deleteSocket)
	{
		TCPSocketControl::sharedTCPSocketControl()->stopSocket();
		deleteSocket=false;
	}
}
void GameLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox();
		break;
	default:
		break;
	}
}
void GameLobbyScene::menuSetUp(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			PopDialogBox *pdbSetUp = PopDialogBoxSetUp::create();
			this->addChild(pdbSetUp);
		}
		break;
	default:
		break;
	}
}
void GameLobbyScene::menuSelectMode(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *button = (UIButton*)pSender;
		enterLobbyByMode(button->getTag());
	}
		break;
	default:
		break;
	}
}

void GameLobbyScene::popDialogBox(){
	PopDialogBox *pdb = PopDialogBoxUserInfo::create();
	this->addChild(pdb);
}
void GameLobbyScene::enterLobbyByMode(int mode){
	switch (mode)
	{
	case MODE_CLASSIC:
		{
				Tools::setTransitionAnimation(0, 0, ClassicLobbyScene::scene());
		}
	
		break;
	default:
		break;
	}
}
void GameLobbyScene::initTCPLogon(){
	TCPSocketControl *tcp=TCPSocketControl::sharedTCPSocketControl();
	tcp->ip="125.88.145.41";
	//tcp->ip="192.168.1.122";
	tcp->port=8100;
	tcp->listerner=new LogonListerner();
	tcp->startSendThread();
	//delete tcp->listerner;
	/*SocketThread *socketThread=SocketThread::GetInstance();
	//SocketThread::GetInstance()->getSocket().SetListerner(new DefaultListerner());
	socketThread->start();
	*/
	PopDialogBox *pdb = PopDialogBoxLoading::create();
	this->addChild(pdb);
	pdb->setTag(189);
}
/*bool GameLobbyScene::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize){
	CCLog("========================");
	return 1;
}*/

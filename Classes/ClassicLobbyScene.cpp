//
//  ClassicLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "ClassicLobbyScene.h"
#include "Tools.h"
#include "GameConfig.h"
#include "PopDialogBoxUserInfo.h"
#include "GameLobbyScene.h"
#include "MainScene.h"
#include "CMD_GameServer.h"
#include "TCPSocketControl.h"
#include "GameListerner.h"
#include "DataModel.h"
#include "PopDialogBoxLoading.h"
#include "MD5.h"
#include "SEvent.h"
ClassicLobbyScene::ClassicLobbyScene()
:isDeleteList(false)
,isEnterGame(false)
{
	DataModel::sharedDataModel()->isSit=false;
	scheduleUpdate();
}
ClassicLobbyScene::~ClassicLobbyScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	//
	unscheduleUpdate();
	this->removeAllChildrenWithCleanup(true);
	GUIReader::purge();
	CCArmatureDataManager::purge();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
CCScene* ClassicLobbyScene::scene()
{
    CCScene *scene = CCScene::create();
    ClassicLobbyScene *layer = ClassicLobbyScene::create();
    scene->addChild(layer);
    return scene;
}
void ClassicLobbyScene::onEnter(){
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


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIClassicLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);

	/*UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuResetUser));*/
	UIButton* button=NULL;
	for (int i = 0; i < 4; i++)
	{
		button = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("ButtonScene%d",i+1)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuStar));
		//房间描述
		UILabel *description=static_cast<UILabel*>(m_pWidget->getWidgetByName(CCString::createWithFormat("Label%d",i)->getCString()));
		description->setText(Tools::GBKToUTF8(DataModel::sharedDataModel()->tagGameServerList[i]->szDescription));
	}
	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onPlay),S_L_PLAY,NULL);

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onConfigFinish),S_L_CONFIG_FINISH,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onOpen),S_L_OPEN,NULL);

	//initTCPLogon();
}
void ClassicLobbyScene::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_PLAY); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_CONFIG_FINISH); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_OPEN); 

	BaseLobbyScene::onExit();
}
void ClassicLobbyScene::initTCPLogon(int index){

	for (int i = 0; i < DataModel::sharedDataModel()->tagGameServerList.size(); i++)
	{
		CCLog("%d  %s %d",DataModel::sharedDataModel()->tagGameServerList[i]->wSortID,
			Tools::GBKToUTF8(DataModel::sharedDataModel()->tagGameServerList[i]->szServerName)
			,DataModel::sharedDataModel()->tagGameServerList[i]->wServerPort);
	}
	/*TCPLogonID *tcpID=TCPLogonID::create();
	this->addChild(tcpID);
	tcpID->setTag(999);
	tcpID->startSendThread();*/
	PopDialogBox *pdb = PopDialogBoxLoading::create();
	this->addChild(pdb);
	pdb->setTag(189);

	TCPSocketControl *tcp=TCPSocketControl::sharedTCPSocketControl();
	tcp->ip=DataModel::sharedDataModel()->tagGameServerList[index]->szServerAddr;
	tcp->port=DataModel::sharedDataModel()->tagGameServerList[index]->wServerPort;
	CCLog("%s:%d",tcp->ip,tcp->port);
	tcp->listerner=new GameListerner();
	tcp->startSendThread();
}
void ClassicLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox();
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::menuStar(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *button=(UIButton*)pSender;
		initTCPLogon(button->getTag()-1);
		//TCPLogonID *tcpID=(TCPLogonID *)this->getChildByTag(999);
		/**/
		//tcpID->sendData();
		//enterMainSceneByMode(((UIButton*)pSender)->getTag());
	}
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::popDialogBox(){
	TCPSocketControl::sharedTCPSocketControl()->stopSocket();
	Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene());
}
void ClassicLobbyScene::enterMainSceneByMode(int mode){
	switch (mode)
	{
	case LEVEL_0:
		Tools::setTransitionAnimation(0, 0, MainScene::scene());
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::update(float delta){
	if (DataModel::sharedDataModel()->isSit)
	{
		DataModel::sharedDataModel()->isSit=false;
		enterMainSceneByMode(1);
	}
}
void ClassicLobbyScene::onPlay(CCObject *obj){
	//enterMainSceneByMode(1);
	PopDialogBox *pdb=(PopDialogBoxLoading*)this->getChildByTag(189);
	pdb->removeFromParentAndCleanup(true);

	isEnterGame=true;
}
void ClassicLobbyScene::onConfigFinish(CCObject *obj){
	//
	CMD_GR_UserSitDown sit;
	sit.wTableID=39;
	sit.wChairID=1;

	bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
	//bool isSend=tcpID->ts.SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
	CCLog("Classic 坐下:%d",isSend);
}
void ClassicLobbyScene::onOpen(CCObject *obj){
	CMD_GR_LogonUserID logonUserID;
	memset(&logonUserID, 0, sizeof(CMD_GR_LogonUserID));

	logonUserID.dwFrameVersion=VERSION_FRAME;
	logonUserID.dwPlazaVersion=VERSION_PLAZA;
	logonUserID.dwProcessVersion= VERSION_CLIENT;
	logonUserID.dwUserID=DataModel::sharedDataModel()->logonSuccessUserInfo->dwUserID;
	strcpy(logonUserID.szMachineID,"12");
	strcpy(logonUserID.szPassPortID,"12");

	MD5 m;
	MD5::char8 str[] = "z12345678";
	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();
	strcpy(logonUserID.szPassword,md5PassWord.c_str());

	strcpy(logonUserID.szPhoneVerifyID,"1");
	logonUserID.wKindID=DataModel::sharedDataModel()->tagGameServerList[0]->wKindID;

	CCLog("房间名:%d",DataModel::sharedDataModel()->tagGameServerList[0]->wServerPort);

	int luidSize=sizeof(CMD_GR_LogonUserID);
	bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GR_LOGON, SUB_GR_LOGON_USERID, &logonUserID, sizeof(logonUserID));
	CCLog("send:%d", isSend);

	if (!isSend)
	{
		//stopTcpSocket();
		return;
	}
}
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
#include "cmd_ox.h"
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
	pBUserInfo->loadTextureNormal("back.png",UI_TEX_TYPE_PLIST);
	pBUserInfo->loadTexturePressed("back_press.png",UI_TEX_TYPE_PLIST);
	pBUserInfo->setScale9Enabled(false);
	pBUserInfo->ignoreContentAdaptWithSize(true);
	pBUserInfo->setSize(CCSize(51,40));
	
	pBUserInfo->getChildByName("Image_23")->setVisible(false);

	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIClassicLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);

	userName->setText(Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName));
	/*UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuResetUser));*/
	UIButton* button=NULL;
	for (int i = 0; i < DataModel::sharedDataModel()->tagGameServerListOxTwo.size(); i++)
	{
		button = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("ButtonScene%d",i+1)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuStar));
		//房间描述
		UILabel *description=static_cast<UILabel*>(m_pWidget->getWidgetByName(CCString::createWithFormat("Label%d",i)->getCString()));
		description->setText(Tools::GBKToUTF8(DataModel::sharedDataModel()->tagGameServerListOxTwo[i]->szDescription));
	}

	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onPlay),S_L_PLAY,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onConfigFinish),S_L_CONFIG_FINISH,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onOpen),S_L_OPEN,NULL);
}
void ClassicLobbyScene::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_PLAY); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_CONFIG_FINISH); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_OPEN); 

	BaseLobbyScene::onExit();
}
void ClassicLobbyScene::initTCPLogon(int index){
	PopDialogBox *pdb = PopDialogBoxLoading::create();
	this->addChild(pdb);
	pdb->setTag(TAG_POP_DIALOG_BOX);

	TCPSocketControl *tcp=TCPSocketControl::sharedTCPSocketControl();
	tcp->ip=DataModel::sharedDataModel()->tagGameServerListOxTwo[index]->szServerAddr;
	tcp->port=DataModel::sharedDataModel()->tagGameServerListOxTwo[index]->wServerPort;
	CCLog("-----------ip:%s     port:%d<<%s>>",tcp->ip,tcp->port,__FUNCTION__);
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
	}
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::popDialogBox(){
	if (TCPSocketControl::sharedTCPSocketControl()->listerner)
	{
		TCPSocketControl::sharedTCPSocketControl()->stopSocket();
	}
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
	MessageQueue::update(delta);
	/*if (DataModel::sharedDataModel()->isSit)
	{
		DataModel::sharedDataModel()->isSit=false;
		enterMainSceneByMode(1);
	}*/
}
void ClassicLobbyScene::onPlay(CCObject *obj){
	//enterMainSceneByMode(1);
	PopDialogBox *pdb=(PopDialogBoxLoading*)this->getChildByTag(TAG_POP_DIALOG_BOX);
	pdb->removeFromParentAndCleanup(true);

	isEnterGame=true;
}
void ClassicLobbyScene::onConfigFinish(CCObject *obj){
#if (DEBUG_TEST==0)
	CMD_GR_UserSitDown sit;
	sit.wTableID=38;
	//sit.wChairID=abs(rand()%2);
	sit.wChairID=1;
	bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
	CCLog("Classic 坐下:%d",isSend);
#endif
}
void ClassicLobbyScene::onOpen(CCObject *obj){
#if (DEBUG_TEST==0)
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
	logonUserID.wKindID=DataModel::sharedDataModel()->tagGameServerListOxTwo[0]->wKindID;

	CCLog("房间名:%d",DataModel::sharedDataModel()->tagGameServerListOxTwo[0]->wServerPort);

	int luidSize=sizeof(CMD_GR_LogonUserID);
	bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GR_LOGON, SUB_GR_LOGON_USERID, &logonUserID, sizeof(logonUserID));
	CCLog("send:%d", isSend);
#endif
#if (DEBUG_TEST==1)
	CMD_GR_LogonMobile logonMobile;
	memset(&logonMobile, 0, sizeof(CMD_GR_LogonMobile));

	logonMobile.wGameID=KIND_ID;
	logonMobile.dwProcessVersion=VERSION_CLIENT;
	//设备类型
	logonMobile.cbDeviceType=DEVICE_TYPE_ANDROID;
	logonMobile.wBehaviorFlags=BEHAVIOR_LOGON_IMMEDIATELY;
	logonMobile.wPageTableCount=10;

	logonMobile.dwUserID=DataModel::sharedDataModel()->userInfo->dwUserID;

	MD5 m;
	//MD5::char8 str[] = "z12345678";
	//m.ComputMd5(str,sizeof(str)-1);
	m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(),DataModel::sharedDataModel()->sLogonPassword.length());
	std::string md5PassWord = m.GetMd5();
	strcpy(logonMobile.szPassword,md5PassWord.c_str());

	strcpy(logonMobile.szMachineID,"123");

	bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &logonMobile, sizeof(logonMobile));
#endif
	
}
void ClassicLobbyScene::onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_GR_LOGON:
		onEventLogon(wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GR_USER:
		onSubUserState(wSubCmdID,pDataBuffer,wDataSize);
		break;
	default:
		CCLog("main:%d  sub:%d<<%s>>",wMainCmdID,wSubCmdID,__FUNCTION__);
		break;
	}
}
//登录
void ClassicLobbyScene::onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_UPDATE_NOTIFY:
		{
			//效验参数
				assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
			if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return;

			CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
			CCLog("升级提示");
		}
		break;
	case SUB_GR_LOGON_SUCCESS:
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_LogonSuccess));
			if (wDataSize < sizeof(CMD_GR_LogonSuccess)) return;

			CMD_GR_LogonSuccess *lf = (CMD_GR_LogonSuccess*)pDataBuffer;
			CCLog("登录成功");
		}
		break;
	case SUB_GR_LOGON_FINISH:{
			CCLog("登录完成");
		 }
		break;
	case SUB_GR_LOGON_FAILURE:
		{
			CMD_GR_LogonFailure *lf = (CMD_GR_LogonFailure*)pDataBuffer;
			CCLog("登录失败:%s",Tools::GBKToUTF8(lf->szDescribeString));
		}
		break;
	default:
		break;
	}
}
//用户状态
void ClassicLobbyScene::onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	CMD_GR_UserStatus *info= (CMD_GR_UserStatus*)pDataBuffer;
	switch (info->UserStatus.cbUserStatus)
	{
	case US_SIT://坐下
		{
			//CCLog("state==sit-----------%ld<<%s>>",info->dwUserID,__FUNCTION__);
			if (info->dwUserID==DataModel::sharedDataModel()->userInfo->dwUserID){
				unscheduleUpdate();
				//DataModel::sharedDataModel()->isSit=true;
				CCLog("坐下:table: %d desk:%d",info->UserStatus.wTableID,info->UserStatus.wChairID);
				DataModel::sharedDataModel()->userInfo->wTableID=info->UserStatus.wTableID;
				DataModel::sharedDataModel()->userInfo->wChairID=info->UserStatus.wChairID;

				enterMainSceneByMode(1);
			/*	//构造数据
				CMD_GF_GameOption GameOption;
				GameOption.dwFrameVersion=VERSION_FRAME;
				GameOption.cbAllowLookon=0;
				GameOption.dwClientVersion=VERSION_CLIENT;
				//发送
				bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
				if (isSend)
				{
					enterMainSceneByMode(1);
				}*/
			}
		}
		break;
	case US_FREE://站立
		{
			CCLog("state==free-----------%ld",info->dwUserID);
			if (info->dwUserID==DataModel::sharedDataModel()->userInfo->dwUserID)
			{
				//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_US_FREE,NULL);
			}else
			{

			}
		}
		break;
	case US_READY://同意
		{
			CCLog("state==ready-----------%ld<<%s>>",info->dwUserID,__FUNCTION__);
		}
		break;
	case US_PLAYING:
		{
			if (info->dwUserID==DataModel::sharedDataModel()->userInfo->dwUserID)
			{
				//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_CONFIG_FINISH,NULL);
			}
			CCLog("state==playing-----------%ld",info->dwUserID);
		}
		break;
	default:
		CCLog("state==Other userID:%ld 状态：%d",info->dwUserID,info->UserStatus.cbUserStatus);
		break;
	}
}
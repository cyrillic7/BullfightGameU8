//
//  LogonScene.cpp
//  
//
//  Created by on 15/3/16.
//
//

#include "LogonScene.h"
#include "Tools/Tools.h"
#include "PopDialogBox/PopDialogBoxLoading.h"
#include "PopDialogBox/PopDialogBoxLogonAccount.h"
#include "GameLobby/GameLobbyScene.h"
#include "Network/ListernerThread/LogonGameListerner.h"
#include "Network/MD5/MD5.h"
#include "Network/CMD_Server/cmd_ox.h"
LogonScene::LogonScene(){
	readRMS();
	scheduleUpdate();
}
LogonScene::~LogonScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_GAME);
	//移除对象
	this->removeAllChildrenWithCleanup(true);
	//清理数据
	GUIReader::purge();
	CCArmatureDataManager::purge();
	//清理纹理 
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
CCScene* LogonScene::scene()
{
    CCScene *scene = CCScene::create();
    LogonScene *layer = LogonScene::create();
    scene->addChild(layer);
    return scene;
}
void LogonScene::onEnter(){
	CCLayer::onEnter();
	//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/logon.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
	//////////////////////////////////////////////////////////////////////////
	//创建UI层
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);
	//加载UI层
	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UILogon.ExportJson)));
	m_pWidget->addWidget(pWidget);
	//绑定按键
	UIButton* button;
	for (int i = 0; i < 4; i++)
	{
		button  = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("ButtonLogon%d",i)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&LogonScene::onMenuLogon));
	}
	logonGameByAccount();
}
void LogonScene::onExit(){
	CCLayer::onExit();
}
void LogonScene::onMenuLogon(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			UIButton *pBtn =(UIButton *)pSender;
			switch (pBtn->getTag())
			{
			case LOGON_ACCOUNT:
				{
					//logonGame();
					PopDialogBox *pLoading = PopDialogBoxLogonAccount::create();
					this->addChild(pLoading);
				}
				break;
			case LOGON_QQ:
				{
					//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_GAME);
				}
				break;
			default:
				break;
			}
			
		}
		break;
	default:
		break;
	}
}
void LogonScene::update(float delta){
	MessageQueue::update(delta);
}
//登录游戏////////////////////////////////////////////////////////////////////////
void LogonScene::logonGameByAccount(){
	PopDialogBox *box=PopDialogBoxLoading::create();
	this->addChild(box,10,TAG_LOADING);
	TCPSocket *tcp=getSocket();
	if (tcp)
	{
		tcp->createSocket("125.88.145.41",PORT_LOGON,new LogonGameListerner());
	}
}
//读取网络消息回调
void LogonScene::onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
		case MDM_MB_SOCKET://socket连接成功
			onEventConnect(wSubCmdID,pDataBuffer,wDataSize);
			break;
		case MDM_MB_LOGON://登录 
			onEventLogon(wSubCmdID,pDataBuffer,wDataSize);
			break;
		case MDM_MB_SERVER_LIST://列表信息
			onEventServerList(wSubCmdID,pDataBuffer,wDataSize);
			break;
	default:
		CCLog("other:%d   %d<<%s>>",wMainCmdID,wSubCmdID,__FUNCTION__);
		break;
	}
}
//连接成功
void LogonScene::onEventConnect(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_SOCKET_OPEN:
		{
			CMD_MB_LogonAccounts logonAccounts;
			logonAccounts.cbDeviceType = 2;
			logonAccounts.dwPlazaVersion=VERSION_PLAZA;
			strcpy(logonAccounts.szAccounts,DataModel::sharedDataModel()->sLogonAccount.c_str());
			//strcpy(logonAccounts.szAccounts,"zhangh189");
			strcpy(logonAccounts.szMachineID,"12");
			strcpy(logonAccounts.szMobilePhone,"32");
			strcpy(logonAccounts.szPassPortID,"12");
			strcpy(logonAccounts.szPhoneVerifyID,"1");
			//游戏标识
			for (int i = 0; i < 10; i++)
			{
				logonAccounts.wModuleID[i] =0;
			}
			logonAccounts.wModuleID[0] = 210; //210为二人牛牛标示
			logonAccounts.wModuleID[1] = 30; //30为百人牛牛标示
			logonAccounts.wModuleID[2] = 130; //1002为通比牛牛标示

			MD5 m;
			m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(),DataModel::sharedDataModel()->sLogonPassword.length());
			std::string md5PassWord = m.GetMd5();
			strcpy(logonAccounts.szPassword,md5PassWord.c_str());
			bool isSend =getSocket()->SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
			CCLog("Logon:send:%d", isSend);
		}
		break;
	default:
		break;
	}
}
//登录消息
void LogonScene::onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_MB_LOGON_SUCCESS:
		{
			//效验参数
			if (wDataSize != sizeof(CMD_MB_LogonSuccess)) return ;
			CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)pDataBuffer;
			CCLog("登录成功 %ld %s",ls->dwUserID,Tools::GBKToUTF8(ls->szNickName));
			//赋值
			strcpy(DataModel::sharedDataModel()->userInfo->szNickName,ls->szNickName);
			DataModel::sharedDataModel()->userInfo->lScore=ls->lUserScore;
			DataModel::sharedDataModel()->userInfo->dwGameID=ls->dwGameID;
			DataModel::sharedDataModel()->userInfo->dwUserID=ls->dwUserID;
			DataModel::sharedDataModel()->userInfo->cbGender=ls->cbGender;
			DataModel::sharedDataModel()->userInfo->wFaceID=ls->wFaceID;

			Tools::saveStringByRMS(RMS_LOGON_ACCOUNT,DataModel::sharedDataModel()->sLogonAccount);
			Tools::saveStringByRMS(RMS_LOGON_PASSWORD,DataModel::sharedDataModel()->sLogonPassword);
		}
		break;
	case SUB_MB_LOGON_FAILURE:
		{
			//效验参数
			//assert(wDataSize >= sizeof(CMD_MB_LogonFailure));
			//if (wDataSize < sizeof(CMD_MB_LogonFailure)) return;
			CMD_MB_LogonFailure *lf = (CMD_MB_LogonFailure*)pDataBuffer;
			long code = lf->lResultCode;
			char *describeStr = lf->szDescribeString;
			this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
			CCLog("登录失败:%s",Tools::GBKToUTF8(describeStr));
		}
		break;
	case SUB_MB_UPDATE_NOTIFY:
		{
			assert(wDataSize >= sizeof(CMD_MB_UpdateNotify));
			if (wDataSize < sizeof(CMD_MB_UpdateNotify)) return;
			CMD_MB_UpdateNotify *lf = (CMD_MB_UpdateNotify*)pDataBuffer;
			CCLog("升级提示");
		}
		break;
	default:
		break;
	}
}
//获取列表
void LogonScene::onEventServerList(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_MB_LIST_KIND:
		{
			//效验参数
			//if (wDataSize != sizeof(tagGameKind)) return false;
			int size=sizeof(tagGameKind);
			int count=wDataSize/sizeof(tagGameKind);
			BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
			CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

			for (int i = 0; i < count; i++)
			{
				tagGameKind *gs = (tagGameKind*)(cbDataBuffer + i*sizeof(tagGameKind));
				CCLog("---<<%s>>",__FUNCTION__);
			}
			CCLog("获取游戏种类");
		}
		break;
	case SUB_MB_LIST_SERVER://房间列表
		{
			int gameServerSize = sizeof(tagGameServer);
			int serverCount = wDataSize / gameServerSize;

			BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
			CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

			DataModel::sharedDataModel()->removeTagGameServerList(DataModel::sharedDataModel()->tagGameServerListOxTwo);
			DataModel::sharedDataModel()->removeTagGameServerList(DataModel::sharedDataModel()->tagGameServerListOxHundred);
			DataModel::sharedDataModel()->removeTagGameServerList(DataModel::sharedDataModel()->tagGameServerListOxOneByOne);
			for (int i = 0; i < serverCount; i++)
			{
				void * pDataBuffer = cbDataBuffer + i*sizeof(tagGameServer);
				tagGameServer *gameServer = (tagGameServer*)pDataBuffer;
				tagGameServer *tempTag=new tagGameServer();
				memcpy(tempTag,gameServer,sizeof(tagGameServer));
				//memcoyp(gameServer,0,sizeof(tagGameServer));
				if (tempTag->wKindID==KIND_ID)
				{
					DataModel::sharedDataModel()->tagGameServerListOxTwo.push_back(tempTag);
				}else if(tempTag->wKindID==30)
				{
					DataModel::sharedDataModel()->tagGameServerListOxHundred.push_back(tempTag);
				}else
				{
					DataModel::sharedDataModel()->tagGameServerListOxOneByOne.push_back(tempTag);
					CCLog("port %d  %d  %s<<%s>>",tempTag->wServerPort,tempTag->wSortID,Tools::GBKToUTF8(tempTag->szDescription),__FUNCTION__);
				}
				
				//sort(DataModel::sharedDataModel()->tagGameServerList.begin(), DataModel::sharedDataModel()->tagGameServerList.end(), less_second);
				
              //  CCLog("port-----:%d ",tempTag->wServerPort);
			}
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListOxTwo);
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListOxHundred);
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListOxOneByOne);
		}
		break;
	case SUB_MB_LIST_FINISH:
		{
			unscheduleUpdate();
			TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_GAME);
			Tools::setTransitionAnimation(0,0,GameLobbyScene::scene());
		}
		break;
	default:
		CCLog("other:%d<<%s>>",wSubCmdID,__FUNCTION__);
		break;
	}
}
/************************************************************************/
/* 存档                                                                     */
/************************************************************************/
void LogonScene::readRMS(){
	if (isHaveSaveFile()) {
		DataModel::isMusic = Tools::getBoolByRMS(RMS_IS_MUSIC);
		DataModel::isSound = Tools::getBoolByRMS(RMS_IS_SOUND);
	}
	else{
		this->initRSM();
	}
}
void LogonScene::initRSM(){
	Tools::saveBoolByRMS(RMS_IS_MUSIC, true);
	Tools::saveBoolByRMS(RMS_IS_SOUND, true);
	Tools::saveStringByRMS(RMS_LOGON_ACCOUNT,"");
	Tools::saveStringByRMS(RMS_LOGON_PASSWORD,"");
}
bool LogonScene::isHaveSaveFile(){
	if (!Tools::getBoolByRMS("isHaveSaveFileXml"))
	{
		Tools::saveBoolByRMS("isHaveSaveFileXml", true);
		return false;
	}
	else{
		return true;
	}
}

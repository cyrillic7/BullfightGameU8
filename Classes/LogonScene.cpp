//
//  LogonScene.cpp
//  
//
//  Created by on 15/3/16.
//
//

#include "LogonScene.h"
#include "Tools.h"
#include "PopDialogBoxLoading.h"
#include "PopDialogBoxLogonAccount.h"
#include "GameLobbyScene.h"
#include "LogonGameListerner.h"
#include "MD5.h"
LogonScene::LogonScene(){
	readRMS();
	scheduleUpdate();
}
LogonScene::~LogonScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
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
					TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_GAME);
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
		tcp->createSocket("125.88.145.41",8100,new LogonGameListerner());
	}
}
//连接成功
void LogonScene::onEventSokcet(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_SOCKET_OPEN:
		{
			CMD_GP_LogonAccounts lAccounts;
			lAccounts.dwPlazaVersion=VERSION_PLAZA;
			MD5 m;
			m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(),DataModel::sharedDataModel()->sLogonPassword.length());
			std::string md5PassWord = m.GetMd5();
			strcpy(lAccounts.szPassword,md5PassWord.c_str());
			strcpy(lAccounts.szAccounts,DataModel::sharedDataModel()->sLogonAccount.c_str());
			bool isSend=getSocket()->SendData(MDM_GP_LOGON,SUB_GP_LOGON_ACCOUNTS,&lAccounts,sizeof(CMD_GP_LogonAccounts));
			CCLog("send:%d<<%s>>",isSend,__FUNCTION__);
		}
		break;
	default:
		break;
	}
}
void LogonScene::onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_SUCCESS:
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_GP_LogonSuccess));
			if (wDataSize < sizeof(CMD_GP_LogonSuccess)) return;
			CMD_GP_LogonSuccess *ls = (CMD_GP_LogonSuccess*)pDataBuffer;
			//拷贝数据
			DataModel::sharedDataModel()->userInfo->dwUserID=ls->dwUserID;
			DataModel::sharedDataModel()->userInfo->dwGameID=ls->dwGameID;
			DataModel::sharedDataModel()->userInfo->lScore=ls->lUserScore;
			DataModel::sharedDataModel()->userInfo->cbGender=ls->cbGender;
			strcpy(DataModel::sharedDataModel()->userInfo->szNickName,ls->szNickName);
			
			Tools::saveStringByRMS(RMS_LOGON_ACCOUNT,DataModel::sharedDataModel()->sLogonAccount);
			Tools::saveStringByRMS(RMS_LOGON_PASSWORD,DataModel::sharedDataModel()->sLogonPassword);
			
			
		}
		break;
	case SUB_GP_LOGON_FAILURE:
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_MB_LogonSuccess));
			if (wDataSize < sizeof(CMD_MB_LogonSuccess)) return;

			CMD_MB_LogonFailure *lf = (CMD_MB_LogonFailure*)pDataBuffer;
			long code = lf->lResultCode;
			char *describeStr = lf->szDescribeString;
			//CCLog("%s", describeStr);
			this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
			CCLog("登录失败:%s",Tools::GBKToUTF8(describeStr));
		}
		break;
	case SUB_GP_LOGON_FINISH:
		{
			TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_GAME);
			Tools::setTransitionAnimation(0,0,GameLobbyScene::scene());
		}
		break;
	case SUB_GP_VALIDATE_MBCARD:
		CCLog("validate_mbcard<<%s>>",__FUNCTION__);
		break;
	case SUB_GP_UPDATE_NOTIFY:
		CCLog("update_notify<<%s>>",__FUNCTION__);
		break;
	default:
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
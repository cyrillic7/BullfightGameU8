//
//  LogonScene.cpp
//  
//
//  Created by on 15/3/16.
//
//

#include "LogonScene.h"
#include "../Tools/Tools.h"
#include "../PopDialogBox/PopDialogBoxLoading.h"
#include "../PopDialogBox/PopDialogBoxLogonAccount.h"
#include "../PopDialogBox/PopDialogBoxTipInfo.h"

#include "../GameLobby/GameLobbyScene.h"
#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/ListernerThread/LobbyGameListerner.h"
#include "../Network/MD5/MD5.h"
#include "../Network/CMD_Server/cmd_ox.h"
#include "../Tools/BaseAttributes.h"
#include "../Platform/coPlatform.h"

LogonScene::LogonScene()
	:eLogonType(LOGON_ACCOUNT)
{
	DataModel *m = DataModel::sharedDataModel();
	CC_SAFE_RELEASE_NULL(m);

	readRMS();
	scheduleUpdate();

	/*char szJson[512] = { 0 };
	sprintf(szJson,
		"{\"act\":20,\"type\":%d,\"item\":%d,\"cost\":%d,\"amount\":%d,\"name\":\"%s\",\"uid\":\"%s\",\"order\":\"%s\"}",
		type, item, coPayGetCostAmount(item), coPayGetProductAmount(item), coPayGetProductName(item).c_str(),
		uid.c_str(), order.c_str());
	log("%s", szJson);*/
	/*const char* text = "{\"data\": [\"UIActivity.plist\",\"UIVip.plist\"]}";
	//const char* text = "{\"record\":{\"data\":20130101,\"rate\":23,\"buy\":1},\"record\":{\"data\":45,\"rate\":67,\"buy\":1},\"record\":{\"data\":45,\"rate\":76,\"buy\":10}}";
	Json *root = Json_create(text);

	Json* _date=Json_getItem(root, "data");
	if (_date->type == Json_Array)
	{
		for (int i = 0; i <Json_getSize(_date); i++)
		{
			Json *s=Json_getItemAt(_date,i);
			CCLog("--------===================== %s<<%s>>",s->valuestring, __FUNCTION__);
		}
	}
	Json* _record = root->child;
	while (_record)
	{
		//Json* _date = Json_getItem(_record, "data");
		//if (_date->type == Json_String)
		{
		//	const char * date = _date->valuestring;
			//CCLOG("Date: %s", date);
		}
		
		Json* _rate = Json_getItem(_record, "rate");
		if (_rate->type == Json_Number)
		{
			int rate = _rate->valueint;
			CCLOG("Rate: %d", rate);
		}
		Json* _buy = Json_getItem(_record, "buy");
		if (_buy->type == Json_Number)
		{
			int buy = _buy->valueint;
			CCLOG("Buy: %d", buy);
		}
		_record = _record->next;
	}
	Json_dispose(root);*/



	CCLog("--------:%s <<%s>>", platformAction("{\"act\":100}").c_str(), __FUNCTION__);
	
}
LogonScene::~LogonScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_GAME);
	
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
    m_pWidget = UILayer::create();
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
	//logonGameByAccount();
}
void LogonScene::onExit(){
    //移除对象
    this->removeAllChildrenWithCleanup(true);
    //清理数据
    GUIReader::purge();
    CCArmatureDataManager::purge();
    //清理纹理
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCLayer::onExit();
}
void LogonScene::onMenuLogon(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			UIButton *pBtn =(UIButton *)pSender;
			setLogonType((LogonType)pBtn->getTag());
			switch (pBtn->getTag())
			{
			case LOGON_ACCOUNT:
				{
					//logonGame();
					PopDialogBox *pLogon = PopDialogBoxLogonAccount::create();
					this->addChild(pLogon);
				}
				break;
			case LOGON_REGISTER://注册
			{
				PopDialogBoxRegistered *pRegistered = PopDialogBoxRegistered::create();
				this->addChild(pRegistered);
				pRegistered->setIPopAssistRegistered(this);
			}
				break;
			case LOGON_QQ:
			{
                m_pWidget->setTouchEnabled(false);
				platformAction("{\"act\":200 ,\"url\":\"http://www.999xw.com/QQLogin.aspx\"}").c_str();
			}
				break;
			case LOGON_QUICK://快速登录
				{
					PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
					this->addChild(tipInfo);
					tipInfo->setTipInfoContent(BaseAttributes::sharedAttributes()->sWaitCodeing.c_str());
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
//连接服务器
void LogonScene::connectServer(){
	PopDialogBox *box = PopDialogBoxLoading::create();
	this->addChild(box, 10, TAG_LOADING);
	box->setSocketName(SOCKET_LOGON_GAME);

	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_GAME);
	TCPSocket *tcp = getSocket();
	if (tcp)
	{
		tcp->createSocket(GAME_IP, PORT_LOGON, new LogonGameListerner());
	}
}
//登录游戏////////////////////////////////////////////////////////////////////////
void LogonScene::logonGameByAccount(){
	connectServer();
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
//登录游戏
void LogonScene::logonGame(){
	CMD_MB_LogonAccounts logonAccounts;
	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = VERSION_PLAZA;
	strcpy(logonAccounts.szAccounts, DataModel::sharedDataModel()->sLogonAccount.c_str());
	//strcpy(logonAccounts.szAccounts,"zhangh189");
	strcpy(logonAccounts.szMachineID, "12");
	strcpy(logonAccounts.szMobilePhone, "32");
	strcpy(logonAccounts.szPassPortID, "12");
	strcpy(logonAccounts.szPhoneVerifyID, "1");
	//游戏标识
	for (int i = 0; i < 10; i++)
	{
		logonAccounts.wModuleID[i] = 0;
	}
	logonAccounts.wModuleID[0] = 210; //210为二人牛牛标示
	logonAccounts.wModuleID[1] = 30; //30为百人牛牛标示
	logonAccounts.wModuleID[2] = 130; //1002为通比牛牛标示
	//logonAccounts.wModuleID[3] = 430; //六人换牌
	CCLog("passWord:%s <<%s>>", DataModel::sharedDataModel()->sLogonPassword.c_str(), __FUNCTION__);
	MD5 m;
	//std::string passWord = GBKToUTF8(DataModel::sharedDataModel()->sLogonPassword.c_str());
	//m.ComputMd5(passWord.c_str(), passWord.length());
	m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
	std::string md5PassWord = m.GetMd5();
	strcpy(logonAccounts.szPassword, md5PassWord.c_str());
	CCLog("%s  --  :%s   %d<<%s>>", logonAccounts.szAccounts, logonAccounts.szPassword, sizeof(logonAccounts), __FUNCTION__);
	bool isSend = getSocket()->SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("Logon:send:%d", isSend);
}
//注册游戏
void LogonScene::registeredGame(){
	CMD_MB_RegisterAccounts registeredAccount;
	
	//游戏标识
	for (int i = 0; i < 10; i++)
	{
		registeredAccount.wModuleID[i] = 0;
	}
	registeredAccount.wModuleID[0] = 210; //210为二人牛牛标示
	registeredAccount.wModuleID[1] = 30; //30为百人牛牛标示
	registeredAccount.wModuleID[2] = 130; //1002为通比牛牛标示

	registeredAccount.dwPlazaVersion = VERSION_PLAZA;//广场版本

	registeredAccount.cbDeviceType = 2;


	
	MD5 m;
	m.ComputMd5(sRegisterPasswrod.c_str(), sRegisterPasswrod.length());
	std::string md5PassWord = m.GetMd5();
	strcpy(registeredAccount.szLogonPass, md5PassWord.c_str());//登录密码
	strcpy(registeredAccount.szInsurePass, md5PassWord.c_str());//银行密码
	CCLog("registeredPassword:%s <<%s>>",registeredAccount.szLogonPass, __FUNCTION__);

	registeredAccount.wFaceID = 1;//头像标识
	registeredAccount.cbGender = 1;//用户性别
	strcpy(registeredAccount.szAccounts, sRegisterAccount.c_str());//登录帐号
	strcpy(registeredAccount.szNickName, UTF8ToGBK(sRegisterNickname.c_str()));//用户昵称
	//strcpy(registeredAccount.szSpreader, "");//推荐帐号
	//strcpy(registeredAccount.szPassPortID, "");//证件号码
	//strcpy(registeredAccount.szCompellation, "");//真实名字

	//registeredAccount.cbValidateFlags = 1;//校验标识		   
	strcpy(registeredAccount.szMachineID, "12");//机器序列
	strcpy(registeredAccount.szMobilePhone, "");//电话号码


	getSocket()->SendData(MDM_MB_LOGON, SUB_MB_REGISTER_ACCOUNTS, &registeredAccount, sizeof(CMD_GP_RegisterAccounts));
}
//连接成功
void LogonScene::onEventConnect(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_SOCKET_OPEN:
		{
			switch (eLogonType)
			{
			case LogonScene::LOGON_ACCOUNT:
			{
				logonGame();
			}
				break;
			case LogonScene::LOGON_QQ:
				break;
			case LogonScene::LOGON_REGISTER:
			{
				registeredGame();
			}
				break;
			case LogonScene::LOGON_QUICK:
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
//登录消息
void LogonScene::onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_MB_LOGON_SUCCESS:
		{
			//效验参数
			//if (wDataSize != sizeof(CMD_MB_LogonSuccess)) return ;
			if (wDataSize < sizeof(CMD_MB_LogonSuccess)) return;
			CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)pDataBuffer;
			CCLog("登录成功 %ld %s",ls->dwUserID,GBKToUTF8(ls->szNickName));
			//赋值
			strcpy(DataModel::sharedDataModel()->userInfo->szNickName,ls->szNickName);
			DataModel::sharedDataModel()->userInfo->lScore=ls->lUserScore;
			DataModel::sharedDataModel()->userInfo->dwGameID=ls->dwGameID;
			DataModel::sharedDataModel()->userInfo->dwUserID=ls->dwUserID;
			DataModel::sharedDataModel()->userInfo->cbGender=ls->cbGender;
			DataModel::sharedDataModel()->userInfo->wFaceID=ls->wFaceID;
			DataModel::sharedDataModel()->cbInsurePwd=ls->cbInsurePwd;
			DataModel::sharedDataModel()->userInfo->lIngot = ls->lIngot;
			DataModel::sharedDataModel()->userInfo->lIngotScore = ls->lIngotScore;

			if (strcmp(sRegisterAccount.c_str(),"")!=0)
			{
				DataModel::sharedDataModel()->sLogonAccount = sRegisterAccount;
			}
			if (strcmp(sRegisterPasswrod.c_str(), "") != 0)
			{
				DataModel::sharedDataModel()->sLogonPassword = sRegisterPasswrod;
			}
			

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
			CCLog("登录失败:%s",GBKToUTF8(describeStr));
			TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_GAME);
			
			PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
			this->addChild(tipInfo);
			tipInfo->setTipInfoContent(GBKToUTF8(describeStr));
			sRegisterAccount = "";
			sRegisterPasswrod = "";
			sRegisterNickname = "";
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
	case SUB_GP_LOBBY_IP:
	{
		//效验参数
		if (wDataSize < sizeof(CMD_GP_LobbyIp)) return;
		CMD_GP_LobbyIp *pLobbyIp = (CMD_GP_LobbyIp*)pDataBuffer;
		DataModel::sharedDataModel()->sLobbyIp = pLobbyIp->szServerIP;
		DataModel::sharedDataModel()->lLobbyProt = pLobbyIp->dwServerPort;
		/*TCPSocket *tcp = TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOBBY);
		if (tcp&&tcp->eSocketState!=TCPSocket::SOCKET_STATE_CONNECT_SUCCESS){
			tcp->createSocket(pLobbyIp->szServerIP, pLobbyIp->dwServerPort, new LobbyGameListerner());
			//tcp->createSocket("112.1.1.1", pLobbyIp->dwServerPort, new LobbyGameListerner());
		}*/
	}
		break;
	default:
		CCLog("--------%d <<%s>>", wSubCmdID, __FUNCTION__);
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
			DataModel::sharedDataModel()->removeTagGameServerList(DataModel::sharedDataModel()->tagGameServerListSixSwap);
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
				}
				else if (tempTag->wKindID == 130)
				{
					DataModel::sharedDataModel()->tagGameServerListOxOneByOne.push_back(tempTag);
					CCLog("port %d  %d  %s<<%s>>", tempTag->wServerPort, tempTag->wSortID, GBKToUTF8(tempTag->szDescription), __FUNCTION__);

				}
				else if (tempTag->wKindID == 430)
				{
					DataModel::sharedDataModel()->tagGameServerListSixSwap.push_back(tempTag);
					CCLog("port %d  %d  six swap %s<<%s>>", tempTag->wServerPort, tempTag->wSortID, GBKToUTF8(tempTag->szDescription), __FUNCTION__);

				}
				else
				{
					CCLog("port %d  %d  %s<<%s>>", tempTag->wServerPort, tempTag->wSortID, GBKToUTF8(tempTag->szDescription), __FUNCTION__);
				}
				
				//sort(DataModel::sharedDataModel()->tagGameServerList.begin(), DataModel::sharedDataModel()->tagGameServerList.end(), less_second);
				
              //  CCLog("port-----:%d ",tempTag->wServerPort);
			}
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListOxTwo);
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListOxHundred);
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListOxOneByOne);
			DataModel::sharedDataModel()->sortVector(DataModel::sharedDataModel()->tagGameServerListSixSwap);
		}
		break;
	case SUB_MB_LIST_FINISH:
		{
			unscheduleUpdate();
			TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_GAME);
			Tools::setTransitionAnimation(0,0,GameLobbyScene::scene(false));
		}
		break;
	default:
		CCLog("other:%d<<%s>>",wSubCmdID,__FUNCTION__);
		break;
	}
}
//注册回调
void LogonScene::onRegistered(const char *sAccount, const char*sNickname, const char*sPassword){
	sRegisterAccount = sAccount;
	sRegisterNickname = sNickname;
	sRegisterPasswrod = sPassword;
	connectServer();
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

/////////////////////////////////////////////////////////////////////////////
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>

//call c
#ifdef __cplusplus
extern "C"
{
#endif


	//java 调用(签名验证)
	JNIEXPORT void JNICALL Java_com_xw_BullfightGame_BullfightGame_JniQQLogin(JNIEnv* env, jstring account, jstring password)
	{

		CCLog("-%s  %s",jstringTostring(env,account),jstringTostring(env,password));
		
	} 
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

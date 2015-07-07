//
//  LogonScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#ifndef __BullfightGame__LogonScene__
#define __BullfightGame__LogonScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../MTNotificationQueue/MessageQueue.h"
#include "../Tools/DataModel.h"
#include "../Tools/CStringAide.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "../PopDialogBox/PopDialogBoxRegistered.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class LogonScene:public CCLayer,public MessageQueue,public CStringAide,public IPopAssistRegistered
{
private:
	enum LogonType
	{
		LOGON_ACCOUNT=1,					//帐号登录
		LOGON_QQ,							//QQ登录
		LOGON_REGISTER,						//注册
		LOGON_QUICK,						//快速登录
	};
	CC_SYNTHESIZE(LogonType, eLogonType, LogonType);
	std::string sRegisterAccount;
	std::string sRegisterNickname;
	std::string sRegisterPasswrod;
    
    UILayer *m_pWidget;
public:
    static LogonScene* pLScene;
	bool isReadMessage;
public:
    LogonScene();
    ~LogonScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    
    void closeWebView();
    void logonQQ(const char*id,const char*pwd);
    CREATE_FUNC(LogonScene);
private:
	void onMenuLogon(CCObject* pSender, TouchEventType type);

	void update(float delta);
	TCPSocket *getSocket(){return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_GAME);}
public:
	//登录游戏(帐号登录)
	void logonGameByAccount(float dt);
private:
	//初始化签到信息
	void initSignInfo();
	//连接服务器
	void connectServer();
	//登录
	void logonGame();
	//注册
	void registeredGame();
	//网络回调////////////////////////////////////////////////////////////////////////
	void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//socket连接成功
	void onEventConnect(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//登录回调
	void onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//获取列表
	void onEventServerList(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//注册回调
	virtual void onRegistered(const char *sAccount, const char*sNickname, const char*sPassword);
	//存档
	void readRMS();
	void initRSM();
	bool isHaveSaveFile();
};

#endif /* defined(__BullfightGame__LogonScene__) */

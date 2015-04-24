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
#include "MessageQueue.h"
#include "DataModel.h"
#include "TCPSocketControl.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class LogonScene:public CCLayer,public MessageQueue
{
private:
	enum LogonType
	{
		LOGON_ACCOUNT=1,					//帐号登录
		LOGON_QQ,									//QQ登录
		LOGON_REGISTER,						//注册
		LOGON_QUICK,							//快速登录
	};
public:
    LogonScene();
    ~LogonScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    
    CREATE_FUNC(LogonScene);
private:
	void onMenuLogon(CCObject* pSender, TouchEventType type);

	void update(float delta);
	TCPSocket *getSocket(){return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_GAME);}
public:
	//登录游戏(帐号登录)
	void logonGameByAccount();
private:
	//网络回调////////////////////////////////////////////////////////////////////////
	void onEventSokcet(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	void onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//存档
	void readRMS();
	void initRSM();
	bool isHaveSaveFile();
};

#endif /* defined(__BullfightGame__LogonScene__) */

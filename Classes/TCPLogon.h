//
//  TCPLogon.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TCPSocket.h"
USING_NS_CC;
USING_NS_CC_EXT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "pthread/pthread.h"
#endif

class TCPLogon:public CCNode,public TCPSocket
{
private:
	static pthread_t threadLogon;
	static TCPLogon *logon;
	bool isReadData;
public:
    TCPLogon();
    ~TCPLogon();
	virtual void onEnter();
	virtual void onExit();
    
    CREATE_FUNC(TCPLogon);
public:
	int startSendThread();
	void sendData();
private:
	static void* networkThread(void*);
	void stopTcpSocket();
	bool OnEventTCPSocketRead(unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
};

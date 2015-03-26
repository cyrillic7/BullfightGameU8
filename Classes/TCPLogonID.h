//
//  TCPLogonID.h
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

class TCPLogonID:public CCNode,public TCPSocket
{
private:
	static pthread_t threadLogonID;
	static TCPLogonID *logonID;
	bool isReadData;
public:
    TCPLogonID();
    ~TCPLogonID();
	virtual void onEnter();
	virtual void onExit();
    
    CREATE_FUNC(TCPLogonID);
public:
	int startSendThread();
	void sendData(const char* ip, unsigned short port);
private:
	static void* networkThread(void*);
	void stopTcpSocket();
	bool OnEventTCPSocketRead(unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
};

/*
 * CSocketControl.h
 *
 *  Created on: 2015年3月30日
 *      Author: xw007
 */

#pragma once
#include "cocos2d.h"
#include "CGameSocket.h"
#include "../ListernerThread/SocketListerner.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "pthread/pthread.h"
#endif


class CSocketControl {
public:
	SocketListerner *listerner;
private:
	bool isReadMsg;
	//CGameSocket gameSocket;
	std::map<std::string , CGameSocket*> mGameSocket;
public:
	CSocketControl();
	virtual ~CSocketControl();
public:
	static CSocketControl* sharedSocketControl();
	void initSocketControl();
	int startSendThread();
	static void* networkThread(void* object);
	void initNetwork();

	CGameSocket* getTCPSocket(std::string key);
	void removeTCPSocket(std::string key);
private:
	//更新socket收发数据
	void updateSocketData();
};

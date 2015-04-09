/*
 * TCPSocketControl.h
 *
 *  Created on: 2015年3月30日
 *      Author: xw007
 */

#ifndef TCPSOCKETCONTROL_H_
#define TCPSOCKETCONTROL_H_
#include "cocos2d.h"
#include "TCPSocket.h"
#include "SocketListerner.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "pthread/pthread.h"
#endif

class TCPSocketControl {
public:
	const char* ip;
	unsigned short port;
	SocketListerner *listerner;
	bool isRun;
private:
	TCPSocket *tcpSocket;
	std::map<std::string , TCPSocket> mTcpSocket;
public:
	TCPSocketControl();
	virtual ~TCPSocketControl();
public:
	static TCPSocketControl* sharedTCPSocketControl();
	void initTCPSocket();
	int startSendThread();
	static void* networkThread(void* object);
	void initNetwork();
	void stopSocket();
	void deleteControl();
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);
};

#endif /* TCPSOCKETCONTROL_H_ */

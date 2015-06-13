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
#include "../ListernerThread/SocketListerner.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "pthread/pthread.h"
#endif


//Socket名字////////////////////////////////////////////////////////////////////////
#define SOCKET_LOGON_GAME						"socketLogonGame"				//登录游戏
#define SOCKET_LOGON_ROOM						"socketLogonRoom"				//登录房间
#define SOCKET_SHOP								"socketShop"					//商店
#define SOCKET_KNAPSACK							"socketKnapsack"				//我的背包
#define SOCKET_BANK								"socketBank"					//我的保险箱
#define SOCKET_AUCTION_INFO						"socketAuctionInfo"				//拍卖信息列表
#define SOCKET_CHANGE_PASSWORD					"socketChangePassword"			//修改密码
#define SOCKET_LOBBY							"socketLobby"					//大厅长连接
#define SOCKET_RANKING							"socketRanking"					//排行榜

class TCPSocketControl {
public:
	const char* ip;
	unsigned short port;
	SocketListerner *listerner;
	bool isRun;
private:
	TCPSocket *tcpSocket;
	std::map<std::string , TCPSocket*> mTcpSocket;
public:
	TCPSocketControl();
	virtual ~TCPSocketControl();
public:
	static TCPSocketControl* sharedTCPSocketControl();
	void initTCPSocket();
	int startSendThread();
	static void* networkThread(void* object);
	void initNetwork();
	//void stopSocket();
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);

	TCPSocket* getTCPSocket(std::string key);
	void stopSocket(std::string key);
	void removeTCPSocket(std::string key);
};

#endif /* TCPSOCKETCONTROL_H_ */

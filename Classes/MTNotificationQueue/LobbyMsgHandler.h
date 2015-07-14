
#pragma once
/************************************************************************/
/* 大厅 任务、消息                                                                     */
/************************************************************************/
#include "cocos2d.h"
#include "../Network/TCPSocket/CGameSocket.h"
USING_NS_CC;
class LobbyMsgHandler:public CCNode,public IGameSocket{
public:
	CGameSocket gameSocket;
public:
	LobbyMsgHandler();
	~LobbyMsgHandler();
	static LobbyMsgHandler* sharedLobbyMsgHandler();
	void update(float dt);
	//连接服务器
	void connectServer(std::string sLobbyIp, long lLobbyProt);

private:
	//网络回调////////////////////////////////////////////////////////////////////////
	virtual void onOpen();
	virtual void onError(const char* e);
	virtual bool onMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};

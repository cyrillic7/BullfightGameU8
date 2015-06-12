
#pragma once
#include "cocos2d.h"
#include "../Network/CMD_Server/CMD_GameServer.h"
USING_NS_CC;
class MessageQueueLobby {
public:
	MessageQueueLobby();
	~MessageQueueLobby();
	void update(float dt);
	virtual void onEventReadMessageLobby(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize)=0;
	static void pushQueue(ReadData rData);
    
	/*//登录
	virtual void onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){}
	//用户状态
	virtual void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){}
	//游戏中
	virtual void onEventGameIng(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){}   
	//socket消息
	virtual void onEventSokcet(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){}   */
};

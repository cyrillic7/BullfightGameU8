
#pragma once
#include "cocos2d.h"
#include "../Network/CMD_Server/CMD_GameServer.h"
USING_NS_CC;
class MessageQueueGameIng {
public:
	MessageQueueGameIng();
	~MessageQueueGameIng();
	void update(float dt);
	virtual void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize)=0;
	static void pushQueue(ReadData rData);
   
};

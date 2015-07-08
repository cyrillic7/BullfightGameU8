/* 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "GameIngListerner.h"
#include "../CMD_Server/CMD_LogonServer.h"
#include "../MD5/MD5.h"
#include "../../Tools/DataModel.h"
#include "../TCPSocket/TCPSocketControl.h"
#include "cocos2d.h"
#include "../../MTNotificationQueue/MTNotificationQueue.h"
#include "../../MTNotificationQueue/MessageQueueGameIng.h"
#include "../SEvent.h"
using namespace std;
GameIngListerner::GameIngListerner()
{
}
GameIngListerner::~GameIngListerner()
{
    
}
void GameIngListerner::OnClose(TCPSocket* so, bool fromRemote)
{
	CCLog("close------------------<<%s>>",__FUNCTION__);
}

void GameIngListerner::OnError(TCPSocket* so, const char* e)
{
	CCLog("error--------------------<<%s>>",__FUNCTION__);
}

void GameIngListerner::OnIdle(TCPSocket* so)
{
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
bool GameIngListerner::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	ReadData rData;
	rData.wMainCmdID=Command.wMainCmdID;
	rData.wSubCmdID=Command.wSubCmdID;
	rData.wDataSize=wDataSize;
	memcpy(rData.sReadData,pDataBuffer,wDataSize);
	MessageQueueGameIng::pushQueue(rData);
	return true;
}
void GameIngListerner::OnOpen(TCPSocket* so)
{
	ReadData rData;
	rData.wMainCmdID=MDM_MB_SOCKET;
	rData.wSubCmdID=SUB_GP_SOCKET_OPEN;
	rData.wDataSize=0;
	memcpy(rData.sReadData,0,0);
	MessageQueueGameIng::pushQueue(rData);
}

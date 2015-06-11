/* 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "LobbyGameListerner.h"
#include "../CMD_Server/CMD_LogonServer.h"
#include "../MD5/MD5.h"
#include "../../Tools/DataModel.h"
#include "../TCPSocket/TCPSocketControl.h"
#include "cocos2d.h"
#include "../../MTNotificationQueue/MTNotificationQueue.h"
#include "../../MTNotificationQueue/MessageQueueLobby.h"
#include "../SEvent.h"
using namespace std;
LobbyGameListerner::LobbyGameListerner()
{
}
LobbyGameListerner::~LobbyGameListerner()
{
    
}
void LobbyGameListerner::OnClose(TCPSocket* so, bool fromRemote)
{
	CCLog("close------------------<<%s>>",__FUNCTION__);
}

void LobbyGameListerner::OnError(TCPSocket* so, const char* e)
{
	CCLog("error--------------------<<%s>>",__FUNCTION__);
}

void LobbyGameListerner::OnIdle(TCPSocket* so)
{
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
bool LobbyGameListerner::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	ReadData rData;
	rData.wMainCmdID=Command.wMainCmdID;
	rData.wSubCmdID=Command.wSubCmdID;
	rData.wDataSize=wDataSize;
	memcpy(rData.sReadData,pDataBuffer,wDataSize);
	//MessageQueue::pushQueue(rData);
	MessageQueueLobby::pushQueue(rData);
	return true;
}
void LobbyGameListerner::OnOpen(TCPSocket* so)
{
	ReadData rData;
	rData.wMainCmdID=MDM_MB_SOCKET;
	rData.wSubCmdID=SUB_GP_SOCKET_OPEN;
	rData.wDataSize=0;
	memcpy(rData.sReadData,0,0);
	//MessageQueue::pushQueue(rData);
	MessageQueueLobby::pushQueue(rData);
}

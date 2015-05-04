/* 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "LogonGameListerner.h"
#include "CMD_LogonServer.h"
#include "MD5.h"
#include "DataModel.h"
#include "TCPSocketControl.h"
#include "cocos2d.h"
#include "MTNotificationQueue.h"
#include "QueueData.h"
#include "SEvent.h"
//#include "MessageQueue.cpp"
using namespace std;
LogonGameListerner::LogonGameListerner()
{
}
LogonGameListerner::~LogonGameListerner()
{
}
void LogonGameListerner::OnClose(TCPSocket* so, bool fromRemote)
{
	CCLog("close------------------<<%s>>",__FUNCTION__);
}

void LogonGameListerner::OnError(TCPSocket* so, const char* e)
{
	CCLog("error--------------------<<%s>>",__FUNCTION__);
}

void LogonGameListerner::OnIdle(TCPSocket* so)
{
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
bool LogonGameListerner::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	//LifeCircleMutexLock1(&sharedNotificationQueueLock1); 
	ReadData rData;
	rData.wMainCmdID=Command.wMainCmdID;
	rData.wSubCmdID=Command.wSubCmdID;
	rData.wDataSize=wDataSize;
	memcpy(rData.sReadData,pDataBuffer,wDataSize);
	MessageQueue::pushQueue(rData);
	return true;
}
void LogonGameListerner::OnOpen(TCPSocket* so)
{
	ReadData rData;
	rData.wMainCmdID=MDM_MB_SOCKET;
	rData.wSubCmdID=SUB_GP_SOCKET_OPEN;
	rData.wDataSize=0;
	memcpy(rData.sReadData,0,0);
	MessageQueue::pushQueue(rData);
}

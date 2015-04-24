/* 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "LogonListerner.h"
#include "CMD_LogonServer.h"
#include "MD5.h"
#include "DataModel.h"
#include "TCPSocketControl.h"
#include "cocos2d.h"
#include "MTNotificationQueue.h"
#include "QueueData.h"
#include "SEvent.h"


using namespace std;
LogonListerner::LogonListerner()
{
}

LogonListerner::~LogonListerner()
{
}

void LogonListerner::OnClose(TCPSocket* so, bool fromRemote)
{
	TCPSocketControl::sharedTCPSocketControl()->deleteControl();
	CCLog("close<<%s>>",__FUNCTION__);
	//End();
}

void LogonListerner::OnError(TCPSocket* so, const char* e)
{
	CCLog("%s\n","error connection");
}

void LogonListerner::OnIdle(TCPSocket* so)
{
	CCLog("%s\n","listerner-- connection idle");
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
bool LogonListerner::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	if (Command.wMainCmdID == MDM_GP_LOGON)
	{
		if (Command.wSubCmdID == SUB_MB_UPDATE_NOTIFY)
		{
			/*//效验参数
			assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
			if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return false;

			CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
			CCLog("升级提示");*/
		}
	}
	//登录失败
	if (Command.wMainCmdID == MDM_MB_LOGON)
	{
		if (Command.wSubCmdID == SUB_MB_LOGON_FAILURE)
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_MB_LogonSuccess));
			if (wDataSize < sizeof(CMD_MB_LogonSuccess)) return false;

			CMD_MB_LogonFailure *lf = (CMD_MB_LogonFailure*)pDataBuffer;
			long code = lf->lResultCode;
			char *describeStr = lf->szDescribeString;
			//CCLog("%s", describeStr);
			CCLog("登录失败");
		}
	}
	//登录成功
	if (Command.wMainCmdID == MDM_MB_LOGON)
	{
		if (Command.wSubCmdID == SUB_MB_LOGON_SUCCESS)
		{
			//效验参数
			if (wDataSize != sizeof(CMD_MB_LogonSuccess)) return false;

			

			CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)pDataBuffer;

			/*DataModel::sharedDataModel()->logonSuccessUserInfo->cbGender=ls->cbGender;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwExperience=ls->dwExperience;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwGameID=ls->dwGameID;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwLoveLiness=ls->dwLoveLiness;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwUserID=ls->dwUserID;
			strcpy(DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName,ls->szNickName);
			DataModel::sharedDataModel()->logonSuccessUserInfo->wFaceID=ls->wFaceID;*/
		//	memcpy(DataModel::sharedDataModel()->logonSuccessUserInfo,ls,sizeof(CMD_MB_LogonSuccess));
			CCLog("登录成功 %ld %s",ls->dwUserID,ls->szNickName);

           /* QueueData queueData;
            queueData.wSubCmdID=Command.wSubCmdID;
            memcpy(queueData.sendData.sSendData, pDataBuffer, wDataSize);
            MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_LOGON,&queueData);
            
            CMD_MB_LogonSuccess *lss = (CMD_MB_LogonSuccess*)queueData.sendData.sSendData;
            CCLog("lss:%ld",lss->dwUserID);*/
           
           // CCLog("%d =====%d",pCommand.wMainCmdID,pCommand.wSubCmdID);
			/*SendData data;
			int nHeadLen = sizeof(TCP_Command);
			memcpy(data.sSendData, &Command, nHeadLen);
			memcpy(data.sSendData+nHeadLen, pDataBuffer, wDataSize < MAX_TCP_LEN-nHeadLen? wDataSize: MAX_TCP_LEN-1-nHeadLen );
			data.dwDataLen = wDataSize;*/
		}
	}
	//获取列表
	if (Command.wMainCmdID == MDM_GP_SERVER_LIST)
	{
		if (Command.wSubCmdID == SUB_GP_LIST_KIND)
		{
			//效验参数
			if (wDataSize != sizeof(tagGameKind)) return false;
			tagGameKind *gs = (tagGameKind*)pDataBuffer;

			CCLog("获取游戏种类");
		}
	}
	if (Command.wMainCmdID==MDM_MB_SERVER_LIST)
	{
		if (Command.wSubCmdID==SUB_MB_LIST_SERVER)
		{
			int gameServerSize = sizeof(tagGameServer);
			int serverCount = wDataSize / gameServerSize;

			BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
			CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

			DataModel::sharedDataModel()->removeTagGameServerList();
			for (int i = 0; i < serverCount; i++)
			{
				void * pDataBuffer = cbDataBuffer + i*sizeof(tagGameServer);
				tagGameServer *gameServer = (tagGameServer*)pDataBuffer;
				tagGameServer *tempTag=new tagGameServer();
				/*tempTag->dwFullCount=gameServer->dwFullCount;
				tempTag->dwOnLineCount=gameServer->dwOnLineCount;
				strcpy(tempTag->szDescription,gameServer->szDescription);
				strcpy(tempTag->szGameLevel,gameServer->szGameLevel);
				strcpy(tempTag->szServerAddr,gameServer->szServerAddr);
				strcpy(tempTag->szServerName,gameServer->szServerName);
				tempTag->wKindID=gameServer->wKindID;
				tempTag->wNodeID=gameServer->wNodeID;
				tempTag->wServerID=gameServer->wServerID;
				tempTag->wServerPort=gameServer->wServerPort;
				tempTag->wServerType=gameServer->wServerType;
				tempTag->wSortID=gameServer->wSortID;*/
				memcpy(tempTag,gameServer,sizeof(tagGameServer));
				//memcoyp(gameServer,0,sizeof(tagGameServer));
				DataModel::sharedDataModel()->tagGameServerList.push_back(tempTag);

				//sort(DataModel::sharedDataModel()->tagGameServerList.begin(), DataModel::sharedDataModel()->tagGameServerList.end(), less_second);

                CCLog("port-----:%d ",tempTag->wServerPort);
			}
			DataModel::sharedDataModel()->sortVector();
		}
	}
	//获取列表完成
	if (Command.wMainCmdID == MDM_MB_SERVER_LIST)
	{
		if (Command.wSubCmdID == SUB_MB_LIST_FINISH)
		{
			
			CCLog("列表完成");
			//GameLobbyScene *gls=(GameLobbyScene*)this->getParent();
			//gls->userName->setText(DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName);
			
			//CCNotificationCenter::sharedNotificationCenter()->postNotification(LISTENER_LOGON,(CCObject*)pDataBuffer);

            QueueData qData;
			MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_LOGON,&qData);
		}
	}
	return true;
}

void LogonListerner::OnOpen(TCPSocket* so)
{
	CCLog("open==========================");
	MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_OPEN,NULL);

}

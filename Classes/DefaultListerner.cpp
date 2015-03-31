/* 
 * File:   DefaultListerner.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午7:33
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "DefaultListerner.h"
#include "CMD_LogonServer.h"
#include "MD5.h"
#include "DataModel.h"
#include "TCPSocketControl.h"
#include "cocos2d.h"
#include "MTNotificationQueue.h"
using namespace std;



DefaultListerner::DefaultListerner()
{
}

DefaultListerner::~DefaultListerner()
{
}

void DefaultListerner::OnClose(TCPSocket* so, bool fromRemote)
{
    CCLog("%s\n","00000000000000---closed");
}

void DefaultListerner::OnError(TCPSocket* so, const char* e)
{
	CCLog("%s\n","error connection");
}

void DefaultListerner::OnIdle(TCPSocket* so)
{
	CCLog("%s\n","connection idle");
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
bool DefaultListerner::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	if (Command.wMainCmdID == 1)
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

			DataModel::sharedDataModel()->logonSuccessUserInfo->cbGender=ls->cbGender;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwExperience=ls->dwExperience;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwGameID=ls->dwGameID;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwLoveLiness=ls->dwLoveLiness;
			DataModel::sharedDataModel()->logonSuccessUserInfo->dwUserID=ls->dwUserID;
			strcpy(DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName,ls->szNickName);
			DataModel::sharedDataModel()->logonSuccessUserInfo->wFaceID=ls->wFaceID;
			//CCLog("登录成功 %s",gb23122utf8(ls->szNickName));
            //tagGameServer *tempTag=new tagGameServer();
			
		}
	}
	//获取列表
	if (Command.wMainCmdID == 2)
	{
		if (Command.wSubCmdID == SUB_MB_LIST_SERVER)
		{
			//效验参数
			if (wDataSize != sizeof(tagGameKind)) return false;
			tagGameKind *gs = (tagGameKind*)pDataBuffer;

			CCLog("获取游戏种类");
		}
	}
	if (Command.wMainCmdID==101)
	{
		if (Command.wSubCmdID==101)
		{
			int gameServerSize = sizeof(tagGameServer);
			int serverCount = wDataSize / gameServerSize;

			//void *pDataBuffer = pDataBuffer + sizeof(tagGameServer);
			BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
			CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);
			
			for (int i = 0; i < serverCount; i++)
			{
				void * pDataBuffer = cbDataBuffer + i*sizeof(tagGameServer);
				tagGameServer *gameServer = (tagGameServer*)pDataBuffer;
				tagGameServer *tempTag=new tagGameServer();
				tempTag->dwFullCount=gameServer->dwFullCount;
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
				tempTag->wSortID=gameServer->wSortID;
				//memcoyp(gameServer,0,sizeof(tagGameServer));
				DataModel::sharedDataModel()->tagGameServerList.push_back(tempTag);
               // CCLog("%s",tempTag->szServerName);
			}
			//tagGameServer *gameServer = (tagGameServer*)pDataBuffer;
			//TCPSocket::OnSocketNotifyRead(0, 0);
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
			MTNotificationQueue::sharedNotificationQueue()->postNotification(LISTENER_LOGON,(CCObject*)pDataBuffer);
			//TCPSocketControl::sharedTCPSocketControl()->stopSocket();
			//delete TCPSocketControl::sharedTCPSocketControl();
			//so->Close();
			//return 0;
			//tagGameServer *gs = (tagGameServer*)pDataBuffer;
		}
	}
	return true;
}

void DefaultListerner::OnOpen(TCPSocket* so)
{
	CCLog("open==========================");
	CMD_MB_LogonAccounts logonAccounts;
	//memset(&logonAccounts, 0, sizeof(CMD_MB_LogonAccounts));
	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = 17235969;



	strcpy(logonAccounts.szAccounts,"z40144322");
	//strcpy(logonAccounts.szAccounts,"zhangh189");

	strcpy(logonAccounts.szMachineID,"12");
	strcpy(logonAccounts.szMobilePhone,"32");
	strcpy(logonAccounts.szPassPortID,"12");
	strcpy(logonAccounts.szPhoneVerifyID,"1");
	//_tcscpy(logonAccounts.szMachineID, _TEXT("12"));
	//_tcscpy(logonAccounts.szMobilePhone, _TEXT("32"));
	//_tcscpy(logonAccounts.szPassPortID, _TEXT("12"));
	//_tcscpy(logonAccounts.szPhoneVerifyID, _TEXT("1"));

	logonAccounts.wModuleID = 210; //210为二人牛牛标示


	MD5 m;
	MD5::char8 str[] = "z12345678";
	//MD5::char8 str[] = "z123456789";
	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();

	strcpy(logonAccounts.szPassword,md5PassWord.c_str());
	//_tcscpy(logonAccounts.szPassword, _TEXT(md5PassWord.c_str()));

	bool isSend = so->SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("send:%d", isSend);

}

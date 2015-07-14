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
#include "../../Tools/Tools.h"
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
	ReadData rData;
	rData.wMainCmdID = MDM_MB_SOCKET_FAILURE;
	rData.wSubCmdID = SUB_GP_SOCKET_OPEN;
	rData.wDataSize = 0;
	memcpy(rData.sReadData, 0, 0);
	//MessageQueue::pushQueue(rData);
	MessageQueueLobby::pushQueue(rData);
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
	if (Command.wMainCmdID == MDM_GL_C_DATA)
	{
		if (Command.wSubCmdID == SUB_GL_C_TASK_LOAD 
			|| Command.wSubCmdID == SUB_GL_C_TASK_REWARD)
		{
			ReadData rData;
			rData.wMainCmdID = Command.wMainCmdID;
			rData.wSubCmdID = Command.wSubCmdID;
			rData.wDataSize = wDataSize;
			memcpy(rData.sReadData, pDataBuffer, wDataSize);
			MessageQueueLobby::pushQueue(rData);
		}
		else if (Command.wSubCmdID == SUB_GL_C_MESSAGE)//消息
		{
			CMD_GL_MsgNode *msgNode = (CMD_GL_MsgNode*)pDataBuffer;

			std::string msg = msgNode->szMsgcontent;
			if (msgNode->dwUserID==0)
			{
				
				DataModel::sharedDataModel()->vecSystemMsg.push_back(msg);
			}
			else 
			{
				DataModel::sharedDataModel()->vecMyMsg.push_back(msg);
			}
 			CCLog("msg main:%d sub:%d %s<<%s>>", Command.wMainCmdID, Command.wSubCmdID, Tools::GBKToUTF8(msgNode->szMsgcontent), __FUNCTION__);
		}
		else
		{
			CCLog("else main:%d sub:%d <<%s>>", Command.wMainCmdID, Command.wSubCmdID, __FUNCTION__);
		}
	}
	return true;
}
void LobbyGameListerner::OnOpen(TCPSocket* so)
{
	/*CMD_GL_LogonAccounts LogonAccounts;
	strcpy(LogonAccounts.szAccounts, DataModel::sharedDataModel()->sLogonAccount.c_str());
	strcpy(LogonAccounts.szMachineID, "12");

	MD5 m;
	m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
	std::string md5PassWord = m.GetMd5();
	strcpy(LogonAccounts.szPassword, md5PassWord.c_str());

	//连接大厅长连接
	TCPSocket *tcp = TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOBBY);
	tcp->SendData(MDM_GL_C_DATA, SUB_GL_MB_LOGON_ACCOUNTS, &LogonAccounts, sizeof(LogonAccounts));
	*/
	
	/*//////////////////////////////////////////////////////////////////////////
	ReadData rData;
	rData.wMainCmdID = MDM_MB_SOCKET;
	rData.wSubCmdID = SUB_GP_SOCKET_OPEN;
	rData.wDataSize = 0;
	memcpy(rData.sReadData, 0, 0);
	MessageQueueLobby::pushQueue(rData);*/
}

/* 
* File:   DefaultListerner1.cpp
* Author: beykery
* 
* Created on 2013年12月30日, 下午7:33
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "GameListerner.h"
#include "CMD_GameServer.h"
#include "MD5.h"
#include "DataModel.h"
#include "MTNotificationQueue.h"
//#include "iconv.h"
#include "cmd_ox.h"
#include "cocos2d.h"
#include "QueueData.h"
#include "GameConfig.h"
#include "TCPSocketControl.h"
#include "SEvent.h"
using namespace std;

GameListerner::GameListerner()
{
}

GameListerner::~GameListerner()
{
}

void GameListerner::OnClose(TCPSocket* so, bool fromRemote)
{
	//End();
	TCPSocketControl::sharedTCPSocketControl()->deleteControl();
	CCLog("%s\n","-----111111closed");
}

void GameListerner::OnError(TCPSocket* so, const char* e)
{
	CCLog("%s\n","error connection");
}

void GameListerner::OnIdle(TCPSocket* so)
{
	CCLog("%s\n","connection idle");
}

/**
* 有数据到来
* @param so
* @param message
*/
bool GameListerner::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON://登录
		return logonEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GR_CONFIG://配置
		return configEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GR_USER://用户信息
		return userEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GR_STATUS://状态信息
		return statusEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GF_FRAME://框架命令
		return frameEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GF_GAME://游戏命令
		return gameEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	default:
		CCLog("other---------- %d    %d",Command.wMainCmdID,Command.wSubCmdID);
		break;
	}
	return true;
}

void GameListerner::OnOpen(TCPSocket* so)
{
	MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_OPEN,NULL);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool GameListerner::logonEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	if (wSubCmdID == SUB_GR_UPDATE_NOTIFY)
	{
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
		if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return false;

		CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
		CCLog("升级提示");
	}
	if (wSubCmdID == SUB_GR_LOGON_SUCCESS)
	{
		int size=sizeof(CMD_GR_LogonSuccess);
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_LogonSuccess));
		if (wDataSize < sizeof(CMD_GR_LogonSuccess)) return false;

		CMD_GR_LogonSuccess *lf = (CMD_GR_LogonSuccess*)pDataBuffer;
		CCLog("登录成功");
	}
	if (wSubCmdID==SUB_GR_LOGON_FAILURE)
	{
		//assert(wDataSize >= sizeof(CMD_GR_LogonFailure));
		//if (wDataSize < sizeof(CMD_GR_LogonFailure)) return false;

		CMD_GR_LogonFailure *lf = (CMD_GR_LogonFailure*)pDataBuffer;
		CCLog("登录失败:%s",lf->szDescribeString);
	}
	return true;
}
bool GameListerner::configEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:
		{
			/*int size=sizeof(tagColumnItem);
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_ConfigColumn));
			if (wDataSize < sizeof(CMD_GR_ConfigColumn)) return false;
			*/
			CMD_GR_ConfigColumn *lf = (CMD_GR_ConfigColumn*)pDataBuffer;
			CCLog("列表配置");
		}
		break;
	case SUB_GR_CONFIG_SERVER:
		{
			int size=sizeof(CMD_GR_ConfigServer);
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_ConfigServer));
			if (wDataSize < sizeof(CMD_GR_ConfigServer)) return false;

			CMD_GR_ConfigServer *lf = (CMD_GR_ConfigServer*)pDataBuffer;
			CCLog("房间配置");
		}
		break;
	case SUB_GR_CONFIG_PROPERTY:
		{
			CMD_GR_ConfigProperty *lf = (CMD_GR_ConfigProperty*)pDataBuffer;
			CCLog("道具配置");
		}
		break;
	case SUB_GR_CONFIG_FINISH:
		{
			CCLog("配置完成");

			MTNotificationQueue::sharedNotificationQueue()->postNotification("configFinish",NULL);
			/*CMD_GR_UserSitDown sit;
			sit.wTableID=28;
			sit.wChairID=1;
			strcpy(sit.szPassword,"");

			bool isSend=so->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
			CCLog("Classic:%d",isSend);*/
		}
		break;
	default:
		break;
	}
	return true;
}
bool GameListerner::userEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER://用户进入
		{
			int wSize=wDataSize;
			int size =sizeof(tagUserInfoHead);
			tagUserInfoHead *info= (tagUserInfoHead*)pDataBuffer;
			//CCLog("用户进入");
		}
		break;
	case SUB_GR_USER_SCORE:
		{
			CMD_GR_UserScore *userScore=(CMD_GR_UserScore*)pDataBuffer;
			//CCLog("用户分数");
		}
		break;
	case SUB_GR_USER_STATUS://用户状态
		{
			int wSize=wDataSize;
			int size =sizeof(CMD_GR_UserStatus);
			CMD_GR_UserStatus *info= (CMD_GR_UserStatus*)pDataBuffer;
			if (info->dwUserID==DataModel::sharedDataModel()->logonSuccessUserInfo->dwUserID)
			{
				tagUserStatus state=info->UserStatus;
				if (state.cbUserStatus==US_SIT)
				{
					DataModel::sharedDataModel()->isSit=true;
					CCLog("坐下:table: %d desk:%d",state.wTableID,state.wChairID);
					// CCNotificationCenter::sharedNotificationCenter()->postNotification(LISTENER_LOGON,NULL);
					CCLog("==============dd");
					// 
					CMD_GF_GameOption GameOption;
					//ZeroMemory(&GameOption,sizeof(GameOption));

					//构造数据
					GameOption.dwFrameVersion=VERSION_FRAME;
					GameOption.cbAllowLookon=0;
					GameOption.dwClientVersion=VERSION_CLIENT;

					bool isSend = pSocket->SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
					CCLog("send---:%d", isSend);
				}else if (state.cbUserStatus==US_PLAYING)
				{
					//CMD_S_StatusFree *gameStatue=(CMD_S_StatusFree*)pDataBuffer;
					CCLog("游戏状态 ");
				}
				CCLog("状态：%d",state.cbUserStatus);
			}
			//CCLog("state %d",info->dwUserID);
		}
		break;

	default:
		break;
	}
	return true;
}
bool GameListerner::statusEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:
		{
			CMD_GR_TableInfo *lf = (CMD_GR_TableInfo*)pDataBuffer;
			
			CCLog("桌子信息");
		}
		break;
	default:
		break;
	}
	return true;
}
bool GameListerner::frameEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_STATUS:
		{
			CCLog("游戏状态 ");
		}
		break;
	case SUB_GF_SYSTEM_MESSAGE:
		{
			CCLog("系统消息");
		}
		break;
	case SUB_GF_GAME_SCENE:
		{
			CCLog("游戏场景");
		}
		break;
	default:
		break;
	}
	return true;
}
bool GameListerner::gameEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	//创建数据
	QueueData *pData=new QueueData();
	pData->wSubCmdID=wSubCmdID;
	pData->wDataSize=wDataSize;
	memcpy(pData->pDataBuffer,pDataBuffer,wDataSize);
	//发送消息
	MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_GAME_ING,pData);
	/*
	switch (wSubCmdID)
	{
	
	case SUB_S_SEND_CARD:
		{
		
		}
		break;
	case SUB_S_OPEN_CARD:
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
			CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pDataBuffer;

			CCLog("开牌 ");
		}
		break;
	case SUB_S_GAME_END:
		{
			//效验参数
			if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
			CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pDataBuffer;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					int color=GetCardColor(pGameEnd->cbCardData[i][j]);
					int value=GetCardValue(pGameEnd->cbCardData[i][j]);
					CCLog("---------color:%d  value:%d",color,value);
				}
			}

			CCLog("游戏结束");
		}
		break;
	default:
		CCLog("游戏命令:%d",wSubCmdID);
		break;
	}*/
	return true;
}
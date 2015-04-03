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
#include "DefaultListerner1.h"
#include "CMD_GameServer.h"
#include "MD5.h"
#include "DataModel.h"
#include "MTNotificationQueue.h"
//#include "iconv.h"
#include "cmd_ox.h"
#include "cocos2d.h"
using namespace std;
#define VERSION_FRAME				16777217
#define VERSION_CLIENT				17170433
#define VERSION_PLAZA 				17235969
DefaultListerner1::DefaultListerner1()
{
}

DefaultListerner1::~DefaultListerner1()
{
}

void DefaultListerner1::OnClose(TCPSocket* so, bool fromRemote)
{
    CCLog("%s\n","-----111111closed");
}

void DefaultListerner1::OnError(TCPSocket* so, const char* e)
{
	CCLog("%s\n","error connection");
}

void DefaultListerner1::OnIdle(TCPSocket* so)
{
	CCLog("%s\n","connection idle");
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
bool DefaultListerner1::OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize)
{
	//CCLog("onMessage------------------------------");
	if (Command.wMainCmdID == MDM_GR_LOGON)
	{
		if (Command.wSubCmdID == SUB_GR_UPDATE_NOTIFY)
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
			if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return false;

			CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
			CCLog("升级提示");
		}
		if (Command.wSubCmdID == SUB_GR_LOGON_SUCCESS)
		{
			int size=sizeof(CMD_GR_LogonSuccess);
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_LogonSuccess));
			if (wDataSize < sizeof(CMD_GR_LogonSuccess)) return false;

			CMD_GR_LogonSuccess *lf = (CMD_GR_LogonSuccess*)pDataBuffer;
			CCLog("登录成功");
		}
	}
	else if (Command.wMainCmdID==MDM_GR_CONFIG)
	{
		switch (Command.wSubCmdID)
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
	}
	else if (Command.wMainCmdID==MDM_GR_USER){
		switch (Command.wSubCmdID)
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

						 bool isSend = so->SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
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
	}else if(Command.wMainCmdID==MDM_GR_STATUS)
	{
		switch (Command.wSubCmdID)
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
	}
	else if(Command.wMainCmdID==MDM_GF_FRAME)
	{
		
		switch (Command.wSubCmdID)
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
	}
	else if(Command.wMainCmdID==MDM_GF_GAME)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_S_CALL_BANKER://用户叫庄
			{
				int size=wDataSize;
				CMD_S_CallBanker *callBanker=(CMD_S_CallBanker*)pDataBuffer;
				CCLog("用户叫庄");
			}
			break;
		case SUB_S_GAME_START:
			{
				//效验数据
				if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
				CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pDataBuffer;
				//int size=wDataSize;
				CCLog("游戏开始 ");
			}
			break;
		case SUB_S_ADD_SCORE:
			{
				//效验数据
				if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
				CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pDataBuffer;

				CCLog("加注结果");
			}
			break;
		case SUB_S_SEND_CARD:
			{
				CCLog("发牌");
				//效验数据
				if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
				CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pDataBuffer;
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

				CCLog("游戏结束");
			}
			break;
		default:
			CCLog("游戏命令:%d",Command.wSubCmdID);
			break;
		}
	}
	else 
	{
		CCLog("other---------- %d    %d",Command.wMainCmdID,Command.wSubCmdID);
	}
	return true;
}

void DefaultListerner1::OnOpen(TCPSocket* so)
{
	CCLog("open22222==========================");
	CMD_GR_LogonUserID logonUserID;
	memset(&logonUserID, 0, sizeof(CMD_GR_LogonUserID));

	logonUserID.dwFrameVersion=VERSION_FRAME;
	logonUserID.dwPlazaVersion=VERSION_PLAZA;
	logonUserID.dwProcessVersion= VERSION_CLIENT;
	logonUserID.dwUserID=DataModel::sharedDataModel()->logonSuccessUserInfo->dwUserID;
	strcpy(logonUserID.szMachineID,"12");
	strcpy(logonUserID.szPassPortID,"12");

	MD5 m;
	MD5::char8 str[] = "z12345678";
	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();
	strcpy(logonUserID.szPassword,md5PassWord.c_str());

	strcpy(logonUserID.szPhoneVerifyID,"1");
	logonUserID.wKindID=DataModel::sharedDataModel()->tagGameServerList[0]->wKindID;

	CCLog("房间名:%d",DataModel::sharedDataModel()->tagGameServerList[0]->wServerPort);

	int luidSize=sizeof(CMD_GR_LogonUserID);
	bool isSend = so->SendData(MDM_GR_LOGON, SUB_GR_LOGON_USERID, &logonUserID, sizeof(logonUserID));
	CCLog("send:%d", isSend);

	if (!isSend)
	{
		//stopTcpSocket();
		return;
	}
}
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
//#include "iconv.h"
#include "cocos2d.h"
using namespace std;

DefaultListerner1::DefaultListerner1()
{
}

DefaultListerner1::~DefaultListerner1()
{
}

void DefaultListerner1::OnClose(TCPSocket* so, bool fromRemote)
{
    CCLog("%s\n","closed");
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

				CMD_GR_UserSitDown sit;
				sit.wTableID=1;
				sit.wChairID=1;
				strcpy(sit.szPassword,"");

				bool isSend=so->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
				CCLog("Classic:%d",isSend);
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
		case SUB_GR_USER_STATUS://用户状态
			{
				int wSize=wDataSize;
				int size =sizeof(CMD_GR_UserStatus);
				 CMD_GR_UserStatus *info= (CMD_GR_UserStatus*)pDataBuffer;
				// CCLog("state %d",info->dwUserID);
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
	else 
	{
		CCLog("error---------- %d    %d",Command.wMainCmdID,Command.wSubCmdID);
	}
	return true;
}

void DefaultListerner1::OnOpen(TCPSocket* so)
{
	CCLog("open22222==========================");
	CMD_GR_LogonUserID logonUserID;
	memset(&logonUserID, 0, sizeof(CMD_GR_LogonUserID));
	//logonUserID.dwFrameVersion=17235969;
	//logonUserID.dwPlazaVersion=16777217;
	//logonUserID.dwProcessVersion=16908289;
	logonUserID.dwFrameVersion=16777217;
	logonUserID.dwPlazaVersion=17235969;
	logonUserID.dwProcessVersion= 17104897;
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


	int luidSize=sizeof(CMD_GR_LogonUserID);
	bool isSend = so->SendData(MDM_GR_LOGON, SUB_GR_LOGON_USERID, &logonUserID, sizeof(logonUserID));
	CCLog("send:%d", isSend);

	if (!isSend)
	{
		//stopTcpSocket();
		return;
	}
}
//
//  TCPLogon.cpp
//  
//
//  Created by on 15/3/16.
//
//

#include "TCPLogon.h"
#include "StructLogon.h"
#include "MD5.h"
#include "DataModel.h"
//#include "GameLobbyScene.h"
pthread_t TCPLogon::threadLogon;
TCPLogon * TCPLogon::logon;
TCPLogon::TCPLogon():
isReadData(true)
{
	logon=this;
}
TCPLogon::~TCPLogon(){
	CCLog("~ <<%s>>", __FUNCTION__);
}

void TCPLogon::onEnter(){
	CCNode::onEnter();
}
void TCPLogon::onExit(){
	CCNode::onExit();
}
int TCPLogon::startSendThread(){
	int errCode = 0;
	do{
		pthread_attr_t tAttr;
		errCode = pthread_attr_init(&tAttr);

		CC_BREAK_IF(errCode != 0);

		errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);

		if (errCode != 0) {
			pthread_attr_destroy(&tAttr);
			break;
		}

		errCode = pthread_create(&threadLogon, &tAttr, networkThread, this);
		pthread_detach(threadLogon);
	} while (0);
	return errCode;
}
void* TCPLogon::networkThread(void*){
	logon->sendData();
	return 0;
}
void TCPLogon::stopTcpSocket(){
	//pthread_exit(&threadLogon);
	isReadData = false;
	pthread_detach(threadLogon);
	//Select();
	Close();
}
void TCPLogon::sendData(){
	/*Init();
	Create(AF_INET, SOCK_STREAM, 0);
	bool isConnect=Connect("125.88.145.41", 8211);
	CCLog("Connect:%d", isConnect);
	if (!isConnect)
	{
		return;
	}
	//bool isConnect=Connect("192.168.0.104", 8100);
	
	CMD_GR_LogonUserID logonUserID;
	logonUserID.dwFrameVersion = 17235969;
	logonUserID.dwPlazaVersion =17235969;
	logonUserID.dwProcessVersion=17235969;
	logonUserID.dwUserID = 4945;//
	int s=sizeof(CMD_GR_LogonUserID);
	
	strcpy(logonUserID.szMachineID,"1232345");
	strcpy(logonUserID.szPassPortID,"12354343");

	MD5 m;
	MD5::char8 str[] = "z12345678";
	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();
	strcpy(logonUserID.szPassword,md5PassWord.c_str());

	strcpy(logonUserID.szPhoneVerifyID,"123");

	logonUserID.wKindID=210;
										 
	bool isSend = SendData(MDM_MB_LOGON, SUB_GR_LOGON_USERID, &logonUserID, sizeof(logonUserID));
	CCLog("send:%d", isSend);
	if (!isSend)
	{
		return;
	}
	//while (isReadData)
	{
		CCLog("begin-------------------------------------------");
		bool isReadData = OnSocketNotifyRead(0, 0);
		CCLog("read:%d", isReadData);
	}*/
	Init();
	Create(AF_INET, SOCK_STREAM, 0);
	bool isConnect=Connect("125.88.145.41", 8100);
	//bool isConnect=Connect("192.168.0.104", 8100);
	CCLog("Connect:%d", isConnect);
	if (!isConnect)
	{
		stopTcpSocket();
		return;
	}
	
	

	CMD_MB_LogonAccounts logonAccounts;
	//memset(&logonAccounts, 0, sizeof(CMD_MB_LogonAccounts));
	logonAccounts.cbDeviceType = 2;
	logonAccounts.dwPlazaVersion = 17235969;
	

	//_tcscpy(logonAccounts.szPassword, _TEXT("123456"));
	//_tcscpy(logonAccounts.szAccounts, _TEXT("z40144322"));
	strcpy(logonAccounts.szAccounts,"z40144322");

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
	m.ComputMd5(str, sizeof(str)-1);
	std::string md5PassWord = m.GetMd5();

	strcpy(logonAccounts.szPassword,md5PassWord.c_str());
	//_tcscpy(logonAccounts.szPassword, _TEXT(md5PassWord.c_str()));

	bool isSend = SendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	CCLog("send:%d", isSend);
	
	if (!isSend)
	{
		stopTcpSocket();
		return;
	}
	while (isReadData)
	{
		CCLog("begin-------------------------------------------");
		isReadData = OnSocketNotifyRead(0, 0);
		CCLog("read:%d", isReadData);
	}
	stopTcpSocket();
}
bool TCPLogon::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize){
	if (Command.wMainCmdID == MDM_GP_LOGON)
	{
		if (Command.wSubCmdID == SUB_GP_UPDATE_NOTIFY)
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
			if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return false;

			CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
			CCLog("升级提示");
		}
	}
	//登录失败
	if (Command.wMainCmdID == MDM_MB_LOGON)
	{
		if (Command.wSubCmdID == SUB_GP_LOGON_FAILURE)
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_MB_LogonSuccess));
			if (wDataSize < sizeof(CMD_MB_LogonSuccess)) return false;

			CMD_MB_LogonFailure *lf = (CMD_MB_LogonFailure*)pDataBuffer;
			long code = lf->lResultCode;
			char *describeStr = lf->szDescribeString;
			CCLog("%s", describeStr);
		}
	}
	//登录成功
	if (Command.wMainCmdID == MDM_MB_LOGON)
	{
		if (Command.wSubCmdID == SUB_GP_LOGON_SUCCESS)
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
			CCLog("登录成功");
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
			return 0;
			//tagGameServer *gs = (tagGameServer*)pDataBuffer;
		}
	}
	return 1;
}
//
//  TCPLogonID.cpp
//  
//
//  Created by on 15/3/16.
//
//

#include "TCPLogonID.h"
#include "CMD_GameServer.h"

#include "MD5.h"
#include "DataModel.h"
#include "DefaultListerner1.h"
pthread_t TCPLogonID::threadLogonID;
TCPLogonID * TCPLogonID::logonID;
TCPLogonID::TCPLogonID():
isReadData(true)
{
	logonID=this;
}
TCPLogonID::~TCPLogonID(){
	CCLog("~ <<%s>>", __FUNCTION__);
}

void TCPLogonID::onEnter(){
	CCNode::onEnter();
}
void TCPLogonID::onExit(){
	CCNode::onExit();
}
int TCPLogonID::startSendThread(){
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

		errCode = pthread_create(&threadLogonID, &tAttr, networkThread, this);
		pthread_detach(threadLogonID);
	} while (0);
	return errCode;
}
void* TCPLogonID::networkThread(void*){
	logonID->sendData(DataModel::sharedDataModel()->tagGameServerList[0]->szServerAddr,DataModel::sharedDataModel()->tagGameServerList[0]->wServerPort);
	return 0;
}
void TCPLogonID::stopTcpSocket(){
	//pthread_exit(&threadLogon);
	isReadData = false;
	pthread_detach(threadLogonID);
	//Select();
	Close();
}
void TCPLogonID::sendData(const char* ip, unsigned short port){
	Init();
	Create(AF_INET, SOCK_STREAM, 0);
	SetListerner(new DefaultListerner1());
	bool isConnect=Connect(ip, port);
	//bool isConnect=Connect("192.168.0.104", 8100);

	CCLog("Connect:%d", isConnect);
	if (!isConnect)
	{
		stopTcpSocket();
		return;
	}
	/*Init();
	Create(AF_INET, SOCK_STREAM, 0);
	bool isConnect=Connect(ip, port);
	CCLog("Connect:%d", isConnect);
	if (!isConnect)
	{
		stopTcpSocket();
		return;
	}
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
	bool isSend = SendData(MDM_GR_LOGON, SUB_GR_LOGON_USERID, &logonUserID, sizeof(logonUserID));
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
	stopTcpSocket();*/
}
bool TCPLogonID::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command Command, void * pDataBuffer, unsigned short wDataSize){
	if (Command.wMainCmdID == MDM_GR_LOGON)
	{
		if (Command.wSubCmdID == SUB_GR_UPDATE_NOTIFY)
		{
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
			if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return false;

			CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
			CCLog("==");
		}
		if (Command.wSubCmdID == SUB_GR_LOGON_SUCCESS)
		{
			int size=sizeof(CMD_GR_LogonSuccess);
			//效验参数
			assert(wDataSize >= sizeof(CMD_GR_LogonSuccess));
			if (wDataSize < sizeof(CMD_GR_LogonSuccess)) return false;

			CMD_GR_LogonSuccess *lf = (CMD_GR_LogonSuccess*)pDataBuffer;
			CCLog("==");
		}
	}
	if (Command.wMainCmdID==MDM_GR_CONFIG)
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
			}
			break;
		default:
			break;
		}
		
		
	}
	if (Command.wMainCmdID==MDM_GR_USER){
		switch (Command.wSubCmdID)
		{
		case SUB_GR_USER_ENTER://用户进入
			{
				int wSize=wDataSize;
				int size =sizeof(tagUserInfoHead);
				tagUserInfoHead *info= (tagUserInfoHead*)pDataBuffer;
  				CCLog("用户进入");
			}
			break;
		default:
			break;
		}
	}
	CCLog("main:%d  sub:%d",Command.wMainCmdID,Command.wSubCmdID);
	return 1;
}
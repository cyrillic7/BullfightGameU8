

#include "LobbyMsgHandler.h"
#include "../Tools/DataModel.h"
#include "../Tools/Tools.h"
#include "../LogonScene/LogonScene.h"
#include "../Network/MD5/MD5.h"
#include "../MTNotificationQueue/MessageQueueLobby.h"
#include "../MTNotificationQueue/MTNotificationQueue.h"
#include "../Network/SEvent.h"
LobbyMsgHandler* _sharedSocketControl;
LobbyMsgHandler::LobbyMsgHandler()
{
	this->onEnter();
	this->onEnterTransitionDidFinish();
	this->scheduleUpdate();
}
LobbyMsgHandler::~LobbyMsgHandler() {
}
LobbyMsgHandler* LobbyMsgHandler::sharedLobbyMsgHandler()
{
	if (_sharedSocketControl == 0) {
		_sharedSocketControl = new LobbyMsgHandler();
		//_sharedSocketControl->initSocketControl();
		return _sharedSocketControl;
	}
	return _sharedSocketControl;
}
//连接服务器
void LobbyMsgHandler::connectServer(std::string sLobbyIp, long lLobbyProt){
	if (gameSocket.getSocketState() != CGameSocket::SOCKET_STATE_CONNECT_SUCCESS)
	{
		gameSocket.setIGameSocket(this);
		gameSocket.Create(sLobbyIp.c_str(), lLobbyProt);


		CMD_GL_LogonAccounts LogonAccounts;
		strcpy(LogonAccounts.szAccounts, DataModel::sharedDataModel()->sLogonAccount.c_str());
		strcpy(LogonAccounts.szMachineID, "12");

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(LogonAccounts.szPassword, md5PassWord.c_str());
		gameSocket.SendData(MDM_GL_C_DATA, SUB_GL_MB_LOGON_ACCOUNTS, &LogonAccounts, sizeof(LogonAccounts));
		
	}
}
void LobbyMsgHandler::update(float dt){
	gameSocket.updateSocketData(dt);
}
void LobbyMsgHandler::onOpen(){
	CCLOG("open <<%s>>", __FUNCTION__);
}
void LobbyMsgHandler::onError(const char* sError){
	DataModel::sharedDataModel()->vecSystemMsg.clear();
	DataModel::sharedDataModel()->vecMyMsg.clear();
	connectServer(DataModel::sharedDataModel()->sLobbyIp, DataModel::sharedDataModel()->lLobbyProt);
	return;
	ReadData rData;
	rData.wMainCmdID = MDM_MB_UNCONNECT;
	rData.wSubCmdID = SUB_MB_SOCKET_NETWORK_ERROR;
	rData.wDataSize = 0;
	memcpy(rData.sReadData, 0, 0);
	//MessageQueue::pushQueue(rData);
	MessageQueueLobby::pushQueue(rData);
}
bool LobbyMsgHandler::onMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	//onEventReadMessage(wMainCmdID, wSubCmdID, pDataBuffer, wDataSize);
	if (wMainCmdID == MDM_GL_C_DATA)
	{
		switch (wSubCmdID)
		{
		case SUB_GL_C_TASK_LOAD:
		case SUB_GL_C_TASK_REWARD:
		{
			ReadData rData;
			rData.wMainCmdID = wMainCmdID;
			rData.wSubCmdID = wSubCmdID;
			rData.wDataSize = wDataSize;
			memcpy(rData.sReadData, pDataBuffer, wDataSize);
			MessageQueueLobby::pushQueue(rData);
		}
			break;
		case SUB_GL_C_MESSAGE://消息
		{
			CMD_GL_MsgNode *msgNode = (CMD_GL_MsgNode*)pDataBuffer;

			std::string msg = msgNode->szMsgcontent;

			//////////////////////////////////////////////////////////////////////////
			if (msgNode->dwMsgType == Msg_Notice)
			{
				newNoticeMsgTip(msgNode);
			}
			else if (msgNode->dwMsgType == Msg_Sell_Success)
			{
				DataModel::sharedDataModel()->isShowNewAuctionMsg = true;
				MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_NEW_MSG_STATE, NULL);
			}
			else if (msgNode->dwMsgType == Msg_Rewards)
			{
				DataModel::sharedDataModel()->isShowNewMsg = true;
				DataModel::sharedDataModel()->isShowNewTaskMsg = true;
				int index = msg.find("#");
				if (index > 0)
				{
					msg = msg.substr(0, index);
				}
				MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_NEW_MSG_STATE, NULL);
			}
			else if (msgNode->dwMsgType == Msg_Delta){//充值成功
				DataModel::sharedDataModel()->isShowNewMsg = true;
				MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_NEW_MSG_STATE, NULL);


				CCArray *strArray = split(msg.c_str(), "#");
				if (strArray->count() >= 1)
				{
					CCString *strNum = (CCString*)strArray->objectAtIndex(1);
					DataModel::sharedDataModel()->userInfo->lIngot = strtoll(strNum->getCString(), NULL, 10);
				}
				MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_DELTA, NULL);

				int index = msg.find("#");
				if (index>0)
				{
					msg = msg.substr(0, index);
				}
			}
			else if (msgNode->dwMsgType == Msg_Vip)//VIP使用
			{
				std::string msgContent=msgNode->szMsgcontent;
				DataModel::sharedDataModel()->userInfo->dwVipLevel = strtol(msgContent.c_str(), NULL, 10);
				MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_VIP, NULL);
			}
			//////////////////////////////////////////////////////////////////////////
			if (msgNode->dwMsgType != Msg_Vip)
			{
				if (msgNode->dwUserID == 0)
				{

					DataModel::sharedDataModel()->vecSystemMsg.push_back(msg);
				}
				else
				{
					DataModel::sharedDataModel()->vecMyMsg.push_back(msg);
				}
			}
			
			//CCLOG("msg main:%d sub:%d %s<<%s>>", wMainCmdID, wSubCmdID, (msgNode->szMsgcontent), __FUNCTION__);
		}
			break;
		case SUB_GL_C_SYSTEM_MESSAGE://大厅系统消息(被迫下线，被挤掉)
		{
			CMD_GL_SystemMessage* info = (CMD_GL_SystemMessage*)pDataBuffer;
			if (info->wType & SMT_CLOSE_HALL)//
			{
				GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.Destroy(true);
				gameSocket.Destroy(true);
				MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_COLSE_GAMEING_UPDATE, NULL);

				PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
				CCDirector::sharedDirector()->getRunningScene()->addChild(tipInfo,10000);
				tipInfo->setTipInfoContent(GBKToUTF8(info->szString).c_str());
				tipInfo->setIPopAssistTipInfo(this);
			}

		}
			break;

		default:
		{
			CCLOG("else main:%d sub:%d <<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		}
			break;
		}
	}
	return true;
}
//关闭回调
void LobbyMsgHandler::onCloseTipInfo(CCLayer *pTipInfo){
	DataModel *m = DataModel::sharedDataModel();
	CC_SAFE_RELEASE_NULL(m);

	pTipInfo->removeAllChildrenWithCleanup(true);


	Tools::setTransitionAnimation(0, 0, LogonScene::scene());
}
//查找消息ID
bool LobbyMsgHandler::checkMsgById(const char *strMsgId, DWORD dwCurMsgID){
	CCArray *strArray = split(strMsgId, ",");
	CCObject *object = NULL;
	CCARRAY_FOREACH(strArray, object)
	{
		CCString *str = (CCString*)object;
		DWORD dwMsgID = strtol(str->getCString(), NULL, 10);
		if (dwMsgID==dwCurMsgID)
		{
			return true;
		}
	}
	return false;
}
//新消息提示
void LobbyMsgHandler::newNoticeMsgTip(CMD_GL_MsgNode *msgNode){
	map<long, NewMsg >::iterator l_it;
	l_it = DataModel::sharedDataModel()->mNewMsg.find(DataModel::sharedDataModel()->userInfo->dwUserID);
	if (l_it != DataModel::sharedDataModel()->mNewMsg.end())
	{
		if (checkMsgById(l_it->second.sMsgId.c_str(), msgNode->dwMsgID))
		{

		}
		else
		{
			l_it->second.sMsgId += CCString::createWithFormat(",%ld", msgNode->dwMsgID)->getCString();
			DataModel::sharedDataModel()->isShowNewMsg = true;
		}
	}
	else
	{
		NewMsg newMsg;
		newMsg.sMsgId = CCString::createWithFormat("%ld", msgNode->dwMsgID)->getCString();
		DataModel::sharedDataModel()->mNewMsg.insert(map<long, NewMsg>::value_type(DataModel::sharedDataModel()->userInfo->dwUserID, newMsg));
		DataModel::sharedDataModel()->isShowNewMsg = true;
	}



	MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_NEW_MSG_STATE, NULL);
}
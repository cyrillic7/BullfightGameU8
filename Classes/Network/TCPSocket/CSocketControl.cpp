/*
 * CSocketControl.cpp
 *
 *  Created on: 2015年3月30日
 *      Author: xw007
 */

#include "CSocketControl.h"
#include "../../Tools/DataModel.h"
#include "../../MTNotificationQueue/MessageQueueLobby.h"
#define SOCKET_LOBBY							"socketLobby"					//大厅长连接

CSocketControl* _sharedSocketControl;
pthread_t threadSocket;


CSocketControl::CSocketControl()
:listerner(NULL)
, isReadMsg(true)
{

}


CSocketControl::~CSocketControl() {

}
CSocketControl* CSocketControl::sharedSocketControl()
{
	if (_sharedSocketControl == 0) {
		_sharedSocketControl = new CSocketControl();
		_sharedSocketControl->initSocketControl();
		return _sharedSocketControl;
	}
	return _sharedSocketControl;
}
void CSocketControl::initSocketControl(){
	
}
int CSocketControl::startSendThread(){
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

		errCode = pthread_create(&threadSocket, &tAttr, networkThread, this);
		pthread_detach(threadSocket);
	} while (0);
	return errCode; 
}
void* CSocketControl::networkThread(void* object){
	CSocketControl *app=(CSocketControl*)object;
	app->initNetwork();
	return 0;
}
//更新socket收发数据
void CSocketControl::updateSocketData(){
	std::map<std::string, CGameSocket*>::iterator iter;
	iter = mGameSocket.find(SOCKET_LOBBY);
	if (iter != mGameSocket.end())
	{
		if (iter->second->getSocketState() == CGameSocket::SOCKET_STATE_FREE) {
			return;
		}
		if (iter->second->getSocketState() == CGameSocket::SOCKET_STATE_ERROR) {
			/*PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
			this->addChild(tipInfo);
			tipInfo->setTipInfoContent("与服务器断开连接");

			PopDialogBoxOtherLoading *pLoading = (PopDialogBoxOtherLoading*)this->getChildByTag(TAG_LOADING);
			if (pLoading)
			{
			pLoading->removeFromParentAndCleanup(true);
			}*/
			CCLog("与服务器断开连接 <<%s>>", __FUNCTION__);
			iter->second->setSocketState(CGameSocket::SOCKET_STATE_FREE);
			//gameSocket.End();
			return;
		}
		if (iter->second->getSocketState() == CGameSocket::SOCKET_STATE_CONNECT_FAILURE) {
			/*PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
			this->addChild(tipInfo);
			tipInfo->setTipInfoContent(" 连接服务器失败 ");

			PopDialogBoxOtherLoading *pLoading = (PopDialogBoxOtherLoading*)this->getChildByTag(TAG_LOADING);
			if (pLoading)
			{
			pLoading->removeFromParentAndCleanup(true);
			}*/
			CCLog(" 连接服务器失败  <<%s>>", __FUNCTION__);
			iter->second->setSocketState(CGameSocket::SOCKET_STATE_FREE);
			//gameSocket.End();
			return;
		}


		if (!iter->second->Check()) {
			//gameSocket = NULL;
			//onConnectionAbort();
			iter->second->setSocketState(CGameSocket::SOCKET_STATE_ERROR);
			// 掉线了
			CCLog("abort------------- <<%s>>", __FUNCTION__);
			return;
		}
		iter->second->Flush();
		// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
		while (true)
		{
			char buffer[_MAX_MSGSIZE] = { 0 };
			int nSize = sizeof(buffer);
			char* pbufMsg = buffer;
			if (iter->second->getSocketState() != CGameSocket::SOCKET_STATE_CONNECT_SUCCESS)
			{
				if (iter->second->getSocketState() != CGameSocket::SOCKET_STATE_FREE)
				{
					iter->second->setSocketState(CGameSocket::SOCKET_STATE_ERROR);
				}
				break;
			}
			if (!iter->second->ReceiveMsg(pbufMsg, nSize)) {
				break;
			}
			// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
			TCP_Head* pHead = (TCP_Head*)pbufMsg;
			WORD wDataSize = nSize - sizeof(TCP_Head);
			void * pDataBuffer = pbufMsg + sizeof(TCP_Head);


			if (pHead->CommandInfo.wMainCmdID == MDM_GL_C_DATA)
			{
				if (pHead->CommandInfo.wSubCmdID == SUB_GL_C_TASK_LOAD
					|| pHead->CommandInfo.wSubCmdID == SUB_GL_C_TASK_REWARD)
				{
					ReadData rData;
					rData.wMainCmdID = pHead->CommandInfo.wMainCmdID;
					rData.wSubCmdID = pHead->CommandInfo.wSubCmdID;
					rData.wDataSize = wDataSize;
					memcpy(rData.sReadData, pDataBuffer, wDataSize);
					MessageQueueLobby::pushQueue(rData);
				}
				else if (pHead->CommandInfo.wSubCmdID == SUB_GL_C_MESSAGE)//消息
				{
					CMD_GL_MsgNode *msgNode = (CMD_GL_MsgNode*)pDataBuffer;

					std::string msg = msgNode->szMsgcontent;
					if (msgNode->dwUserID == 0)
					{

						DataModel::sharedDataModel()->vecSystemMsg.push_back(msg);
					}
					else
					{
						DataModel::sharedDataModel()->vecMyMsg.push_back(msg);
					}
					//CCLog("msg main:%d sub:%d %s<<%s>>", Command.wMainCmdID, Command.wSubCmdID, Tools::GBKToUTF8(msgNode->szMsgcontent), __FUNCTION__);
				}
				else
				{
					//CCLog("else main:%d sub:%d <<%s>>", Command.wMainCmdID, Command.wSubCmdID, __FUNCTION__);
				}
			}
			//onEventReadMessage(pHead->CommandInfo.wMainCmdID, pHead->CommandInfo.wSubCmdID, pDataBuffer, wDataSize);
		}
		//mGameSocket.erase(iter);

		}
}
void CSocketControl::initNetwork(){
	while (isReadMsg)
	{
		updateSocketData();


		
		/*
		std::map<std::string, CGameSocket>::iterator iter;
		//map<int, string*>::iterator it;
		for (iter = mGameSocket.begin(); iter != mGameSocket.end(); ++iter)
		{
			//cout << "key: " << it->first << " value: " << *it->second << endl;
			
			*/

		//Sleep(100);
		//CCLog("=========================== <<%s>>", __FUNCTION__);
	}
}

CGameSocket* CSocketControl::getTCPSocket(std::string key){
	std::map<std::string,CGameSocket*>::iterator iter;
	iter=mGameSocket.find(key);
	if (iter==mGameSocket.end())
	{
		CGameSocket *gameSocket=new CGameSocket();
		
		mGameSocket.insert(std::map<std::string, CGameSocket*>::value_type(key, gameSocket));
		return gameSocket;
	}
	return iter->second;
}
void CSocketControl::removeTCPSocket(std::string key){
	isReadMsg = false;
	delete getTCPSocket(key);
	mGameSocket.erase(key);
}
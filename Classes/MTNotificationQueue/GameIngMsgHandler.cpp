

#include "GameIngMsgHandler.h"
#include "../Tools/DataModel.h"
#include "../Network/MD5/MD5.h"
#include "../MTNotificationQueue/MessageQueueGameIng.h"
GameIngMsgHandler* _sharedGameIng;
GameIngMsgHandler::GameIngMsgHandler()
{
	this->onEnter();
	this->onEnterTransitionDidFinish();
	this->scheduleUpdate();
}
GameIngMsgHandler::~GameIngMsgHandler() {
}
GameIngMsgHandler* GameIngMsgHandler::sharedGameIngMsgHandler()
{
	if (_sharedGameIng == 0) {
		_sharedGameIng = new GameIngMsgHandler();
		//_sharedSocketControl->initSocketControl();
		return _sharedGameIng;
	}
	return _sharedGameIng;
}
//连接服务器
void GameIngMsgHandler::connectServer(std::string sLobbyIp, int lLobbyProt){
	if (gameSocket.getSocketState() != CGameSocket::SOCKET_STATE_CONNECT_SUCCESS)
	{
		gameSocket.setIGameSocket(this);
		gameSocket.Create(sLobbyIp.c_str(), lLobbyProt);
	}
}
void GameIngMsgHandler::update(float dt){
	gameSocket.updateSocketData(dt);
}
void GameIngMsgHandler::onOpen(){
	CCLOG("open <<%s>>", __FUNCTION__);
}
void GameIngMsgHandler::onError(const char* sError){
	CCLOG("error <<%s>>", __FUNCTION__);
	ReadData rData;
	rData.wMainCmdID = MDM_MB_UNCONNECT;
	rData.wSubCmdID = SUB_MB_SOCKET_NETWORK_ERROR;
	rData.wDataSize = 0;
	memcpy(rData.sReadData, 0, 0);
	//MessageQueue::pushQueue(rData);
	MessageQueueGameIng::pushQueue(rData);
}
bool GameIngMsgHandler::onMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	//onEventReadMessage(wMainCmdID, wSubCmdID, pDataBuffer, wDataSize);
	
	ReadData rData;
	rData.wMainCmdID = wMainCmdID;
	rData.wSubCmdID = wSubCmdID;
	rData.wDataSize = wDataSize;
	memcpy(rData.sReadData, pDataBuffer, wDataSize);
	MessageQueueGameIng::pushQueue(rData);
	return true;
}


#include "MessageQueue.h"
#include "DataModel.h"
#include "CMD_GameServer.h"
MessageQueue::MessageQueue()
{
}
MessageQueue::~MessageQueue() {
}
void MessageQueue::update(float dt){
	if (DataModel::sharedDataModel()->readDataList.size()<=0)
	{
		return;
	}
	//CCLog("size::%d<<%s>>",DataModel::sharedDataModel()->readDataList.size(),__FUNCTION__);
	std::list<ReadData>::iterator iter;
	iter=DataModel::sharedDataModel()->readDataList.begin();
	switch (iter->wMainCmdID)
	{
	case MDM_GR_LOGON://登录
		onEventLogon(iter->wSubCmdID,iter->sReadData,iter->wDataSize);
		break;
	case MDM_GR_CONFIG://配置
		//return configEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GR_USER://用户信息
		onSubUserState(iter->wSubCmdID,iter->sReadData,iter->wDataSize);
		break;
	case MDM_GR_STATUS://状态信息
			break;
	case MDM_GF_FRAME://框架命令
		//return frameEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GF_GAME://游戏命令
		onEventGameIng(iter->wSubCmdID,iter->sReadData,iter->wDataSize);
		break;
	default:
		CCLog("other---------- %d    %d<<%s>>",iter->wMainCmdID,iter->wSubCmdID,__FUNCTION__);
		break;
	}
	DataModel::sharedDataModel()->readDataList.pop_front();
}
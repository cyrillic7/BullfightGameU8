

#include "MessageQueueLobby.h"
#include "../Tools/DataModel.h"
#include "../Network/CMD_Server/CMD_GameServer.h"
//////////////////////////////////////////////////////////////////////////
#include <pthread.h>
pthread_mutex_t sharedNotificationQueueLock2;

class LifeManager_PThreadMutex2  
{  
	pthread_mutex_t* mutex;  
public:  
	LifeManager_PThreadMutex2(pthread_mutex_t* mut) : mutex(mut)  
	{  
		pthread_mutex_init(mutex, NULL);  
	}  
	~LifeManager_PThreadMutex2()  
	{  
		pthread_mutex_destroy(mutex);  
	}  
}__LifeManager_sharedNotificationQueueLock2(&sharedNotificationQueueLock2);
class LifeCircleMutexLocker2
{  
	pthread_mutex_t* mutex1;  

public:  
	LifeCircleMutexLocker2(pthread_mutex_t* aMutex) : mutex1(aMutex)  
	{  
		pthread_mutex_lock(mutex1);  
	}  
	~LifeCircleMutexLocker2(){  
		pthread_mutex_unlock(mutex1);  
	}  
};  

#define LifeCircleMutexLock2(mutex) LifeCircleMutexLocker2 __locker__(mutex) 
//////////////////////////////////////////////////////////////////////////
MessageQueueLobby::MessageQueueLobby()
{
}
MessageQueueLobby::~MessageQueueLobby() {
}
void MessageQueueLobby::update(float dt){
	if (DataModel::sharedDataModel()->readDataQueueLobby.size() <= 0)
	{
		return;
	}
	if (DataModel::sharedDataModel()->readDataQueueLobby.empty())
	{
		return;
	}
	LifeCircleMutexLock2(&sharedNotificationQueueLock2);
    

	ReadData iter = DataModel::sharedDataModel()->readDataQueueLobby.front();
	onEventReadMessageLobby(iter.wMainCmdID,iter.wSubCmdID,iter.sReadData,iter.wDataSize);
	/*switch (iter.wMainCmdID)
	{
//	case MDM_MB_LOGON://登录
	//case MDM_GP_LOGON:
	//	onEventLogon(iter.wSubCmdID,iter.sReadData,iter.wDataSize);
	//	break;
	case MDM_GR_CONFIG://配置
		//return configEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GR_USER://用户信息
		onSubUserState(iter.wSubCmdID,iter.sReadData,iter.wDataSize);
		break;
	case MDM_GR_STATUS://状态信息
			break;
	case MDM_GF_FRAME://框架命令
		//return frameEvent(so,Command.wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GF_GAME://游戏命令
		onEventGameIng(iter.wSubCmdID,iter.sReadData,iter.wDataSize);
		break;
	
	default:
		CCLog("other---------- %d    %d<<%s>>",iter.wMainCmdID,iter.wSubCmdID,__FUNCTION__);
		break;
	}*/
	DataModel::sharedDataModel()->readDataQueueLobby.pop();
}
void MessageQueueLobby::pushQueue(ReadData rData){
	LifeCircleMutexLock2(&sharedNotificationQueueLock2); 
	DataModel::sharedDataModel()->readDataQueueLobby.push(rData);
}

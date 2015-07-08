

#include "MessageQueueGameIng.h"
#include "../Tools/DataModel.h"
#include "../Network/CMD_Server/CMD_GameServer.h"
//////////////////////////////////////////////////////////////////////////
#include <pthread.h>
pthread_mutex_t sharedNotificationQueueLock3;

class LifeManager_PThreadMutex3  
{  
	pthread_mutex_t* mutex;  
public:  
	LifeManager_PThreadMutex3(pthread_mutex_t* mut) : mutex(mut)  
	{  
		pthread_mutex_init(mutex, NULL);  
	}  
	~LifeManager_PThreadMutex3()  
	{  
		pthread_mutex_destroy(mutex);  
	}  
}__LifeManager_sharedNotificationQueueLock3(&sharedNotificationQueueLock3);
class LifeCircleMutexLocker3
{  
	pthread_mutex_t* mutex3;  

public:  
	LifeCircleMutexLocker3(pthread_mutex_t* aMutex) : mutex3(aMutex)  
	{  
		pthread_mutex_lock(mutex3);  
	}  
	~LifeCircleMutexLocker3(){  
		pthread_mutex_unlock(mutex3);  
	}  
};  

#define LifeCircleMutexLock3(mutex) LifeCircleMutexLocker3 __locker__(mutex) 
//////////////////////////////////////////////////////////////////////////
MessageQueueGameIng::MessageQueueGameIng()
{
}
MessageQueueGameIng::~MessageQueueGameIng() {
}
void MessageQueueGameIng::update(float dt){
	if (DataModel::sharedDataModel()->readDataQueueGameIng.size() <= 0)
	{
		return;
	}
	if (DataModel::sharedDataModel()->readDataQueueGameIng.empty())
	{
		return;
	}
	LifeCircleMutexLock3(&sharedNotificationQueueLock3);
    

	ReadData iter = DataModel::sharedDataModel()->readDataQueueGameIng.front();
	onEventReadMessage(iter.wMainCmdID,iter.wSubCmdID,iter.sReadData,iter.wDataSize);
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
	DataModel::sharedDataModel()->readDataQueueGameIng.pop();
}
void MessageQueueGameIng::pushQueue(ReadData rData){
	LifeCircleMutexLock3(&sharedNotificationQueueLock3); 
	DataModel::sharedDataModel()->readDataQueueGameIng.push(rData);
}



#include "MessageQueue.h"
#include "Tools/DataModel.h"
#include "Network/CMD_Server/CMD_GameServer.h"
//////////////////////////////////////////////////////////////////////////
#include <pthread.h>
pthread_mutex_t sharedNotificationQueueLock1;

class LifeManager_PThreadMutex1  
{  
	pthread_mutex_t* mutex;  
public:  
	LifeManager_PThreadMutex1(pthread_mutex_t* mut) : mutex(mut)  
	{  
		pthread_mutex_init(mutex, NULL);  
	}  
	~LifeManager_PThreadMutex1()  
	{  
		pthread_mutex_destroy(mutex);  
	}  
}__LifeManager_sharedNotificationQueueLock1(&sharedNotificationQueueLock1);
class LifeCircleMutexLocker1
{  
	pthread_mutex_t* mutex1;  

public:  
	LifeCircleMutexLocker1(pthread_mutex_t* aMutex) : mutex1(aMutex)  
	{  
		pthread_mutex_lock(mutex1);  
	}  
	~LifeCircleMutexLocker1(){  
		pthread_mutex_unlock(mutex1);  
	}  
};  

#define LifeCircleMutexLock1(mutex) LifeCircleMutexLocker1 __locker__(mutex) 
//////////////////////////////////////////////////////////////////////////
MessageQueue::MessageQueue()
{
}
MessageQueue::~MessageQueue() {
}
void MessageQueue::update(float dt){
	if (DataModel::sharedDataModel()->readDataQueue.size()<=0)
	{
		return;
	}
	if (DataModel::sharedDataModel()->readDataQueue.empty())
	{
		return;
	}
	LifeCircleMutexLock1(&sharedNotificationQueueLock1); 

 	ReadData iter =DataModel::sharedDataModel()->readDataQueue.front();
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
	DataModel::sharedDataModel()->readDataQueue.pop();
}
void MessageQueue::pushQueue(ReadData rData){
	LifeCircleMutexLock1(&sharedNotificationQueueLock1); 
	DataModel::sharedDataModel()->readDataQueue.push(rData);
}

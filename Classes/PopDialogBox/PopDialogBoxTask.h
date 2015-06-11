/*
 * PopDialogBoxTask.h
 *	任务
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MTNotificationQueue/MessageQueueLobby.h"
class PopDialogBoxTask : public PopDialogBox,public MessageQueueLobby {
private:

    
public:
	PopDialogBoxTask();
	~PopDialogBoxTask();
	CREATE_FUNC(PopDialogBoxTask);
private:
	virtual void onEnter();
	virtual void onExit();
	TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOBBY); }
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络数据
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};

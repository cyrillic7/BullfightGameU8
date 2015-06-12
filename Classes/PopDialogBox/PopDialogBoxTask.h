/*
 * PopDialogBoxTask.h
 *	任务
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MTNotificationQueue/MessageQueueLobby.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
class PopDialogBoxTask : public PopDialogBox,public MessageQueueLobby {
private:
	//任务列表
	std::vector<CMD_GL_TaskInfo> vecTaskInfo;
	//任务列表
	UIListView *pLVTask;
	//连接失败容器
	UIPanel *pPFailure;
public:
	PopDialogBoxTask();
	~PopDialogBoxTask();
	CREATE_FUNC(PopDialogBoxTask);
private:
	virtual void onEnter();
	virtual void onExit();
	TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOBBY); }
	//获取任务列表
	void getTashInfoList();
	//领取奖励按键
	void onMenuReward(CCObject *object, TouchEventType type);
	//更新任务列表
	void updateListTask();
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络数据
	virtual void onEventReadMessageLobby(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//任务
	void onEventTask(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//任务列表
	void onSubTaskList(void * pDataBuffer, unsigned short wDataSize);
};

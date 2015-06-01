/*
 * PopDialogBoxKnapsack.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
//#include "../Network/TCPSocket/TCPSocketControl.h"
//#include "../Network/CMD_Server/CMD_LogonServer.h"
#include "../MTNotificationQueue/MessageQueue.h"
#define MAX_SHOP_ITEM_COUNT				2			//最大商店项总数
class PopDialogBoxKnapsack: public PopDialogBox,public MessageQueue {
private:
    
	enum KnapsackItem
	{
		KNAPSACK_LIST=0,				//背包列表
	};
	CC_SYNTHESIZE(KnapsackItem, knapsackItem, KnapsackItem);
	
public:
	PopDialogBoxKnapsack();
	~PopDialogBoxKnapsack();
	CREATE_FUNC(PopDialogBoxKnapsack);
private:
	virtual void onEnter();
	virtual void onExit();
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_KNAPSACK); }
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};

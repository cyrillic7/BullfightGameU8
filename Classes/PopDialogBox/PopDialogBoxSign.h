/*
 * PopDialogBoxSign.h
 *	签到
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MTNotificationQueue/MessageQueue.h"
class PopDialogBoxSign : public PopDialogBox, public MessageQueue {
private:

    
public:
	PopDialogBoxSign();
	~PopDialogBoxSign();
	CREATE_FUNC(PopDialogBoxSign);
private:
	virtual void onEnter();
	virtual void onExit();

	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};

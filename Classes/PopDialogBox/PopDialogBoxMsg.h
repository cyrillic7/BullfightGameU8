/*
 * PopDialogBoxMsg.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxMsg: public PopDialogBox {
private:
	enum MsgType
	{
		MSG_SYSTEM=1,				//系统消息
		MSG_MY,						//我的消息
	};
	CC_SYNTHESIZE(MsgType, eMsgType, MsgType);
	//游标
	UIImageView *pIVCursor;
	//消息名称
	UILabel *pLMsgName;
	//没有消息
	UIImageView *pIVNothing;
public:
	PopDialogBoxMsg();
	~PopDialogBoxMsg();
	CREATE_FUNC(PopDialogBoxMsg);
private:
	virtual void onEnter();
	virtual void onExit();
	//消息按键回调
	void onMenuMsg(CCObject *object, TouchEventType type);
};

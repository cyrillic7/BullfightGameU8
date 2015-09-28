/*
 * PopDialogBoxFirstRecharge.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxFirstRecharge: public PopDialogBox {
private:
	enum FRechargeActionType
	{
		FR_ACTION_SEND_ORDER=0,					//下订单
	};
	CC_SYNTHESIZE(FRechargeActionType, eFRechargeActionType, FRechargeActionType);

	//订单号
	std::string orderID;
public:
	PopDialogBoxFirstRecharge();
	~PopDialogBoxFirstRecharge();
	CREATE_FUNC(PopDialogBoxFirstRecharge);
private:
	virtual void onEnter();
	virtual void onExit();
	//兑换按键
	void onMenuExchange(CCObject *object, TouchEventType type);

	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();

};

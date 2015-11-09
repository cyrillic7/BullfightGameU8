/*
 * PopDialogBoxGiftExchang.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxGiftExchang: public PopDialogBox {
private:
	//兑换码
	TextField *pTFExchangCode;
public:
	PopDialogBoxGiftExchang();
	~PopDialogBoxGiftExchang();
	CREATE_FUNC(PopDialogBoxGiftExchang);
private:
	virtual void onEnter();
	virtual void onExit();
	//兑换
	void onMenuExchang(CCObject *object, TouchEventType type);


	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
};

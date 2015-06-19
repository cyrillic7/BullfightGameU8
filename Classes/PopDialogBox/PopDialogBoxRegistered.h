/*
 * PopDialogBoxRegistered.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
struct IPopAssistRegistered//弹框辅助接口注册
{
	virtual ~IPopAssistRegistered(){}
	//注册回调
	virtual void onRegistered(const char *sAccount,const char*sNickname,const char*sPassword) = 0;
};
class PopDialogBoxRegistered: public PopDialogBox {
public:
	CC_SYNTHESIZE(IPopAssistRegistered*, pIPopAssistRegistered, IPopAssistRegistered);
private:
	//帐号
	UITextField *pTFAccount;
	//昵称
	UITextField *pTFNickname;
	//密码
	UITextField *pTFPassword;
public:
	PopDialogBoxRegistered();
	~PopDialogBoxRegistered();
	CREATE_FUNC(PopDialogBoxRegistered);
private:
	virtual void onEnter();
	virtual void onExit();
	//注册
	void onMenuRegistered(CCObject *object, TouchEventType type);
};

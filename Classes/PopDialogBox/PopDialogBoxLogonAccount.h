/*
 * PopDialogBoxLogonAccount.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxLogonAccount: public PopDialogBox {
private:
    
	UITextField *pTAccount;
	UITextField *pTPassword; 
public:
	PopDialogBoxLogonAccount();
	~PopDialogBoxLogonAccount();
	CREATE_FUNC(PopDialogBoxLogonAccount);
private:
	virtual void onEnter();
	virtual void onExit();

	void onTextFieldAccount(CCObject* obj, TextFiledEventType type);
	void onMenuLogon(CCObject *object, TouchEventType type);
};

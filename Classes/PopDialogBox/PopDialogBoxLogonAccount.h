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
	//更多帐号列表
	UIListView *pLVMoreAccount;
	//std::string sTempAccount;
public:
	PopDialogBoxLogonAccount();
	~PopDialogBoxLogonAccount();
	CREATE_FUNC(PopDialogBoxLogonAccount);
private:
	virtual void onEnter();
	virtual void onExit();

	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//显示更多帐号
	void showMoreAccount(bool isShow);

	//更新更多帐号列表
	void updateListViewMoreAccount();

	void onTextFieldAccount(CCObject* obj, TextFiledEventType type);
	void onMenuLogon(CCObject *object, TouchEventType type);
	//选择帐号
	void onMenuSelectAccount(CCObject *object, TouchEventType type);
	//删除更多帐号
	void onMenuDeleteAccount(CCObject *object, TouchEventType type);
	//更多帐号
	void onMenuMoreAccount(CCObject *object, TouchEventType type);
	//找回密码
	void onMenuRetrievePwd(CCObject *object, TouchEventType type);

	//输入框回调
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
};

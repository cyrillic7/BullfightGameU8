/*
 * PopDialogBoxUserInfo.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxUserInfo: public PopDialogBox {
private:

    
	//昵称输入框
	UITextField *pLabelNickName;
	UIImageView *piNickNameBg;
	UICheckBox *pcbSexGirl;
	UICheckBox *pcbSexBoy;
	//UILabel *plSexBoy;
	//UILabel *plSexGirl;
	UIImageView *piSexIcon;
	UILabel *plSexBoyInfo;
	bool isShowChange;

	UIPanel *ppSexInfo,*ppSexSelect;
public:
	PopDialogBoxUserInfo();
	~PopDialogBoxUserInfo();
	CREATE_FUNC(PopDialogBoxUserInfo);
private:
	virtual void onEnter();
	virtual void onExit();

	//virtual void playAnimation();
	//
	void menuChange(CCObject *object, TouchEventType type);
	//绑定手机按键
	void onMenuBindingPhone(CCObject *object, TouchEventType type);
	void setShowChangeView();
	//更新性别
	void updateSex();
	//复选框回调（选择性别）
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
};

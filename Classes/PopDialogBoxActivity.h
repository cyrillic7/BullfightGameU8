/*
 * PopDialogBoxActivity.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxActivity: public PopDialogBox {
private:
	//切换活动复选框
	UICheckBox *pcbActivity[2];
	//
	UIListView *plvReward;
	//
	UIPanel *ppLogonReward;
public:
	PopDialogBoxActivity();
	~PopDialogBoxActivity();
	CREATE_FUNC(PopDialogBoxActivity);
private:
	virtual void onEnter();
	virtual void onExit();
	//复选框回调（选择排名类型）
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
};

/*
 * PopDialogBoxRanking.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxRanking: public PopDialogBox {
private:
	UICheckBox *pcbRanking[2];
public:
	PopDialogBoxRanking();
	~PopDialogBoxRanking();
	CREATE_FUNC(PopDialogBoxRanking);
private:
	virtual void onEnter();
	virtual void onExit();
	//复选框回调（选择排名类型）
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
};

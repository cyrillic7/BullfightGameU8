/*
 * PopDialogBoxHelp.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#define MAX_HELP_COUNT 5
class PopDialogBoxHelp: public PopDialogBox {
private:
    
	std::string helpText[MAX_HELP_COUNT];
	UILabel *plHelpText;
	UIScrollView *scrollViewHelp;

	UICheckBox *pcbHelp[MAX_HELP_COUNT];
public:
	PopDialogBoxHelp();
	~PopDialogBoxHelp();
	CREATE_FUNC(PopDialogBoxHelp);
private:
	virtual void onEnter();
	virtual void onExit();
	//读取帮助内容
	void readHelpText();
	//更新内容
	void updateContent(int index);
	//复选框回调
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
};

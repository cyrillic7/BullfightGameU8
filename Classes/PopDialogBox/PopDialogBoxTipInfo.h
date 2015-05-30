/*
 * PopDialogBoxTipInfo.h
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#pragma once
#include "PopDialogBox.h"
class PopDialogBoxTipInfo :public PopDialogBox{
private:
	UILabel *pLInfo;
public:
	PopDialogBoxTipInfo();
	virtual ~PopDialogBoxTipInfo();
	CREATE_FUNC(PopDialogBoxTipInfo);
	//设置提示内容
	void setTipInfoContent(const char *content);
private:
	virtual void onEnter();
	virtual void onExit();
};

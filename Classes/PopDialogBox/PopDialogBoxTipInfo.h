/*
 * PopDialogBoxTipInfo.h
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#pragma once
#include "PopDialogBox.h"
struct IPopAssistTipInfo//提示语回调
{
	virtual ~IPopAssistTipInfo(){}
	//关闭回调
	virtual void onCloseTipInfo() = 0;
};
class PopDialogBoxTipInfo :public PopDialogBox{
public:
	CC_SYNTHESIZE(IPopAssistTipInfo*, pIPopAssistTipInfo, IPopAssistTipInfo);
private:
	Label *pLInfo;
public:
	PopDialogBoxTipInfo();
	virtual ~PopDialogBoxTipInfo();
	CREATE_FUNC(PopDialogBoxTipInfo);
	//设置提示内容
	void setTipInfoContent(const char *content);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭////////////////////////////////////////////////////////////////////////
	void onMenuCloseTipInfo(CCObject *object, TouchEventType type);
};

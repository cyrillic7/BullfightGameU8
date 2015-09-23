/*
 * PopDialogBoxUpdateTipInfo.h
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#pragma once
#include "PopDialogBox.h"
class PopDialogBoxUpdateTipInfo :public PopDialogBox{
public:
	//关闭
	Button *pBClose;
	int iUpdateType;
private:
	Label *pLInfo;
	std::string strContent;
public:
	PopDialogBoxUpdateTipInfo();
	virtual ~PopDialogBoxUpdateTipInfo();
	CREATE_FUNC(PopDialogBoxUpdateTipInfo);
	//设置提示内容
	void setTipInfoContent(const char *content);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭////////////////////////////////////////////////////////////////////////
	void onMenuCloseTipInfo(CCObject *object, TouchEventType type);
	//确定
	void onMenuSure(CCObject *object, TouchEventType type);
};

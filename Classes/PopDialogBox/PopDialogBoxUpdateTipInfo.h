/*
 * PopDialogBoxUpdateTipInfo.h
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#pragma once
#include "PopDialogBox.h"
#include "../LogonScene/LogonScene.h"
class PopDialogBoxUpdateTipInfo :public PopDialogBox{
public:
	//关闭
	Button *pBClose;
	//int iUpdateType;
private:
	Label *pLInfo;
	//std::string strContent;
	updateInfo uInfo;
public:
	PopDialogBoxUpdateTipInfo();
	virtual ~PopDialogBoxUpdateTipInfo();
	CREATE_FUNC(PopDialogBoxUpdateTipInfo);
	//设置提示内容
	void setTipInfoData(updateInfo upInfo);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭////////////////////////////////////////////////////////////////////////
	void onMenuCloseTipInfo(CCObject *object, TouchEventType type);
	//确定
	void onMenuSure(CCObject *object, TouchEventType type);
};

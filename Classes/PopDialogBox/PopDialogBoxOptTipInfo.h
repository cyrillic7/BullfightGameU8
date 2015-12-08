/*
 * PopDialogBoxOptTipInfo.h
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#pragma once
#include "PopDialogBox.h"
#include "../LogonScene/LogonScene.h"
#include "../MTNotificationQueue/IHornMsgAssist.h"
class PopDialogBoxOptTipInfo :public PopDialogBox{
public:
	CC_SYNTHESIZE(IHornMsgAssist *, iHornMsgAssist, IHornMsgAssist);
	//关闭
	Button *pBClose;
private:
	Label *pLInfo;
	std::string strContent;
public:
	PopDialogBoxOptTipInfo();
	virtual ~PopDialogBoxOptTipInfo();
	CREATE_FUNC(PopDialogBoxOptTipInfo);
	//设置提示内容
	void setTipInfoData(std::string strContent);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭////////////////////////////////////////////////////////////////////////
	void onMenuCloseTipInfo(CCObject *object, TouchEventType type);
	//确定
	void onMenuSure(CCObject *object, TouchEventType type);
};

/*
 * PopDialogBoxSetUp.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxSetUp: public PopDialogBox {
private:

public:
    
	PopDialogBoxSetUp();
	~PopDialogBoxSetUp();
	CREATE_FUNC(PopDialogBoxSetUp);
private:
	virtual void onEnter();
	virtual void onExit();
	//帮助
	void onMenuHelp(CCObject *object, TouchEventType type);
	void onMenuFeedback(CCObject *object, TouchEventType type);
	void onMenuAbout(CCObject *object, TouchEventType type);
	//修改密码
	void onMenuChangePassword(CCObject *object, TouchEventType type);
	//切换帐号
	void onMenuChangeAccount(CCObject *object, TouchEventType type);
};

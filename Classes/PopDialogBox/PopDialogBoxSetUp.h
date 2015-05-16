/*
 * PopDialogBoxSetUp.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxSetUp: public PopDialogBox {
public:
	PopDialogBoxSetUp();
	~PopDialogBoxSetUp();
	CREATE_FUNC(PopDialogBoxSetUp);
private:
	virtual void onEnter();
	virtual void onExit();
	//帮助
	void menuHelp(CCObject *object, TouchEventType type);
	void menuFeedback(CCObject *object, TouchEventType type);
	void menuAbout(CCObject *object, TouchEventType type);
};

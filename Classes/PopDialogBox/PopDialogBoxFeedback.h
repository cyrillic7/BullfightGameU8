/*
 * PopDialogBoxFeedback.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxFeedback: public PopDialogBox {
private:
	//联系方式
	CCEditBox *pEBFeedbackQQ;
	
public:
	PopDialogBoxFeedback();
	~PopDialogBoxFeedback();
	CREATE_FUNC(PopDialogBoxFeedback);
private:
	virtual void onEnter();
	virtual void onExit();
	//反馈////////////////////////////////////////////////////////////////////////
	void onMenuFeedback(CCObject *object, TouchEventType type);
    //输入回调
    void onTextFeild(CCObject* object, TextFiledEventType type);
};

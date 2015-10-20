/*
 * PopDialogBoxShare.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxShare: public PopDialogBox {
private:

public:
	PopDialogBoxShare();
	~PopDialogBoxShare();
	CREATE_FUNC(PopDialogBoxShare);
private:
	virtual void onEnter();
	virtual void onExit();
	//分享朋友圈////////////////////////////////////////////////////////////////////////
	void onMenuShareFriend(CCObject *object, TouchEventType type);
	//分享微信
	void onMenuShareWX(CCObject *object, TouchEventType type);
};

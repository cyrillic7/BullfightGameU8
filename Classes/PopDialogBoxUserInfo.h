/*
 * PopDialogBoxUserInfo.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxUserInfo: public PopDialogBox {
private:
	UIImageView *imageBg;
public:
	PopDialogBoxUserInfo();
	~PopDialogBoxUserInfo();
	CREATE_FUNC(PopDialogBoxUserInfo);
private:
	virtual void onEnter();
	virtual void onExit();

	virtual void playAnimation();
};

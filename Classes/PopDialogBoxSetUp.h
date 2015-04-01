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
	UIImageView *imageBg;
public:
	PopDialogBoxSetUp();
	~PopDialogBoxSetUp();
	CREATE_FUNC(PopDialogBoxSetUp);
private:
	virtual void onEnter();
	virtual void onExit();

	virtual void playAnimation();
};

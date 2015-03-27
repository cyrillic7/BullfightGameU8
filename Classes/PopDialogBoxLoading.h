/*
 * PopDialogBoxLoading.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxLoading: public PopDialogBox {
private:
	UIImageView *imageBg;
public:
	PopDialogBoxLoading();
	~PopDialogBoxLoading();
	CREATE_FUNC(PopDialogBoxLoading);
private:
	virtual void onEnter();
	virtual void onExit();

	virtual void playAnimation();
};

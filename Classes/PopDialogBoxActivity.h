/*
 * PopDialogBoxActivity.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxActivity: public PopDialogBox {
private:

public:
	PopDialogBoxActivity();
	~PopDialogBoxActivity();
	CREATE_FUNC(PopDialogBoxActivity);
private:
	virtual void onEnter();
	virtual void onExit();
};

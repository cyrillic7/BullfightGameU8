/*
 * PopDialogBoxTask.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxTask: public PopDialogBox {
private:

public:
	PopDialogBoxTask();
	~PopDialogBoxTask();
	CREATE_FUNC(PopDialogBoxTask);
private:
	virtual void onEnter();
	virtual void onExit();
};

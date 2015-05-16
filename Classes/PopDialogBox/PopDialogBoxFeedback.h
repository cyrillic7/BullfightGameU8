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

public:
	PopDialogBoxFeedback();
	~PopDialogBoxFeedback();
	CREATE_FUNC(PopDialogBoxFeedback);
private:
	virtual void onEnter();
	virtual void onExit();
};

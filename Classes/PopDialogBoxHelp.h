/*
 * PopDialogBoxHelp.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxHelp: public PopDialogBox {
private:

public:
	PopDialogBoxHelp();
	~PopDialogBoxHelp();
	CREATE_FUNC(PopDialogBoxHelp);
private:
	virtual void onEnter();
	virtual void onExit();
};

/*
 * PopDialogBoxUpBank.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxUpBank: public PopDialogBox {
private:

public:
	PopDialogBoxUpBank();
	~PopDialogBoxUpBank();
	CREATE_FUNC(PopDialogBoxUpBank);
private:
	virtual void onEnter();
	virtual void onExit();
};

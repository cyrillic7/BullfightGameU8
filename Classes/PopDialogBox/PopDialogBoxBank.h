/*
 * PopDialogBoxBank.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxBank: public PopDialogBox {
private:

public:
	PopDialogBoxBank();
	~PopDialogBoxBank();
	CREATE_FUNC(PopDialogBoxBank);
private:
	virtual void onEnter();
	virtual void onExit();
};

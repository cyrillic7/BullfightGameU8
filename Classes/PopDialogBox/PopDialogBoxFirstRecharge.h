/*
 * PopDialogBoxFirstRecharge.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxFirstRecharge: public PopDialogBox {
private:

public:
	PopDialogBoxFirstRecharge();
	~PopDialogBoxFirstRecharge();
	CREATE_FUNC(PopDialogBoxFirstRecharge);
private:
	virtual void onEnter();
	virtual void onExit();
};

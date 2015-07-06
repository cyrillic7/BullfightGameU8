/*
 * PopDialogBoxBindingPhone.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxBindingPhone: public PopDialogBox {
private:

public:
	PopDialogBoxBindingPhone();
	~PopDialogBoxBindingPhone();
	CREATE_FUNC(PopDialogBoxBindingPhone);
private:
	virtual void onEnter();
	virtual void onExit();
};

/*
 * PopDialogBoxShop.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxShop: public PopDialogBox {
private:
public:
	PopDialogBoxShop();
	~PopDialogBoxShop();
	CREATE_FUNC(PopDialogBoxShop);
private:
	virtual void onEnter();
	virtual void onExit();
};

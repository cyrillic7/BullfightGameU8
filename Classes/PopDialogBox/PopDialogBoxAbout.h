/*
 * PopDialogBoxAbout.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxAbout: public PopDialogBox {
private:

public:
	PopDialogBoxAbout();
	~PopDialogBoxAbout();
	CREATE_FUNC(PopDialogBoxAbout);
private:
	virtual void onEnter();
	virtual void onExit();
};

/*
 * PopDialogBoxSign.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxSign: public PopDialogBox {
private:

    
public:
	PopDialogBoxSign();
	~PopDialogBoxSign();
	CREATE_FUNC(PopDialogBoxSign);
private:
	virtual void onEnter();
	virtual void onExit();
};

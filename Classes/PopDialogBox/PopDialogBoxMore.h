/*
 * PopDialogBoxMore.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxMore: public PopDialogBox {
private:

public:
	PopDialogBoxMore();
	~PopDialogBoxMore();
	CREATE_FUNC(PopDialogBoxMore);
private:
	virtual void onEnter();
	virtual void onExit();
};

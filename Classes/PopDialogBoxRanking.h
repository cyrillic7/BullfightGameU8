/*
 * PopDialogBoxRanking.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxRanking: public PopDialogBox {
private:

public:
	PopDialogBoxRanking();
	~PopDialogBoxRanking();
	CREATE_FUNC(PopDialogBoxRanking);
private:
	virtual void onEnter();
	virtual void onExit();
};

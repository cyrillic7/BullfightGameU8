/*
 * PopDialogBoxAuction.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxAuction: public PopDialogBox {
private:

    
public:
	PopDialogBoxAuction();
	~PopDialogBoxAuction();
	CREATE_FUNC(PopDialogBoxAuction);
private:
	virtual void onEnter();
	virtual void onExit();
};

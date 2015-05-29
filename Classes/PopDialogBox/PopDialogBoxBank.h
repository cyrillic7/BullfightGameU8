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
    
	//标题
	UILabel *pLTitle0;
	UILabel *pLTitle1;
public:
	PopDialogBoxBank();
	~PopDialogBoxBank();
	CREATE_FUNC(PopDialogBoxBank);
private:
	virtual void onEnter();
	virtual void onExit();
	//设置标题
	void setTitle(const char * sTitle);
};

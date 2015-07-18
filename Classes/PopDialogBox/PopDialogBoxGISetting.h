/*
 * PopDialogBoxGISetting.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxGISetting: public PopDialogBox {
private:
public:
	PopDialogBoxGISetting();
	~PopDialogBoxGISetting();
	CREATE_FUNC(PopDialogBoxGISetting);
private:
	virtual void onEnter();
	virtual void onExit();

	//音乐开关
	void onMenuMusic(CCObject *object, TouchEventType type);
	//音效开关
	void onMenuSound(CCObject *object, TouchEventType type);
	//更改音乐状态
	void changeMusicButtonState(UIButton *pButton, bool isOpen);
};

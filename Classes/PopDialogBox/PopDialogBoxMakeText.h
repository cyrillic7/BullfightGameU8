/*
 * PopDialogBoxMakeText.h
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#pragma once
#include "PopDialogBox.h"
#include "../LogonScene/LogonScene.h"
class PopDialogBoxMakeText :public PopDialogBox{
public:
	UIImageView *pIVBg;
private:
	Label *pLContent;
public:
	PopDialogBoxMakeText();
	virtual ~PopDialogBoxMakeText();
	CREATE_FUNC(PopDialogBoxMakeText);
	//设置提示内容
	void showMakeText(std::string sContents);
	//向上移动plane
	void movePlane(int y);
private:
	virtual void onEnter();
	virtual void onExit();
	//移除自己
	void removeSelf();
};

/*
 * PopDialogBoxSetUp.h
 *	设置
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxSetUp: public PopDialogBox {
private:

public:
    
	PopDialogBoxSetUp();
	~PopDialogBoxSetUp();
	CREATE_FUNC(PopDialogBoxSetUp);
private:
	virtual void onEnter();
	virtual void onExit();
	//帮助
	void onMenuHelp(CCObject *object, TouchEventType type);
	void onMenuFeedback(CCObject *object, TouchEventType type);
	void onMenuAbout(CCObject *object, TouchEventType type);
	//修改密码
	void onMenuChangePassword(CCObject *object, TouchEventType type);
	//切换帐号
	void onMenuChangeAccount(CCObject *object, TouchEventType type);
	//音乐开关
	void onMenuMusic(CCObject *object, TouchEventType type);
	//音效开关
	void onMenuSound(CCObject *object, TouchEventType type);

	//更改音乐状态
	void changeMusicButtonState(UIButton *pButton,bool isOpen);
};

/*
 * PopDialogBoxForgetPassword.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxForgetPassword: public PopDialogBox {
private:
	//是否是输入帐号界面
	bool isInputId;
	//输入帐号容器
	UIPanel *pPInputId;
	//找到密码容器
	UIPanel *pPForgetPwd;
public:
	PopDialogBoxForgetPassword();
	~PopDialogBoxForgetPassword();
	CREATE_FUNC(PopDialogBoxForgetPassword);
private:
	virtual void onEnter();
	virtual void onExit();
};

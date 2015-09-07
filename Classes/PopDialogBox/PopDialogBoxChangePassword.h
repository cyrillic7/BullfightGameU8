/*
 * PopDialogBoxChangePassword.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxChangePassword : public PopDialogBox {
private:
	enum ChangePasswordType
	{
		CHANGE_LOGON_PASSWORD=1,			//登录密码
		CHANGE_BANK_PASSWORD,				//银行密码
	};
	CC_SYNTHESIZE(ChangePasswordType, eChangePasswordType, ChangePasswordType);

	//标题
	//UILabel *pLTitle0;
	//UILabel *pLTitle1;
	UIImageView *pIVTitle;
	//旧密码
	UITextField *pTFOldPassword;
	//新密码0
	UITextField *pTFNewPassword0;
	//新密码1
	UITextField *pTFNewPassword1;
public:
	PopDialogBoxChangePassword();
	~PopDialogBoxChangePassword();
	CREATE_FUNC(PopDialogBoxChangePassword);
	//设置类型
	void setPasswordType(int type);
private:
	virtual void onEnter();
	virtual void onExit();
	//设置标题
	void setTitle(const char * sTitle);
	//创建密码菜单////////////////////////////////////////////////////////////////////////
	void onMenuChangePassword(CCObject *object, TouchEventType type);

	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};

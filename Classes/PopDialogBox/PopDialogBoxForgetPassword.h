/*
 * PopDialogBoxForgetPassword.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MTNotificationQueue/MessageQueue.h"
#include "PopDialogBoxTipInfo.h"
class PopDialogBoxForgetPassword : public PopDialogBox, public MessageQueue,public IPopAssistTipInfo {
private:
	enum ForgetPwdType
	{
		FORGET_GET_ID=0,					//获取ID
		FORGET_CODE,						//获取验证码
		FORGET_CHANGE_PWD,					//修改密码
	};
	CC_SYNTHESIZE(ForgetPwdType, eForgetPwdType, ForgetPwdType);
	
	//是否是输入帐号界面
	bool isInputId;
	//输入帐号容器
	UIPanel *pPInputId;
	//找到密码容器
	UIPanel *pPForgetPwd;
	//输入帐号框
	UITextField *pTFInputId;
	//下一步按键
	UIButton *pBNext;
	//输入手机号
	CCEditBox *pEBInputPhone;
	//输入新密码
	CCEditBox *pEBInputNewPwd;
	//输入验证码
	CCEditBox *pEBInputCode;
	//用户ID
	DWORD dwUserID;
	//获取验证码按键
	UIButton *pBGetCode;
	//计时器
	int iCurTimeCount;
public:
	PopDialogBoxForgetPassword();
	~PopDialogBoxForgetPassword();
	CREATE_FUNC(PopDialogBoxForgetPassword);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭提示回调
	virtual void onCloseTipInfo();
	//找回密码////////////////////////////////////////////////////////////////////////
	void onMenuForgetPassword(CCObject *object, TouchEventType type);
	//获取验证码
	void onMenuGetCode(CCObject *object, TouchEventType type);


	//1分种重置获取验证码按键
	void updateResetGetCode(float dt);
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//获取ID
	void onSubGetID(void * pDataBuffer, unsigned short wDataSize);
	//获取验证码
	void onSubGetCode(void * pDataBuffer, unsigned short wDataSize);
	//修改密码
	void onSubChangePwd(void * pDataBuffer, unsigned short wDataSize);
};

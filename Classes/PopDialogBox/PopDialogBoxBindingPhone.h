/*
 * PopDialogBoxBindingPhone.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
struct IPopAssistBindingPhone//弹框绑定手机辅助接口
{
	virtual ~IPopAssistBindingPhone(){}
	//关闭绑定phone
	virtual void onCloseBindingPhone() = 0;
};
class PopDialogBoxBindingPhone: public PopDialogBox {
private:
	CC_SYNTHESIZE(IPopAssistBindingPhone*, pIPopAssistBindingPhone, IPopAssistBindingPhone);

	enum BindingPhoneType
	{
		BINDING_GET_CODE=0,						//获取验证码
		BINDING_PHONE,							//绑定手机
	};
	CC_SYNTHESIZE(BindingPhoneType, eBindingPhoneType, BindingPhoneType);

	//输入手机号
	UITextField *pTFPhone;
	//输入验证码
	UITextField *pTFCode;
	//获取验证码
	UIButton *pBGetCode;
public:
	PopDialogBoxBindingPhone();
	~PopDialogBoxBindingPhone();
	CREATE_FUNC(PopDialogBoxBindingPhone);
private:
	virtual void onEnter();
	virtual void onExit();
	//1分种重置获取验证码按键
	void updateResetGetCode(float dt);
	//计时器
	int iCurTimeCount;
	//手机号码
	std::string sPhone;
	//获取验证码按键
	void onMenuGetCode(CCObject *object, TouchEventType type);
	//绑定按键
	void onMenuBinding(CCObject *object, TouchEventType type);
	//////////////////////////////////////////////////////////////////////////
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_BINDING_PHONE); }
	void update(float delta);
	//连接成功
	void connectSuccess();
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};

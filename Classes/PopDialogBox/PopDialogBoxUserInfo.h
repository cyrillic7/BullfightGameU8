/*
 * PopDialogBoxUserInfo.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "PopDialogBoxBindingPhone.h"
class PopDialogBoxUserInfo : public PopDialogBox, public IPopAssistBindingPhone
	, public IPopDialogBoxAssistCloseView {
public:
	CC_SYNTHESIZE(IPopDialogBoxAssist *, pIPopDialogBoxAssist, IPopDialogBoxAssist);
private:
	

	enum UserInfoType
	{
		USER_GET_MONEY=0,					//获取财富
		USER_CHANGE_INFO,					//修改信息
	};
	CC_SYNTHESIZE(UserInfoType, eUserInfoType, UserInfoType);
    
	//昵称输入框
	UITextField *pLabelNickName;
	UIImageView *piNickNameBg;
	UICheckBox *pcbSexGirl;
	UICheckBox *pcbSexBoy;
	//UILabel *plSexBoy;
	//UILabel *plSexGirl;
	//修改按键
	UIButton *bChange;

	UIImageView *piSexIcon;
	UILabel *plSexBoyInfo;
	bool isShowChange;

	UIPanel *ppSexInfo,*ppSexSelect;
	//金币数
	UILabel *pLGoldCount;
	//元宝数
	UILabel *pLBigGoldCount;
	//奖券数
	UILabel *pLVoucherCount;
	//绑定手机
	UIButton *pBBindingPhone;
public:
	PopDialogBoxUserInfo();
	~PopDialogBoxUserInfo();
	CREATE_FUNC(PopDialogBoxUserInfo);
private:
	virtual void onEnter();
	virtual void onExit();
	//重设绑定按键
	void resetBindingButton();
	//关闭绑定手机回调
	virtual void onCloseBindingPhone();
	//关闭窗口回调
	virtual void onCloseView();
	//virtual void playAnimation();
	//
	void onMenuChange(CCObject *object, TouchEventType type);
	//绑定手机按键
	void onMenuBindingPhone(CCObject *object, TouchEventType type);
	//兑换
	void onMenuExchange(CCObject *object, TouchEventType type);
	//充值
	void onMenuRecharge(CCObject *object, TouchEventType type);

	void setShowChangeView();
	//更新性别
	void updateSex();
	//复选框回调（选择性别）
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);

	//////////////////////////////////////////////////////////////////////////
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_USER_INFO); }
	void update(float delta);
	//连接成功
	void connectSuccess();
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//财富
	void onSubTreasure(void * pDataBuffer, unsigned short wDataSize);
};

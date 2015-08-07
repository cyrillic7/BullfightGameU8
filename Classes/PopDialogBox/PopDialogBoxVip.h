/*
 * PopDialogBoxVip.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
#define maxVipLevel  7			//最大等级
class PopDialogBoxVip : public PopDialogBox, public IPopDialogBoxAssistCloseView{
private:
	CC_SYNTHESIZE(IPopDialogBoxAssist *, iIPopAssistVip, IPopDialogBoxAssist);

	enum VipActionType
	{
		VIP_GET_LIST=0,						//获取VIP列表
		VIP_REWARD,							//领取奖励
	};
	CC_SYNTHESIZE(VipActionType, eVipActionType, VipActionType);

	//领取奖励类型
	enum RewardType
	{
		REWARD_GOLD = 1,					//领取金币
		REWARD_RED_MONEY,					//领取红包
		REWARD_SHOP,						//前往商城
	};
	CC_SYNTHESIZE(RewardType, eRewardType, RewardType);
	//VIP等级
	UIImageView *pIVVipGradeIcon;
	//VIP等级数
	UILabelAtlas *pLAVipGradeNum;
	//VIP下一等级数
	UILabelAtlas *pLANextVipGradeNum;
	//vip满级
	UIImageView *pIVFullLevel;
	//VIP字
	UIImageView *pIVVipText;
	//VIP下一等级字
	UIImageView *pIVNextVipText;
	//vip奖励列表
	UIListView *pLVVipReward;
	//下一VIP等级进度
	UILoadingBar *pLBVipNextGrade;
	//下一等级内容描述
	UILabel *pLVipNextContent;
	//VIP信息
	CMD_GP_VipPower vipPower;
	//vip等级复选框
	UICheckBox *pCBVip[maxVipLevel];
public:
	PopDialogBoxVip();
	~PopDialogBoxVip();
	CREATE_FUNC(PopDialogBoxVip);
private:
	virtual void onEnter();
	virtual void onExit();

	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_VIP); }
	
	//关闭窗口回调
	virtual void onCloseView(){ isReadMessage = true; }
	//vip复选框
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
	//充值
	void onMenuRecharge(CCObject *object, TouchEventType type);
	//领取奖励按键
	void onMenuReward(CCObject *object, TouchEventType type);
	//设置按键状态
	void setButtonState(UIButton *button,int state);
	//设置VIP等级
	void setVipGrade(int iGrade);
	//设置VIP下一等级
	void setNextVipGrade(int iGrade);
	//更新列表
	void updateListVip();
	//更新列表
	void updateListVipByIndex(int index);
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//VIP信息
	void onSubVipInfo(void * pDataBuffer, unsigned short wDataSize);
	//VIP领取奖励
	void onSubVipReward(void * pDataBuffer, unsigned short wDataSize);
};

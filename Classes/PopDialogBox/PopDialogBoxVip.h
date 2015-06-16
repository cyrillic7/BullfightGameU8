/*
 * PopDialogBoxVip.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MTNotificationQueue/MessageQueue.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
struct IPopAssistVip//VIP辅助接口
{
	virtual ~IPopAssistVip(){}
	//关闭VIP跳转到商城
	virtual void onCloseVipToShop() = 0;
};
class PopDialogBoxVip: public PopDialogBox ,public MessageQueue{
private:
	CC_SYNTHESIZE(IPopAssistVip *, iIPopAssistVip, IPopAssistVip);

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
	//VIP字
	UIImageView *pIVVipText;
	//vip奖励列表
	UIListView *pLVVipReward;
	//下一VIP等级进度
	UILoadingBar *pLBVipNextGrade;
	//下一等级内容描述
	UILabel *pLVipNextContent;
	//VIP信息
	CMD_GP_VipPower vipPower;
public:
	PopDialogBoxVip();
	~PopDialogBoxVip();
	CREATE_FUNC(PopDialogBoxVip);
private:
	virtual void onEnter();
	virtual void onExit();

	TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_VIP); }
	
	//领取奖励按键
	void onMenuReward(CCObject *object, TouchEventType type);
	//设置VIP等级
	void setVipGrade(int iGrade);
	//更新列表
	void updateListVip();
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

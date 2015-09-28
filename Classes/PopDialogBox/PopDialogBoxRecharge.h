/*
 * PopDialogBoxRecharge.h
 *	充值
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "IPopDialogBoxAssist.h"
#include "PopDialogBoxTipInfo.h"
class PopDialogBoxRecharge : public PopDialogBox, public IPopAssistTipInfo{
public:
	CC_SYNTHESIZE(IPopDialogBoxAssistCloseView*, pIPopDialogBoxAssistCloseView, IPopDialogBoxAssistCloseView);
	bool isShowExchangeGold;				//是否显示兑换金币
private:
	enum RechargeActionType
	{
		R_Action_GET_MONEY = 0,					//获取财富
		R_Action_EXCHANGE,						//兑换
		R_Action_SEND_ORDER,					//下订单
	};
	CC_SYNTHESIZE(RechargeActionType, eRechargeActionType, RechargeActionType);

private:
	enum RechargeType
	{
		RECHARGE_GOLD=1,				//金币
		RECHARGE_BIG_GOLD,				//元宝
	};
	CC_SYNTHESIZE(RechargeType, eRechargeType, RechargeType);
	//游标
	//UIImageView *pIVCursor;
	//兑换名称
	//UILabel *pLRechargeName;
	//兑换列表
	UIListView *pLVRechargeList;
	//兑换金币
	UIButton *pBExchangeGold;
	//兑换元宝
	UIButton *pBExchangeBigGold;

	//我的当前金币
	UILabel *pLCurGoldCount;
	//我的当前元宝
	UILabel *pLCurBigGoldCount;
	//当前按键索引
	int iCurSelectIndex;
	//订单号
	std::string orderID;
private:
	struct RechargeData
	{
		std::string sImageIconName;						//图片名称
		std::string sExchangeNum;						//兑换数
		std::string sOtherNum;							//加送数
		long lExchangePice;								//兑换价格
	};
	std::vector<RechargeData> vecRechargeGold;			//金币
	std::vector<RechargeData> vecRechargeBigGold;		//元宝
public:
	PopDialogBoxRecharge();
	~PopDialogBoxRecharge();
	CREATE_FUNC(PopDialogBoxRecharge);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭回调
	void onCloseTipInfo(CCLayer *pTipInfo);
	//关闭窗口
	void onMenuCloseView(CCObject *object, TouchEventType type);
	//更改兑换类型
	void onMenuChangeExchangeType(CCObject *object, TouchEventType type);
	//兑换按键
	void onMenuExchange(CCObject *object, TouchEventType type);

	//更新兑换列表
	void updateListRecharge(std::vector<RechargeData> qMsg);

	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	//连接成功
	void connectSuccess();
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//财富
	void onSubTreasure(void * pDataBuffer, unsigned short wDataSize);
	//兑换元宝
	void onSubExchangeBigGold(void * pDataBuffer, unsigned short wDataSize);

	//付费成功回调
	void onUpdateDelta(CCObject *obj);
};

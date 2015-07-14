/*
 * PopDialogBoxShop.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
#include "PopDialogBoxKnapsack.h"
#define MAX_SHOP_ITEM_COUNT				2			//最大商店项总数
class PopDialogBoxShop: public PopDialogBox,public IPopAssist,public IPopAssistKnapsack {

private:
    
	enum ShopItem
	{
		SHOP_GIFT_PACKAGE=0,			//礼品包
		SHOP_PROP,						//道具馆
		SHOP_BUY_GIFT,					//购买礼品
		SHOP_BUY_PROP,					//购买道具
		//SHOP_KNAPSACK_LIST,				//背包列表
		SHOP_GET_PROPERTY,				//获取财产

	};
	CC_SYNTHESIZE(ShopItem, shopItem, ShopItem);
	//商店分类选项框
	UICheckBox *pCBShopItems[MAX_SHOP_ITEM_COUNT];
	//商品列表
	UIListView *pListViewCommodity;

	//礼品
	std::vector<CMD_GP_Gift> vecGift;
	//道具
	std::vector<CMD_GP_Gift> vecProp;
	//购买道具索引
	int iBuyPropIndex;

	//奖券
	UIImageView *pIVoucher;
	//元宝
	UIImageView *pIBigGold;
	//保险箱
	UIImageView *pIInsure;
	//当前购买数量
	long lCurBuyNum;
public:
	PopDialogBoxShop();
	~PopDialogBoxShop();
	CREATE_FUNC(PopDialogBoxShop);
private:
	virtual void onEnter();
	virtual void onExit();
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_SHOP); }
	//初始化商品列表
	void initListCommodity();
	//设置奖券
	void setVoucher(long long llVoucher);
	//设置元宝
	void setBigGold(long long llBigGold);
	//设置保险箱
	void setInsure(long long llInsure);
	
	//////////////////////////////////////////////////////////////////////////
	//购买数量回调
	virtual void onBuyNum(long lNum);
	//背包关闭回调
	virtual void onCloseKnapsack();
	//////////////////////////////////////////////////////////////////////////

	//我的背包////////////////////////////////////////////////////////////////////////
	void onMenuMyPackaga(CCObject *object, TouchEventType type);
	//购买按键
	void onMenuBuyProp(CCObject *object, TouchEventType type);
	//复选框回调
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
	//切换商店项
	void changeSelectItem(ShopItem eItem);
	//更新商品列表
	void updateListCommodity(std::vector<CMD_GP_Gift> *vec);
	
	
	void update(float delta);
	//购买道具
	void buyPropForType();

	//////////////////////////////////////////////////////////////////////////
	//连接成功
	void connectSuccess();
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//礼品列表
	void onSubGiftList(void * pDataBuffer, unsigned short wDataSize, std::vector<CMD_GP_Gift> &vec);
	//购买礼品
	void onSubBuyGift(void * pDataBuffer, unsigned short wDataSize);
	//财富
	void onSubTreasure(void * pDataBuffer, unsigned short wDataSize);
};

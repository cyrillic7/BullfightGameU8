/*
 * PopDialogBoxShop.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
#include "../MTNotificationQueue/MessageQueue.h"
#define MAX_SHOP_ITEM_COUNT				2			//最大商店项总数
class PopDialogBoxShop: public PopDialogBox,public MessageQueue {
public:
	bool isReadMessage;
private:
    
	enum ShopItem
	{
		SHOP_GIFT_PACKAGE=0,			//礼品包
		SHOP_PROP,						//道具馆
		SHOP_BUY_GIFT,					//购买礼品
		SHOP_BUY_PROP,					//购买道具
		//SHOP_KNAPSACK_LIST,				//背包列表
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
public:
	PopDialogBoxShop();
	~PopDialogBoxShop();
	CREATE_FUNC(PopDialogBoxShop);
private:
	virtual void onEnter();
	virtual void onExit();
	TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_SHOP); }
	//初始化商品列表
	void initListCommodity();
	//我的背包////////////////////////////////////////////////////////////////////////
	void onMenuMyPackaga(CCObject *object, TouchEventType type);
	void onMenuBack(CCObject *object, TouchEventType type);
	//购买按键
	void onMenuButProp(CCObject *object, TouchEventType type);
	//复选框回调
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
	//切换商店项
	void changeSelectItem(ShopItem eItem);
	//更新商品列表
	void updateListCommodity(std::vector<CMD_GP_Gift> *vec);
	//设置父结节是否读取网络消息
	void setParentReadMessage(bool isRead);
	
	void update(float delta);
	//购买道具
	void buyPropForType();

	//////////////////////////////////////////////////////////////////////////
	//连接服务器
	void connectServer();
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//礼品列表
	void onSubGiftList(void * pDataBuffer, unsigned short wDataSize, std::vector<CMD_GP_Gift> &vec);
	//购买礼品
	void onSubButGift(void * pDataBuffer, unsigned short wDataSize);
};

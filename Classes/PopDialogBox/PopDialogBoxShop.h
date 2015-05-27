/*
 * PopDialogBoxShop.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "../MTNotificationQueue/MessageQueue.h"
#define MAX_SHOP_ITEM_COUNT				2			//最大商店项总数
class PopDialogBoxShop: public PopDialogBox,public MessageQueue {
private:
	enum ShopItem
	{
		SHOP_GIFT_PACKAGE=0,			//礼品包
		SHOP_PROP,						//道具馆
	};
	CC_SYNTHESIZE(ShopItem, shopItem, ShopItem);
	//商店分类选项框
	UICheckBox *pCBShopItems[MAX_SHOP_ITEM_COUNT];
	//商品列表
	UIListView *pListViewCommodity;

	int tempSize;
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
	void updateListCommodity();
	//设置父结节是否读取网络消息
	void setParentReadMessage(bool isRead);
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	void update(float delta);
};

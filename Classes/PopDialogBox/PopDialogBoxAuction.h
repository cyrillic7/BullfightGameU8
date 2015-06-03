/*
 * PopDialogBoxAuction.h
 *	拍卖
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once
#include "PopDialogBox.h"
#define MAX_AUCTION_ITEM_COUNT			3			//最大拍卖项总数
class PopDialogBoxAuction: public PopDialogBox {
private:
	enum AuctionItem
	{
		AUCTION_INFO = 0,			//拍卖信息
		AUCTION_MY,					//我的拍卖
		AUCTION_RECORD,				//拍卖记录
	};
	CC_SYNTHESIZE(AuctionItem, auctionItem, AuctionItem);
	//元宝
	UIImageView *pIBigGold;
	//保险箱
	UIImageView *pIInsure;
	//拍卖
	UIImageView *pIAuction;
	//商店分类选项框
	UICheckBox *pCBAuctionItems[MAX_AUCTION_ITEM_COUNT];
	//列表
	UIListView *pLVAuction[MAX_AUCTION_ITEM_COUNT];
	//单项内容
	UIImageView *pICellItemContent[MAX_AUCTION_ITEM_COUNT];
	//线条
	UIPanel *pPLine[MAX_AUCTION_ITEM_COUNT];
public:
	PopDialogBoxAuction();
	~PopDialogBoxAuction();
	CREATE_FUNC(PopDialogBoxAuction);
private:
	virtual void onEnter();
	virtual void onExit();
	//设置元宝
	void setBigGold(long long llGold);
	//设置保险箱
	void setInsure(long long llInsure);
	//设置拍卖
	void setAuction(long long llAuction);
	//复选框回调
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
	//切换拍卖项
	void changeSelectItem(AuctionItem eItem);
};

/*
 * PopDialogBoxAuction.h
 *	拍卖
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once
#include "PopDialogBox.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
#include "../MTNotificationQueue/MessageQueue.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#define MAX_AUCTION_ITEM_COUNT			3			//最大拍卖项总数
class PopDialogBoxAuction: public PopDialogBox,public MessageQueue {
private:
	enum AuctionItem
	{
		AUCTION_INFO = 0,			//拍卖信息
		AUCTION_MY,					//我的拍卖
		AUCTION_RECORD,				//拍卖记录
		AUCTION_BUY,				//拍卖购买
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
	//拍卖信息
	//UILabel 
	std::vector <CMD_GP_AuctionRecordItem> vecAuctionInfo;
	//购买索引
	int auctionBuyIndex;
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
	//购买按键
	void onMenuButProp(CCObject *object, TouchEventType type);
	//复选框回调
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
	//切换拍卖项
	void changeSelectItem(AuctionItem eItem);
	//更新拍卖信息列表
	void updateListAuctionInfo();
	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_AUCTION_INFO); }
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//拍卖记录
	void onSubAuctionRecord(void * pDataBuffer, unsigned short wDataSize);

	
};
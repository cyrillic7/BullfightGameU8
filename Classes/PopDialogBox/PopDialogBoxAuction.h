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
class PopDialogBoxAuction: public PopDialogBox,public MessageQueue,public IPopAssist {
private:
	enum AuctionItem
	{
		AUCTION_INFO = 0,			//拍卖信息
		AUCTION_MY,					//我的拍卖
		AUCTION_RECORD,				//拍卖记录
		AUCTION_BUY,				//拍卖购买
		AUCTION_CANCEL,				//撤销拍卖
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
	std::vector <CMD_GP_AuctionRecordItem> vecAuctionInfo;
	//我的拍卖
	std::vector <CMD_GP_MyAuctionRecordItem> vecMyAuction;
	//拍卖记录
	std::vector	<CMD_GP_HistoryAuctionRecordItem> vecHistoryAuction;
	//购买索引
	int auctionBuyIndex;
	//当前购买数量
	long lCurBuyNum;
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
	void onMenuBuyProp(CCObject *object, TouchEventType type);
	//取消拍卖按键
	void onMenuCancelAuction(CCObject *object, TouchEventType type);
	//我的背包
	void onMenupMyPackage(CCObject *object, TouchEventType type);
	
	//////////////////////////////////////////////////////////////////////////
	//购买数量回调
	virtual void onBuyNum(long lNum);
	//////////////////////////////////////////////////////////////////////////

	//复选框回调
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
	//切换拍卖项
	void changeSelectItem(AuctionItem eItem);
	//更新拍卖信息列表
	void updateListAuctionInfo();
	//更新我的列表
	void updateListMyAuctionRecord();
	//更新拍卖历史记录列表
	void updateListHistoryAuctionRecord();
	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_AUCTION_INFO); }
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//拍卖信息
	void onSubAuctionInfo(void * pDataBuffer, unsigned short wDataSize);
	//我的拍卖
	void onSubMyAuction(void * pDataBuffer, unsigned short wDataSize);
	//拍卖历史记录
	void onSubHistoryAuctionRecord(void * pDataBuffer, unsigned short wDataSize);
	//购买拍卖品
	void onSubBuyAuction(void * pDataBuffer, unsigned short wDataSize);

};
/*
 * PopDialogBoxMore.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxMore: public PopDialogBox {
private:
	//更多列表
	UIListView *pLVMoreList;
	//拍卖信息
	std::vector <std::string> vecMoreInfo;
public:
	PopDialogBoxMore();
	~PopDialogBoxMore();
	CREATE_FUNC(PopDialogBoxMore);
private:
	virtual void onEnter();
	virtual void onExit();
	//更新
	void update(float delta);
	//打开游戏
	void onMenuOpenGame(CCObject *object, TouchEventType type);
	//更新更多游戏列表
	void updateListMore();

	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//排名列表
	void onSubRankingList(void * pDataBuffer, unsigned short wDataSize);
};

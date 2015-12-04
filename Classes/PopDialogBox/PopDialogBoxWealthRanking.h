/*
 * PopDialogBoxWealthRanking.h
 *	排行
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
class PopDialogBoxWealthRanking : public PopDialogBox, public IPopDialogBoxAssistCloseView {
    
private:
	//排名列表
	UIListView *pLVRanking;
	//获奖者
	UILabel *pLPrizeWinner;
	//我的排名背景
	UIImageView *pIVMyRankingBg;
	//我的排名
	UILabel *pLMyRankingNum;
	//排名计数
	int iMyRankingNum;
public:
	PopDialogBoxWealthRanking();
	~PopDialogBoxWealthRanking();
	CREATE_FUNC(PopDialogBoxWealthRanking);
private:
	virtual void onEnter();
	virtual void onExit();
	//关闭窗口回调
	virtual void onCloseView();

	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_RANKING); }

	//更新列表
	void updateListRanking(CCObject *obj);
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//排名列表
	void onSubRankingList(void * pDataBuffer, unsigned short wDataSize);
};

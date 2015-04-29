//
//
//
#pragma once
#include "GameControlBase.h"
#include "SeatData.h"
#define MAX_SEAT_COUNT 4
class GameControlOxHundred:public GameControlBase
{
private:
	SeatData *pSeatData[MAX_SEAT_COUNT];
public:
	GameControlOxHundred();
	~GameControlOxHundred();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxHundred);
private:
	//初始化计时器
	void initTimer(UILayer *pWidget);
	//初始化座位
	void initSeatData(UILayer *pWidget);
private:
	 int getChairIndex(int meChairID,int chairID);
	//菜单////////////////////////////////////////////////////////////////////////
	void onMenuBack(CCObject* pSender, TouchEventType type);

	virtual void update(float delta);
	//更新计时器
	virtual void delayedAction();
	//读取网络消息回调事件
	virtual void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//游戏中
	void onEventGameIng(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户状态
	virtual void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//////////////////////////////////////////////////////////////////////////
	//游戏空闲
	void onSubGameFree(const void * pBuffer, WORD wDataSize);
	//开始游戏
	void onSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户下注
	void onSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes);
	//游戏结束
	void onSubGameEnd(const void * pBuffer, WORD wDataSize);
};

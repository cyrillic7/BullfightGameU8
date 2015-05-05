//
//
//
#pragma once
#include "GameControlBase.h"
#include "SeatData.h"
#include "JettonNode.h"
//#include "cmd_game.h"
class MainSceneBase;
#define MAX_SEAT_COUNT 4
class GameControlOxHundred:public GameControlBase
{
	//下注信息
//protected:
	//long long						m_lUserJettonScore[AREA_COUNT+1];	//个人总注
	//long long						m_lAllJettonScore[AREA_COUNT+1];	//全体总注
private:
	SeatData *pSeatData[MAX_SEAT_COUNT];
	UIButton *pBOnline ;
public:
	GameControlOxHundred();
	~GameControlOxHundred();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxHundred);
private:
	MainSceneBase*getMainScene();
	//初始化计时器
	void initTimer(UILayer *pWidget);
	//初始化座位
	void initSeatData(UILayer *pWidget);
	//获取筹码对象
	JettonNode *getJettonNode();
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
private:
	//加注按键
	void onMenuPlaceJetton(CCObject* pSender, TouchEventType type);
};

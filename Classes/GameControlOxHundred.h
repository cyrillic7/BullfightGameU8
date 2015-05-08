//
//
//
#pragma once
#include "GameControlBase.h"
#include "SeatData.h"
#include "JettonNode.h"
//#include "cmd_game.h"
#include "PlayerDataHundred.h"
#include "GameLogic/GameLogicHundred.h"
class MainSceneBase;
//class cmd_game;
#define MAX_SEAT_COUNT 4
#define MAX_JETTON_BUTTON_COUNT			5			//筹码按键总数
#define MAX_AREA_COUNT								4			//区域数目
#define MAX_PLAYER_HUNDRED_COUNT			2			//百人牛牛用户数
#define MAX_SCORE_HISTORY							15		//游戏记录最大数
#pragma pack(1)
//记录信息
struct tagGameRecord
{
	bool							bWinShunMen;						//顺门胜利
	bool							bWinDuiMen;							//对门胜利
	bool							bWinDaoMen;							//倒门胜利
	bool							bWinHuang;							//倒门胜利
};
#pragma pack()

class GameControlOxHundred:public GameControlBase,public GameLogicHundred
{
public:

public:
	//tagGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	std::list <tagGameRecord> listGameRecord;
	//限制信息
protected:
	//LONGLONG						m_lMeMaxScore;						//最大下注
	long long						m_lAreaLimitScore;					//区域限制
	//下注信息
protected:
	long long						m_lUserJettonScore[MAX_AREA_COUNT+1];	//个人总注
	long long						m_lAllJettonScore[MAX_AREA_COUNT+1];		//全体总注

	
	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	long long						m_lBankerScore;						//庄家积分

private:
	long long						m_lMeMaxScore;						//最大下注
	int nJetton[MAX_JETTON_BUTTON_COUNT];
	SeatData *pSeatData[MAX_SEAT_COUNT];
	UIButton *pBOnline ;
	//筹码按钮
	UIImageView *pIJettonButton[MAX_JETTON_BUTTON_COUNT];
	//筹码选择光标
	UIImageView *pIJettonSelect;
	//当前所选筹码索引
	int iCurSelectJettonIndex;
	//用户信息
	PlayerDataHundred *pPlayerData[MAX_PLAYER_HUNDRED_COUNT];
	//上庄按键
	UIImageView *pIUpBank;
	//游戏状态
protected:
	bool								m_bAllowLookon;						//允许旁观
	BYTE							m_cbGameStatus;						//游戏状态
public:
	GameControlOxHundred();
	~GameControlOxHundred();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxHundred);
	//设置座位结算
	void setSeatResult(int iSeatIndex,int iOXType);
	//显示总结果
	void showAllResult();
	//更新状态
	virtual void updateState();
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
	virtual void update(float delta);
	//更新计时器
	virtual void delayedAction();
	//读取网络消息回调事件
	virtual void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//框架命令
	void onSubGameFrame(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//游戏中
	void onEventGameIng(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户信息
	void onSubUserInfo(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户进入
	void onSubUserEnter(void * pDataBuffer, unsigned short wDataSize);
	//用户状态
	void onSubUserState(void * pDataBuffer, unsigned short wDataSize);
	//用户状态
	virtual void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//////////////////////////////////////////////////////////////////////////
	//游戏空闲
	void onSubGameFree(const void * pBuffer, WORD wDataSize);
	//开始游戏
	void onSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户下注
	void onSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes);
	//下注失败
	void onSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//游戏记录
	void onSubGameRecord(const void * pBuffer, WORD wDataSize);
	//申请庄家
	void onSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//游戏结束
	void onSubGameEnd(const void * pBuffer, WORD wDataSize);
private:
	//菜单返回////////////////////////////////////////////////////////////////////////
	void onMenuBack(CCObject* pSender, TouchEventType type);
	//在线用户
	void onMenuOnLine(CCObject* pSender, TouchEventType type);
	//趋势图
	void onMenuTrend(CCObject* pSender, TouchEventType type);
	//加注区域
	void onMenuPlaceJetton(CCObject* pSender, TouchEventType type);
	//选择筹码
	void onMenuSelectJetton(CCObject* pSender, TouchEventType type);
	//上庄
	void onMenuUpBank(CCObject* pSender, TouchEventType type);
	//最大下注
	long long getUserMaxJetton();
	//更新控制
	void updateButtonContron();
	//设置庄家
	void setBankerInfo(unsigned short  wBanker,long long lScore);
	//重置数据
	void resetData();
	//插入历史记录
	void insertGameHistory(tagGameRecord tagRecord);
	//推断赢家
	void deduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );
};

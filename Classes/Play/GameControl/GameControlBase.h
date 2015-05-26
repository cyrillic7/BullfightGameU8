//
//  GameHUD.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#ifndef __BullfightGame__GameHUD__
#define __BullfightGame__GameHUD__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../Network/TCPSocket/TCPSocket.h"
#include "../GameEndLayer.h"
#include "../../MTNotificationQueue/MessageQueue.h"
#include "../../Tools/CMath.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
#define MAX_TIMER		10		//计时器最大值
class MainSceneBase;
class GameControlBase:public CCLayer,public MessageQueue,public CMath
{
public:
	//配置牛牛容器
	UIPanel *pOptOx;
	//庄家用户
	BYTE wBankerUser;
	//加注按键
	UIButton *pbBetting[4];
	//换牌操作容器
	UIPanel *pPanelSwapCard;
private:
	//操作者提示动画
	CCArmature *pArmatureActionPrompt;
	//准备容器
	UIPanel *pPanelReady;
	//抢庄容器
	UIPanel *pFightForBanker;
	//投注容器
	UIPanel *pBetting;

	
	//游戏结算层
	GameEndLayer *pEndLayer;
	//是否已提示牛牛
	bool isPromptOx;
public:
	//计时器
	UIImageView *pITimer;
	//计时器数字
	UILabelAtlas *pLTimerNum;
	//计时器提示内容
	UILabel *pLTimerPromptContent;
	//计时变量
	int iTimerCount;
public:
	GameControlBase();
	~GameControlBase();
	virtual void onEnter();
	virtual void onExit();
	//更新消息
	virtual void update(float delta);
	//更新状态
	virtual void updateState();
	//获取主场景
	MainSceneBase*getMainScene();
public:
	//显示指定索引提示动画
	void showActionPrompt(int promptIndex);
	//隐藏提示动画
	void hideActionPrompt();
	//获取庄家视图位置
	int getBankViewID();
	//设置计时器
	void resetTimer(float time,const char * promptContent);
	//显示计时器
	void showTimer();
	//隐藏计时器
	void hideTimer(bool bIsReset);

private:
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//初始化操作者提示动画
	void initActionPrompt();
	//初始化计时器
	void initTimer(UILayer *pWidget);
	//更新计时器
	void updateTimer(float dt);
	//延时操作
	virtual void delayedAction();
public:
	//获取我的椅子位置
	int getMeChairID();
	//获取视图位置
	virtual int getViewChairID(int severChairID);
	//站立并退出
	virtual void standUpWithExit();
	//是不是观察者
	bool IsLookonMode();
private:
	//菜单////////////////////////////////////////////////////////////////////////
	void menuPause(CCObject* pSender, TouchEventType type);
	//开牌
	virtual void menuOpenCard(CCObject* pSender, TouchEventType type);
	//提示按键
	void menuPrompt(CCObject* pSender, TouchEventType type);
	//更换桌子
	void menuChangeChair(CCObject* pSender, TouchEventType type);
	//准备按键
	void menuReady(CCObject* pSender, TouchEventType type);
	//不抢按键
	void menuNotFight(CCObject* pSender, TouchEventType type);
	//抢庄按键
	void menuFight(CCObject* pSender, TouchEventType type);
	//投注
	void menuBetting(CCObject* pSender, TouchEventType type);
	//换牌
	void menuSwapCard(CCObject* pSender, TouchEventType type);
	//不换牌
	void menuDontSwapCard(CCObject* pSender, TouchEventType type);
	//换牌消息
	virtual void onUserChangeCard(int wParam, long lParam){}
	
	//获得椅子索引
	virtual int getChairIndex(int meChairID,int chairID)=0;
	//隐藏用户
	virtual void hidePlayer(CMD_GR_UserStatus *userInfo){}
//////////////////////////////////////////////////////////////////////////
public:
	//网络消息
	//void OnEventGameMessage(CCObject *pObj);

	virtual void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//游戏中
	virtual void onEventGameIng(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户叫庄
	virtual bool OnSubCallBanker(const void * pBuffer, WORD wDataSize);
	//游戏开始
	virtual bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户下注
	virtual bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//发牌消息
	virtual bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//开牌
	virtual bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//用户强退
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//游戏结束
	virtual bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//用户进入
	void onSubUserEnter(void * pDataBuffer, unsigned short wDataSize);
	//框架命令
	void frameEvent(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户站立
	void OnUserFree(CCObject *obj);
	//用户进入
	virtual void onUserEnter();
	//用户准备消息
	virtual void onUserReady(CMD_GR_UserStatus *info);
	//virtual void onUserEnterWithUpdate(tagUserInfo *user){}
	//用户状态
	void OnEventUserState(WORD	 wSubCmdID,const void * pBuffer, WORD wDataSize);

	//用户状态
	virtual void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
private:
	void goldJump(int index,CCPoint beginPos,CCPoint endPos);
	void onGoldJump(CCNode *node);
};

#endif /* defined(__BullfightGame__GameHUD__) */

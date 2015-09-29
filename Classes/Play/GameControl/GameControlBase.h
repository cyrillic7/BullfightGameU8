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
#include "../GameEndLayer.h"
#include "../../MTNotificationQueue/MessageQueueGameIng.h"
#include "../../MTNotificationQueue/GameIngMsgHandler.h"
#include "../../Tools/CMath.h"
#include "../../PopDialogBox/PopDialogBoxTipInfo.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
#define MAX_TIMER		10		//计时器最大值
class MainSceneBase;
class GameControlBase :public CCLayer, public MessageQueueGameIng, public CMath,public IPopAssistTipInfo,public CStringAide
{
public:
	enum ActionPromptType
	{
		ACTION_PROMPT_CALL_BANK=1,					//等待玩家叫庄
		ACTION_PROMPT_ADD_SCORE,					//等待玩家下注
		ACTION_PROMPT_OPEN_CARD,					//等待玩家摊牌
		ACTION_PROMPT_OPT_CARD,						//选择要换的牌
		ACTION_PROMPT_WAIT_SWAP_CARD,				//等待玩家换牌
	};
	//配置牛牛容器
	UIPanel *pOptOx;
	//庄家用户
	BYTE wBankerUser;
	//加注按键
	Button *pbBetting[4];
	//换牌操作容器
	UIPanel *pPanelSwapCard;
	//换牌按键
	ImageView *pIVChangeCard;
	//准备容器
	UIPanel *pPanelReady;
	//开牌
	Button *pBOpenCard;
	//提示
	Button *pBPrompt;
	//换牌
	Button *pBSwapCard;
	//不换
	Button *pBDontSwapCard;
	//开始动画
	CCArmature *pArmatureBeginGame;
	//游戏结算层
	GameEndLayer *pEndLayer;
	//服务费用
	long long lServiceScore;                      
private:
	//操作者提示动画
	CCArmature *pArmatureActionPrompt;

	//抢庄容器
	UIPanel *pFightForBanker;
	//投注容器
	UIPanel *pBetting;

	//是否主动退出
	bool isExitGame;
	

	//更多框
	ImageView *pIVMoer;
	//是否显示更多
	bool isShowMoer;
	//设置
	Button *pBSetting;
	//退出
	Button *pBExit;

	//游戏状态
protected:
	bool							m_bAllowLookon;						//允许旁观
	BYTE							m_cbGameStatus;						//游戏状态
public:
	//计时器
	ImageView *pITimer;
	//计时器数字
	UILabelAtlas *pLTimerNum;
	//计时器提示内容
	Label *pLTimerPromptContent;
	//计时变量
	int iTimerCount;
	//延时显示结算最大数
	int iDelayedMaxCount;
	//当前延时回调计数器
	int iCurDelayedCount;
	//是否播放倒计时音效
	bool isPalySoundWarn;
	//是否百人牛牛播放倒计时音效
	bool isPalySoundHundedWarn;
	//是否换桌
	bool isChangeChair;
	//不否刷新用户
	bool isUpdateUserInfo;
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
	//获取socket
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM); }
public:
	//显示指定索引提示动画
	void showActionPrompt(int promptIndex, CCPoint offsetPos);
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
	//初始化开始动画
	void initBeginGame();
	//初始化计时器
	void initTimer(UILayer *pWidget);
	//更新计时器
	void updateTimer(float dt);
	//延时操作
	virtual void delayedAction();
	//关闭回调
	virtual void onCloseTipInfo(CCLayer *pTipInfo);
public:
	//获取我的椅子位置
	int getMeChairID();
	//获取视图位置
	virtual int getViewChairID(int severChairID);
	//站立并退出
	void standUpWithExit();
	//是不是观察者
	bool IsLookonMode();
	//是否是游戏状态
	virtual bool isPalyerState(){ return true; }
	//延时显示牌
	virtual void delayedShowOx(CCNode *pNode);
private:
	//菜单////////////////////////////////////////////////////////////////////////
	void menuPause(CCObject* pSender, TouchEventType type);
	//设置
	void onMenuSetting(CCObject* pSender, TouchEventType type);
	//退出
	void onMenuExit(CCObject* pSender, TouchEventType type);
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
	virtual void onEventGameIng(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){}
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
	//游戏状态
	virtual bool OnSocketSubGameStatus(void * pData, WORD wDataSize);
	//游戏场景
	virtual bool OnEventSceneMessage(void * pData, WORD wDataSize);
	//用户站立
	void OnUserFree(std::string userFreeTipContent);
	//用户进入
	virtual void onUserEnter();
	//用户准备消息
	virtual void onUserReady(CMD_GR_UserStatus *info);
	//virtual void onUserEnterWithUpdate(tagUserInfo *user){}
	//用户状态
	void OnEventUserState(WORD	 wSubCmdID,const void * pBuffer, WORD wDataSize);
	//断开连接
	void onUnconnect(WORD wSubCmdID);
    
	//用户状态
	virtual void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//隐藏所有操作按键
	void hideAllActionPanel();

	//开始动画播放完成回调
	virtual void onAnimationBeginGameFinsh(){}
private:
	void goldJump(int index,CCPoint beginPos,CCPoint endPos);
	void onGoldJump(CCNode *node);
	
	//显示结算（）
	virtual void showResultAnimation(){};

	//////////////////////////////////////////////////////////////////////////
	void onCloseGameIngUpdate(CCObject *pObj);
	//动画回调
	void onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID);
	void onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);

	//税收提示文本回调
	void onServiceScoreText(CCNode *node);
};

#endif /* defined(__BullfightGame__GameHUD__) */

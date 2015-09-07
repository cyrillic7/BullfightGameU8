//
//  BaseLobbyScene.h
//  游戏大厅基类
//
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Tools/CStringAide.h"
#include "../MTNotificationQueue/GameIngMsgHandler.h"
#include "../PopDialogBox/PopDialogBoxKnapsack.h"
#include "../PopDialogBox/IPopDialogBoxAssist.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class BaseLobbyScene :public CCLayer, public CStringAide, public IPopAssistKnapsack, public IPopDialogBoxAssist,public IPopDialogBoxAssistCloseView
{
public:
	static BaseLobbyScene *lobbyScene;
	Label *userName;
	Label *pLabelGoldCount;
	Button *pBUserInfo;
	bool isReadMessage;
	UILayer *m_pWidget;
	//创建UI层
	UILayer *m_pWidgetBase;
	enum PopType
	{
		POP_USER_INFO=0,			//用户信息
		POP_SETUP,					//设置
		POP_MORE,					//更多
		POP_ACTIVITY,				//活动
		POP_TASK,					//任务
		POP_BANK,					//银行
		POP_SHOP,					//商店
		POP_AUCTION,				//拍卖
		POP_RANKING,				//排行
		POP_MSG,					//消息
		POP_VIP,					//VIP
		POP_KNAPSACK,				//背包
		POP_RECHARGE,				//充值
	};
private:
	//金币闪光动画
	CCArmature *pAnimate;
public:
    BaseLobbyScene();
    ~BaseLobbyScene();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(BaseLobbyScene);

	//关闭wap网站
	void closeWebView();
	//获取socket
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM); }
	//VIP
	void onMenuVip(CCObject* pSender, TouchEventType type);
	//弹出框
	void popDialogBox(PopType type);
	//首充值
	void onMenuFirstRecharge(CCObject* pSender, TouchEventType type);
private:
	//立即游戏
	void onMenuQuickGame(CCObject* pSender, TouchEventType type);
	//菜单回调（由于菜单回调处理逻辑简单，可设为同一个回调）
	void onMenuCallback(CCObject* pSender, TouchEventType type);
	//菜单（设置用户信息）
	virtual void menuResetUser(CCObject* pSender, TouchEventType type);
	//////////////////////////////////////////////////////////////////////////
	//关闭背包回调
	virtual void onCloseKnapsack();
	//关闭VIP回调
	virtual void onCloseViewToShop();
	//关闭跳转到VIP
	virtual void onCloseViewToVip();
	//关闭窗口回调
	virtual void onCloseView(){ isReadMessage = true; }
	//post回调
	void onHttpRequestCompleted(CCHttpClient* client, CCHttpResponse* response);

	//快速游戏
	virtual void quickGame(){}
	virtual void keyBackClicked();  //响应返回键  
	//金币闪光动画
	void updateGoldLight(float dt);
};
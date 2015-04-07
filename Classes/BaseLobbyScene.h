//
//  BaseLobbyScene.h
//  游戏大厅基类
//
#pragma once
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class BaseLobbyScene:public CCLayer
{
public:
	UILabel *userName;
	UILabel *pLabelGoldCount;
private:
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
	};
public:
    BaseLobbyScene();
    ~BaseLobbyScene();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(BaseLobbyScene);
private:
	//弹出框
	void popDialogBox(PopType type);
	//菜单回调（由于菜单回调处理逻辑简单，可设为同一个回调）
	void onMenuCallback(CCObject* pSender, TouchEventType type);
	//菜单（设置用户信息）
	virtual void menuResetUser(CCObject* pSender, TouchEventType type);
};
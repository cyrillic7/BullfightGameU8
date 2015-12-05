//
//  GameLobbyScene.h
//  游戏大厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
#include "../MTNotificationQueue/MessageQueueGameIng.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class GameLobbyScene:public BaseLobbyScene,public MessageQueueGameIng
{
    
public:
	
	enum ModeType
	{
		MODE_CLASSIC=1,					//经典模式
		MODE_Hundred,						//百人场
		MODE_ONE_BY_ONE,				//通比
		MODE_SIX_SWAP,					//六人换牌
	};
private:
	//CGameSocket gameSocket;
	Label *pLabelGoldCount;
	ScrollView *scroll;
	//是否显示站立提示
	static bool isShowUpTip;
	std::string strUpTipContent;
	//是否显示签到
	bool isShowSign();
	//喇叭按键
	UIImageView *pIVHorn;
public:
    GameLobbyScene();
    ~GameLobbyScene();
	virtual void onEnter();
	virtual void onExit();

	static CCScene* scene(bool showUpTip,std::string strTipContent);
    CREATE_FUNC(GameLobbyScene);
	//显示喇叭框
	void showHorn();
private:
	
	//弹出框
	void popDialogBoxUserInfo();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	//菜单设置
	void menuSetUp(CCObject* pSender, TouchEventType type);
	//选择游戏模式
	void menuSelectMode(CCObject* pSender, TouchEventType type);
	//
	void enterLobbyByMode(int mode);
	//开始游戏
	void onMenuStartGame(cocos2d::CCObject* pSender);
	//更新
	void update(float delta);
	//读取网络消息回调事件
	void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void onEventConnect(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//登录
	void onSubLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//配置
	void onSubConfig(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户信息
	void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户进入
	void onSubUserEnter(void * pDataBuffer, unsigned short wDataSize);
	//用户状态
	void onSubUserState(void * pDataBuffer, unsigned short wDataSize);
	//显示站立提示
	void showUpTip();

	//快速游戏
	virtual void quickGame();
	//创建菜单
	CCMenuItemSprite *createMenuItem(int index);
	//显示喇叭框
	void onMenuShowHornMsg(CCObject* pSender, TouchEventType type);
	//跑马灯
	void showPMD();
};

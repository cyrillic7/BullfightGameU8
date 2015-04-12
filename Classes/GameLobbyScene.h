//
//  GameLobbyScene.h
//  游戏大厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;



class GameLobbyScene:public BaseLobbyScene
{
public:
	enum ModeType
	{
		MODE_CLASSIC=1,//经典模式
		MODE_,
		MODE_1,
		MODE_2
	};
private:
	UILabel *pLabelGoldCount;
	UIScrollView *scroll;

public:
    GameLobbyScene();
    ~GameLobbyScene();
	virtual void onEnter();
	virtual void onExit();

    static CCScene* scene();
    CREATE_FUNC(GameLobbyScene);

	void callbackData(CCObject *obj);
	void onOpen(CCObject *obj);
private:
	//
	bool deleteSocket;
	void update(float dt);
	void initTCPLogon();
	//弹出框
	void popDialogBox();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	//菜单设置
	void menuSetUp(CCObject* pSender, TouchEventType type);
	void menuSelectMode(CCObject* pSender, TouchEventType type);

	void enterLobbyByMode(int mode);
//	bool OnEventTCPSocketRead(unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
};
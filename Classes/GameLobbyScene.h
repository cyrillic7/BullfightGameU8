//
//  GameLobbyScene.h
//  游戏大厅
//
#pragma once
#include "cocos-ext.h"
#include "TCPLogon.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class GameLobbyScene:public CCLayer,public ITCPSocket
{
public:
	enum ModeType
	{
		MODE_=1,
		MODE_CLASSIC,//经典模式
		MODE_1
	};
	UILabel *userName;
private:
	TCPLogon *tcpLogon;
	UIScrollView *scroll;
public:
    GameLobbyScene();
    ~GameLobbyScene();
	virtual void onEnter();
	virtual void onExit();

    static CCScene* scene();
    CREATE_FUNC(GameLobbyScene);

	void callbackData(CCObject *obj);
private:
	//
	void update(float dt);
	void initTCPLogon();
	//弹出框
	void popDialogBoxUserInfo();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	void menuSelectMode(CCObject* pSender, TouchEventType type);

	void enterLobbyByMode(int mode);

	bool OnEventTCPSocketRead(unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
};
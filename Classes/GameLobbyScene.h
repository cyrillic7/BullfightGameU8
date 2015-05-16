//
//  GameLobbyScene.h
//  游戏大厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
#include "TCPSocketControl.h"
#include "MessageQueue.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class GameLobbyScene:public BaseLobbyScene,public MessageQueue
{
public:
	enum ModeType
	{
		MODE_CLASSIC=1,					//经典模式
		MODE_Hundred,						//百人场
		MODE_ONE_BY_ONE,				//通比
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
private:
	//获取socket
	TCPSocket *getSocket(){return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM);}
	//弹出框
	void popDialogBox();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	//菜单设置
	void menuSetUp(CCObject* pSender, TouchEventType type);
	//选择游戏模式
	void menuSelectMode(CCObject* pSender, TouchEventType type);
	//
	void enterLobbyByMode(int mode);
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
};
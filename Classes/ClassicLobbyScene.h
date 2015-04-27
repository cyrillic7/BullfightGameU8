//
//  ClassicLobbyScene.h
//	经典厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
#include "MessageQueue.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class ClassicLobbyScene:public BaseLobbyScene,public MessageQueue
{
private:
	enum GameLevel
	{
		LEVEL_0=1,//初级
	};
	bool isDeleteList;
	bool isEnterGame;
public:
    ClassicLobbyScene();
    ~ClassicLobbyScene();
	virtual void onEnter();
	virtual void onExit();

    static CCScene* scene();
    CREATE_FUNC(ClassicLobbyScene);
private:
	void update(float delta);
	
	void initTCPLogon(int index);
	//弹出框
	void popDialogBox();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	void menuStar(CCObject* pSender, TouchEventType type);
	//进入主场景
	void enterMainSceneByMode(int mode);

	void onPlay(CCObject *obj);
	void onConfigFinish(CCObject *obj);
	void onOpen(CCObject *obj);
	
	void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//登录
	void onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户状态
	void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
};
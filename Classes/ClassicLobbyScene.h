//
//  ClassicLobbyScene.h
//	经典厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class ClassicLobbyScene:public BaseLobbyScene
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
	
	void initTCPLogon();
	//弹出框
	void popDialogBox();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	void menuStar(CCObject* pSender, TouchEventType type);

	void enterMainSceneByMode(int mode);

	void callbackData(CCObject *obj);
	void callbackData1(CCObject *obj);
	void onOpen(CCObject *obj);
};
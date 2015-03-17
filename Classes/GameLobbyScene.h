//
//  GameLobbyScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class GameLobbyScene:public CCLayer
{
	enum ModeType
	{
		MODE_=1,
		MODE_CLASSIC,//经典模式
		MODE_1
	};
public:
    GameLobbyScene();
    ~GameLobbyScene();
	virtual void onEnter();
	virtual void onExit();

    static CCScene* scene();
    CREATE_FUNC(GameLobbyScene);
private:
	//弹出框
	void popDialogBoxUserInfo();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	void menuSelectMode(CCObject* pSender, TouchEventType type);

	void enterLobbyByMode(int mode);
};
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
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class GameControl:public CCLayer
{
private:
	//准备按键
	UIButton *bReady;
	//抢庄容器
	UIPanel *pFightForBanker;
	//配置牛牛容器
	UIPanel *pOptOx;
	//投注容器
	UIPanel *pBetting;
public:
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControl);
	//更新状态
	void updateState();
private:
	//菜单////////////////////////////////////////////////////////////////////////
	void menuPause(CCObject* pSender, TouchEventType type);
	//无牛按键
	void menuCancel(CCObject* pSender, TouchEventType type);
	//提示按键
	void menuPrompt(CCObject* pSender, TouchEventType type);
	//准备按键
	void menuReady(CCObject* pSender, TouchEventType type);
	//不抢按键
	void menuNotFight(CCObject* pSender, TouchEventType type);
	//抢庄按键
	void menuFight(CCObject* pSender, TouchEventType type);
	//投注
	void menuBetting(CCObject* pSender, TouchEventType type);
};

#endif /* defined(__BullfightGame__GameHUD__) */

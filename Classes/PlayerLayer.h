
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameConfig.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class PlayerLayer:public CCLayer {
public:
	UIPanel *playerPanel[MAX_PLAYER];
public:
	PlayerLayer();
	~PlayerLayer();
	CREATE_FUNC(PlayerLayer);
	virtual void onEnter();
	virtual void onExit();
	//更新状态
	void updateState();
private:
	//初始化玩家信息
	void initPlayerInfo();
	//获取玩家信息
	void getPlayerInfo();
	//隐藏玩家
	void hidePlayer(UIPanel *panel);
};

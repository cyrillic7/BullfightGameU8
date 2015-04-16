
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
	//结算
	UILabelAtlas *pLResult[MAX_PLAYER];
private:
	//用户昵称
	UILabel *pLUserName[MAX_PLAYER];
	//庄家标识
	UIImageView *pIBankIcon[MAX_PLAYER];
	//操作者光标
	UIImageView *pILight[MAX_PLAYER];
public:
	PlayerLayer();
	~PlayerLayer();
	CREATE_FUNC(PlayerLayer);
	virtual void onEnter();
	virtual void onExit();
	//更新状态
	void updateState();
	//显示结算动画
	void showResultAnimation(int iPanelIndex,long long lGameScore);
	//设置用户昵称
	void setUserName(int panelIndex,const char *name);
	
	//设置庄家
	void setBankIcon(int bankIndex);
private:
	//初始化玩家信息
	void initPlayerInfo();
	//重置数据
	void resetPlayerData();
	//获取玩家信息
	void getPlayerInfo();
	//隐藏玩家
	void hidePlayer(UIPanel *panel);
	//结算动画结束
	void resultAnimationFinish();
	//设置光标
	void setBankLight(int userIndex);
};

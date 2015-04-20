
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Struct.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class PlayerData:public CCNode {
public:
	//用户容器
	UIPanel *pPlayerPanel;
	//结算
	UILabelAtlas *pLResult;
	//用户昵称
	UILabel *pLUserName;
	//用户金币
	UILabel *pLGoldCount;
	//庄家标识
	UIImageView *pIBankIcon;
	//操作者光标
	UIImageView *pILight;
	//用户头像
	UIImageView *pIPlayerIcon;
private:
	//用户信息
	tagUserInfo userInfoPlayer;
public:
	PlayerData();
	~PlayerData();
	CREATE_FUNC(PlayerData);
public:
	//隐藏用户
	void hidePlayer();
	//设置庄家
	void setBankIcon();
	//设置用户信息
	void setUserInfo(tagUserInfo userInfo);
	//显示结算动画
	void showResultAnimation(long long lGameScore);
private:
	//结算动画结束
	void onResultAnimationFinish();
	//改变用户金币
	void changePlayerGole(long long lGold);
};


#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../Network/CMD_Server/Struct.h"
#include "../../Tools/CMath.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class MainSceneBase;
class PlayerData:public CCNode,public CMath {
public:
	enum ActionType
	{
		ACTION_READY=0,					//行为准备
		ACTION_CALL_BANK,				//叫庄
		ACTION_NOT_CALL,				//不叫庄
	};
public:
    
	//用户容器
	UIPanel *pPlayerPanel;
	//结算
	UILabelAtlas *pLResult;
	//用户昵称
	Label *pLUserName;
	//用户金币
	Label *pLGoldCount;
	//庄家标识
	ImageView *pIBankIcon;
	//操作者光标
	ImageView *pILight;
	//用户头像
	ImageView *pIPlayerIcon;
	//行为类型背景
	ImageView *pIActionTypeBg;
	//行为内容
	ImageView *pIActionContent;
private:
	//用户信息
	tagUserInfo userInfoPlayer;
	//获取主场景对象
	MainSceneBase *getMainScene();
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
	//显示行为类型
	void showActionType(ActionType type);
	//隐藏行为类型
	void hideActionType();
private:
	//结算动画结束
	void onResultAnimationFinish();
public:
	//改变用户金币
	void changePlayerGole(long long lGold);
};


#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../Network/CMD_Server/Struct.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class PlayerDataHundred:public CCNode {
public:
	/*enum ActionType
	{
		ACTION_READY=0,					//行为准备
		ACTION_CALL_BANK,				//叫庄
		ACTION_NOT_CALL,				//不叫庄
	};*/
public:
    
	//用户容器
	ImageView *pIPlayerBg;
	//结算
	UILabelAtlas *pLResult;
	//结算成绩
	long long lGameScore;
	//用户昵称
	Label *pLUserName;
	//用户金币
	Label *pLGoldCount;
	/*//庄家标识
	UIImageView *pIBankIcon;
	//操作者光标
	UIImageView *pILight;
	//用户头像
	UIImageView *pIPlayerIcon;
	//行为类型背景
	UIImageView *pIActionTypeBg;
	//行为内容
	UIImageView *pIActionContent;*/
private:
	//用户信息
	tagUserInfo userInfoPlayer;
public:
	PlayerDataHundred();
	~PlayerDataHundred();
	CREATE_FUNC(PlayerDataHundred);
public:
	//隐藏用户
	void hidePlayer();
	/*//设置庄家
	void setBankIcon();*/
	//设置用户信息
	void setUserInfo(tagUserInfo userInfo);
	//显示结算动画
	void showResultAnimation();
	/*
	//显示行为类型
	void showActionType(ActionType type);
	//隐藏行为类型
	void hideActionType();*/
	//改变用户金币
	void changePlayerGold(long long lGold);
private:
	//结算动画结束
	void onResultAnimationFinish();
	
	//用户进入效果动画
	void userEnterEffect(CCPoint pos, int tag);
	void onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID);
	void onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);
};


#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../Tools/GameConfig.h"
#include "../../Tools/CMath.h"
#include "PlayerData.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class MainSceneBase;
class PlayerLayer:public CCLayer,public CMath {
public:
	PlayerData *pPlayerData[MAX_PLAYER];
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
	//设置用户信息
	void setUserInfo(int panelIndex,tagUserInfo tagUser);
	//设置用户金币
	void setUserGold(int panelIndex,long long goldCount);
	//设置庄家
	void setBankIcon(int bankIndex);
	//隐藏二人牛牛玩家
	void hideOxTwoPlayer();
	//设置牌位置
	virtual void doSetCardMovePos(int i);
	//获取主场景对象
	MainSceneBase*getMainScene();
private:
	//初始化玩家信息
	void initPlayerInfo();
	//重置数据
	void resetPlayerData();
	//隐藏玩家
	void hidePlayer(UIPanel *panel);

	//设置光标
	void setBankLight(int userIndex);


	//用户进入效果动画
	void userEnterEffect(CCPoint pos,int tag);
	void onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID);
	void onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);

};


#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class GameEndLayer:public CCLayer{
private:
	//游戏结束动画
	CCArmature *pAGameEnd;
public:
	GameEndLayer();
	~GameEndLayer();
	CREATE_FUNC(GameEndLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
    
	//显示结束
	void showEnd(bool isWin);
private:
	//动画监听
	void initAnimationEventListener();
	//动画结束回调
	void onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID);
	//动画帧事件回调
	void onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);

};


#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameConfig.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class CardLayer:public CCLayer {
private:
	//扑克集
	CCBatchNode *batchCard;
	//发牌状态
	enum SendCardState
	{
		SEND_STATE_WAIT=0,			//等待发牌
		SEND_STATE_ING,				//发牌中
		SEND_STATE_END,				//发牌结束
	};
	CC_SYNTHESIZE(SendCardState, sendCardState, SendCardState);
	//能否发牌
	bool canSendCard[MAX_PLAYER];
public:
	CardLayer();
	~CardLayer();
	CREATE_FUNC(CardLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setCanSendCard();
public:
	//更新状态
	void updateState();
private:
	//发牌
	void sendCard();
	//发牌中
	void sendCardIng();
	//发5张牌
	void sendFiveCard(int index);
	//移动扑克
	void moveCardAction(CCArmature *armature, CCPoint targetPos);
	
};

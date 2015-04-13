
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameConfig.h"
#include "GameLogic.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class Card;
class CardLayer:public CCLayer,GameLogic {
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
	//发牌计数器
	short sSendCardCount;
	//牌
	Card *pCard[MAX_PLAYER*MAX_COUNT];
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
	//显示牌
	void showCard(int index);
private:
	//发牌
	void sendCard();
	//发牌中
	void sendCardIng();
	//发5张牌
	void sendFiveCard(int index, int offsetIndex);
	//移动扑克
	void moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index);
	//获取牌放大倍数
	float getCardScale(int index);
	//单张牌发完回调
	void onSendCardFinish();
	//获取当前局总牌数
	short getCurAllCardCount();

};

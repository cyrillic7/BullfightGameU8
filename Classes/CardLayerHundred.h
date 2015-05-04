
#pragma once
#include "CardLayer.h"
class CardLayerHundred:public CardLayer {
public:
	CardLayerHundred();
	~CardLayerHundred();
	CREATE_FUNC(CardLayerHundred);

	virtual void onEnter();
	virtual void onExit();
public:
	//更新状态
	virtual void updateServerState();
	//
	virtual void updateGameState();
private:
	//设置发牌区域
	virtual void setCanSendCard();
	virtual void sendCardIng();
	//移动单张牌
	virtual void moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index);
	//virtual void sendFiveCard(int index, int offsetIndex){}
};

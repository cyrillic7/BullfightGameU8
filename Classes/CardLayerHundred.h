
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
	virtual void sendCardIng(){}
	virtual void sendFiveCard(int index, int offsetIndex){}
};

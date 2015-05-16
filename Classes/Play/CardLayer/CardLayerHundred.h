
#pragma once
#include "CardLayerBase.h"
#include "GameLogic/GameLogicHundred.h"
class CardLayerHundred:public CardLayerBase,GameLogicHundred {
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
	//发5张牌
	virtual void sendFiveCard(int index, int offsetIndex);
	//获取牌缩放比例
	virtual float getCardScale(int index);
	//显示牛牛点数
	virtual void showOxType(int chairiD,int oxType);
private:
	//单张牌发完回调
	void onSendCardFinish();
	//显示牌
	void showCard();
	void updateShowCardOneByOne(float delta);  

	//获取牛牛类型点数
	int getOxTypeWithValue(int iType);
	
	void onPlayOxAnimation(CCNode *obj);
};

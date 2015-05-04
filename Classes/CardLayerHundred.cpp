

#include "CardLayerHundred.h"
CardLayerHundred::CardLayerHundred()
{
}
CardLayerHundred::~CardLayerHundred() {
}
void CardLayerHundred::onEnter(){
	CardLayer::onEnter();
}
void CardLayerHundred::onExit(){
	CardLayer::onExit();
}
void CardLayerHundred::updateGameState(){
	
	sendCard();
}
void CardLayerHundred::updateServerState(){
	
}
	//设置发牌区域
void CardLayerHundred::setCanSendCard(){
	canSendCard[0] = true;
	canSendCard[1] = true;
	canSendCard[2] = true;
	canSendCard[3] = true;
	canSendCard[4] = true;
	canSendCard[5] = false;
}
void CardLayerHundred::sendCardIng(){
	//偏移索引
	int offsetIndex = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (canSendCard[i])
		{
			offsetIndex++;
			sendFiveCard(i, offsetIndex);
		}
		else{

		}
	}
}
//移动单张牌
void CardLayerHundred::moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index){
	float moveSpeed=0.05;
	CCDelayTime *delayTime = CCDelayTime::create(fTime);
	CCMoveTo *moveTo = CCMoveTo::create(moveSpeed, targetPos);
	CCScaleTo *scaleTo = CCScaleTo::create(moveSpeed, getCardScale(index));
	CCSpawn *spawn = CCSpawn::create(moveTo,scaleTo, NULL);
	//CCCallFunc *callbackFunc = CCCallFunc::create(this,SEL_CallFunc(&CardLayer::onSendCardFinish));
	CCSequence *seq = CCSequence::create(delayTime,spawn,NULL);
	armature->runAction(seq);
}
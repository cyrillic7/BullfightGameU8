

#include "CardLayerHundred.h"
#include "Card.h"
#include "MainSceneBase.h"
#include "DataModel.h"
CardLayerHundred::CardLayerHundred()
{
}
CardLayerHundred::~CardLayerHundred() {
}
void CardLayerHundred::onEnter(){
	CardLayerBase::onEnter();
}
void CardLayerHundred::onExit(){
	CardLayerBase::onExit();
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
//发5张牌
void CardLayerHundred::sendFiveCard(int index,int offsetIndex){
	CCPoint cardPos = getMainScene()->posChair[index];
	for (int i = 0; i < MAX_CARD_COUNT; i++)
	{
		pCard[i+index*MAX_COUNT]->m_cpArmatureCard->setScale(0.42);
		int offx = rand() % 3;
		int offy = rand() % 3;
	    pCard[i+index*MAX_COUNT]->m_cpArmatureCard->setPosition(ccp(DataModel::sharedDataModel()->deviceSize.width / 2 + offx, DataModel::sharedDataModel()->deviceSize.height / 2 + offy));
			/*int offsetX=BaseAttributes::sharedAttributes()->iCardOffsetX[index];
		int offsetY=BaseAttributes::sharedAttributes()->iCardOffsetY[index];
		int offsetSpace=BaseAttributes::sharedAttributes()->iCardOffsetSpace[index];

		CCPoint offPos = ccp(offsetX+i*offsetSpace,offsetY);
		offPos=designResolutionToFrame(offPos);
		moveCardAction(pCard[i+index*MAX_COUNT]->m_cpArmatureCard,
			//(index-offsetIndex)*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT + i*SEND_CARD_DELAY_TIME, 
			offsetIndex*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT+i*SEND_CARD_DELAY_TIME,
			ccpAdd(cardPos, offPos),index);*/
		float offsetX=i*25-(4*25/2);
		CCPoint offPos =ccp(offsetX,0);

		moveCardAction(pCard[i+index*MAX_COUNT]->m_cpArmatureCard,
			//(index-offsetIndex)*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT + i*SEND_CARD_DELAY_TIME, 
			offsetIndex*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT+i*SEND_CARD_DELAY_TIME,
			ccpAdd(cardPos, offPos),index);
	}
}
//移动单张牌
void CardLayerHundred::moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index){
	float moveSpeed=0.05;
	CCDelayTime *delayTime = CCDelayTime::create(fTime);
	CCMoveTo *moveTo = CCMoveTo::create(moveSpeed, targetPos);
	CCScaleTo *scaleTo = CCScaleTo::create(moveSpeed, getCardScale(index));
	CCSpawn *spawn = CCSpawn::create(moveTo,scaleTo, NULL);
	CCCallFunc *callbackFunc = CCCallFunc::create(this,SEL_CallFunc(&CardLayerHundred::onSendCardFinish));
	CCSequence *seq = CCSequence::create(delayTime,spawn,callbackFunc,NULL);
	armature->runAction(seq);
}
//单张牌发完回调
void CardLayerHundred::onSendCardFinish(){
	setSendCardState(SEND_STATE_WAIT);
	sSendCardCount++;
	if (sSendCardCount==getCurAllCardCount()*MAX_CARD_COUNT)
	{
		//CCLog("CardLayer::onSendCardFinish-->sendFinish");
		//DataModel::sharedDataModel()->getMainSceneOxTwo()->setGameStateWithUpdate(MainSceneOxTwo::STATE_OPT_OX);
		//DataModel::sharedDataModel()->getMainSceneOxTwo()->setServerStateWithUpdate(MainScene::STATE_FIGHT_BANKER);
		//showCard(0,DataModel::sharedDataModel()->userInfo->wChairID);
		showCard();
		sSendCardCount=0;
	}
}
float CardLayerHundred::getCardScale(int index){
	return 0.5-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
}
//显示牌
void CardLayerHundred::showCard(){
	for (int i = 0; i <MAX_COUNT; i++)
	{
		int beginCardIndex=i*MAX_COUNT;
		/*bool isOxCard = GetOxCard(card[i], 5);
		//CCLog("niu:%d",GetCardType(card[i],5));
		int iValue=GetCardType(card[i],5);
		if (iValue>10)
		{
			iValue=10;
		}
		showOxType(i,iValue);*/
		BYTE bcTmp[5];
		int iType = GetCardType(card[i],5,bcTmp);
		if(iType==CT_POINT||iType==CT_SPECIAL_BOMEBOME)
		{
			CopyMemory(card[i],bcTmp,5);
		}
		else
		{
			CopyMemory(card[i],bcTmp,3);
			CopyMemory(card[i]+3,bcTmp+3,2);
			//CopyMemory(card[i],bcTmp+3,2);
			//CopyMemory(card[i]+2,bcTmp,3);
		}
		showOxType(i,getOxTypeWithValue(iType));

		for (int j = 0; j < sizeof(card[0]); j++)
		{
			int cardColor = GetCardColor(card[i][j]);
			int cardValue = GetCardValue(card[i][j]);
			pCard[beginCardIndex+j]->changeCard(true,cardColor,cardValue,beginCardIndex+j,getCardScale(0));
		}
	}
}
//获取牛牛类型点数
int CardLayerHundred::getOxTypeWithValue(int iType){
	int iIndex = 0;
	if(iType!= CT_POINT)
	{
		iIndex =iType-2;
		if(iIndex>=13)
		{
			if(iIndex == 13)
			{
				iIndex = 12;
			}
			if(iIndex == 14)
			{
				iIndex = 11;
			}
			if(iIndex == 15)
			{
				iIndex = 13;
			}
		}
		else
		{
			if(iIndex>10)
			{
				iIndex = 10;
			}
			if(iIndex<0)
			{
				iIndex = 0;
			}
		}
	}
	//先不区分金牛银牛////////////////////////////////////////////////////////////////////////
	if(iIndex>10)
	{
		iIndex = 10;
	}
	return iIndex;
}
void CardLayerHundred::showOxType(int chairiD,int oxType){
	float orgCradY=2000;
	for (int i = 0; i < MAX_COUNT; i++)
	{
		orgCradY=MIN(pCard[chairiD*MAX_COUNT+i]->m_cpArmatureCard->getPositionY(),orgCradY);
	}
	CCPoint cardPos=ccp(pCard[chairiD*MAX_COUNT+2]->m_cpArmatureCard->getPositionX(),orgCradY);
	cardPos=ccpAdd(cardPos,ccp(40,-30));

	pAOxType[chairiD]->setTag(oxType);
	pAOxType[chairiD]->setPosition(cardPos);
	CCSequence *seq=CCSequence::create(CCDelayTime::create(0.01+5*0.02),
		CCCallFuncN::create(this,SEL_CallFuncN(&CardLayerHundred::onPlayOxAnimation)),
		NULL);
	if (chairiD==0)
	{
		pAOxType[chairiD]->setColor(ccc3(72,255,253));
	}else
	{
		BYTE bMultiple;
		int copmare=CompareCard(card[chairiD],5,card[0],5,bMultiple);
		if (copmare==1)
		{
				pAOxType[chairiD]->setColor(ccc3(255,255,255));
		}else if (copmare==-1)
		{
				pAOxType[chairiD]->setColor(ccc3(246,252,74));
				pAOxType[chairiD]->setScale(1.1);
		}
	}
	pAOxType[chairiD]->runAction(seq);
}
void CardLayerHundred::onPlayOxAnimation(CCNode *obj){
	CCArmature *oxAnimation=(CCArmature*)obj;
	oxAnimation->setVisible(true);
	int tag=oxAnimation->getTag();
	oxAnimation->getAnimation()->play(CCString::createWithFormat("Ox_gih%d",tag)->getCString());
}
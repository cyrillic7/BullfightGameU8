

#include "CardLayerSixSwap.h"
#include "../../Tools/GameConfig.h"
#include "Card.h"
#include "../../Tools/DataModel.h"
#include "../../Tools/BaseAttributes.h"
#define SELF_SEAT									3						//自己的位置
CardLayerSixSwap::CardLayerSixSwap()
{
}
CardLayerSixSwap::~CardLayerSixSwap() {
	CCLog("~ <<%s>>",__FUNCTION__);
	this->removeAllChildrenWithCleanup(true);
}
void CardLayerSixSwap::onEnter(){
	CardLayerBase::onEnter();
}
void CardLayerSixSwap::onExit(){
	CardLayerBase::onExit();
}




//提示牛牛
bool CardLayerSixSwap::promptOx(int oxIndex){
	BYTE tempCard[5];
	memcpy(tempCard, DataModel::sharedDataModel()->card[oxIndex], sizeof(tempCard));
	//bool isOxCard = GetOxCard(tempCard, 5);
	BYTE bCardValue = GetCardType(tempCard, MAX_COUNT, tempCard);
	if (bCardValue>0)
	{
		for (int i = 0; i < MAX_COUNT; i++)
		{
			if (pCard[3 * MAX_COUNT + i]->getIsUpCard())
			{
				pCard[3 * MAX_COUNT + i]->upCard(true);
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (tempCard[i]==DataModel::sharedDataModel()->card[oxIndex][j])
				{
					float originY=pCard[3*MAX_COUNT+j]->m_cpArmatureCard->getPositionY();
					//pCard[3*MAX_COUNT+j]->m_cpArmatureCard->setPositionY(originY+30);
					if (!pCard[3 * MAX_COUNT + j]->getIsUpCard())
					{
						pCard[3 * MAX_COUNT + j]->upCard(true);
					}
					break;
				}
			}
		}
		CCLog("niu:%d",GetCardType(tempCard,5));
	}
	return bCardValue>0;
}
//
//************************************
// 重排序牛牛牌
// Parameter: int chairID实际桌子位置
// Parameter: int showChairiD显示桌子位置
//************************************
void CardLayerSixSwap::sortingOx(int chairID,int showChairiD){
	//显示牌型
	bool bOxSound = false;
	//扑克数据
	BYTE bCardData[MAX_COUNT];
	memcpy(bCardData, DataModel::sharedDataModel()->card[chairID], sizeof(bCardData));
	BYTE bCardValue = GetCardType(bCardData, MAX_COUNT, bCardData);
	CCLog("====:%d<<%s>>",bCardValue,__FUNCTION__);
	//assert(bCardValue > 0);
	//float orgCradY = 2000;
	//重排牛牛牌顺序
	for (int i = 0; i < MAX_COUNT; i++)
	{
		int cardColor = GetCardColor(bCardData[i]);
		int cardValue = GetCardValue(bCardData[i]);
		pCard[showChairiD*MAX_COUNT + i]->changeCard(false, cardColor, cardValue, i, getCardScale(showChairiD));
		
		if (pCard[showChairiD*MAX_COUNT + i]->getIsUpCard())
		{
			pCard[showChairiD*MAX_COUNT + i]->upCard(true);
		}
		//orgCradY = MIN(pCard[showChairiD*MAX_COUNT + i]->m_cpArmatureCard->getPositionY(), orgCradY);
	}
	//显示牌型点数
	showOxType(showChairiD, bCardValue);
	//动画显示玩家自己的牌
	if (showChairiD==3)
	{
		CCPoint cardPos = CCPointZero;
		for (int i = 0; i < MAX_COUNT; i++)
		{
			CCArmature *pArmature = pCard[showChairiD*MAX_COUNT + i]->m_cpArmatureCard;
			if (i == 0)
			{
				cardPos = pArmature->getPosition();
				//cardPos.y = orgCradY;
			}
			if (bCardValue == 0)
			{
				pArmature->setColor(ccc3(100, 100, 100));
			}
			else{
				if (i < 3)
				{
					pArmature->setColor(ccc3(100, 100, 100));
				}
			}
			int offsetSpace = 25;
			if (showChairiD == 3)
			{
				offsetSpace = 100;
			}
			CCPoint movePos = ccp(i*offsetSpace, 0);
			//movePos = designResolutionToFrame(movePos);

			pArmature->setPosition(cardPos);
			pArmature->runAction(CCMoveTo::create(0.01 + i*0.02, ccpAdd(cardPos, movePos)));
		}
	}
	

	 
	/*bool bOxSound = false;
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		WORD wViewChairID = m_wViewChairID[i];
		if (i == GetMeChairID() && !IsLookonMode())continue;
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayItem(true);

		//牛牛牌型
		if (m_cbHandCardData[i][0]>0)
		{
			if (m_bUserOxCard[i] == TRUE)
			{
				//扑克数据
				BYTE bCardData[MAX_COUNT];
				CopyMemory(bCardData, m_cbHandCardData[i], sizeof(bCardData));

				//获取牛牛数据
				//m_GameLogic.GetOxCard(bCardData,MAX_COUNT);

				BYTE bCardValue = m_GameLogic.GetCardType(bCardData, MAX_COUNT, bCardData);
				ASSERT(bCardValue > 0);

				//加载数据
				m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardData, 3);
				m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bCardData[3], 2);

				//显示点数
				if (bCardValue >= 10)bOxSound = true;
				m_GameClientView.SetUserOxValue(wViewChairID, bCardValue);
			}
			else
			{
				//无牛
				m_GameClientView.SetUserOxValue(wViewChairID, 0);
			}
		}
	}*/

	/*
	CCPoint cardPos = getMainScene()->posChair[showChairiD];

	BYTE tempCard[5];
	memcpy(tempCard, DataModel::sharedDataModel()->card[chairID], sizeof(tempCard));
	bool isOxCard = GetOxCard(tempCard, 5);
	if (isOxCard)
	{
		//重排牛牛牌顺序
		for (int i = 0; i < MAX_COUNT; i++)
		{
			int cardColor = GetCardColor(tempCard[i]);
			int cardValue = GetCardValue(tempCard[i]);
			pCard[showChairiD*MAX_COUNT+i]->changeCard(false,cardColor,cardValue,i,getCardScale(showChairiD));
		}
		
	}
	//显示点数
	int iValue=GetCardType(tempCard,5);
	if (iValue>10)
	{
		iValue=10;
	}
	showOxType(showChairiD,iValue);
	{
		int jumpSpace=0;
		for (int i = 0; i < MAX_COUNT; i++)
		{
			int offsetX=BaseAttributes::sharedAttributes()->iCardOffsetX[showChairiD]+120;
			int offsetY=BaseAttributes::sharedAttributes()->iCardOffsetY[showChairiD];
			int offsetSpace=BaseAttributes::sharedAttributes()->iCardOffsetSpace[showChairiD]-40;
		
			CCArmature *pArmature=pCard[showChairiD*MAX_COUNT+i]->m_cpArmatureCard;
			if (i>2&&isOxCard)
			{
				jumpSpace=0;
			}else 
			{
				pArmature->setColor(ccc3(100,100,100));
			}
			CCPoint offPos = ccp(offsetX,offsetY);
			offPos=designResolutionToFrame(offPos);

			CCPoint movePos = ccp(offsetX+i*offsetSpace+jumpSpace,offsetY);
			movePos=designResolutionToFrame(movePos);

			//pArmature->setScale(pArmature->getScale()-0.2);
			pArmature->setPosition(ccpAdd(cardPos,offPos));
			pArmature->runAction(CCMoveTo::create(0.01+i*0.02,ccpAdd(cardPos,movePos)));
		}
	}*/
}
void CardLayerSixSwap::showOxType(int chairiD,int oxType){
	//float orgCradY=2000;
	for (int i = 0; i < MAX_COUNT; i++)
	{
		if (pCard[chairiD*MAX_COUNT + i]->getIsUpCard())
		{
			pCard[chairiD*MAX_COUNT + i]->upCard(true);
		}
		//orgCradY=MIN(pCard[chairiD*MAX_COUNT+i]->m_cpArmatureCard->getPositionY(),orgCradY);
	}
	CCPoint cardPos = ccp(pCard[chairiD*MAX_COUNT + 2]->m_cpArmatureCard->getPositionX(), pCard[chairiD*MAX_COUNT + 2]->m_cpArmatureCard->getPositionY() - 20);

	pAOxType[chairiD]->setTag(oxType);
	pAOxType[chairiD]->setPosition(cardPos);
	pAOxType[chairiD]->setScale(getCardScale(chairiD));
	CCSequence *seq=CCSequence::create(CCDelayTime::create(0.01+5*0.02),
	CCCallFuncN::create(this,SEL_CallFuncN(&CardLayerSixSwap::onPlayOxAnimation)),
	NULL);
	pAOxType[chairiD]->runAction(seq);
}
void CardLayerSixSwap::onPlayOxAnimation(CCNode *obj){
	CCArmature *oxAnimation=(CCArmature*)obj;
	oxAnimation->setVisible(true);
	oxAnimation->getAnimation()->play(CCString::createWithFormat("Ox%d",oxAnimation->getTag())->getCString());
}
//发牌中
void CardLayerSixSwap::sendCardIng(){
	sSendCardCount = 0;
	//偏移索引
	int offsetIndex = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
//		int bankerUser=DataModel::sharedDataModel()->getMainSceneOxTwo()->getGameControlOxTwo()->getBankViewID();
		int bankerUser = 0;
		int index=(bankerUser+i)%MAX_PLAYER;
		if (canSendCard[index])
		{
			offsetIndex++;
			sendFiveCard(index, offsetIndex);
		}
		else{
			
		}
	}

	
}

//发5张牌
void CardLayerSixSwap::sendFiveCard(int index,int offsetIndex){
	CCPoint cardPos = getMainScene()->posChair[index];
	int jg = 25;
	if (index == 3)
	{
		jg = 100;
	}
	for (int i = 0; i < MAX_CARD_COUNT; i++)
	{
		
		pCard[i + index*MAX_COUNT]->m_cpArmatureCard->setScale(0.42);
		int offx = rand() % 3;
		int offy = rand() % 3;
		pCard[i + index*MAX_COUNT]->m_cpArmatureCard->setPosition(ccp(DataModel::sharedDataModel()->deviceSize.width / 2 + offx, SCENE_SIZE.height/2 + offy));
		float offsetX = i * jg - (4 * jg / 2);
		CCPoint offPos = ccp(offsetX, 0);
		moveCardAction(pCard[i + index*MAX_COUNT]->m_cpArmatureCard,
			//(index-offsetIndex)*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT + i*SEND_CARD_DELAY_TIME, 
			offsetIndex*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT + i*SEND_CARD_DELAY_TIME,
			ccpAdd(cardPos, offPos), index);
	}
}
//移动单张牌
void CardLayerSixSwap::moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index){
	float moveSpeed = 0.35;
	CCDelayTime *delayTime = CCDelayTime::create(fTime);
	//CCMoveTo *moveTo = CCMoveTo::create(moveSpeed, targetPos);
	CCJumpTo *moveTo = CCJumpTo::create(moveSpeed, targetPos, 100, 1);
	CCScaleTo *scaleTo = CCScaleTo::create(moveSpeed, getCardScale(index));
	CCSpawn *spawn = CCSpawn::create(moveTo, scaleTo, NULL);
	CCCallFunc *callbackFunc = CCCallFunc::create(this, SEL_CallFunc(&CardLayerSixSwap::onSendCardFinish));
	CCSequence *seq = CCSequence::create(delayTime, spawn, callbackFunc, NULL);
	armature->runAction(seq);
}
//单张牌发完回调
void CardLayerSixSwap::onSendCardFinish(){
	setSendCardState(SEND_STATE_WAIT);
	sSendCardCount++;
	if (sSendCardCount==getCurAllCardCount()*MAX_CARD_COUNT)
	{
		CCLog("CardLayer::onSendCardFinish-->sendFinish");
		getMainScene()->setGameStateWithUpdate(MainSceneBase::STATE_SWAP_CARD);
		//DataModel::sharedDataModel()->getm()->setGameStateWithUpdate(MainSceneOxTwo::STATE_OPT_OX);
		//DataModel::sharedDataModel()->getMainSceneOxTwo()->setServerStateWithUpdate(MainScene::STATE_FIGHT_BANKER);
		showCard(true,SELF_SEAT,DataModel::sharedDataModel()->userInfo->wChairID);
		sSendCardCount=0;
	}
}

void CardLayerSixSwap::updateServerState(){
	switch (getMainScene()->getServerState())
	{
	case MainSceneOxTwo::STATE_SEND_CARD:
	{
		sendCard();
	}
		break;
	default:
		break;
	}
}
void CardLayerSixSwap::updateGameState(){
	switch (getMainScene()->getGameState())
	{
	case MainSceneOxTwo::STATE_READY:
		{
			resetCard();
		}
		break;
	default:
		break;
	}
}
void CardLayerSixSwap::setCanSendCard(){
	canSendCard[0] = false;
	canSendCard[1] = false;
	canSendCard[2] = false;
	canSendCard[3] = false;
	canSendCard[4] = false;
	canSendCard[5] = false;
}
float CardLayerSixSwap::getCardScale(int index){
	if (index==SELF_SEAT)
	{
		return 0.72-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
	}
	return 0.5-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
}
//显示牌
void CardLayerSixSwap::showCard(bool isAction, int index, int dataIndex){
	/*int beginCardIndex=index*MAX_COUNT;
	for (int i = 0; i < MAX_COUNT; i++)
	{
		int cardColor = GetCardColor(DataModel::sharedDataModel()->card[dataIndex][i]);
		int cardValue = GetCardValue(DataModel::sharedDataModel()->card[dataIndex][i]);
		pCard[beginCardIndex + i]->changeCard(isAction, cardColor, cardValue, beginCardIndex + i, getCardScale(index));
	}*/
	int beginCardIndex = index*MAX_COUNT;
	//设置位置
	CCPoint cardPos = getMainScene()->posChair[index];
	int jg = 25;
	if (index == 3)
	{
		jg = 100;
	}

	for (int i = 0; i < MAX_COUNT; i++)
	{
		int cardColor = GetCardColor(DataModel::sharedDataModel()->card[dataIndex][i]);
		int cardValue = GetCardValue(DataModel::sharedDataModel()->card[dataIndex][i]);
		pCard[beginCardIndex + i]->changeCard(isAction, cardColor, cardValue, beginCardIndex + i, getCardScale(index));


		CCArmature *pArmature = pCard[beginCardIndex + i]->m_cpArmatureCard;

		pArmature->setScale(getCardScale(index));

		int offx = rand() % 3;
		int offy = rand() % 3;
		float offsetX = i * jg - (4 * jg / 2);
		CCPoint offPos = ccp(offsetX, 0);

		pArmature->setPosition(ccpAdd(cardPos, offPos));
	}
}
//触摸牌
void CardLayerSixSwap::touchCard(unsigned short beginPos, CCPoint pos){
	bool isAction = true;
	int limitCount = 0;
	if (getMainScene()->getGameState()==MainSceneBase::STATE_OPT_OX)
	{
		limitCount = 2;
	}
	
	if (getAllUpCardCount(beginPos)>limitCount)
	{
		isAction = false;
	}
	for (int i = beginPos*MAX_CARD_COUNT; i < beginPos*MAX_CARD_COUNT + MAX_CARD_COUNT; i++)
	{
		pCard[i]->touchCard(pos, isAction);
	}
}


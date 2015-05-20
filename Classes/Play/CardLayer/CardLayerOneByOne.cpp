

#include "CardLayerOneByOne.h"
#include "../../Tools/GameConfig.h"
#include "Card.h"
#include "../../Tools/DataModel.h"
#include "../../Tools/BaseAttributes.h"
#define SELF_SEAT									3						//自己的位置
CardLayerOneByOne::CardLayerOneByOne()
{
}
CardLayerOneByOne::~CardLayerOneByOne() {
	CCLog("~ <<%s>>",__FUNCTION__);
	this->removeAllChildrenWithCleanup(true);
}
void CardLayerOneByOne::onEnter(){
	CardLayerBase::onEnter();
}
void CardLayerOneByOne::onExit(){
	CardLayerBase::onExit();
}



//提示牛牛
bool CardLayerOneByOne::promptOx(int oxIndex){
	BYTE tempCard[5];
	memcpy(tempCard, DataModel::sharedDataModel()->card[oxIndex], sizeof(tempCard));
	bool isOxCard = GetOxCard(tempCard, 5);
	if (isOxCard)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (tempCard[i]==DataModel::sharedDataModel()->card[oxIndex][j])
				{
					float originY=pCard[3*MAX_COUNT+j]->m_cpArmatureCard->getPositionY();
					pCard[3*MAX_COUNT+j]->m_cpArmatureCard->setPositionY(originY+30);
					break;
				}
			}
		}
		CCLog("niu:%d",GetCardType(tempCard,5));
	}
	return isOxCard;
}
//
//************************************
// 重排序牛牛牌
// Parameter: int chairID实际桌子位置
// Parameter: int showChairiD显示桌子位置
//************************************
void CardLayerOneByOne::sortingOx(int chairID,int showChairiD){
	UIPanel *playerPanel =getMainScene()->playerLayer->pPlayerData[showChairiD]-> pPlayerPanel;
	UIImageView *iPlayerIcon = (UIImageView*)playerPanel->getChildByName("headPortrait");
	CCPoint playerPos = playerPanel->getPosition();
	CCPoint cardPos = ccpAdd(playerPos, iPlayerIcon->getPosition());

	BYTE tempCard[5];
	memcpy(tempCard, DataModel::sharedDataModel()->card[chairID], sizeof(tempCard));
	bool isOxCard = GetOxCard(tempCard, 5);
	if (isOxCard)
	{
		//重排牛牛牌顺序
		for (int i = 0; i < MAX_COUNT; i++)
		{
			int cardColor = GetCardColor(tempCard[i])/16;
			int cardValue = GetCardValue(tempCard[i]);
			pCard[showChairiD*MAX_COUNT+i]->changeCard(false,cardColor,cardValue,i,1);
		}
		
	}

	//显示点数
	/*BYTE bValue=GetCardLogicValue(tempCard[3])+GetCardLogicValue(tempCard[4]);
	if(bValue>10)bValue-=10;
	if(bValue>=10)bValue=10;
	showOxType(showChairiD,bValue);*/
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
	}
}
void CardLayerOneByOne::showOxType(int chairiD,int oxType){
	float orgCradY=2000;
	for (int i = 0; i < MAX_COUNT; i++)
	{
			orgCradY=MIN(pCard[chairiD*MAX_COUNT+i]->m_cpArmatureCard->getPositionY(),orgCradY);
	}
	CCPoint cardPos=ccp(pCard[chairiD*MAX_COUNT+2]->m_cpArmatureCard->getPositionX(),orgCradY);

	pAOxType[chairiD]->setTag(oxType);
	pAOxType[chairiD]->setPosition(cardPos);
	CCSequence *seq=CCSequence::create(CCDelayTime::create(0.01+5*0.02),
	CCCallFuncN::create(this,SEL_CallFuncN(&CardLayerOneByOne::onPlayOxAnimation)),
	NULL);
	pAOxType[chairiD]->runAction(seq);
}
void CardLayerOneByOne::onPlayOxAnimation(CCNode *obj){
	CCArmature *oxAnimation=(CCArmature*)obj;
	oxAnimation->setVisible(true);
	oxAnimation->getAnimation()->play(CCString::createWithFormat("Ox%d",oxAnimation->getTag())->getCString());
}
//发牌中
void CardLayerOneByOne::sendCardIng(){
	//偏移索引
	int offsetIndex = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
//		int bankerUser=DataModel::sharedDataModel()->getMainSceneOxTwo()->getGameControlOxTwo()->getBankViewID();
		int bankerUser = 0;
		int index=(bankerUser+i)%MAX_PLAYER;
		CCLog("index:%d<<%s>>",index,__FUNCTION__);
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
void CardLayerOneByOne::sendFiveCard(int index,int offsetIndex){
	CCPoint cardPos = getMainScene()->posChair[index];
	for (int i = 0; i < MAX_CARD_COUNT; i++)
	{
		pCard[i+index*MAX_COUNT]->m_cpArmatureCard->setScale(0.42);
		int offx = rand() % 3;
		int offy = rand() % 3;
		pCard[i+index*MAX_COUNT]->m_cpArmatureCard->setPosition(ccp(DataModel::sharedDataModel()->deviceSize.width / 2 + offx, DataModel::sharedDataModel()->deviceSize.height / 2 + offy));
		int offsetX=BaseAttributes::sharedAttributes()->iCardOffsetX[index];
		int offsetY=BaseAttributes::sharedAttributes()->iCardOffsetY[index];
		int offsetSpace=BaseAttributes::sharedAttributes()->iCardOffsetSpace[index];
		
		CCPoint offPos = ccp(offsetX+i*offsetSpace,offsetY);
		offPos=designResolutionToFrame(offPos);
		moveCardAction(pCard[i+index*MAX_COUNT]->m_cpArmatureCard,
			//(index-offsetIndex)*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT + i*SEND_CARD_DELAY_TIME, 
			offsetIndex*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT+i*SEND_CARD_DELAY_TIME,
			ccpAdd(cardPos, offPos),index);
	}
}
//移动单张牌
void CardLayerOneByOne::moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index){
	float moveSpeed=0.05;
	CCDelayTime *delayTime = CCDelayTime::create(fTime);
	CCMoveTo *moveTo = CCMoveTo::create(moveSpeed, targetPos);
	CCScaleTo *scaleTo = CCScaleTo::create(moveSpeed, getCardScale(index));
	CCSpawn *spawn = CCSpawn::create(moveTo,scaleTo, NULL);
	CCCallFunc *callbackFunc = CCCallFunc::create(this,SEL_CallFunc(&CardLayerOneByOne::onSendCardFinish));
	CCSequence *seq = CCSequence::create(delayTime,spawn,callbackFunc,NULL);
	armature->runAction(seq);
}
//单张牌发完回调
void CardLayerOneByOne::onSendCardFinish(){
	setSendCardState(SEND_STATE_WAIT);
	sSendCardCount++;
	if (sSendCardCount==getCurAllCardCount()*MAX_CARD_COUNT)
	{
		CCLog("CardLayer::onSendCardFinish-->sendFinish");
		getMainScene()->setGameStateWithUpdate(MainSceneBase::STATE_OPT_OX);
		//DataModel::sharedDataModel()->getm()->setGameStateWithUpdate(MainSceneOxTwo::STATE_OPT_OX);
		//DataModel::sharedDataModel()->getMainSceneOxTwo()->setServerStateWithUpdate(MainScene::STATE_FIGHT_BANKER);
		showCard(SELF_SEAT,DataModel::sharedDataModel()->userInfo->wChairID);
		sSendCardCount=0;
	}
}

void CardLayerOneByOne::updateServerState(){
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
void CardLayerOneByOne::updateGameState(){
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
void CardLayerOneByOne::setCanSendCard(){
	canSendCard[0] = true;
	canSendCard[1] = false;
	canSendCard[2] = false;
	canSendCard[3] = true;
	canSendCard[4] = false;
	canSendCard[5] = false;
	
	/*canSendCard[0] = true;
	canSendCard[1] = true;
	canSendCard[2] = true;
	canSendCard[3] = true;
	canSendCard[4] = true;
	canSendCard[5] = true;*/
}
float CardLayerOneByOne::getCardScale(int index){
	if (index==SELF_SEAT)
	{
		return 0.9-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
	}
	return 0.8-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
}
//显示牌
void CardLayerOneByOne::showCard(int index,int dataIndex){
	int beginCardIndex=index*MAX_COUNT;
	for (int i = 0; i < MAX_COUNT; i++)
	{
		int cardColor = GetCardColor(DataModel::sharedDataModel()->card[dataIndex][i])/16;
		int cardValue = GetCardValue(DataModel::sharedDataModel()->card[dataIndex][i]);
		pCard[beginCardIndex+i]->changeCard(true,cardColor,cardValue,beginCardIndex+i,1);
	}
}



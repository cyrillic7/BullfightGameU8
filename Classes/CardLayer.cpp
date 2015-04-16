

#include "CardLayer.h"
#include "GameConfig.h"
#include "Card.h"
#include "DataModel.h"
#include "BaseAttributes.h"
#define MAX_CARD_COUNT								5						//最大牌数
#define SEND_CARD_DELAY_TIME						0.05					//发牌延时时长 
#define SELF_SEAT									3						//自己的位置
CardLayer::CardLayer()
:sendCardState(SEND_STATE_WAIT)
, sSendCardCount(0)
{
}
CardLayer::~CardLayer() {
}
bool CardLayer::init(){
	if (!CCLayer::init())
	{
		return false;
	}
	//初始化扑克集
	batchCard = CCBatchNode::create();
	this->addChild(batchCard, 1);
	//加载扑克动画文件
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationCard.ExportJson));

	setCanSendCard();
	return true;
}
void CardLayer::onEnter(){
	CCLayer::onEnter();
	initAllCard();
}
void CardLayer::onExit(){
	CCLayer::onExit();
}
//发牌
void CardLayer::sendCard(){
	switch (getSendCardState())
	{
	case SEND_STATE_WAIT:
	{
		resetCard();
		setSendCardState(SEND_STATE_ING);
		sendCard();
	}
		break;
	case SEND_STATE_ING:
	{
		sendCardIng();
	}
		break;
	default:
		break;
	}
	/*BYTE mCrad[5] = {0x02,0x11,0x22,8,6};

	BYTE cardColor;
	BYTE cardValue;
	for (int i = 0; i < 5;i++)
	{
		cardColor = DataModel::sharedDataModel()->gameLogic->GetCardColor(mCrad[i]);
		cardValue = DataModel::sharedDataModel()->gameLogic->GetCardValue(mCrad[i]);

		Card *card = Card::create();
		card->createCardArmature(batchCard,cardColor/16,cardValue);
		this->addChild(card);
		card->setTag(i);
		card->m_cpArmatureCard->setPosition(ccp(100+i*140, 120));
	}
	
	BYTE tempCard[5];
	CopyMemory(tempCard, mCrad, sizeof(tempCard));
	bool isOxCard = DataModel::sharedDataModel()->gameLogic->GetOxCard(tempCard, 5);
	if (isOxCard)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (tempCard[i]==mCrad[j])
				{
					CCLog("--%d", j);
					Card *card = (Card*)this->getChildByTag(j);
					float y = card->m_cpArmatureCard->getPositionY() + 30;
					card->m_cpArmatureCard->setPositionY(y);
					break;
				}
			}
		}
	}*/
}
//发牌中
void CardLayer::sendCardIng(){
	//偏移索引
	int offsetIndex = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (canSendCard[i])
		{
			sendFiveCard(i, offsetIndex);
		}
		else{
			offsetIndex++;
		}
	}
}
//初始化所有牌
void CardLayer::initAllCard(){
	for (int i = 0; i < MAX_PLAYER; i++){
		for (int j = 0; j < MAX_CARD_COUNT; j++)
		{
			pCard[j+i*MAX_CARD_COUNT] = Card::create();
			this->addChild(pCard[j+i*MAX_CARD_COUNT]);
			pCard[j+i*MAX_CARD_COUNT]->createCardArmature(batchCard, 5, 0, 1);
		}
	}
}
//重置牌
void CardLayer::resetCard(){
	for (int i = 0; i < MAX_PLAYER; i++){
		for (int j = 0; j < MAX_CARD_COUNT; j++)
		{
			pCard[j+i*MAX_CARD_COUNT]->setScale(0.42);
			pCard[j+i*MAX_CARD_COUNT]->changeCard(false, 5, 0, 1);
		}
	}
}
//发5张牌
void CardLayer::sendFiveCard(int index,int offsetIndex){
	UIPanel *playerPanel = DataModel::sharedDataModel()->getMainScene()->playerLayer->playerPanel[index];
	UIImageView *iPlayerIcon = (UIImageView*)playerPanel->getChildByName("headPortrait");
	CCPoint playerPos = playerPanel->getPosition();
	CCPoint cardPos = ccpAdd(playerPos, iPlayerIcon->getPosition());

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
		moveCardAction(pCard[i+index*MAX_COUNT]->m_cpArmatureCard, (index-offsetIndex)*SEND_CARD_DELAY_TIME*MAX_CARD_COUNT + i*SEND_CARD_DELAY_TIME, ccpAdd(cardPos, offPos),index);
	}
}
//移动单张牌
void CardLayer::moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index){
	float moveSpeed=0.05;
	CCDelayTime *delayTime = CCDelayTime::create(fTime);
	CCMoveTo *moveTo = CCMoveTo::create(moveSpeed, targetPos);
	CCScaleTo *scaleTo = CCScaleTo::create(moveSpeed, getCardScale(index));
	CCSpawn *spawn = CCSpawn::create(moveTo,scaleTo, NULL);
	CCCallFunc *callbackFunc = CCCallFunc::create(this,SEL_CallFunc(&CardLayer::onSendCardFinish));
	CCSequence *seq = CCSequence::create(delayTime,spawn,callbackFunc,NULL);
	armature->runAction(seq);
}
void CardLayer::onSendCardFinish(){
	setSendCardState(SEND_STATE_WAIT);
	sSendCardCount++;
	if (sSendCardCount==getCurAllCardCount()*MAX_CARD_COUNT)
	{
		CCLog("CardLayer::onSendCardFinish-->sendFinish");
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
		//DataModel::sharedDataModel()->getMainScene()->setServerStateWithUpdate(MainScene::STATE_FIGHT_BANKER);
		showCard(SELF_SEAT,DataModel::sharedDataModel()->userInfo->wChairID);
		sSendCardCount=0;
	}
}
short CardLayer::getCurAllCardCount(){
	int count = 0;
	for (int i = 0; i < MAX_CARD_COUNT; i++)
	{
		if (canSendCard[i])
		{
			count++;
		}
	}
	return count;
}
void CardLayer::updateServerState(){
	switch (DataModel::sharedDataModel()->getMainScene()->getServerState())
	{
	case MainScene::STATE_SEND_CARD:
	{
		sendCard();
	}
		break;
	default:
		break;
	}
}
void CardLayer::updateGameState(){
	switch (DataModel::sharedDataModel()->getMainScene()->getGameState())
	{
	case MainScene::STATE_READY:
		{
			for (int i = 0; i < MAX_PLAYER; i++){
				for (int j = 0; j < MAX_CARD_COUNT; j++)
				{
					pCard[j+i*MAX_CARD_COUNT]->m_cpArmatureCard->setPosition(ccp(-1000,-1000));
				}
			}
		}
		break;
	default:
		break;
	}
}
void CardLayer::setCanSendCard(){
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
float CardLayer::getCardScale(int index){
	if (index==SELF_SEAT)
	{
		return 0.9-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
	}
	return 0.8-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
}
//显示牌
void CardLayer::showCard(int index,int dataIndex){
	int beginCardIndex=index*MAX_COUNT;
	for (int i = 0; i < MAX_COUNT; i++)
	{
		int cardColor = GetCardColor(DataModel::sharedDataModel()->card[dataIndex][i])/16;
		int cardValue = GetCardValue(DataModel::sharedDataModel()->card[dataIndex][i]);
		pCard[beginCardIndex+i]->changeCard(true,cardColor,cardValue,i);
	}
}
CCPoint CardLayer::designResolutionToFrame(CCPoint designPos){
	CCPoint pos;
	float posScaleX=designPos.x/SCENE_SIZE.width;
	pos.x=posScaleX*DataModel::sharedDataModel()->deviceSize.width;
	float posScaleY=designPos.y/SCENE_SIZE.height;
	pos.y=posScaleY*DataModel::sharedDataModel()->deviceSize.height;
	return pos;
}
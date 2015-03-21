

#include "CardLayer.h"
#include "GameConfig.h"
#include "Card.h"
#include "DataModel.h"
CardLayer::CardLayer()
:sendCardState(SEND_STATE_WAIT)
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
}
void CardLayer::onExit(){

}
//发牌
void CardLayer::sendCard(){
	switch (getSendCardState())
	{
	case SEND_STATE_WAIT:
	{
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
void CardLayer::sendCardIng(){
	Card *card = Card::create();
	card->createCardArmature(batchCard, 5, 0,0);
	this->addChild(card);
	card->m_cpArmatureCard->setScale(0.3);
	card->m_cpArmatureCard->setPosition(ccp(SCENE_SIZE.width/2, SCENE_SIZE.height/2));

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		sendFiveCard(i);
	}
	
}
//发5张牌
void CardLayer::sendFiveCard(int index){
	if (canSendCard[index])
	{
		CCPoint cardPos = DataModel::sharedDataModel()->getMainScene()->playerLayer->playerPanel[index]->getPosition();
		for (int i = 0; i < 5; i++)
		{
			Card *cardMove = Card::create();
			cardMove->createCardArmature(batchCard, 5, 0, 1);
			this->addChild(cardMove);
			cardMove->m_cpArmatureCard->setScale(0.3);
			int offx = rand() % 3;
			int offy = rand() % 3;
			cardMove->m_cpArmatureCard->setPosition(ccp(SCENE_SIZE.width / 2 + offx, SCENE_SIZE.height / 2 + offy));
			CCPoint offPos = ccp(i*20,0);
			moveCardAction(cardMove->m_cpArmatureCard, ccpAdd(cardPos,offPos));
		}
	}
}
void CardLayer::moveCardAction(CCArmature *armature, CCPoint targetPos){
	float moveSpeed=0.2;
	CCMoveTo *moveTo = CCMoveTo::create(moveSpeed, targetPos);
	CCScaleTo *scaleTo = CCScaleTo::create(moveSpeed, 0.5);
	CCSpawn *spawn = CCSpawn::create(moveTo,scaleTo, NULL);
	CCSequence *seq = CCSequence::create(spawn,NULL);
	armature->runAction(seq);
}
void CardLayer::updateState(){
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
void CardLayer::setCanSendCard(){
	canSendCard[0] = false;
	canSendCard[1] = true;
	canSendCard[2] = true;
	canSendCard[3] = true;
	canSendCard[4] = true;

}


#include "CardLayerBase.h"
#include "Card.h"
#include "../../MainScene/MainSceneBase.h"
#include "../../Tools/DataModel.h"
CardLayerBase::CardLayerBase()
:sendCardState(SEND_STATE_WAIT)
,sSendCardCount(0)
{
}
CardLayerBase::~CardLayerBase() {
}
bool CardLayerBase::init(){
	if (!CCLayer::init())
	{
		return false;
	}
	//初始化扑克集
	batchCard = CCBatchNode::create();
	this->addChild(batchCard, 1);
	//加载扑克动画文件
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationCard.ExportJson));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationOxType.ExportJson));

	setCanSendCard();
	return true;
}
void CardLayerBase::onEnter(){
	CCLayer::onEnter();
	initAllCard();
	initOxType();
}
void CardLayerBase::onExit(){
	CCLayer::onExit();
}
//初始化所有牌
void CardLayerBase::initAllCard(){
	for (int i = 0; i < MAX_PLAYER; i++){
		for (int j = 0; j < MAX_CARD_COUNT; j++)
		{
			pCard[j+i*MAX_CARD_COUNT] = Card::create();
			this->addChild(pCard[j+i*MAX_CARD_COUNT]);
			pCard[j+i*MAX_CARD_COUNT]->createCardArmature(batchCard, 5, 0, 1);
		}
	}
}
//初始化牛牛点数动画
void CardLayerBase::initOxType(){
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pAOxType[i] = CCArmature::create("AnimationOxType");
		pAOxType[i]->setVisible(false);
		this->addChild(pAOxType[i],100);
	}
}
void CardLayerBase::updateGameState(){
	
}
void CardLayerBase::updateServerState(){
	
}
	//设置发牌区域
void CardLayerBase::setCanSendCard(){

}
//发牌
void CardLayerBase::sendCard(){
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
//重置牌
void CardLayerBase::resetCard(){
	for (int i = 0; i < MAX_PLAYER; i++){
		for (int j = 0; j < MAX_CARD_COUNT; j++)
		{
			pCard[j+i*MAX_CARD_COUNT]->setScale(0.42);
			pCard[j+i*MAX_CARD_COUNT]->changeCard(false, 5, 0, 1,1);
			pCard[j+i*MAX_CARD_COUNT]->m_cpArmatureCard->setColor(ccc3(255,255,255));
			pCard[j+i*MAX_CARD_COUNT]->m_cpArmatureCard->setPosition(ccp(-1000,-1000));
		}
	}
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pAOxType[i]->setVisible(false);
	}
}
void CardLayerBase::sendCardIng(){
	
}
//发5张牌
void CardLayerBase::sendFiveCard(int index,int offsetIndex){
	
}
//移动单张牌
void CardLayerBase::moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index){
	
}
//单张牌发完回调
void CardLayerBase::onSendCardFinish(){
	
}
float CardLayerBase::getCardScale(int index){
	return 0.5-(1-DataModel::sharedDataModel()->deviceSize.height/SCENE_SIZE.height);
}
MainSceneBase*CardLayerBase::getMainScene(){
	return (MainSceneBase*)this->getParent();
}
short CardLayerBase::getCurAllCardCount(){
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


#include "CardLayer.h"
#include "GameConfig.h"
#include "Card.h"
CardLayer::CardLayer()
{
}
CardLayer::~CardLayer() {
}
bool CardLayer::init(){
	if (!CCLayer::init())
	{
		return false;
	}
	batchCard = CCBatchNode::create();
	this->addChild(batchCard, 1);
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationCard.ExportJson));

	Card *card = Card::create();
	card->createCard();
	card->m_cpArmatureCard->setPosition(ccp(200, 300));

	return true;
}
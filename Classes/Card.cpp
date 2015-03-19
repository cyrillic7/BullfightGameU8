

#include "Card.h"
#include "DataModel.h"
Card::Card()
{
}
Card::~Card() {
}
void Card::createCardArmature(CCBatchNode *cardBatch,short cardColor,short cardValue)
{
	m_cpArmatureCard = CCArmature::create("AnimationCard");
	cardBatch->addChild(m_cpArmatureCard);
	m_cpArmatureCard->getAnimation()->play(CCString::createWithFormat("card_%d_%d",cardColor,cardValue)->getCString());
}

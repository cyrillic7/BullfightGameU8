

#include "Card.h"
#include "DataModel.h"
Card::Card()
{
}
Card::~Card() {
}
void Card::createCardArmature(CCBatchNode *cardBatch, short cardColor, short cardValue, int zOrder)
{
	m_cpArmatureCard = CCArmature::create("AnimationCard");
	cardBatch->addChild(m_cpArmatureCard,zOrder);
	m_cpArmatureCard->getAnimation()->play(CCString::createWithFormat("card_%d_%d",cardColor,cardValue)->getCString());
	m_cpArmatureCard->setScaleX(-1);
	m_cpArmatureCard->setPosition(ccp(-1000,-1000));
}
void Card::changeCard(bool isAction,short cardColor, short cardValue, int zOrder){
	iCardColor=cardColor;
	iCardValue=cardValue;
	iZOrder=zOrder;
	if (!isAction)
	{
		showCard();
	}else
	{
		CCSequence *seq = CCSequence::create(
			CCScaleTo::create(0.1,0,1),
			CCCallFunc::create(this,SEL_CallFunc(&Card::showCard)),
			CCScaleTo::create(0.1,1,1),
			NULL);
		m_cpArmatureCard->runAction(seq);
	}
}
void Card::showCard(){
	m_cpArmatureCard->getAnimation()->play(CCString::createWithFormat("card_%d_%d",iCardColor,iCardValue)->getCString());
	m_cpArmatureCard->getParent()->reorderChild(m_cpArmatureCard,iZOrder);
}
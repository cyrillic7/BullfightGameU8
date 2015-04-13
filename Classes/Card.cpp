

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
}
void Card::changeCard(short cardColor, short cardValue, int zOrder){
	//m_cpArmatureCard->getParent()->reorderChild(m_cpArmatureCard,zOrder);
	
	CCNode *node=CCNode::create();
	node->setPosition(cardColor,cardValue);
	this->addChild(node);
	m_cpArmatureCard->setScaleX(-1);
	CCSequence *seq = CCSequence::create(
		CCScaleTo::create(2,0,1),
		//CCCallFuncN::create(node,SEL_CallFuncN(&Card::showCard)),
		CCScaleTo::create(2,1,1),NULL);
	m_cpArmatureCard->runAction(seq);
}
void Card::showCard(CCNode *node){
	short cardColor=node->getPositionX();
	short cardValue=node->getPositionY();
	m_cpArmatureCard->getAnimation()->play(CCString::createWithFormat("card_%d_%d",0,0)->getCString());

	node->removeFromParentAndCleanup(true);
}


#include "Card.h"
#include "../../Tools/DataModel.h"
Card::Card()
:isUpCard(false)
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
void Card::changeCard(bool isAction,short cardColor, short cardValue, int zOrder,float fScale){
	iCardColor=cardColor;
	iCardValue=cardValue;
	iZOrder=zOrder;
	if (!isAction)
	{
		showCard();
	}else
	{
		CCSequence *seq = CCSequence::create(
			CCScaleTo::create(0.1,0,fScale),
			CCCallFunc::create(this,SEL_CallFunc(&Card::showCard)),
			CCScaleTo::create(0.1,fScale,fScale),
			NULL);
		m_cpArmatureCard->runAction(seq);
	}
}
void Card::showCard(){
	m_cpArmatureCard->getAnimation()->play(CCString::createWithFormat("card_%d_%d",iCardColor,iCardValue)->getCString());
	m_cpArmatureCard->getParent()->reorderChild(m_cpArmatureCard,iZOrder);
}
//触摸牌
void Card::touchCard(CCPoint pos,bool isAction){
	CCRect rect = CCRect(
		m_cpArmatureCard->getPositionX() - m_cpArmatureCard->getContentSize().width / 2 * m_cpArmatureCard->getScaleX(),
		m_cpArmatureCard->getPositionY() - m_cpArmatureCard->getContentSize().height / 2 * m_cpArmatureCard->getScaleY(),
		m_cpArmatureCard->getContentSize().width*m_cpArmatureCard->getScaleX(),
		m_cpArmatureCard->getContentSize().height*m_cpArmatureCard->getScaleY());

	if (rect.containsPoint(pos))
	{
		upCard(isAction);
	}
}
//抬起牌
void Card::upCard(bool isAction){
	
		if (!isUpCard)
		{
			if (isAction)
			{
				isUpCard = true;
				m_cpArmatureCard->setPositionY(m_cpArmatureCard->getPositionY() + 30);
			}
		}
		else
		{
			isUpCard = false;
			m_cpArmatureCard->setPositionY(m_cpArmatureCard->getPositionY() - 30);
		}
}
bool Card::getIsUpCard(){
	return isUpCard;
}


#include "Card.h"
#include "DataModel.h"
Card::Card()
{
}
Card::~Card() {
}
void Card::createCard(){
	m_cpArmatureCard = CCArmature::create("AnimationCard");
//	DataModel::sharedDataModel()->getMainScene()->batchCard->addChild(m_cpArmatureCard);
}

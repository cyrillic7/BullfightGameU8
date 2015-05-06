

#include "SeatData.h"
SeatData::SeatData()
{
}
SeatData::~SeatData() {
}
//重置数据
void SeatData::resetData(){
	allJettonCount=0;
	hideAllJettonLabel();
	hideUserJettonLabel();
}
//设置所有筹码
void SeatData::setAllJettonByAdd(long long lValue){
	allJettonCount+=lValue;
	if (allJettonCount/10000>0)
	{
		pLAllJetton->setStringValue(CCString::createWithFormat("%lld",allJettonCount)->getCString());
		pLAllJetton->setPositionX(-pIFontWan->getContentSize().width/2);
		pIFontWan->setPositionX(pLAllJetton->getContentSize().width/2+pIFontWan->getContentSize().width/2);
		pIFontWan->setVisible(true);
	}else
	{
		pLAllJetton->setStringValue(CCString::createWithFormat("%lld",allJettonCount)->getCString());
		pLAllJetton->setPositionX(0);
		pIFontWan->setVisible(false);
	}
	pLAllJetton->setVisible(true);
}
void SeatData::hideAllJettonLabel(){
	pLAllJetton->setVisible(false);
}
//设置用户筹码
void SeatData::setUserJetton(long long lValue){
	if (lValue/10000>0)
	{
		pLUserJetton->setStringValue(CCString::createWithFormat("%lld",lValue/10000)->getCString());
		pLUserJetton->setPositionX(-pIUserFontWan->getContentSize().width/2);
		pIUserFontWan->setPositionX(pLUserJetton->getContentSize().width/2+pIUserFontWan->getContentSize().width/2);
		pIUserFontWan->setVisible(true);
	}else
	{
		pLUserJetton->setStringValue(CCString::createWithFormat("%lld",lValue)->getCString());
		pLUserJetton->setPositionX(0);
		pIUserFontWan->setVisible(false);
	}
	if (lValue==0)
	{
		pLUserJetton->setVisible(false);
	}else
	{
		pLUserJetton->setVisible(true);
	}
}
//隐藏用户筹码标签
void SeatData::hideUserJettonLabel(){
	pLUserJetton->setVisible(false);
	pIUserFontWan->setVisible(false);
}
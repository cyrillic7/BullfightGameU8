

#include "PlayerData.h"
#include "Tools.h"
#include "DataModel.h"
PlayerData::PlayerData()
{
}
PlayerData::~PlayerData() {
}
//隐藏用户
void PlayerData::hidePlayer(){
	pIPlayerIcon->setVisible(false);
	pLUserName->setVisible(false);
	pLGoldCount->setVisible(false);
}
//设置为庄家
void PlayerData::setBankIcon(){
	pIBankIcon->setScale(10);
	pIBankIcon->setVisible(true);

	CCScaleTo *sTo=CCScaleTo::create(0.2,1,1);
	pIBankIcon->runAction(CCEaseBackOut::create(sTo));
}
//设置用户信息
void PlayerData::setUserInfo(tagUserInfo userInfo){
	memcpy(&userInfoPlayer,&userInfo,sizeof(tagUserInfo));
	pIPlayerIcon->setVisible(true);
	if (Tools::GBKToUTF8(userInfoPlayer.szNickName))
	{
		pLUserName->setVisible(true);
		pLUserName->setText(Tools::GBKToUTF8(userInfoPlayer.szNickName));
	}
	pLGoldCount->setVisible(true);
	pLGoldCount->setText(CCString::createWithFormat("%lld",userInfoPlayer.lScore)->getCString());
}
//显示结算动画
void PlayerData::showResultAnimation(long long lGameScore){
	changePlayerGole(lGameScore);

	pLResult->setVisible(true);
	long long lScore=lGameScore;
	if (lGameScore>0)
	{
		pLResult->setColor(ccc3(0,208,29));
	}else
	{
		lScore=lScore*-1;
		pLResult->setColor(ccc3(255,0,0));
	}
	pLResult->setStringValue(CCString::createWithFormat("%s%lld",lGameScore>0?":":";",lScore)->getCString());
	pLResult->setPositionY(27);
	//pLResult[iPanelIndex]->setPositionPercent(ccp(0,27));
	float time=1;
	CCSpawn *spa=CCSpawn::create(CCFadeIn::create(time),CCMoveBy::create(time,ccp(0,40)),NULL);
	CCSpawn *spa1=CCSpawn::create(CCFadeOut::create(time),NULL);
	CCSequence *seq=CCSequence::create(spa,CCScaleTo::create(1,1),spa1,
			CCCallFunc::create(this,SEL_CallFunc(&PlayerData::onResultAnimationFinish)),NULL);
	pLResult->runAction(seq);
}
void PlayerData::onResultAnimationFinish(){
	if(DataModel::sharedDataModel()->getMainScene()->getGameState()==MainScene::STATE_GAME_END){
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_READY);
	}
}
//设置用户金币
void PlayerData::changePlayerGole(long long lGold){
	userInfoPlayer.lScore+=lGold;
	pLGoldCount->setText(CCString::createWithFormat("%lld",userInfoPlayer.lScore)->getCString());
}
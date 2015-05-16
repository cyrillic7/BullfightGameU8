

#include "PlayerData.h"
#include "Tools/Tools.h"
#include "Tools/DataModel.h"
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
	pIActionTypeBg->setVisible(false);
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
		std::string nickName=Tools::GBKToUTF8(userInfoPlayer.szNickName);
		pLUserName->setText(Tools::subUTF8(nickName, 0, 4));
	}
	pLGoldCount->setVisible(true);
	pLGoldCount->setText(CCString::createWithFormat("%lld",userInfoPlayer.lScore)->getCString());
}
//显示结算动画
void PlayerData::showResultAnimation(long long lGameScore){
	changePlayerGole(lGameScore);

	pLResult->setVisible(true);
	long long lScore=lGameScore;
	if (lGameScore>=0)
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
	if(DataModel::sharedDataModel()->getMainSceneOxTwo()->getGameState()==MainSceneOxTwo::STATE_GAME_END){
		DataModel::sharedDataModel()->getMainSceneOxTwo()->setGameStateWithUpdate(MainSceneOxTwo::STATE_READY);
	}
}
//设置用户金币
void PlayerData::changePlayerGole(long long lGold){
	userInfoPlayer.lScore+=lGold;
	pLGoldCount->setText(CCString::createWithFormat("%lld",userInfoPlayer.lScore)->getCString());
}
void PlayerData::showActionType(ActionType type){
	switch (type)
	{
	case PlayerData::ACTION_READY:
		pIActionContent->loadTexture("u_gi_ready.png",UI_TEX_TYPE_PLIST);
		break;
	case PlayerData::ACTION_CALL_BANK:
		pIActionContent->loadTexture("u_gi_call_bank.png",UI_TEX_TYPE_PLIST);
		break;
	case PlayerData::ACTION_NOT_CALL:
		pIActionContent->loadTexture("u_gi_not_call.png",UI_TEX_TYPE_PLIST);
		break;
	default:
		break;
	}
	pIActionTypeBg->setVisible(true);
}
void PlayerData::hideActionType(){
	pIActionTypeBg->setVisible(false);
}
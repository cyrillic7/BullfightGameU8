

#include "PlayerDataHundred.h"
#include "../../Tools/Tools.h"
#include "../../Tools/DataModel.h"
#include "../../Tools/SoundConfig.h"
PlayerDataHundred::PlayerDataHundred()
{
}
PlayerDataHundred::~PlayerDataHundred() {
    
}
//隐藏用户
void PlayerDataHundred::hidePlayer(){
	//pIPlayerIcon->setVisible(false);
	pIPlayerBg->setVisible(false);
	//pLGoldCount->setVisible(false);
	//pIActionTypeBg->setVisible(false);
}/*
//设置为庄家
void PlayerDataHundred::setBankIcon(){
	pIBankIcon->setScale(10);
	pIBankIcon->setVisible(true);

	CCScaleTo *sTo=CCScaleTo::create(0.2,1,1);
	pIBankIcon->runAction(CCEaseBackOut::create(sTo));
}*/
//设置用户信息
void PlayerDataHundred::setUserInfo(tagUserInfo userInfo){
	memcpy(&userInfoPlayer,&userInfo,sizeof(tagUserInfo));

	if (!pIPlayerBg->isVisible())
	{
		userEnterEffect(pIPlayerBg->getWorldPosition(), 0);
	}

	pIPlayerBg->setVisible(true);
	if (Tools::GBKToUTF8(userInfoPlayer.szNickName).c_str())
	{
		pLUserName->setVisible(true);
		std::string nickName=Tools::GBKToUTF8(userInfoPlayer.szNickName);
		pLUserName->setText(Tools::subUTF8(nickName, 0, 4));
	}
	//pLGoldCount->setVisible(true);
	pLGoldCount->setText(formatNumber(CCString::createWithFormat("%lld",userInfoPlayer.lScore)->getCString()));
}
//显示结算动画
void PlayerDataHundred::showResultAnimation(){
	//changePlayerGole(lGameScore);

	pLResult->setVisible(true);
	long long lScore=lGameScore;
	if (lGameScore>0)
	{
		pLResult->setColor(ccc3(255,253,38));
		if (userInfoPlayer.dwUserID==DataModel::sharedDataModel()->userInfo->dwUserID)
		{
			Tools::playSound(kSoundHundredEndWin);
		}
	}else if (lGameScore==0)
	{
		pLResult->setColor(ccc3(255, 253, 38));
		if (userInfoPlayer.dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
		{
			Tools::playSound(kSoundHundredEndDraw);
		}
	}
	else
	{
		lScore=lScore*-1;
		pLResult->setColor(ccc3(255,255,255));
		if (userInfoPlayer.dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
		{
			Tools::playSound(kSoundHundredEndLost);
		}
	}
	pLResult->setStringValue(CCString::createWithFormat("%s%lld",lGameScore>=0?":":";",lScore)->getCString());
	pLResult->setPositionY(-28);

	float time=1;
	CCSpawn *spa=CCSpawn::create(CCFadeIn::create(time),CCMoveBy::create(time,ccp(0,40)),NULL);
	CCSpawn *spa1=CCSpawn::create(CCFadeOut::create(time),NULL);
	CCSequence *seq=CCSequence::create(spa,
		//CCScaleTo::create(1,1),
		CCDelayTime::create(2),
		spa1,
			CCCallFunc::create(this,SEL_CallFunc(&PlayerDataHundred::onResultAnimationFinish)),NULL);
	pLResult->runAction(seq);
}

void PlayerDataHundred::onResultAnimationFinish(){
	if(DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState()==MainSceneOxHundred::STATE_GAME_SHOW_CARE_FINISH){
		DataModel::sharedDataModel()->getMainSceneOxHundred()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_END);
	}
}

//设置用户金币
void PlayerDataHundred::changePlayerGold(long long lGold){
	//userInfoPlayer.lScore+=lGold;
	pLGoldCount->setText(formatNumber(CCString::createWithFormat("%lld",lGold)->getCString()));
}
/*
void PlayerDataHundred::showActionType(ActionType type){
	switch (type)
	{
	case PlayerDataHundred::ACTION_READY:
		pIActionContent->loadTexture("u_gi_ready.png",UI_TEX_TYPE_PLIST);
		break;
	case PlayerDataHundred::ACTION_CALL_BANK:
		pIActionContent->loadTexture("u_gi_call_bank.png",UI_TEX_TYPE_PLIST);
		break;
	case PlayerDataHundred::ACTION_NOT_CALL:
		pIActionContent->loadTexture("u_gi_not_call.png",UI_TEX_TYPE_PLIST);
		break;
	default:
		break;
	}
	pIActionTypeBg->setVisible(true);
}
void PlayerDataHundred::hideActionType(){
	pIActionTypeBg->setVisible(false);
}*/
//用户进入效果动画
void PlayerDataHundred::userEnterEffect(CCPoint pos, int tag){
	CCArmature *pAnimate = CCArmature::create("AnimationGameIng");
	this->addChild(pAnimate, 100);

	pAnimate->setPosition(pos);
	pAnimate->setTag(tag);

	pAnimate->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PlayerDataHundred::onAnimationEventOver));//动画播完回调用
	pAnimate->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(PlayerDataHundred::onAnimationEventFrame));

	pAnimate->getAnimation()->play("enterChair");
}
void PlayerDataHundred::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
	switch (movementType)
	{
	case cocos2d::extension::COMPLETE:
	case cocos2d::extension::LOOP_COMPLETE:
	{
		pArmature->removeFromParentAndCleanup(true);
	}
	break;
	default:
		break;
	}
}
void PlayerDataHundred::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	if (strcmp(evt, "userEnter") == 0)
	{

		//pPlayerData[bone->getArmature()->getTag()]->pIPlayerIcon->setVisible(true);
	}
}
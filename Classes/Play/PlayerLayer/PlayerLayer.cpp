

#include "PlayerLayer.h"
#include "../../Tools/GameConfig.h"
#include "../../Tools/DataModel.h"
#include "../../Tools/Tools.h"
#include "../../MainScene/MainSceneBase.h"
PlayerLayer::PlayerLayer()
{
}
PlayerLayer::~PlayerLayer() {
}
void PlayerLayer::onEnter(){
	CCLayer::onEnter();
	initPlayerInfo();
}
void PlayerLayer::onExit(){

    
}
void PlayerLayer::initPlayerInfo(){
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);

	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPalyer.ExportJson)));
	m_pWidget->addWidget(pWidget);

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pPlayerData[i]=PlayerData::create();
		this->addChild(pPlayerData[i]);
		pPlayerData[i]->pPlayerPanel = static_cast<UIPanel*>(m_pWidget->getWidgetByName(CCString::createWithFormat("player%d",i)->getCString()));
		//结算数字
		pPlayerData[i]->pLResult=static_cast<UILabelAtlas*>(pPlayerData[i]->pPlayerPanel->getChildByName("AtlasLabelResult"));
		pPlayerData[i]->pLResult->setVisible(false);
		//用户昵称
		pPlayerData[i]->pLUserName=static_cast<UILabel*>(pPlayerData[i]->pPlayerPanel->getChildByName("userName"));
		//用户金币
		pPlayerData[i]->pLGoldCount=static_cast<UILabel*>(pPlayerData[i]->pPlayerPanel->getChildByName("goldCount"));
		//庄家标识
		pPlayerData[i]->pIBankIcon=static_cast<UIImageView*>(pPlayerData[i]->pPlayerPanel->getChildByName("ImageBankIcon"));
		pPlayerData[i]->pIBankIcon->setVisible(false);
		//操作者光标
		pPlayerData[i]->pILight=static_cast<UIImageView*>(pPlayerData[i]->pPlayerPanel->getChildByName("ImageLight"));
		pPlayerData[i]->pILight->setVisible(false);
		//用户头像
		pPlayerData[i]->pIPlayerIcon=static_cast<UIImageView*>(pPlayerData[i]->pPlayerPanel->getChildByName("headPortrait"));
		//行为类型
		pPlayerData[i]->pIActionTypeBg=static_cast<UIImageView*>(pPlayerData[i]->pPlayerPanel->getChildByName("ImageAction"));
		//行为内容
		pPlayerData[i]->pIActionContent=static_cast<UIImageView*>(pPlayerData[i]->pPlayerPanel->getChildByName("ImageAction")->getChildByName("ImageActionContent"));
		//隐藏用户
		pPlayerData[i]->hidePlayer();

		doSetCardMovePos(i);
	}
}
//设置牌位置
void PlayerLayer::doSetCardMovePos(int i){
	UIPanel *playerPanel = pPlayerData[i]->pPlayerPanel;
	UIImageView *iPlayerIcon = (UIImageView*)playerPanel->getChildByName("headPortrait");
	CCPoint playerPos = playerPanel->getPosition();
	CCPoint cardPos = ccpAdd(playerPos, iPlayerIcon->getPosition());
	getMainScene()->posChair[i] = cardPos;
}
void PlayerLayer::hideOxTwoPlayer(){
	hidePlayer(pPlayerData[1]->pPlayerPanel);
	hidePlayer(pPlayerData[2]->pPlayerPanel);
	hidePlayer(pPlayerData[4]->pPlayerPanel);
	hidePlayer(pPlayerData[5]->pPlayerPanel);

	pPlayerData[0]->pPlayerPanel->setPosition(designResolutionToFrame(ccp(785, 506)));
	UIPanel *playerPanel = pPlayerData[0]->pPlayerPanel;
	UIImageView *iPlayerIcon = (UIImageView*)playerPanel->getChildByName("headPortrait");
	CCPoint playerPos = playerPanel->getPosition();
	CCPoint cardPos = ccpAdd(playerPos, iPlayerIcon->getPosition());
	getMainScene()->posChair[0] = cardPos;
}
void PlayerLayer::resetPlayerData(){
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pPlayerData[i]->pIBankIcon->setVisible(false);
		pPlayerData[i]->pILight->setVisible(false);
	}
}
void PlayerLayer::hidePlayer(UIPanel *panel){
	panel->setVisible(false);
}
void PlayerLayer::updateState(){
	switch (getMainScene()->getGameState())
	{
	case MainSceneBase::STATE_READY:
		{
			resetPlayerData();
		}
		break;
	default:
		break;
	}
}
//显示结算动画
void PlayerLayer::showResultAnimation(int iPanelIndex,long long lGameScore){
	pPlayerData[iPanelIndex]->showResultAnimation(lGameScore);
}

//设置用户信息
void PlayerLayer::setUserInfo(int panelIndex,tagUserInfo tagUser){
	if (!pPlayerData[panelIndex]->pIPlayerIcon->isVisible())
	{
		userEnterEffect(pPlayerData[panelIndex]->pIPlayerIcon->getWorldPosition(), panelIndex);
	}
	pPlayerData[panelIndex]->setUserInfo(tagUser);
}
void PlayerLayer::setUserGold(int panelIndex,long long goldCount){
	//DataModel::sharedDataModel()->
}
void PlayerLayer::setBankLight(int userIndex){
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (i==userIndex)
			{
				pPlayerData[i]->pILight->setVisible(true);
			}else
			{
				pPlayerData[i]->pILight->setVisible(false);
			}
		}
}
void PlayerLayer::setBankIcon(int bankIndex){
	setBankLight(bankIndex);

	pPlayerData[bankIndex]->setBankIcon();
}
MainSceneBase*PlayerLayer::getMainScene(){
	return (MainSceneBase*)this->getParent();
}

//用户进入效果动画
void PlayerLayer::userEnterEffect(CCPoint pos,int tag){
	CCArmature *pAnimate = CCArmature::create("AnimationGameIng");
	this->addChild(pAnimate,100);
	
	pAnimate->setPosition(pos);
	pAnimate->setTag(tag);

	pAnimate->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PlayerLayer::onAnimationEventOver));//动画播完回调用
	pAnimate->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(PlayerLayer::onAnimationEventFrame));

	pAnimate->getAnimation()->play("enterChair");
}
void PlayerLayer::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
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
void PlayerLayer::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	if (strcmp(evt, "userEnter") == 0)
	{
		
		//pPlayerData[bone->getArmature()->getTag()]->pIPlayerIcon->setVisible(true);
	}
}
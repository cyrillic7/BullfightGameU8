

#include "PlayerLayer.h"
#include "GameConfig.h"
#include "DataModel.h"
PlayerLayer::PlayerLayer()
{
}
PlayerLayer::~PlayerLayer() {
}
void PlayerLayer::onEnter(){
	CCLayer::onEnter();
	initPlayerInfo();
	hidePlayer(playerPanel[1]);
	hidePlayer(playerPanel[2]);
	hidePlayer(playerPanel[4]);
	hidePlayer(playerPanel[5]);
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
		playerPanel[i] = static_cast<UIPanel*>(m_pWidget->getWidgetByName(CCString::createWithFormat("player%d",i)->getCString()));
		//结算数字
		pLResult[i]=static_cast<UILabelAtlas*>(playerPanel[i]->getChildByName("AtlasLabelResult"));
		pLResult[i]->setVisible(false);
		//用户昵称
		pLUserName[i]=static_cast<UILabel*>(playerPanel[i]->getChildByName("userName"));
		//庄家标识
		pIBankIcon[i]=static_cast<UIImageView*>(playerPanel[i]->getChildByName("ImageBankIcon"));
		pIBankIcon[i]->setVisible(false);
		//操作者光标
		pILight[i]=static_cast<UIImageView*>(playerPanel[i]->getChildByName("ImageLight"));
		pILight[i]->setVisible(false);
	}
}
void PlayerLayer::resetPlayerData(){
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pIBankIcon[i]->setVisible(false);
	}
}
void PlayerLayer::getPlayerInfo(){

}
void PlayerLayer::hidePlayer(UIPanel *panel){
	panel->setVisible(false);
}
void PlayerLayer::updateState(){
	switch (DataModel::sharedDataModel()->getMainScene()->getGameState())
	{
	case MainScene::STATE_READY:
		{
			resetPlayerData();
		}
		break;
	default:
		break;
	}

}
void PlayerLayer::showResultAnimation(int iPanelIndex,long long lGameScore){
	pLResult[iPanelIndex]->setVisible(true);
	long long lScore=lGameScore;
	if (lGameScore>0)
	{
		pLResult[iPanelIndex]->setColor(ccc3(0,208,29));
	}else
	{
		lScore=lScore*-1;
		pLResult[iPanelIndex]->setColor(ccc3(255,0,0));
	}
	pLResult[iPanelIndex]->setStringValue(CCString::createWithFormat("%s%lld",lGameScore>0?":":";",lScore)->getCString());
	pLResult[iPanelIndex]->setPositionY(27);
	//pLResult[iPanelIndex]->setPositionPercent(ccp(0,27));
	float time=1;
	CCSpawn *spa=CCSpawn::create(CCFadeIn::create(time),CCMoveBy::create(time,ccp(0,40)),NULL);
	CCSpawn *spa1=CCSpawn::create(CCFadeOut::create(time),NULL);
	CCSequence *seq=CCSequence::create(spa,CCScaleTo::create(1,1),spa1,
		CCCallFunc::create(this,SEL_CallFunc(&PlayerLayer::resultAnimationFinish)),NULL);
	pLResult[iPanelIndex]->runAction(seq);
}
void PlayerLayer::resultAnimationFinish(){
	if(DataModel::sharedDataModel()->getMainScene()->getGameState()==MainScene::STATE_GAME_END){
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_READY);
	}
}
void PlayerLayer::setUserName(int panelIndex,const char *name){
	pLUserName[panelIndex]->setText(name);
}
void PlayerLayer::setBankLight(int userIndex){
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (i==userIndex)
			{
				pILight[i]->setVisible(true);
			}else
			{
				pILight[i]->setVisible(false);
			}
		}
}
void PlayerLayer::setBankIcon(int bankIndex){
	setBankLight(bankIndex);

	pIBankIcon[bankIndex]->setScale(10);
	pIBankIcon[bankIndex]->setVisible(true);
	
	CCScaleTo *sTo=CCScaleTo::create(0.2,1,1);
	pIBankIcon[bankIndex]->runAction(CCEaseBackOut::create(sTo));
}
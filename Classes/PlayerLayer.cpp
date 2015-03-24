

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
	hidePlayer(playerPanel[0]);
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
	}
}
void PlayerLayer::getPlayerInfo(){

}
void PlayerLayer::hidePlayer(UIPanel *panel){
	panel->setVisible(false);
}
void PlayerLayer::updateState(){


}
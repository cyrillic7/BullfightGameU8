//
//
//
//

#include "GameControlOxHundred.h"
#include "GameConfig.h"
#include "Tools.h"
#include "GameLobbyScene.h"
GameControlOxHundred::GameControlOxHundred()
{

}
GameControlOxHundred::~GameControlOxHundred(){

}
void GameControlOxHundred::onEnter(){
	CCLayer::onEnter();
	UILayer *pWidget = UILayer::create();
	this->addChild(pWidget);

	UILayout *pLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameIngHundred.ExportJson)));
	pWidget->addWidget(pLayout);

	UIButton* button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonBack"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuBack));
}
void GameControlOxHundred::onExit(){
	BaseGameControl::onExit();
}
int GameControlOxHundred::getChairIndex(int meChairID,int chairID){
	if (meChairID==0)
	{
		if (chairID==0)
		{
			return 3;
		}
	}else
	{
		if (chairID==1)
		{
			return 3;
		}
	}
	return 0;
}
//菜单回调（返回）
void GameControlOxHundred::onMenuBack(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene());
		}
		break;
	default:
		break;
	}
}
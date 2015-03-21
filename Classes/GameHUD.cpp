//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameHUD.h"
#include "GameConfig.h"
#include "DataModel.h"
void GameHUD::onEnter(){
	CCLayer::onEnter();
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameHUD.ExportJson)));
	m_pWidget->addWidget(pWidget);
	
	UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonPause"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameHUD::menuPause));

	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonOx"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameHUD::menuCancel));
	button->setEnabled(false);

	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonPrompt"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameHUD::menuPrompt));
	button->setEnabled(false);
	//绑定准备按键
	bReady = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonReady"));
	bReady->addTouchEventListener(this, SEL_TouchEvent(&GameHUD::menuReady));
	
}
void GameHUD::onExit(){
	CCLayer::onExit();
}
void GameHUD::menuPause(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		CCLog("===================");
		break;
	default:
		break;
	}
}
void GameHUD::menuCancel(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		CCLog("cancel");
		break;
	default:
		break;
	}
}
void GameHUD::menuPrompt(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		//CCLog("prompt");
		break;
	default:
		break;
	}
}
//准备按键
void GameHUD::menuReady(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		//设置主状态为准备状态
		DataModel::sharedDataModel()->getMainScene()->setGameState(MainScene::STATE_READY);
		//处理按键
		UIButton *mButton=(UIButton*)pSender;
		mButton->setBright(false);
		mButton->setTouchEnabled(false);
		//获取按键子控件并隐藏
		CCArray *arrayImage = mButton->getChildren();
		for (int i = 0; i < arrayImage->count(); i++)
		{
			CCNode *image = (CCNode *)arrayImage->objectAtIndex(i);
			image->setVisible(false);
		}
		DataModel::sharedDataModel()->getMainScene()->onEventReadyFnish();
	}
		break;
	default:
		break;
	}
}
void GameHUD::updateState(){
	switch (DataModel::sharedDataModel()->getMainScene()->getGameState())
	{
	case MainScene::STATE_SEND_CARD:
	{
		bReady->setVisible(false);
	}
		break;
	default:
		break;
	}
}
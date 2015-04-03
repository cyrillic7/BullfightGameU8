//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControl.h"
#include "GameConfig.h"
#include "DataModel.h"
#include "GameLobbyScene.h"
#include "Tools.h"
#include "TCPSocketControl.h"
#include "Packet.h"
#include "CMD_GameServer.h"
void GameControl::onEnter(){
	CCLayer::onEnter();
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameHUD.ExportJson)));
	m_pWidget->addWidget(pWidget);
	
	UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonPause"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuPause));

	//设置牛牛容器
	pOptOx = static_cast<UIPanel*>(m_pWidget->getWidgetByName("optOxPanel"));
	pOptOx->setEnabled(false);

	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonOx"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuCancel));
	
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonPrompt"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuPrompt));
	
	//绑定准备按键
	bReady = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonReady"));
	bReady->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuReady));
	//抢庄容器
	pFightForBanker = static_cast<UIPanel*>(m_pWidget->getWidgetByName("fightForBankerPanel"));
	pFightForBanker->setEnabled(false);
	//不抢庄
	button = static_cast<UIButton*>(pFightForBanker->getChildByName("notFightButton"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuNotFight));
	//抢庄
	button = static_cast<UIButton*>(pFightForBanker->getChildByName("fightButton"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuFight));
	//投注容器
	pBetting=static_cast<UIPanel*>(m_pWidget->getWidgetByName("bettingPanel"));
	pBetting->setEnabled(false);
	for (int i = 0; i < 4; i++)
	{
		button = static_cast<UIButton*>(pBetting->getChildByName("bgImage")->getChildByName(CCString::createWithFormat("bettingButton%d",i+1)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuBetting));
	}
	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameControl::onCallBanker),LISTENER_GAMEING,NULL);
}
void GameControl::onExit(){
	CCLayer::onExit();
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LISTENER_GAMEING); 
}
void GameControl::menuPause(CCObject* pSender, TouchEventType type){
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
void GameControl::menuCancel(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_SETTLE_ACCOUNFS);
	}
		break;
	default:
		break;
	}
}
void GameControl::menuPrompt(CCObject* pSender, TouchEventType type){
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
void GameControl::menuReady(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		
		/*CMD_GR_UserSitDown sit;
		sit.wTableID=28;
		sit.wChairID=1;*/

		bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_FRAME,SUB_GF_USER_READY);
		//bool isSend=tcpID->ts.SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
		CCLog("ready send:%d",isSend);
		//DataModel::sharedDataModel()->getMainScene()->stopTcpSocket();
		
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
//不抢庄
void GameControl::menuNotFight(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		pFightForBanker->setEnabled(false);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_BETTING);
	}
		break;
	default:
		break;
	}
}
//抢庄
void GameControl::menuFight(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		pFightForBanker->setEnabled(false);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
	}
		break;
	default:
		break;
	}
}
//投注
void GameControl::menuBetting(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		pBetting->setEnabled(false);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
	}
		break;
	default:
		break;
	}
}
void GameControl::updateState(){
	switch (DataModel::sharedDataModel()->getMainScene()->getGameState())
	{
	case MainScene::STATE_CALL_BANKER:
	{
		bReady->setVisible(false);
		pFightForBanker->setEnabled(true);
	}
		break;
	case MainScene::STATE_OPT_OX:
	{
		pFightForBanker->setEnabled(false);
		pOptOx->setEnabled(true);
	}
		break;
	case MainScene::STATE_BETTING:
	{
		pBetting->setEnabled(true);
	}
		break;
	case MainScene::STATE_SETTLE_ACCOUNFS:
	{
		pOptOx->setEnabled(false);
	}
		break;
	default:
		break;
	}
}
void GameControl::onCallBanker(CCObject *obj){
	DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_CALL_BANKER);
}
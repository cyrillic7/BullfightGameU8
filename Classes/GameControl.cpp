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
#include "cmd_ox.h"
#include "QueueData.h"
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
		pbBetting[i] = static_cast<UIButton*>(pBetting->getChildByName("bgImage")->getChildByName(CCString::createWithFormat("bettingButton%d",i+1)->getCString()));
		pbBetting[i]->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuBetting));
	}
	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameControl::onCallBanker),LISTENER_CALL_BANKER,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameControl::onAddScore),LISTENER_ADD_SCORE,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameControl::onSendCard),LISTENER_SEND_CARD,NULL);

}
void GameControl::onExit(){
	CCLayer::onExit();
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LISTENER_CALL_BANKER); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LISTENER_ADD_SCORE); 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LISTENER_SEND_CARD); 
}
void GameControl::menuPause(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		TCPSocketControl::sharedTCPSocketControl()->stopSocket();
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
		/*//发送消息
		CMD_C_OxCard OxCard;
		OxCard.bOX=(m_GameClientView.m_CardControl[wViewChairID].GetOX())?TRUE:FALSE;
		SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
		//DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_SETTLE_ACCOUNFS);*/

		CMD_C_OxCard OxCard;
		OxCard.bOX=DataModel::sharedDataModel()->gameLogic->GetOxCard(DataModel::sharedDataModel()->card[1],5);
		//发送信息
		bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
		CCLog("OxCard:%d",isSend);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_WAIT);
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
		//DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_BETTING);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_WAIT);

		//设置变量
		CMD_C_CallBanker CallBanker;
		CallBanker.bBanker = (BYTE)0;

		//发送信息
		TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		//SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));
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
		//DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_WAIT);

		//设置变量
		CMD_C_CallBanker CallBanker;
		CallBanker.bBanker = (BYTE)1;

		//发送信息
		TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		//SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));
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
		UIButton *button=(UIButton*)pSender;
		int bTemp=button->getTag();
		long long lCurrentScore=0;
		if(bTemp==1)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/8,1L);
		else if(bTemp==2)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/4,1L);
		else if(bTemp==3)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/2,1L);
		else if(bTemp==4)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore,1L);

		//DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
		DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_WAIT);

		//发送消息
		CMD_C_AddScore AddScore;
		AddScore.lScore=lCurrentScore;
		//发送信息
		TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
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
void GameControl::onAddScore(CCObject *obj){
	for (int i = 0; i < 4; i++)
	{
		long long lCurrentScore=0;
		if(i==0)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/8,1L);
		else if(i==1)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/4,1L);
		else if(i==2)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/2,1L);
		else if(i==3)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore,1L);
		pbBetting[i]->setTitleText(CCString::createWithFormat("%lld",lCurrentScore)->getCString());
	}
	QueueData * pQueueStart=(QueueData*)obj;
	CMD_S_GameStart *pGameStart=(CMD_S_GameStart*)pQueueStart->pDataBuffer;
	CCLog("maxScore:%lld",pGameStart->lTurnMaxScore);

	DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_BETTING);

	CC_SAFE_DELETE(pQueueStart->pDataBuffer);
	CC_SAFE_DELETE(pQueueStart);
}
void GameControl::onSendCard(CCObject *obj){
	//DataModel::sharedDataModel()->getMainScene()->cardLayer
	DataModel::sharedDataModel()->getMainScene()->setServerStateWithUpdate(MainScene::STATE_SEND_CARD);
}

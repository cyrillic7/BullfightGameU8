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
#include "SEvent.h"
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
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuOpenCard));
	
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonPrompt"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuPrompt));
	
	//绑定准备按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonReady"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuReady));
	//准备容器
	pPanelReady=static_cast<UIPanel*>(m_pWidget->getWidgetByName("PanelReady"));
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
		pbBetting[i] = static_cast<UIButton*>(pBetting->getChildByName(CCString::createWithFormat("bettingButton%d",i+1)->getCString()));
		pbBetting[i]->addTouchEventListener(this, SEL_TouchEvent(&GameControl::menuBetting));
	}
	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameControl::OnEventGameMessage),S_L_GAME_ING,NULL);
}
void GameControl::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_GAME_ING); 
	CCLayer::onExit();
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
//开牌
void GameControl::menuOpenCard(CCObject* pSender, TouchEventType type){
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
		OxCard.bOX=GetOxCard(DataModel::sharedDataModel()->card[1],5);
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
//提示
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
		//隐藏准备
		pPanelReady->setEnabled(false);
		//发送准备指使
		bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_FRAME,SUB_GF_USER_READY);
		//设置主状态为准备状态
		DataModel::sharedDataModel()->getMainScene()->setGameState(MainScene::STATE_READY);
		/*//获取按键子控件并隐藏
		CCArray *arrayImage = mButton->getChildren();
		for (int i = 0; i < arrayImage->count(); i++)
		{
			CCNode *image = (CCNode *)arrayImage->objectAtIndex(i);
			image->setVisible(false);
		}*/
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
		pPanelReady->setEnabled(false);
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//网络消息
void GameControl::OnEventGameMessage(CCObject *pObj){
	QueueData *pData=(QueueData*)pObj;
	switch (pData->wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//用户叫庄
		{
			//消息处理
			OnSubCallBanker(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case SUB_S_GAME_START:	//游戏开始
		{
			//消息处理
			OnSubGameStart(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case SUB_S_ADD_SCORE:	//用户下注
		{
			//消息处理
			OnSubAddScore(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case SUB_S_SEND_CARD:	//发牌消息
		{
			//消息处理
			OnSubSendCard(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case SUB_S_OPEN_CARD:	//用户摊牌
		{
			//消息处理
			OnSubOpenCard(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case SUB_S_PLAYER_EXIT:	//用户强退
		{
			//消息处理
			OnSubPlayerExit(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case SUB_S_GAME_END:	//游戏结束
		{
			//结束动画
			//m_GameClientView.FinishDispatchCard();

			//消息处理
			OnSubGameEnd(pData->pDataBuffer,pData->wDataSize);
		}
		break;
	case 1024:
		{
			CCLog("1024------------------");
			//m_GameClientView.OnLockGame(pData->pDataBuffer,pData->wDataSize);
			//return true;
		}
		break;
	default:
		CCLog("--------------------gamt");
		break;
	}
	if (pData)
	{
		delete pData;
		pData=NULL;
	}
	//CC_SAFE_DELETE(pData);
}
//用户叫庄
bool GameControl::OnSubCallBanker(const void * pBuffer, WORD wDataSize){
	//效验数据 
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;
	if (pCallBanker->bFirstTimes)
	{
		if (pCallBanker->wCallBanker==1)
		{
			CCLog("首次自己叫庄");
			DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_CALL_BANKER);
		}else
		{
			CCLog("首次对方叫庄");
		}
	}else
	{
		if (pCallBanker->wCallBanker==1)
		{
			CCLog("自己叫庄");
			DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_CALL_BANKER);
		}else
		{
			CCLog("对方叫庄");
		}
	}
	/*//游戏当前处于叫庄状态
	//SetGameStatus(GS_TK_CALL);
	if(!pCallBanker->bFirstTimes && pCallBanker->wCallBanker==GetMeChairID())
	{
		//InsertSystemString("推庄");
		//PlayGameSound(AfxGetInstanceHandle(),_T("PUSH_BANK"));
	}
	
	//用户信息
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//视图位置
		m_wViewChairID[i]=SwitchViewChairID(i);

		//获取用户
		IClientUserItem * pUserData=GetTableUserItem(i);
		if (pUserData==NULL) continue;

		//游戏信息
		m_cbPlayStatus[i]=TRUE;

		//用户名字
		lstrcpyn(m_szNickName[i],pUserData->GetNickName(),CountArray(m_szNickName[i]));
		m_GameClientView.m_ptJetonMove[i].SetPoint(0,0);
	}

	//旁观者
	if (IsLookonMode())
	{
		//设置界面
		m_GameClientView.ViewDisplayType(false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		//m_GameClientView.SetScoreInfo(0,0);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.ShowScore(i,false);
			m_GameClientView.m_CardControl[i].SetOX(false);
			m_GameClientView.SetUserOxValue(i,0xff);
			m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
			if(i!=m_wViewChairID[GetMeChairID()])m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
			m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
		}

		//加注信息
		m_lTurnMaxScore=0L;
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
		ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));
		ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	}

	//始叫用户
	if(!IsLookonMode() && pCallBanker->wCallBanker==GetMeChairID())
	{
		//控件显示
		//ActiveGameFrame();
		m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btIdler.ShowWindow(SW_SHOW);
	}

	//等待标志
	WORD wViewID=m_wViewChairID[pCallBanker->wCallBanker];
	m_GameClientView.SetWaitCall((BYTE)wViewID);

	//实际定时器
	if(pCallBanker->wCallBanker==GetMeChairID())
	{
		SetGameClock(pCallBanker->wCallBanker,IDI_CALL_BANKER,TIME_USER_CALL_BANKER);
	}
	else SetGameClock(pCallBanker->wCallBanker,IDI_NULLITY,TIME_USER_CALL_BANKER);

	*/
	return true;
}
//游戏开始
bool GameControl::OnSubGameStart(const void * pBuffer, WORD wDataSize){
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//int size=wDataSize;
	CCLog("庄家:%d   最大:::%lld",pGameStart->wBankerUser,pGameStart->lTurnMaxScore);
	//CCLog("游戏开始 ");
	DataModel::sharedDataModel()->m_lTurnMaxScore=pGameStart->lTurnMaxScore;

	DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_BETTING);
	return true;
}

//用户下注
bool GameControl::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	for (int i = 0; i < 4; i++)
	{
		long long lCurrentScore=0;
		if(i==0)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/8,1L);
		else if(i==1)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/4,1L);
		else if(i==2)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore/2,1L);
		else if(i==3)lCurrentScore=MAX(DataModel::sharedDataModel()->m_lTurnMaxScore,1L);
		pbBetting[i]->setTitleText(CCString::createWithFormat("%lld",lCurrentScore)->getCString());
	}
	DataModel::sharedDataModel()->getMainScene()->setGameStateWithUpdate(MainScene::STATE_BETTING);
	/*
	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=m_wViewChairID[wAddScoreUser];

	//加注处理
	//if ((IsLookonMode()==true)||(pAddScore->wAddScoreUser!=wMeChairID))
	{
		//下注财富
		m_lTableScore[pAddScore->wAddScoreUser]=pAddScore->lAddScoreCount;

		//加注界面
		m_GameClientView.SetUserTableScore(wViewChairID,pAddScore->lAddScoreCount);

		//播放声音
		if (m_cbPlayStatus[wAddScoreUser]==TRUE)
		{
			 PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
	}

	//设置筹码
	//m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wAddScoreUser]);
	*/
	return true;
}

//发牌消息
bool GameControl::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	CCLog("发牌");
	
	for (int i = 0; i < 5; i++)
	{
		DataModel::sharedDataModel()->card[1][i]=pSendCard->cbCardData[1][i];
	}
	DataModel::sharedDataModel()->getMainScene()->setServerStateWithUpdate(MainScene::STATE_SEND_CARD);
	/*
	//删除定时器
	KillGameClock(IDI_NULLITY);

	m_GameClientView.SetWaitInvest(false);

	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(m_cbHandCardData));

	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];
	if(IsAllowLookon() || !IsLookonMode())
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
	else
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(false);

	//{修改发牌方式 2010-01-13
	for(WORD i=0;i<MAX_COUNT;i++)
	{
		for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
		{
			WORD w=j%GAME_PLAYER;
			if (m_cbPlayStatus[w]==TRUE)
			{
				WORD wViewChairID=m_wViewChairID[w];
				if(pSendCard->cbCardData[w][i]!=0&&w==GetMeChairID())
				{
					BYTE cbCardData[MAX_COUNT];
					BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

					cbCardData[cbCardCount++]=m_cbHandCardData[w][i];
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
				}else{
					BYTE cbCardData[MAX_COUNT];
					BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

					cbCardData[cbCardCount++]=m_cbHandCardData[w%GAME_PLAYER][i];
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
				}
			}
		}
	}
	OnSendCardFinish(0,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	//修改结束}
	*/

	return true;
}

//用户摊牌
bool GameControl::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;
	/*
	//游戏信息
	WORD wID=pOpenCard->wPlayerID;
	//ASSERT(m_cbPlayStatus[wID]==TRUE);

	//摊牌标志
	if(wID!=GetMeChairID() || IsLookonMode())
	{
		WORD wViewChairID=m_wViewChairID[wID];
		m_GameClientView.ShowOpenCard(wViewChairID);
		//保存牛信息
		m_bUserOxCard[wID]=pOpenCard->bOpen;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
	}
	*/
	return true;
}

//用户强退
bool GameControl::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;
	/*
	WORD wID=pPlayerExit->wPlayerID;

	//游戏信息
	ASSERT(m_cbPlayStatus[wID]==TRUE);
	m_cbPlayStatus[wID]=FALSE;

	//用户名字
	for(WORD i=0;i<32;i++)m_szNickName[wID][i]=0;
	m_szNickName[wID][0] = '#';

	BYTE bCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]!=FALSE)bCount++;
	if(bCount==1)m_GameClientView.SetBankerUser(INVALID_CHAIR);
	*/
	return true;

}

//游戏结束
bool GameControl::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	/*
	CopyMemory(m_cbHandCardData,pGameEnd->cbCardData,sizeof(m_cbHandCardData));

	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (i!=GetMeChairID())
		{
			WORD wViewchair=m_wViewChairID[i];
			m_GameClientView.m_CardControl[wViewchair].SetCardData(NULL,0);
		}
	}

	for(WORD i=0;i<MAX_COUNT;i++)
	{
		for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
		{
			WORD w=j%GAME_PLAYER;
			if (m_cbPlayStatus[w]==TRUE)
			{
				WORD wViewChairID=m_wViewChairID[w];
				if(pGameEnd->cbCardData[w][i]!=0&&w!=GetMeChairID())
				{
					BYTE cbCardData[MAX_COUNT];
					//BYTE cbChangeCardData[MAX_C_COUNT];
					BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

					cbCardData[cbCardCount++]=m_cbHandCardData[w][i];
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
				}
			}
		}
	}

	//删除定时器
	KillGameClock(IDI_NULLITY);
	KillGameClock(IDI_CALL_BANKER);
	KillTimer(IDI_TIME_USER_ADD_SCORE);
	KillTimer(IDI_TIME_OPEN_CARD);

	//清理数据
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.ShowOpenCard(i,FALSE);
	}	
	m_GameClientView.SetWaitInvest(false);
	UpdateScoreControl(NULL,SW_HIDE);
	//m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btReSort.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
	m_GameClientView.SetWaitCall(0xff);
	m_GameClientView.m_bOpenCard=false;

	//状态设置
	SetGameStatus(GAME_STATUS_FREE);

	//播放声音
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameScore[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//显示积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(GetTableUserItem(i)!=NULL)m_GameClientView.ShowScore(m_wViewChairID[i],true);
		m_GameClientView.SetUserTableScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
		m_GameClientView.SetUserTotalScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
	}
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置信息
		if (pGameEnd->lGameScore[i]!=0L || i==m_wBankerUser)
		{
			m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax[i],i);
			if (m_szNickName[i][0]=='#') m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("已离开"),pGameEnd->lGameScore[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,m_szNickName[i],pGameEnd->lGameScore[i]);
		}
	}
	//m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	
	//显示牌型
	bool bOxSound=false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=m_wViewChairID[i];
		if(i==GetMeChairID() && !IsLookonMode())
			continue;
	
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);

		//牛牛牌型
		if(m_cbHandCardData[i][0]>0)
		{
			if(m_bUserOxCard[i]==TRUE)
			{
				//扑克数据
				BYTE bCardData[MAX_COUNT];
				CopyMemory(bCardData,m_cbHandCardData[i],sizeof(bCardData));

				//获取牛牛数据
				bool bOx=m_GameLogic.GetOxCard(bCardData,MAX_COUNT);
				ASSERT(bOx);

				//加载数据
				m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardData,3);
				m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bCardData[3],2);

				//显示点数
				BYTE bValue=m_GameLogic.GetCardLogicValue(bCardData[3])+m_GameLogic.GetCardLogicValue(bCardData[4]);
				if(bValue>10)bValue-=10;
				ASSERT(bValue>0);
				if(bValue>=10)bOxSound=true;
				m_GameClientView.SetUserOxValue(wViewChairID,bValue);
			}
			else
			{
				//无牛
				m_GameClientView.SetUserOxValue(wViewChairID,0);
			}
		}
	}
	m_GameClientView.ViewDisplayType(true);

	//牛牛声音
	if(bOxSound)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_OXOX"));
	}

	//开始按钮
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
	}

	//成绩显示在即时聊天对话框
	TCHAR szBuffer[512]=TEXT("");
	myprintf(szBuffer,CountArray(szBuffer),TEXT("本次对局结果:"));
	m_pIStringMessage->InsertNormalString(szBuffer);
	myprintf(szBuffer,CountArray(szBuffer),TEXT("用户昵称\t成绩"));
	m_pIStringMessage->InsertNormalString(szBuffer);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		IClientUserItem * pUserData=GetTableUserItem(i);
		//成绩输出
		if (pUserData!=NULL)
		{
			if(mystrlen(pUserData->GetNickName())<8)
				myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:\t\t%+d"),pUserData->GetNickName(),pGameEnd->lGameScore[i]);
			else
				myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:\t%+d"),pUserData->GetNickName(),pGameEnd->lGameScore[i]);
			m_pIStringMessage->InsertNormalString(szBuffer);
		}
		else if(m_szNickName[i][0]=='#')
		{
			myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:%+d"),TEXT("用户离开"),pGameEnd->lGameScore[i]);
			m_pIStringMessage->InsertNormalString(szBuffer);
		}
	}
	myprintf(szBuffer,CountArray(szBuffer),TEXT("本局游戏结束。\n--------------------"));
	m_pIStringMessage->InsertNormalString(szBuffer);

	//状态变量
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szNickName,sizeof(m_szNickName));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	*/
	return true;
}
/*void GameControl::onAddScore(CCObject *obj){
	
}
void GameControl::onSendCard(CCObject *obj){
	DataModel::sharedDataModel()->getMainScene()->setServerStateWithUpdate(MainScene::STATE_SEND_CARD);
}*/

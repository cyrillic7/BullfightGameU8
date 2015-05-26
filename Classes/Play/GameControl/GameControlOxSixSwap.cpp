//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControlOxSixSwap.h"
#include "../../Tools/DataModel.h"
#include "../../Network/CMD_Server/CMD_Ox_SixSwap.h"
#include "../../Network/TCPSocket/TCPSocketControl.h"
#include "../../Tools/Tools.h"
GameControlOxSixSwap::GameControlOxSixSwap()
{

}
GameControlOxSixSwap::~GameControlOxSixSwap(){
	DataModel::sharedDataModel()->mTagUserInfo.clear();
}
void GameControlOxSixSwap::onEnter(){
	GameControlBase::onEnter();
}
void GameControlOxSixSwap::onExit(){
	GameControlBase::onExit();
}
int GameControlOxSixSwap::getChairIndex(int meChairID,int chairID){
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
//换牌消息
void GameControlOxSixSwap::onUserChangeCard(int wParam, long lParam)
{
	BYTE bChange = (BYTE)wParam;

	WORD wMeChairID = DataModel::sharedDataModel()->userInfo->wChairID;
	WORD wViewChairID = getViewChairID(wMeChairID);
	BYTE bMyShootCard[MAX_COUNT];
	ZeroMemory(bMyShootCard, sizeof(bMyShootCard));
	CMD_C_ChangeCard ChangeCard;
	ChangeCard.bChange = bChange;
	ChangeCard.wPlayer = wMeChairID;

	if (bChange)
	{
		/*DWORD ShootCount = m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bMyShootCard, true);
		if (ShootCount != 1)
		{

			m_GameClientView.OnChangeTip(true);
			return FALSE;
		}
		else
			m_GameClientView.OnChangeTip(false);
			*/
		ChangeCard.cbChangeCard = bMyShootCard[0];
	}

	//m_GameClientView.m_btChange.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btNoChange.ShowWindow(SW_HIDE);
	bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_CHANGE_CARD, &ChangeCard, sizeof(ChangeCard));
	if (isSend)
	{
		getMainScene()->setGameStateWithUpdate(MainSceneBase::STATE_OPT_OX);
	}
}

//开牌
void GameControlOxSixSwap::menuOpenCard(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		hideTimer(true);
		getMainScene()->cardLayer->sortingOx(getMeChairID(),3);
		showActionPrompt(3);
		pOptOx->setEnabled(false);		
		CMD_C_OxCard OxCard;
		OxCard.bOX=GetOxCard(DataModel::sharedDataModel()->card[getMeChairID()],5);
		//发送信息
		bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);
	}
		break;
	default:
		break;
	}
}
//用户进入
void GameControlOxSixSwap::onUserEnter(){
	//CCLog("<<%s>>",__FUNCTION__);
	/*std::map<long, tagUserInfo>::iterator iter;
	for (iter = DataModel::sharedDataModel()->mTagUserInfo.begin(); iter != DataModel::sharedDataModel()->mTagUserInfo.end(); iter++)
	{
		if (iter->second.wChairID > 6 || iter->second.wChairID < 1)
		{
			continue;
		}
		CCLog("server:%d  view位置 :%d   me:%d %s<<%s>>", iter->second.wChairID, getViewChairID(iter->second.wChairID), DataModel::sharedDataModel()->userInfo->wChairID, Tools::GBKToUTF8(iter->second.szNickName), __FUNCTION__);
		getMainScene()->playerLayer->setUserInfo(getViewChairID(iter->second.wChairID), iter->second);

	}*/
	//std::map<long, tagUserInfo> tempTagUserInfo;w
	//memcpy(&tempTagUserInfo, &DataModel::sharedDataModel()->mTagUserInfo, sizeof(DataModel::sharedDataModel()->mTagUserInfo));

	std::map<long, tagUserInfo> tempTagUserInfo = DataModel::sharedDataModel()->mTagUserInfo;
	//memcpy(&tempTagUserInfo, &DataModel::sharedDataModel()->mTagUserInfo, sizeof(DataModel::sharedDataModel()->mTagUserInfo));
	
	std::map<long, tagUserInfo>::iterator iterUser = tempTagUserInfo.find(DataModel::sharedDataModel()->userInfo->dwUserID);
	if (iterUser!=tempTagUserInfo.end())
	{
		if (iterUser->second.wChairID>5||iterUser->second.wChairID<0)
		{
			return;
		}
		DataModel::sharedDataModel()->userInfo->wChairID = iterUser->second.wChairID;
		DataModel::sharedDataModel()->userInfo->wTableID = iterUser->second.wTableID;
	}

	std::map<long, tagUserInfo>::iterator iter;
	for (iter = tempTagUserInfo.begin(); iter != tempTagUserInfo.end(); iter++)
	{
		if (iter->second.wChairID > 5 || iter->second.wChairID < 0)
		{
			CCLog("------------->6 <1 %d<<%s>>",iter->second.wChairID,__FUNCTION__);
			//tempTagUserInfo.erase(iter++);
			continue;
		}
		CCLog("server:%d  view位置 :%d   me:%d %s<<%s>>",iter->second.wChairID, getViewChairID(iter->second.wChairID),DataModel::sharedDataModel()->userInfo->wChairID,Tools::GBKToUTF8(iter->second.szNickName), __FUNCTION__);
		getMainScene()->playerLayer->setUserInfo(getViewChairID(iter->second.wChairID), iter->second);
	}
	CCLog("=======================================<<%s>>",__FUNCTION__);
	//DataModel::sharedDataModel()->mTagUserInfo.clear();
}
/*void GameControlOxSixSwap::onUserEnterWithUpdate(tagUserInfo *user){
	tagUserInfo *tempUser=user;
	//memset(tempUser, 0, sizeof(tagUserInfo));
	memcpy(tempUser,user,sizeof(tagUserInfo));
	if (user->wChairID > 5 || user->wChairID < 0) return;

	getMainScene()->playerLayer->setUserInfo(getViewChairID(tempUser->wChairID), *tempUser);
}*/
//获取视图位置
int GameControlOxSixSwap::getViewChairID(int severChairID){
	int viewChair = 3 - DataModel::sharedDataModel()->userInfo->wChairID;
	if (viewChair<0)
	{
		viewChair += MAX_CHAIR_COUNT;
	}
	viewChair += severChairID;
	viewChair %= MAX_CHAIR_COUNT;
	return viewChair;
}
//游戏中
void GameControlOxSixSwap::onEventGameIng(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//用户叫庄
	{
		//消息处理
		OnSubCallBanker(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_GAME_START:	//游戏开始
	{
		//消息处理
		OnSubGameStart(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_ADD_SCORE:	//用户下注
	{
		//消息处理
		OnSubAddScore(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_SEND_CARD:	//发牌消息
	{
		//消息处理
		OnSubSendCard(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_OPEN_CARD:	//用户摊牌
	{
		//消息处理
		OnSubOpenCard(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_PLAYER_EXIT:	//用户强退
	{
		//消息处理
		OnSubPlayerExit(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_GAME_END:	//游戏结束
	{
								//结束动画
								//m_GameClientView.FinishDispatchCard();
		//消息处理
		OnSubGameEnd(pDataBuffer, wDataSize);
	}
		break;
	case SUB_S_GAME_BASE:
	{
		OnSubGameBase(pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLog("--------------------gameIng:%d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//设置基数
bool GameControlOxSixSwap::OnSubGameBase(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_GameBase)) return false;
	CMD_S_GameBase * pGameBase = (CMD_S_GameBase *)pBuffer;

	//m_GameClientView.lCellScore = pGameBase->lCellScore;
	return true;
}
//用户准备
void GameControlOxSixSwap::onUserReady(CMD_GR_UserStatus *info){
	int indexPlayer = getViewChairID(info->UserStatus.wChairID);
	getMainScene()->playerLayer->pPlayerData[indexPlayer]->showActionType(PlayerData::ACTION_READY);
}
//隐藏用户
void GameControlOxSixSwap::hidePlayer(CMD_GR_UserStatus *userInfo){
	std::map<long, tagUserInfo>::iterator iterUser = DataModel::sharedDataModel()->mTagUserInfo.find(userInfo->dwUserID);
	if (iterUser != DataModel::sharedDataModel()->mTagUserInfo.end())
	{
		getMainScene()->playerLayer->pPlayerData[getViewChairID(iterUser->second.wChairID)]->hidePlayer();
		DataModel::sharedDataModel()->mTagUserInfo.erase(userInfo->dwUserID);
	}
}
//用户叫庄
bool GameControlOxSixSwap::OnSubCallBanker(const void * pBuffer, WORD wDataSize){
	//效验数据 
	int size = sizeof(CMD_S_CallBanker);
	if (wDataSize != sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker = (CMD_S_CallBanker *)pBuffer;
	/*if(!pCallBanker->bFirstTimes && pCallBanker->wCallBanker==getMeChairID())
	{
	CCLog("推庄");
	}*/
	//始叫用户
	if (!IsLookonMode() && pCallBanker->wCallBanker == getMeChairID())
	{
		hideActionPrompt();
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_CALL_BANKER);
	}
	else
	{
		showActionPrompt(1);
	}
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		getMainScene()->playerLayer->pPlayerData[i]->hideActionType();
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
//用户下注
bool GameControlOxSixSwap::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;
	CCLog("userScore : %lld<<%s>>",pAddScore->lAddScoreCount,__FUNCTION__);
	/*//删除定时器/控制按钮
	if (IsCurrentUser(pAddScore->wAddScoreUser) && m_GameClientView.m_btOneScore.IsWindowVisible() == TRUE)
	{
		KillTimer(IDI_TIME_USER_ADD_SCORE);
		UpdateScoreControl(NULL, SW_HIDE);
	}

	//变量定义
	WORD wAddScoreUser = pAddScore->wAddScoreUser;
	WORD wViewChairID = m_wViewChairID[wAddScoreUser];

	//加注界面
	m_lTableScore[pAddScore->wAddScoreUser] = pAddScore->lAddScoreCount;
	//	m_GameClientView.SetUserTableScore(wViewChairID,pAddScore->lAddScoreCount);

	//播放声音
	//if (!IsCurrentUser(pAddScore->wAddScoreUser) && m_cbPlayStatus[wAddScoreUser]==USEX_PLAYING)
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	*/
	return true;
}
//游戏开始
bool GameControlOxSixSwap::OnSubGameStart(const void * pBuffer, WORD wDataSize){
	//效验数据
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;
	wBankerUser = pGameStart->wBankerUser;
	DataModel::sharedDataModel()->m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	//设置筹码
	for (int i = 0; i < 4; i++)
	{
		long long lCurrentScore = 0;
		if (i == 0)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 8, 1L);
		else if (i == 1)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 4, 1L);
		else if (i == 2)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 2, 1L);
		else if (i == 3)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore, 1L);
		
		UILabel *label = static_cast<UILabel*>(pbBetting[i]->getChildByName("LabelGold"));
		std::string sJetton = formatNumber(CCString::createWithFormat("%lld", lCurrentScore)->getCString());
		label->setText(Tools::GBKToUTF8(sJetton.c_str()));
	}
	//getMainScene()->playerLayer->setBankIcon(getChairIndex(getMeChairID(), wBankerUser));
	getMainScene()->playerLayer->setBankIcon(getViewChairID(wBankerUser));
	if (wBankerUser != getMeChairID())
	{
		hideActionPrompt();
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_BETTING);
	}
	else
	{
		showActionPrompt(2);
	}
	return true;
}
//发牌消息
bool GameControlOxSixSwap::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	CCLog("-----------------------------------发牌");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		int viewChair = getViewChairID(i);
		for (int j = 0; j < MAX_COUNT; j++)
		{
			DataModel::sharedDataModel()->card[viewChair][j] = pSendCard->cbCardData[viewChair][j];
			if (pSendCard->cbPlayStatus[i] == USEX_PLAYING){
				getMainScene()->cardLayer->canSendCard[viewChair] = true;
			}
			else 
			{
				getMainScene()->cardLayer->canSendCard[viewChair] = false;
			}
		}
		CCLog("----------static %d<<%s>>", pSendCard->cbPlayStatus[i], __FUNCTION__);
	}
	CCLog("-++++++++++++++----------------------------------发牌");
	getMainScene()->setServerStateWithUpdate(MainSceneOxTwo::STATE_SEND_CARD);
	/*
	//效验数据
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	//删除定时器
	KillGameClock(IDI_NULLITY);

	m_GameClientView.SetWaitInvest(false);

	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(m_cbHandCardData));

	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];
	if((IsAllowLookon() || !IsLookonMode()) && m_cbPlayStatus[wMeChiarID]==USEX_PLAYING)
	{
	m_GameClientView.m_CardControl[wViewChairID].SetDisplayItem(true);
	if(m_bAutoOpenCard)
	{
	for(WORD i=0;i<MAX_COUNT;i++)
	{
	m_GameClientView.m_CardControl[wViewChairID].m_bShow[i] = true;
	m_GameClientView.m_CardControlOx[wViewChairID].m_bShow[i] = true;
	}
	}
	else
	{
	for(WORD i=0;i<MAX_COUNT;i++)
	m_GameClientView.m_CardControl[wViewChairID].m_bShow[i] = false;
	}
	}

	//派发扑克
	for(WORD i=0;i<MAX_COUNT;i++)
	{
	for (WORD j=m_wWinUser;j<m_wWinUser+GAME_PLAYER;j++)
	{
	WORD w=j%GAME_PLAYER;
	if (m_cbPlayStatus[w]==USEX_PLAYING)
	{
	if (w==GetMeChairID())
	{
	WORD wViewChairID=m_wViewChairID[w];
	if(pSendCard->cbCardData[w][i]!=0)
	{
	m_GameClientView.DispatchUserCard(wViewChairID,m_cbHandCardData[w][i]);
	}
	}else
	{
	WORD wViewChairID=m_wViewChairID[w];
	if(pSendCard->cbCardData[GetMeChairID()][i]!=0)
	{
	m_GameClientView.DispatchUserCard(wViewChairID,m_cbHandCardData[GetMeChairID()][i]);
	}
	}
	}
	}
	}
	*/

	return true;
}
//用户摊牌
bool GameControlOxSixSwap::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard = (CMD_S_Open_Card *)pBuffer;
	CCLog("openCardID:%d  bOpen:%d", pOpenCard->wPlayerID, pOpenCard->bOpen);
	/*
	//设置变量
	WORD wMeChairID=GetMeChairID();
	WORD wID=pOpenCard->wPlayerID;
	WORD wViewChairID=m_wViewChairID[wID];
	m_bUserOxCard[wID]=pOpenCard->bOpen;
	m_GameClientView.ShowOpenCard(wViewChairID);

	//摊牌标志
	if(!IsCurrentUser(wID))
	{
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
	}
	else if(m_GameClientView.m_btOpenCard.IsWindowVisible()==TRUE)
	{
	//删除时间/控件
	KillTimer(IDI_TIME_OPEN_CARD);
	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);

	//显示不构成牛牛
	if(m_bUserOxCard[wID]==FALSE)
	{
	//			m_GameClientView.m_CardControl[wViewChairID].ShootAllCard(false);
	m_GameClientView.SetUserOxValue(wViewChairID,0);
	}
	else
	{
	//提示再分类牛牌
	OnHintOx(0,0);
	m_GameClientView.m_CardControl[wViewChairID].SetOX(true);
	OnSortCard(0,0);
	}
	m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
	}

	for(WORD i=0;i<MAX_COUNT;i++)
	{
	m_GameClientView.m_CardControl[wViewChairID].m_bShow[i] = true;
	m_GameClientView.m_CardControlOx[wViewChairID].m_bShow[i] = true;

	// 		m_GameClientView.m_CardControl[wViewChairID].Invalidate();
	// 		m_GameClientView.m_CardControlOx[wViewChairID].Invalidate();
	}

	m_GameClientView.RefreshGameView();
	*/
	return true;
}
//游戏结束
bool GameControlOxSixSwap::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize != sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;
	hideActionPrompt();
	//显示积分
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		long long lGameScore = pGameEnd->lGameScore[i];
		if (getMainScene()->cardLayer->canSendCard[getViewChairID(i)])
		{
			getMainScene()->playerLayer->showResultAnimation(getViewChairID(i), lGameScore);
		}
	}
	/*for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		long long lGameScore = pGameEnd->lGameScore[i];
		if (i == getMeChairID()){
			getMainScene()->playerLayer->showResultAnimation(3, lGameScore);
			if (lGameScore != 0){
				UIPanel *pPlayer0 = getMainScene()->playerLayer->pPlayerData[0]->pPlayerPanel;
				UIPanel *pPlayer3 = getMainScene()->playerLayer->pPlayerData[3]->pPlayerPanel;
				for (int i = 0; i < 60; i++)
				{
					if (lGameScore>0)
					{
						CCPoint begingPos = ccpAdd(pPlayer0->getPosition(), ccp(pPlayer0->getContentSize().width / 2, pPlayer0->getContentSize().height / 2));
						CCPoint endPos = ccpAdd(pPlayer3->getPosition(), ccp(pPlayer3->getContentSize().width / 2, pPlayer3->getContentSize().height / 2));

						//goldJump(i, begingPos, endPos);
					}
					else
					{
						CCPoint endPos = ccpAdd(pPlayer0->getPosition(), ccp(pPlayer0->getContentSize().width / 2, pPlayer0->getContentSize().height / 2));
						CCPoint begingPos = ccpAdd(pPlayer3->getPosition(), ccp(pPlayer3->getContentSize().width / 2, pPlayer3->getContentSize().height / 2));

						//goldJump(i, begingPos, endPos);
					}
				}
			}
		}
		else
		{
			getMainScene()->playerLayer->showResultAnimation(i, lGameScore);
		}
	}*/
	//显示牌型
	bool bOxSound = false;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		//WORD wViewChairID=m_wViewChairID[i];
		if (i == getMeChairID() && !IsLookonMode())
			continue;
		bool isCardError = false;
		for (int j = 0; j < MAX_COUNT; j++)
		{
			DataModel::sharedDataModel()->card[i][j] = pGameEnd->cbCardData[i][j];
			if (pGameEnd->cbCardData[i][j] == 0)
			{
				isCardError = true;
				break;
			}
		}
		if (!isCardError)
		{
			getMainScene()->cardLayer->showCard(getViewChairID(i),i);
			getMainScene()->cardLayer->sortingOx(i,getViewChairID(i));
			
			//getMainScene()->cardLayer->showCard(getChairIndex(DataModel::sharedDataModel()->userInfo->wChairID, i), i);
			//getMainScene()->cardLayer->sortingOx(i, getChairIndex(DataModel::sharedDataModel()->userInfo->wChairID, i));
		}
	}


	/*pEndLayer = GameEndLayer::create();
	this->addChild(pEndLayer);
	pEndLayer->showEnd(pGameEnd->lGameScore[getMeChairID()] >= 0);*/
	getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_END);
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
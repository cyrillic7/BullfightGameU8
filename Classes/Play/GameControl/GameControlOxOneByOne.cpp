//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControlOxOneByOne.h"
#include "../../Tools/DataModel.h"
#include "../../Network/CMD_Server/CMD_Ox_OneByOne.h"
#include "../../Network/TCPSocket/TCPSocketControl.h"
#include "../../Tools/Tools.h"
GameControlOxOneByOne::GameControlOxOneByOne()
{

}
GameControlOxOneByOne::~GameControlOxOneByOne(){
	DataModel::sharedDataModel()->mTagUserInfo.clear();
}
void GameControlOxOneByOne::onEnter(){
	GameControlBase::onEnter();
}
void GameControlOxOneByOne::onExit(){
	GameControlBase::onExit();
}
int GameControlOxOneByOne::getChairIndex(int meChairID,int chairID){
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
//开牌
void GameControlOxOneByOne::menuOpenCard(CCObject* pSender, TouchEventType type){
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
void GameControlOxOneByOne::onUserEnter(){
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
/*void GameControlOxOneByOne::onUserEnterWithUpdate(tagUserInfo *user){
	tagUserInfo *tempUser=user;
	//memset(tempUser, 0, sizeof(tagUserInfo));
	memcpy(tempUser,user,sizeof(tagUserInfo));
	if (user->wChairID > 5 || user->wChairID < 0) return;

	getMainScene()->playerLayer->setUserInfo(getViewChairID(tempUser->wChairID), *tempUser);
}*/
//获取视图位置
int GameControlOxOneByOne::getViewChairID(int severChairID){
	/*int viewChair = 3-DataModel::sharedDataModel()->userInfo->wChairID + severChairID;
	viewChair %= MAX_CHAIR_COUNT;
	if (viewChair<0)
	{
		viewChair += MAX_CHAIR_COUNT;
	}
	return viewChair;*/
	int viewChair = 3 - DataModel::sharedDataModel()->userInfo->wChairID;
	if (viewChair<0)
	{
		viewChair += MAX_CHAIR_COUNT;
	}
	viewChair += severChairID;
	viewChair %= MAX_CHAIR_COUNT;
	return viewChair;
}
//用户准备
void GameControlOxOneByOne::onUserReady(CMD_GR_UserStatus *info){
	int indexPlayer = getViewChairID(info->UserStatus.wChairID);
	getMainScene()->playerLayer->pPlayerData[indexPlayer]->showActionType(PlayerData::ACTION_READY);
	/*if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
	{
		getMainScene()->playerLayer->pPlayerData[3]->showActionType(PlayerData::ACTION_READY);
	}
	else
	{
		getMainScene()->playerLayer->pPlayerData[0]->showActionType(PlayerData::ACTION_READY);
	}*/
}
//隐藏用户
void GameControlOxOneByOne::hidePlayer(CMD_GR_UserStatus *userInfo){
	CCLog("<<%s>>",__FUNCTION__);
	std::map<long, tagUserInfo>::iterator iterUser = DataModel::sharedDataModel()->mTagUserInfo.find(userInfo->dwUserID);
	if (iterUser != DataModel::sharedDataModel()->mTagUserInfo.end())
	{
		getMainScene()->playerLayer->pPlayerData[getViewChairID(iterUser->second.wChairID)]->hidePlayer();
		//DataModel::sharedDataModel()->mTagUserInfo.erase(userInfo->dwUserID);
	}
}
//用户下注
bool GameControlOxOneByOne::OnSubAddScore(const void * pBuffer, WORD wDataSize)
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
bool GameControlOxOneByOne::OnSubGameStart(const void * pBuffer, WORD wDataSize){
	//效验数据
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;
	CCLog("max: %lld<<%s>>", pGameStart->lTurnMaxScore, __FUNCTION__);
	/*//用户信息
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		//视图位置
		m_wViewChairID[i] = SwitchViewChairID(i);

		//获取用户
		IClientUserItem * pUserData = GetTableUserItem(i);
		if (pUserData == NULL)
		{
			m_cbPlayStatus[i] = USEX_NULL;
		}
		else
		{
			m_cbPlayStatus[i] = USEX_PLAYING;

			//用户名字
			lstrcpyn(m_szNickNames[i], pUserData->GetNickName(), CountArray(m_szNickNames[i]));
		}

		//设置界面
		m_GameClientView.SetUserPlayingStatus(m_wViewChairID[i], m_cbPlayStatus[i]);
	}

	//旁观者清理数据
	if (IsLookonMode()) OnStart(0, 0);
	//删除定时器/按钮
	if (m_GameClientView.m_btIdler.IsWindowVisible() == TRUE)
	{
		KillGameClock(IDI_CALL_BANKER);
		m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
	}
	*/
	//设置变量
	DataModel::sharedDataModel()->m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	/*//m_wBankerUser=pGameStart->wBankerUser;
	m_GameClientView.SetWaitCall(0xff);

	//设置界面
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//设置筹码
	if (!IsLookonMode() && pGameStart->lTurnMaxScore>0)
	{
		LONGLONG lUserMaxScore[4];
		ZeroMemory(lUserMaxScore, sizeof(lUserMaxScore));
		LONGLONG lTemp = m_lTurnMaxScore;
		for (WORD i = 0; i<4; i++)
		{
			if (i>0)lTemp /= 2;
			lUserMaxScore[i] = __max(lTemp, 1L);
		}

		//更新控件
		//ActiveGameFrame();
		UpdateScoreControl(lUserMaxScore, SW_HIDE);

		//实际定时器
		//SetTimer(IDI_TIME_USER_ADD_SCORE,(TIME_USER_ADD_SCORE)*1000,NULL);
	}



	//等待标志
	m_GameClientView.SetWaitInvest(true);

	//辅助显示中心时钟
	//SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_ADD_SCORE);

	//环境设置
	PlayGameSound(AfxGetInstanceHandle(), TEXT("GAME_START"));

	for (WORD i = 0; i<MAX_COUNT; i++)
	{
		m_GameClientView.m_CardControl[i].m_bShow[i] = false;
		m_GameClientView.m_CardControlOx[i].m_bShow[i] = false;
	}

	//用户信息
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		//视图位置
		m_wViewChairID[i] = SwitchViewChairID(i);

		//获取用户
		IClientUserItem * pUserData = GetTableUserItem(i);
		if (pUserData != NULL)
		{
			if (m_cbPlayStatus[i] == USEX_PLAYING)
			{
				m_GameClientView.OnUserAddScore(m_wViewChairID[i], m_lTurnMaxScore, false);
			}
		}
	}
	//	m_GameClientView.PlaceUserJetton(1,100);
	*/
	//发送消息
	CMD_C_AddScore AddScore;
	AddScore.lScore = DataModel::sharedDataModel()->m_lTurnMaxScore;
	TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
	return true;
}
//发牌消息
bool GameControlOxOneByOne::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	CCLog("发牌");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		for (int j = 0; j < MAX_COUNT; j++)
		{
			DataModel::sharedDataModel()->card[i][j] = pSendCard->cbCardData[i][j];
		}
	}
	
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
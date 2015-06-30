//1
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControlOxTwo.h"
#include "../../Tools/DataModel.h"
#include "../../Network/CMD_Server/cmd_ox.h"
#include "../../Network/TCPSocket/TCPSocketControl.h"
GameControlOxTwo::GameControlOxTwo()
{

}
GameControlOxTwo::~GameControlOxTwo(){

}
void GameControlOxTwo::onEnter(){
	GameControlBase::onEnter();
	//添加标题 
	getMainScene()->addTitle();

	getMainScene()->playerLayer->hideOxTwoPlayer();
}
void GameControlOxTwo::onExit(){
	GameControlBase::onExit();
}
int GameControlOxTwo::getChairIndex(int meChairID,int chairID){
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
void GameControlOxTwo::menuOpenCard(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		hideTimer(true);
		DataModel::sharedDataModel()->getMainSceneOxTwo()->cardLayer->sortingOx(getMeChairID(),3);
		/*//发送消息
		CMD_C_OxCard OxCard;
		OxCard.bOX=(m_GameClientView.m_CardControl[wViewChairID].GetOX())?TRUE:FALSE;
		SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
		//DataModel::sharedDataModel()->getMainSceneOxTwo()->setGameStateWithUpdate(MainScene::STATE_SETTLE_ACCOUNFS);*/
		showActionPrompt(ACTION_PROMPT_OPEN_CARD, CCPointZero);
		pOptOx->setEnabled(false);		
		CMD_C_OxCard OxCard;
		OxCard.bOX=GetOxCard(DataModel::sharedDataModel()->card[getMeChairID()],5);
		//发送信息
		bool isSend=getSocket()->SendData(MDM_GF_GAME,SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);
	}
		break;
	default:
		break;
	}
}
//隐藏用户
void GameControlOxTwo::hidePlayer(CMD_GR_UserStatus *userInfo){
	getMainScene()->playerLayer->pPlayerData[0]->hidePlayer();
}

//游戏中
void GameControlOxTwo::onEventGameIng(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//用户叫庄
	{
		//消息处理
		OnSubCallBanker(pDataBuffer, wDataSize);
		CCLog("用户叫庄<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_S_GAME_START:	//游戏开始
	{
		//消息处理
		OnSubGameStart(pDataBuffer, wDataSize);
		CCLog("游戏开始<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_S_ADD_SCORE:	//用户下注
	{
		//消息处理
		OnSubAddScore(pDataBuffer, wDataSize);
		CCLog("用户下注<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_S_SEND_CARD:	//发牌消息
	{
		//消息处理
		OnSubSendCard(pDataBuffer, wDataSize);
		CCLog("发牌消息<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_S_OPEN_CARD:	//用户摊牌
	{
		//消息处理
		OnSubOpenCard(pDataBuffer, wDataSize);
		CCLog("用户摊牌<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_S_PLAYER_EXIT:	//用户强退
	{
		//消息处理
		OnSubPlayerExit(pDataBuffer, wDataSize);
		CCLog("用户强退<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_S_GAME_END:	//游戏结束
	{
		//结束动画
		//m_GameClientView.FinishDispatchCard();
		//消息处理
		OnSubGameEnd(pDataBuffer, wDataSize);
		CCLog("游戏结束<<%s>>", __FUNCTION__);
	}
	break;
	/*case SUB_S_GAME_BASE:
	{
	OnSubGameBase(pData, wDataSize);
	}
	break;*/
	case 1024:
	{
		CCLog("1024-----------------------<<%s>>", __FUNCTION__);
		//m_GameClientView.OnLockGame(pData->pDataBuffer,pData->wDataSize);
		//return true;
	}
	break;
	default:
		CCLog("--------------------gameIng:%d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
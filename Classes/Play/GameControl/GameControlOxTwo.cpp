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
		showActionPrompt(3, CCPointZero);
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
//隐藏用户
void GameControlOxTwo::hidePlayer(CMD_GR_UserStatus *userInfo){
	getMainScene()->playerLayer->pPlayerData[0]->hidePlayer();
}
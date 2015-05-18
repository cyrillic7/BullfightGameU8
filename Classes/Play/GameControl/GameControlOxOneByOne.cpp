//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControlOxOneByOne.h"
#include "../../Tools/DataModel.h"
#include "../../Network/CMD_Server/cmd_ox.h"
#include "../../Network/TCPSocket/TCPSocketControl.h"
GameControlOxOneByOne::GameControlOxOneByOne()
{

}
GameControlOxOneByOne::~GameControlOxOneByOne(){

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
		DataModel::sharedDataModel()->getMainSceneOxTwo()->cardLayer->sortingOx(getMeChairID(),3);
		showActionPrompt(3);
		pOptOx->setEnabled(false);		
		CMD_C_OxCard OxCard;
		OxCard.bOX=GetOxCard(DataModel::sharedDataModel()->card[getMeChairID()],5);
		//发送信息
		bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
		DataModel::sharedDataModel()->getMainSceneOxTwo()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);
	}
		break;
	default:
		break;
	}
}
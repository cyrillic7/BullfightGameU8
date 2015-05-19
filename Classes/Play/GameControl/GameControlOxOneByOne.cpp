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
#include "../../Tools/Tools.h"
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
//用户进入
void GameControlOxOneByOne::onUserEnter(){
	//long userChairID = 0;
	std::map<long, tagUserInfo>::iterator iterUser= DataModel::sharedDataModel()->mTagUserInfo.find(DataModel::sharedDataModel()->userInfo->dwUserID);
	if (iterUser!=DataModel::sharedDataModel()->mTagUserInfo.end())
	{
		if (iterUser->second.wChairID>6||iterUser->second.wChairID<1)
		{
			return;
		}
		DataModel::sharedDataModel()->userInfo->wChairID = iterUser->second.wChairID;
		DataModel::sharedDataModel()->userInfo->wTableID = iterUser->second.wTableID;
	}

	std::map<long, tagUserInfo>::iterator iter;
	for (iter = DataModel::sharedDataModel()->mTagUserInfo.begin(); iter != DataModel::sharedDataModel()->mTagUserInfo.end(); iter++)
	{
		/*if (iter->second.dwUserID != DataModel::sharedDataModel()->userInfo->dwUserID)
		{
			CCLog("ID:%ld otherID:%ld   name:%s<<%s>>", iter->second.dwUserID, DataModel::sharedDataModel()->userInfo->dwUserID,
				Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName), __FUNCTION__);
			getMainScene()->playerLayer->setUserInfo(0, iter->second);
			//DataModel::sharedDataModel()->getMainSceneOxOneByOne()->playerLayer->setUserInfo(0,iter->second);
		}
		else
		{
			getMainScene()->playerLayer->setUserInfo(3, iter->second);
		}*/
		if (iter->second.wChairID > 6 || iter->second.wChairID < 1)
			continue;
			
		CCLog("server:%d  view位置 :%d   me:%d %s<<%s>>",iter->second.wChairID, getViewChairID(iter->second.wChairID),DataModel::sharedDataModel()->userInfo->wChairID,Tools::GBKToUTF8(iter->second.szNickName), __FUNCTION__);
		getMainScene()->playerLayer->setUserInfo(getViewChairID(iter->second.wChairID), iter->second);
	}
	CCLog("=======================================<<%s>>",__FUNCTION__);
	//DataModel::sharedDataModel()->mTagUserInfo.clear();
}
//获取视图位置
int GameControlOxOneByOne::getViewChairID(int severChairID){
	int viewChair = 3-DataModel::sharedDataModel()->userInfo->wChairID + severChairID;
	viewChair %= (MAX_CHAIR_COUNT+1);
	//转换椅子
	//int wChairCount = MAX_CHAIR_COUNT;
	//int wViewChairID = (severChairID + wChairCount * 3 / 2 - m_UserAttribute.wChairID) % wChairCount;

	return viewChair;
}
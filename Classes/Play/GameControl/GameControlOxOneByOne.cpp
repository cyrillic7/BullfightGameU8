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
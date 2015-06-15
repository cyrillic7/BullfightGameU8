/*
 * PopDialogBoxRanking.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxRanking.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxRanking::PopDialogBoxRanking()
	:iMyRankingNum(-1)
{
	scheduleUpdate();
}
PopDialogBoxRanking::~PopDialogBoxRanking() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_RANKING);
}
void PopDialogBoxRanking::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxRanking.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//列表
	pLVRanking = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewRanking"));
	//获奖者
	pLPrizeWinner = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelPrizeWinner"));
	//我的名字
	UILabel *pLMyName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelMyName"));
	pLMyName->setText(GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName));
	//我的金币
	UILabel *pLMyGold = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelMyGold"));
	pLMyGold->setText(CCString::createWithFormat("%lld",DataModel::sharedDataModel()->userInfo->lScore)->getCString());
	//我的排名背景
	pIVMyRankingBg=static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageMyRanking"));
	//我的排名
	pLMyRankingNum = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelMyRankingNum"));
	//设置cell样式
	pLVRanking->setItemModel(pLVRanking->getItem(0));
	pLVRanking->removeAllItems();

	//设置父结点不读取网络数据
	setLobbyReadMessage(false);
	//连接服务器
	connectServer(SOCKET_RANKING);
	//播放动画
	playAnimation();
}
void PopDialogBoxRanking::onExit(){
	CCLayer::onExit();
}
//更新列表
void PopDialogBoxRanking::updateListRanking(){
	UIListView *pLVTemp = pLVRanking;
	pLVTemp->removeAllItems();

	int tempSize = vecRanking.size();
	if (tempSize == 0)
	{
		pIVMyRankingBg->setVisible(false);
		pLMyRankingNum->setText("未入榜 ");
		return;
	}
	//前三名////////////////////////////////////////////////////////////////////////
	std::string strContent;
	strContent += "恭喜";
	for (int i = 0; i < 3; i++)
	{
		if (i>=vecRanking.size()-1)
		{
			continue;
		}
		UIImageView *pLNum = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageChampion")->getChildByName(CCString::createWithFormat("ImageNumber%d", i + 1)->getCString()));
		//名称
		UILabel *pLName = static_cast<UILabel*>(pLNum->getChildByName("LabelName"));
		pLName->setText(GBKToUTF8(vecRanking[i].szNickName));
		//金币
		UILabel *pLGold = static_cast<UILabel*>(pLNum->getChildByName("LabelGold"));
		pLGold->setText(CCString::createWithFormat("%lld",vecRanking[i].lScore)->getCString());
		strContent += GBKToUTF8(vecRanking[i].szNickName);
		if (i!=2)
		{
			strContent += "、 ";
		}
		if (strcmp(DataModel::sharedDataModel()->userInfo->szNickName,vecRanking[i].szNickName)==0)
		{
			iMyRankingNum = i;
		}
	}
	strContent += "成为榜单前三名 ";
	pLPrizeWinner->setText(strContent.c_str());
	//4名已后////////////////////////////////////////////////////////////////////////
	for (int i = 3; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//名次
		UILabel *pLNum = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("ImageNum")->getChildByName("LabelRankingNum"));
		pLNum->setText(CCString::createWithFormat("%d",i+1)->getCString());
		//昵称
		UILabel *pName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelName"));
		pName->setText(GBKToUTF8(vecRanking[i].szNickName));
		//金币数
		UILabel *pGold = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelGold"));
		pGold->setText(CCString::createWithFormat("%lld",vecRanking[i].lScore)->getCString());
		
		if (strcmp(DataModel::sharedDataModel()->userInfo->szNickName, vecRanking[i].szNickName) == 0)
		{
			iMyRankingNum = i;
		}
		
	}
	if (iMyRankingNum!=-1)
	{
		pIVMyRankingBg->setVisible(true);
		pLMyRankingNum->setText(CCString::createWithFormat("%d", iMyRankingNum+1)->getCString());
	}
	else
	{
		pIVMyRankingBg->setVisible(false);
		pLMyRankingNum->setText("未入榜 ");
	}
}
//更新
void PopDialogBoxRanking::update(float delta){
	MessageQueue::update(delta);
}
//////////////////////////////////////////////////////////////////////////
//网络消息
void PopDialogBoxRanking::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_TREASURE_RANK);
	}
		break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
	}
	break;
	default:
		CCLog("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//用户服务
void PopDialogBoxRanking::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_TREASURE_RANK://排名列表
		onSubRankingList(pDataBuffer, wDataSize);
		break;
	default:
		CCLog("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//排名列表
void PopDialogBoxRanking::onSubRankingList(void * pDataBuffer, unsigned short wDataSize){
	//assert(wDataSize >= sizeof(CMD_GP_TreasureRank));
	int count = wDataSize / sizeof(CMD_GP_TreasureRank);

	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

	vecRanking.clear();
	for (int i = 0; i < count; i++)
	{
		void * pDataBuffer = cbDataBuffer + i*sizeof(CMD_GP_TreasureRank);
		CMD_GP_TreasureRank *pRanking = (CMD_GP_TreasureRank*)pDataBuffer;
		
		CMD_GP_TreasureRank ranking;
		ranking.dwFaceID = pRanking->dwFaceID;
		ranking.lScore = pRanking->lScore;
		strcpy(ranking.szNickName,pRanking->szNickName);

		vecRanking.push_back(ranking);
	}
	//更新列表
	updateListRanking();
	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	//关闭网络
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_RANKING);
}

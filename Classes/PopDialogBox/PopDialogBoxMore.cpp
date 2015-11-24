/*
 * PopDialogBoxMore.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxMore.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Platform/coPlatform.h"
#include "../Network/CMD_Server/Packet.h"
#include "../Tools/StatisticsConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxMore::PopDialogBoxMore()
{
	
	scheduleUpdate();
}
PopDialogBoxMore::~PopDialogBoxMore() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	//unscheduleUpdate();
}
void PopDialogBoxMore::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxMore.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//更多列表
	pLVMoreList = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewMore"));
	//设置cell模式
	pLVMoreList->setItemModel(pLVMoreList->getItem(0));
	pLVMoreList->removeAllItems();

	

	//连接服务器
	connectServer();
	connectSuccess();
	

	playAnimation();
}
void PopDialogBoxMore::onExit(){
	CCLayer::onExit();
}
//更新
void PopDialogBoxMore::update(float delta){
	gameSocket.updateSocketData(delta);
}
//打开游戏
void PopDialogBoxMore::onMenuOpenGame(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pButton = (UIButton*)object;
		int tag = pButton->getTag();
		//vecMoreInfo[tag].szAppName;
		std::string sUrlAndroid = vecMoreInfo[tag].szUrlAndroid;
		//int index = sUrlAndroid.find(".apk");
		//sUrlAndroid.insert(index,CCString::createWithFormat("_%s",DataModel::sharedDataModel()->sSessionID.c_str())->getCString());
		CCString *sAction = CCString::createWithFormat("{\"act\":500 ,\"packageName\":\"%s\",\"activity\":\"%s\",\"url\":\"%s\"}", vecMoreInfo[tag].szPackageName, vecMoreInfo[tag].szActivityName, sUrlAndroid.c_str());
		platformAction(sAction->getCString());
	}
}
//更新更多游戏列表
void PopDialogBoxMore::updateListMore(){
	UIListView *pLVTemp = pLVMoreList;
	pLVTemp->removeAllItems();

	int tempSize = vecMoreInfo.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		
		UIImageView *pIVBg = static_cast<UIImageView*>(pLVTemp->getItem(inserterPos)->getChildByName("ImageBg")); 
		addDownloadImage(pIVBg, vecMoreInfo[i].szICO, ccp(-176, 0), 1, -1, false);
		//游戏名称
		UILabel *pLGameName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelName"));
		pLGameName->setText(GBKToUTF8(vecMoreInfo[i].szGameName).c_str());
		//游戏说明
		UILabel *pLGameContent = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelContent"));
		pLGameContent->setText(GBKToUTF8(vecMoreInfo[i].szDescribeString).c_str());
		//按键
		UIButton *pButton = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonOpenGame"));
		pButton->setTag(i);
		pButton->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxMore::onMenuOpenGame));
	}
}

//////////////////////////////////////////////////////////////////////////
//网络消息
void PopDialogBoxMore::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{

	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxMore::connectSuccess(){
	CMD_GP_GetMoreGame moreGame;
	moreGame.dwOpTerminal = 2;
	moreGame.wPlatformType = 1;
	gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_MORE_GAME,&moreGame,sizeof(CMD_GP_GetMoreGame));
}
//用户服务
void PopDialogBoxMore::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_MORE_GAME://更多游戏列表
		onSubMoreGameList(pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//更多游戏列表
void PopDialogBoxMore::onSubMoreGameList(void * pDataBuffer, unsigned short wDataSize){
	
	int size = sizeof(CMD_GP_MoreGame);
	int count = wDataSize / sizeof(CMD_GP_MoreGame);

	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

	vecMoreInfo.clear();
	for (int i = 0; i < count; i++)
	{
		void * pDataBuffer = cbDataBuffer + i*sizeof(CMD_GP_MoreGame);
		CMD_GP_MoreGame *pMoreGame = (CMD_GP_MoreGame*)pDataBuffer;

		CMD_GP_MoreGame moreGame;
		strcpy(moreGame.szActivityName, pMoreGame->szActivityName);
		strcpy(moreGame.szDescribeString,pMoreGame->szDescribeString);
		strcpy(moreGame.szGameName, pMoreGame->szGameName);
		strcpy(moreGame.szICO, pMoreGame->szICO);
		strcpy(moreGame.szPackageName, pMoreGame->szPackageName);
		strcpy(moreGame.szUrlAndroid, pMoreGame->szUrlAndroid);
		strcpy(moreGame.szUrlIos, pMoreGame->szUrlIos);


		vecMoreInfo.push_back(moreGame);
	}
	updateListMore();
	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	gameSocket.Destroy(true);
}
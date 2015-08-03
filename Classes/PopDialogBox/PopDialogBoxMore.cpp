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

	vecMoreInfo.push_back("sdd");
	updateListMore();

	//连接服务器
	connectServer();
	gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_TREASURE_RANK);

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
		
		CCString *sAction = CCString::createWithFormat("{\"act\":500 ,\"packageName\":\"%s\",\"activity\":\"%s\"}", "com.foxgame.ForestGuard", "ForestGuard");
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
		
		//按键
		UIButton *pButton = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonOpenGame"));
		pButton->setTag(inserterPos);
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
//用户服务
void PopDialogBoxMore::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_TREASURE_RANK://排名列表
		onSubRankingList(pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//排名列表
void PopDialogBoxMore::onSubRankingList(void * pDataBuffer, unsigned short wDataSize){
	//assert(wDataSize >= sizeof(CMD_GP_TreasureRank));
	int count = wDataSize / sizeof(CMD_GP_TreasureRank);

	
	
	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	gameSocket.Destroy(true);
}
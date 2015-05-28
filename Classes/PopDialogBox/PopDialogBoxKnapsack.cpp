/*
 * PopDialogBoxKnapsack.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxKnapsack.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "PopDialogBoxLoading.h"
#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/MD5/MD5.h"
#include "../GameLobby/BaseLobbyScene.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxKnapsack::PopDialogBoxKnapsack()
{
	scheduleUpdate();
}
PopDialogBoxKnapsack::~PopDialogBoxKnapsack() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_SHOP);
}
void PopDialogBoxKnapsack::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxKnapsack.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);

	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();
	
}
void PopDialogBoxKnapsack::onExit(){
	CCLayer::onExit();
}
//读取网络消息回调
void PopDialogBoxKnapsack::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		
	}
	break;
	default:
		CCLog("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
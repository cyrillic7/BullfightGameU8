/*
 * PopDialogBoxFirstRecharge.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxFirstRecharge.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/MD5/MD5.h"
#include "../Platform/coPlatform.h"
#include "../Network/SEvent.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxFirstRecharge::PopDialogBoxFirstRecharge()
{
	scheduleUpdate();
}
PopDialogBoxFirstRecharge::~PopDialogBoxFirstRecharge() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
}
void PopDialogBoxFirstRecharge::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxFirstRecharge.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//首充
	UIButton *pBRcharge = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonRcharge"));
	pBRcharge->addTouchEventListener(this, toucheventselector(PopDialogBoxFirstRecharge::onMenuExchange));
	

	playAnimation();
	
}
void PopDialogBoxFirstRecharge::onExit(){
	CCLayer::onExit();
}
//兑换按键
void PopDialogBoxFirstRecharge::onMenuExchange(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		orderID = platformAction("{\"act\":800}");
		setFRechargeActionType(FR_ACTION_SEND_ORDER);
		connectServer();
		connectSuccess();
	}
}
void PopDialogBoxFirstRecharge::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//读取网络消息回调
void PopDialogBoxFirstRecharge::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		//connectSuccess();
	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
		//移除loading
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//关闭网络
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_RECHARGE);
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxFirstRecharge::connectSuccess(){
	switch (eFRechargeActionType)
	{
	case FR_ACTION_SEND_ORDER://发送订单号
	{
		CMD_GP_RechargeOrder rOrder;
		rOrder.dwFirst = 1;
		rOrder.dwOpTerminal = 2;
		rOrder.dwOrderAmount = 10;
		rOrder.dwShareID = 7;

		strcpy(rOrder.szAccounts, DataModel::sharedDataModel()->sLogonAccount.c_str());
		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(rOrder.szLogonPass, md5PassWord.c_str());

		strcpy(rOrder.szRechargeOrder, orderID.c_str());

		CCLOG("orderID:%s <<%s>>", orderID.c_str(), __FUNCTION__);
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_RECHARGE_ORDER, &rOrder, sizeof(rOrder));
	}
	break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxFirstRecharge::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_RECHARGE_ORDER:
	{
		CMD_GP_RechargeOrderLog *rOrderLog = (CMD_GP_RechargeOrderLog*)pDataBuffer;
		if (rOrderLog->dwRet == 0)
		{
			CCString *sAction = CCString::createWithFormat("{\"act\":300 ,\"propName\":\"%s\",\"propInfo\":\"%s\",\"propPice\":\"%ld\"}", "首充豪礼", "送100000金币再送50000金币",10);
			platformAction(sAction->getCString());
		}
		else
		{
			showTipInfo(GBKToUTF8(rOrderLog->szDescribeString).c_str());
			if (rOrderLog->dwRet == 10){
				DataModel::sharedDataModel()->userInfo->dwFirstOnLineOrder = 1;
			}
		}
	}
	break;
	default:
		CCLOG("   %d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}

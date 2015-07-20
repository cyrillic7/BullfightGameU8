/*
 * PopDialogBoxRecharge.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxRecharge.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/MD5/MD5.h"
#include "../Platform/coPlatform.h"

#define MAX_GOLD_COUNT    6

std::string sGoldIconName[MAX_GOLD_COUNT] = { "u_recharge_gold0.png", "u_recharge_gold1.png", "u_recharge_gold2.png", "u_recharge_gold3.png", "u_recharge_gold4.png", "u_recharge_gold5.png" };
std::string sGoldExchangeNum[MAX_GOLD_COUNT] = { " 1万 ", " 3万 ", " 5万 ", " 10万 ", " 50万 ", " 100万 " };
long lGoldPice[MAX_GOLD_COUNT] = { 1, 3, 5, 10, 50, 100 };

#define MAX_BIG_GOLD_COUNT    6
std::string sBigGoldIconName[MAX_BIG_GOLD_COUNT] = { "u_recharge_gold0.png", "u_recharge_gold1.png", "u_recharge_gold2.png", "u_recharge_gold3.png", "u_recharge_gold4.png", "u_recharge_gold5.png" };
std::string sBigGoldExchangeNum[MAX_BIG_GOLD_COUNT] = { " 1个 ", " 3个 ", " 5个 ", " 10个 ", " 50个 ", " 100个 " };
long lBigGoldPice[MAX_GOLD_COUNT] = { 1, 3, 5, 10, 50, 100 };

//////////////////////////////////////////////////////////////////////////
PopDialogBoxRecharge::PopDialogBoxRecharge()
	:eRechargeType(RECHARGE_GOLD)
	, eRechargeActionType(R_Action_GET_MONEY)
	, pIPopDialogBoxAssistCloseView(NULL)
	, isShowExchangeGold(true)
{
	for (int i = 0; i < MAX_GOLD_COUNT; i++)
	{
		RechargeData rData;
		rData.sImageIconName = sGoldIconName[i];
		rData.sExchangeNum = sGoldExchangeNum[i];
		rData.lExchangePice = lGoldPice[i];
		vecRechargeGold.push_back(rData);
	}
	for (int i = 0; i < MAX_BIG_GOLD_COUNT; i++)
	{
		RechargeData rData;
		rData.sImageIconName = sBigGoldIconName[i];
		rData.sExchangeNum = sBigGoldExchangeNum[i];
		rData.lExchangePice = lBigGoldPice[i];
		vecRechargeBigGold.push_back(rData);
	}
	

	
	scheduleUpdate();
}
PopDialogBoxRecharge::~PopDialogBoxRecharge() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_RECHARGE);
	//gameSocket.Destroy(true);
}
void PopDialogBoxRecharge::onEnter(){
	CCLayer::onEnter();																					
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxRecharge.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxRecharge::onMenuCloseView));
	//兑换金币
	UIButton *pBExchangeGold = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonGold"));
	pBExchangeGold->addTouchEventListener(this, toucheventselector(PopDialogBoxRecharge::onMenuChangeExchangeType));
	pBExchangeGold->setVisible(false);
	//兑换元宝
	UIButton *pBExchangeBigGold = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBigGold"));
	pBExchangeBigGold->addTouchEventListener(this, toucheventselector(PopDialogBoxRecharge::onMenuChangeExchangeType));
	pBExchangeBigGold->setVisible(false);
	//游标
	pIVCursor = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageCursor"));
	//类型名称
	pLRechargeName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelTypeName"));
	//我的当前金币
	pLCurGoldCount=static_cast<UILabel*>(pUILayer->getWidgetByName("LabelCurGoldCount"));
	//我的当前元宝
	pLCurBigGoldCount = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelCurBigGoldCount"));
	//消息列表
	pLVRechargeList = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewRecharge"));
	//设置cell模式
	pLVRechargeList->setItemModel(pLVRechargeList->getItem(0));
	pLVRechargeList->removeAllItems();

	if (isShowExchangeGold)
	{
		onMenuChangeExchangeType(pBExchangeGold, TOUCH_EVENT_ENDED);
	}
	else
	{
		onMenuChangeExchangeType(pBExchangeBigGold, TOUCH_EVENT_ENDED);
	}
	//DataModel::sharedDataModel()->vecSystemMsg.push_back("1233");
	//更新列表
	updateListRecharge(vecRechargeGold);
	//获取财富 
	connectServer();
	connectSuccess();

	playAnimation();
}
void PopDialogBoxRecharge::onExit(){
	CCLayer::onExit();
}
//关闭窗口
void PopDialogBoxRecharge::onMenuCloseView(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		if (getIPopDialogBoxAssistCloseView())
		{
			getIPopDialogBoxAssistCloseView()->onCloseView();
		}
		this->removeFromParentAndCleanup(true);
	}
}
//更改兑换类型
void PopDialogBoxRecharge::onMenuChangeExchangeType(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton *)object;
		setRechargeType((RechargeType)pBTemp->getTag());
		switch (pBTemp->getTag())
		{
		case PopDialogBoxRecharge::RECHARGE_GOLD:
		{
			pIVCursor->stopAllActions();
			pIVCursor->runAction(CCMoveTo::create(0.1,ccp(-88,0)));
			pLRechargeName->setText("金币");
			//更新列表
			updateListRecharge(vecRechargeGold);
		}
			break;
		case PopDialogBoxRecharge::RECHARGE_BIG_GOLD:
		{
			pIVCursor->stopAllActions();
			pIVCursor->runAction(CCMoveTo::create(0.1, ccp(88, 0)));
			pLRechargeName->setText("元宝");
			//更新列表
			updateListRecharge(vecRechargeBigGold);
		}
			break;
		default:
			break;
		}
	}
}
//兑换按键
void PopDialogBoxRecharge::onMenuExchange(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton*)object;
		iCurSelectIndex = pBTemp->getTag();
		switch (eRechargeType)
		{
		case PopDialogBoxRecharge::RECHARGE_GOLD:
		{
			setRechargeActionType(R_Action_EXCHANGE);
			connectServer();
			connectSuccess();
		}
			break;
		case PopDialogBoxRecharge::RECHARGE_BIG_GOLD:
		{
			platformAction("{\"act\":300}");
		}
			break;
		default:
			break;
		}
	}
}
//更新兑换列表
void PopDialogBoxRecharge::updateListRecharge(std::vector<RechargeData> qMsg){
	pLVRechargeList->removeAllItems();
	
	int tempSize = qMsg.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		pLVRechargeList->insertDefaultItem(pLVRechargeList->getItems()->count());
		//背景框
		UIImageView *pIVItem = static_cast<UIImageView*>(pLVRechargeList->getItem(pLVRechargeList->getItems()->count() - 1)->getChildByName("ImageBg"));
		//兑换品图片
		UIImageView *pIVRechargeIcon = static_cast<UIImageView*>(pIVItem->getChildByName("ImageLight")->getChildByName("ImageRechargeIcon"));
		pIVRechargeIcon->loadTexture(qMsg[i].sImageIconName.c_str(), UI_TEX_TYPE_PLIST);
		
		//兑换按键
		UIButton *pButton = static_cast<UIButton*>(pIVItem->getChildByName("ButtonExchange"));
		pButton->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxRecharge::onMenuExchange));
		pButton->setTag(i);
		
		//价格
		UILabel *pLPice = static_cast<UILabel*>(pButton->getChildByName("LabelExchangePice"));
		pLPice->setText(CCString::createWithFormat("%ld", qMsg[i].lExchangePice)->getCString());
		//兑换币种
		UIImageView *pPiceType = static_cast<UIImageView*>(pButton->getChildByName("ImageExchangeIcon"));
		if (eRechargeType == RECHARGE_GOLD)
		{
			pPiceType->loadTexture("u_a_icon_yb.png", UI_TEX_TYPE_PLIST);
		}
		else if (eRechargeType==RECHARGE_BIG_GOLD)
		{
			pPiceType->loadTexture("u_a_icon_rmb.png", UI_TEX_TYPE_PLIST);
		}
		
		//兑换数量		
		UILabel *pLContent0 = static_cast<UILabel*>(pIVItem->getChildByName("LabelGoldNum0"));
		pLContent0->setText(qMsg[i].sExchangeNum.c_str());
		UILabel *pLContent1 = static_cast<UILabel*>(pIVItem->getChildByName("LabelGoldNum1"));
		pLContent1->setText(qMsg[i].sExchangeNum.c_str());
		
		
	}
}
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxRecharge::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//读取网络消息回调
void PopDialogBoxRecharge::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		connectSuccess();
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
void PopDialogBoxRecharge::connectSuccess(){
	switch (eRechargeActionType)
	{
	case R_Action_GET_MONEY:	//获取财富
	{
		CMD_GP_UserID userID;
		userID.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(userID.szPassword, md5PassWord.c_str());

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_TREASURE, &userID, sizeof(CMD_GP_UserID));
	}
	break;
	case R_Action_EXCHANGE:
	{
		CMD_GP_UserExchangeIngot userExchange;
		userExchange.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		userExchange.dwIngot = lGoldPice[iCurSelectIndex];

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(userExchange.szPassword, md5PassWord.c_str());


		strcpy(userExchange.szMachineID, platformAction("{\"act\":100}").c_str());

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_INGOT, &userExchange, sizeof(userExchange));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxRecharge::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_TREASURE://财富详细 
	{
		onSubTreasure(pDataBuffer, wDataSize);
	}
	break;
	case SUB_GP_EXCHANGE_INGOT://兑换元宝
	{
		onSubExchangeBigGold(pDataBuffer, wDataSize);
	}
		break;
	case SUB_GP_OPERATE_FAILURE:
	{
		CMD_GP_OperateFailure *pFailure = (CMD_GP_OperateFailure*)pDataBuffer;
		showTipInfo(GBKToUTF8(pFailure->szDescribeString));
	}
	break;
	default:
		CCLOG("   %d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//财富
void PopDialogBoxRecharge::onSubTreasure(void * pDataBuffer, unsigned short wDataSize){
	if (wDataSize != sizeof(CMD_GP_UserTreasure)) return;
	CMD_GP_UserTreasure * pUserTreasure = (CMD_GP_UserTreasure *)pDataBuffer;
	pLCurGoldCount->setText(CCString::createWithFormat("%lld", pUserTreasure->lInsureScore)->getCString());
	pLCurBigGoldCount->setText(CCString::createWithFormat("%lld", pUserTreasure->lIngotScore)->getCString());
	//pLVoucherCount->setText(CCString::createWithFormat("%lld", pUserTreasure->lLottery)->getCString());
}
//兑换元宝
void PopDialogBoxRecharge::onSubExchangeBigGold(void * pDataBuffer, unsigned short wDataSize){
	if (wDataSize > sizeof(CMD_GP_ExchangeIngotSuccess)) return;
	CMD_GP_ExchangeIngotSuccess *pExchangeSuccess = (CMD_GP_ExchangeIngotSuccess*)pDataBuffer;

	pLCurGoldCount->setText(CCString::createWithFormat("%lld", pExchangeSuccess->lInsure)->getCString());
	pLCurBigGoldCount->setText(CCString::createWithFormat("%lld", pExchangeSuccess->lIngot)->getCString());

	showTipInfo(GBKToUTF8(pExchangeSuccess->szDescribeString));

}
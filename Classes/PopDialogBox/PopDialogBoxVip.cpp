/*
 * PopDialogBoxVip.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxVip.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/MD5/MD5.h"
#include "PopDialogBoxRecharge.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxVip::PopDialogBoxVip()
	:eVipActionType(VIP_GET_LIST)
{
	scheduleUpdate();
}
PopDialogBoxVip::~PopDialogBoxVip() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_VIP);
	//gameSocket.Destroy(true);
}
void PopDialogBoxVip::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxVip.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//充值
	UIButton *pBRecharge = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonRecharge"));
	pBRecharge->addTouchEventListener(this, toucheventselector(PopDialogBoxVip::onMenuRecharge));

	//VIP等级icon
	pIVVipGradeIcon = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageVipGrade"));
	//VIP等级数
	pLAVipGradeNum = static_cast<UILabelAtlas*>(pUILayer->getWidgetByName("AtlasLabelVipGrade"));
	//VIP字
	pIVVipText = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageVipText"));
	//VIP奖励列表
	pLVVipReward = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewVip"));
	//VIP下一等级进度
	pLBVipNextGrade = static_cast<UILoadingBar*>(pUILayer->getWidgetByName("ProgressBarVipGrade"));
	pLBVipNextGrade->setPercent(0);
	//下一等级内容描述
	pLVipNextContent = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelVipNext"));
	pLVipNextContent->setText("");
	//设置默认VIP等级为0级
	setVipGrade(0);
	//
	updateListVip();
	//设置父节点不读取网络数据
	setLobbyReadMessage(false);
	//连接服务器
	connectServer();
	connectSuccess();
	//播放动画
	playAnimation();
}
void PopDialogBoxVip::onExit(){
	CCLayer::onExit();
}
//充值
void PopDialogBoxVip::onMenuRecharge(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		isReadMessage = false;
		PopDialogBoxRecharge *pRecharge = PopDialogBoxRecharge::create();
		this->addChild(pRecharge);
		pRecharge->setIPopDialogBoxAssistCloseView(this);
	}
}
//领取奖励按键
void PopDialogBoxVip::onMenuReward(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *pBTemp = (UIButton*)object;
		switch (pBTemp->getTag())
		{
		case REWARD_GOLD:
		case REWARD_RED_MONEY:
		{
			setVipActionType(VIP_REWARD);
			setRewardType((RewardType)pBTemp->getTag());
			connectServer();
			connectSuccess();
		}
			break;
		case REWARD_SHOP:
		{
			getIPopDialogBoxAssist()->onCloseViewToShop();
			this->removeFromParentAndCleanup(true);
		}
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}
//设置VIP等级
void PopDialogBoxVip::setVipGrade(int iGrade){
	if (iGrade==0)
	{
		pIVVipText->setVisible(false);
		pLAVipGradeNum->setVisible(false);
	}
	else
	{
		pIVVipText->setVisible(true);
		pLAVipGradeNum->setVisible(true);

		pIVVipGradeIcon->loadTexture("u_vip_icon1.png", UI_TEX_TYPE_PLIST);
		pLAVipGradeNum->setStringValue(CCString::createWithFormat("%d", iGrade)->getCString());
	}
}
//更新列表
void PopDialogBoxVip::updateListVip(){
	int colorH = 125;
	//登录金币奖励
	UILabel *pLRewardGold = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("ImageVipItem0")->getChildByName("LabelVipRewardNum"));
	pLRewardGold->setText(CCString::createWithFormat("%lld金币", vipPower.lLoginScore)->getCString());
	UIButton *pBRewardGold = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ImageVipItem0")->getChildByName("ButtonVipReward"));
	pBRewardGold->setTag(REWARD_GOLD);
	pBRewardGold->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxVip::onMenuReward));

	switch (vipPower.dwLoginScoreStatus)
	{
	case 0://无效
	{
		pBRewardGold->setColor(ccc3(colorH, colorH, colorH));
		pBRewardGold->setTitleColor(ccc3(colorH, colorH, colorH));
		pBRewardGold->setTouchEnabled(false);
	}
	break;
	case 1://未领取
	{
		pBRewardGold->setColor(ccc3(255, 255, 255));
		pBRewardGold->setTitleColor(ccc3(255, 255, 255));
		pBRewardGold->setTouchEnabled(true);
	}
		break;
	case 2://已领取
	{
		pBRewardGold->setColor(ccc3(colorH, colorH, colorH));
		pBRewardGold->setTitleColor(ccc3(colorH, colorH, colorH));
		pBRewardGold->setTouchEnabled(false);
		pBRewardGold->setTitleText(" 已领取 ");
	}
	break;
	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	//红包奖励
	UILabel *pLRewardRedMoney = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("ImageVipItem1")->getChildByName("LabelVipRewardNum"));
	if (vipPower.dwRedPieces != 0)
	{
		pLRewardRedMoney->setText(CCString::createWithFormat("%ld红包碎片", vipPower.dwRedPieces)->getCString());
	}
	else
	{
		pLRewardRedMoney->setText(CCString::createWithFormat("%ld红包", vipPower.dwRedPaper)->getCString());
	}
	
	UIButton *pBRewardRedMoney = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ImageVipItem1")->getChildByName("ButtonVipReward"));
	pBRewardRedMoney->setTag(REWARD_RED_MONEY);
	pBRewardRedMoney->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxVip::onMenuReward));

	switch (vipPower.dwRedPaperStatus)
	{
	case 0://无效
	{
		pBRewardRedMoney->setColor(ccc3(colorH, colorH, colorH));
		pBRewardRedMoney->setTitleColor(ccc3(colorH, colorH, colorH));
		pBRewardRedMoney->setTouchEnabled(false);
	}
	break;
	case 1://未领取
	{
		pBRewardRedMoney->setColor(ccc3(255, 255, 255));
		pBRewardRedMoney->setTitleColor(ccc3(255, 255, 255));
		pBRewardRedMoney->setTouchEnabled(true);
	}
		break;
	case 2://已领取
	{
		pBRewardRedMoney->setColor(ccc3(colorH, colorH, colorH));
		pBRewardRedMoney->setTitleColor(ccc3(colorH, colorH, colorH));
		pBRewardRedMoney->setTouchEnabled(false);
		pBRewardRedMoney->setTitleText(" 已领取 ");
	}
	break;
	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	//商城折扣
	UILabel *pLRewardDiscount = static_cast<UILabel *>(pLVVipReward->getItem(1)->getChildByName("ImageVipItem0")->getChildByName("LabelVipRewardNum"));
	pLRewardDiscount->setText(CCString::createWithFormat(" VIP享%ld折优惠 ", vipPower.dwShopping)->getCString());
	UIButton *pBShop = static_cast<UIButton *>(pLVVipReward->getItem(1)->getChildByName("ImageVipItem0")->getChildByName("ButtonVipReward"));
	pBShop->setTag(REWARD_SHOP);
	pBShop->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxVip::onMenuReward));
}
//更新
void PopDialogBoxVip::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData();
}
//////////////////////////////////////////////////////////////////////////
//网络消息
void PopDialogBoxVip::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET:
	{
		connectSuccess();
	}
		break;
	case MDM_GP_USER_SERVICE:
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);

		//移除loading
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//关闭网络
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_VIP);
		gameSocket.Destroy(true);
	}
		break;
	default:
		CCLOG("main:%d <<%s>>",wMainCmdID, __FUNCTION__);
		break;
	}
}
//用户服务
void PopDialogBoxVip::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_VIP_POWER:
	{
		onSubVipInfo(pDataBuffer,wDataSize);

	}
		break;
	case SUB_GP_VIP_AWARD:
	{
		onSubVipReward(pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLOG("wSubCmdID:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxVip::connectSuccess(){
	switch (eVipActionType)
	{
	case PopDialogBoxVip::VIP_GET_LIST:
	{		
		CMD_GP_VipUserID vipUserID;
		vipUserID.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		vipUserID.dwOpTerminal = 2;
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_VIP_POWER, &vipUserID, sizeof(vipUserID));
	}
		break;
	case PopDialogBoxVip::VIP_REWARD:
	{
		CMD_GP_VipAward vipAward;
		vipAward.dwAwardType = getRewardType();
		vipAward.dwOpTerminal = 2;
		vipAward.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(vipAward.szLogonPass, md5PassWord.c_str());

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_VIP_AWARD, &vipAward, sizeof(vipAward));
	}
		break;
	default:
		break;
	}
}
//VIP信息
void PopDialogBoxVip::onSubVipInfo(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_VipPower *pVipPower = (CMD_GP_VipPower*)pDataBuffer;
	memcpy(&vipPower, pVipPower, sizeof(CMD_GP_VipPower));
	//设置VIP等级
	setVipGrade(pVipPower->dwVipID);
	//下一VIP等级进度
	float fNextPercent = (float)pVipPower->dwIngot / pVipPower->dwAllIngot;
	pLBVipNextGrade->setPercent(fNextPercent*100);
	//设置下一级
	pLVipNextContent->setText(CCString::createWithFormat("还需要充值%ld元宝，您即可享VIP%ld特权",pVipPower->dwIngot,pVipPower->dwVipID+1)->getCString());

	//更新列表
	updateListVip();
}
//VIP领取奖励
void PopDialogBoxVip::onSubVipReward(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_VipAwardLog *pVipLog = (CMD_GP_VipAwardLog*)pDataBuffer;
	if (pVipLog->dwRet==0)
	{
		if (getRewardType()==REWARD_GOLD)
		{
			vipPower.dwLoginScoreStatus = 2;
		}
		else if (getRewardType() == REWARD_RED_MONEY)
		{
			vipPower.dwRedPaperStatus = 2;
		}
		updateListVip();
	}
	showTipInfo(GBKToUTF8(pVipLog->szDescribeString));
	CCLOG(" <<%s>>", __FUNCTION__);
}
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
	//VIP下一等级数
	pLANextVipGradeNum = static_cast<UILabelAtlas*>(pUILayer->getWidgetByName("AtlasLabelNextVipGrade"));
	//vip满级
	pIVFullLevel = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageFullLevel"));
	pIVFullLevel->setVisible(false);
	//VIP字
	pIVVipText = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageVipText"));
	//VIP下一等级字
	pIVNextVipText = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageNextVipText"));
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
	//vip复选框
	for (int i = 0; i < maxVipLevel; i++)
	{
		pCBVip[i] = static_cast<UICheckBox*>(pUILayer->getWidgetByName(CCString::createWithFormat("CheckBoxLevel%d",i+1)->getCString()));
		pCBVip[i]->addEventListenerCheckBox(this, SEL_SelectedStateEvent(&PopDialogBoxVip::onCheckBoxSelectedStateEvent));
		pCBVip[i]->setTag(i);
	}
	pCBVip[0]->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pCBVip[0], CHECKBOX_STATE_EVENT_SELECTED);
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
//vip复选框
void PopDialogBoxVip::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
	{
		UICheckBox *pCBTemp = (UICheckBox*)pSender;
		int iCurTag = pCBTemp->getTag();
		for (int i = 0; i < maxVipLevel; i++)
		{
			if (iCurTag == i)
			{
				pCBVip[i]->setTouchEnabled(false);
			}
			else
			{
				pCBVip[i]->setTouchEnabled(true);
				pCBVip[i]->setSelectedState(false);
			}
		}

		updateListVipByIndex(iCurTag);
		/*//设置奖励金币数
		UILabel *pLRewardGold = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("LabelVipRewardNum"));
		pLRewardGold->setText(CCString::createWithFormat("%lld金币", vipPower.VipPowerInfo[iCurTag].lLoginScore)->getCString());
	
		if (iCurTag != vipPower.dwVipID-1)
		{
			UIButton *pBRewardGold = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ButtonVipReward"));
			pBRewardGold->set
		}*/
	}
		break;
	case CHECKBOX_STATE_EVENT_UNSELECTED:
	{

	}
		break;
	default:
		break;
	}
}
//充值
void PopDialogBoxVip::onMenuRecharge(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		isReadMessage = false;
		PopDialogBoxRecharge *pRecharge = PopDialogBoxRecharge::create();
		pRecharge->isShowExchangeGold = false;
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
//设置VIP等级
void PopDialogBoxVip::setNextVipGrade(int iGrade){
	if (iGrade >=8)
	{
		pIVNextVipText->setVisible(false);
		pIVFullLevel->setVisible(true);
	}
	else
	{
		pIVNextVipText->setVisible(true);
		pIVFullLevel->setVisible(false);

		//pIVVipGradeIcon->loadTexture("u_vip_icon1.png", UI_TEX_TYPE_PLIST);
		pLANextVipGradeNum->setStringValue(CCString::createWithFormat("%d", iGrade)->getCString());
	}
}
//更新列表
void PopDialogBoxVip::updateListVip(){
	int colorH = 125;
	//登录金币奖励
	UILabel *pLRewardGold = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("LabelVipRewardNum"));
	pLRewardGold->setText(CCString::createWithFormat("%lld金币", vipPower.lLoginScore)->getCString());
	UIButton *pBRewardGold = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ButtonVipReward"));
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
	UILabel *pLRewardRedMoney = static_cast<UILabel *>(pLVVipReward->getItem(1)->getChildByName("LabelVipRewardNum"));
	if (vipPower.dwRedPieces != 0)
	{
		pLRewardRedMoney->setText(CCString::createWithFormat("%ld红包碎片", vipPower.dwRedPieces)->getCString());
	}
	else
	{
		pLRewardRedMoney->setText(CCString::createWithFormat("%ld红包", vipPower.dwRedPaper)->getCString());
	}
	
	UIButton *pBRewardRedMoney = static_cast<UIButton *>(pLVVipReward->getItem(1)->getChildByName("ButtonVipReward"));
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
	UILabel *pLRewardDiscount = static_cast<UILabel *>(pLVVipReward->getItem(2)->getChildByName("LabelVipRewardNum"));

	if (vipPower.dwShopping % 10 == 0)
	{
		pLRewardDiscount->setText(CCString::createWithFormat(" VIP享%.0f折优惠 ", vipPower.dwShopping/10.0)->getCString());
	}
	else
	{
		pLRewardDiscount->setText(CCString::createWithFormat(" VIP享%.1f折优惠 ", vipPower.dwShopping / 10.0)->getCString());
	}

	UIButton *pBShop = static_cast<UIButton *>(pLVVipReward->getItem(2)->getChildByName("ButtonVipReward"));
	pBShop->setTag(REWARD_SHOP);
	pBShop->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxVip::onMenuReward));
}
//更新列表
void PopDialogBoxVip::updateListVipByIndex(int index){
	int colorH = 125;
	//登录金币奖励
	UILabel *pLRewardGold = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("LabelVipRewardNum"));
	pLRewardGold->setText(CCString::createWithFormat("%lld金币", vipPower.VipPowerInfo[index].lLoginScore)->getCString());
	UIButton *pBRewardGold = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ButtonVipReward"));
	//金币图标
	UIImageView *pIVGoldIcon = static_cast<UIImageView *>(pLVVipReward->getItem(0)->getChildByName("ImageVipGoodsIcon")); 
	pIVGoldIcon->loadTexture(CCString::createWithFormat("u_vip_reward_gold%d.png",index)->getCString(), UI_TEX_TYPE_PLIST);
	if (index==vipPower.dwVipID-1)
	{
		setButtonState(pBRewardGold, vipPower.dwLoginScoreStatus);
	}
	else
	{
		setButtonState(pBRewardGold,0);
	}
	//////////////////////////////////////////////////////////////////////////
	//红包奖励
	UILabel *pLRewardRedMoney = static_cast<UILabel *>(pLVVipReward->getItem(1)->getChildByName("LabelVipRewardNum"));
	//红包图标
	UIImageView *pIVRedMoney = static_cast<UIImageView *>(pLVVipReward->getItem(1)->getChildByName("ImageVipGoodsIcon"));
	if (vipPower.VipPowerInfo[index].dwRedPieces != 0)
	{
		pLRewardRedMoney->setText(CCString::createWithFormat("%ld红包碎片", vipPower.VipPowerInfo[index].dwRedPieces)->getCString());
		pIVRedMoney->loadTexture("u_vip_reward_goods1.png", UI_TEX_TYPE_PLIST);
	}
	else
	{
		pLRewardRedMoney->setText(CCString::createWithFormat("%ld红包", vipPower.VipPowerInfo[index].dwRedPaper)->getCString());
		pIVRedMoney->loadTexture("u_vip_reward_goods11.png", UI_TEX_TYPE_PLIST);
	}

	UIButton *pBRewardRedMoney = static_cast<UIButton *>(pLVVipReward->getItem(1)->getChildByName("ButtonVipReward"));
	
	if (index == vipPower.dwVipID - 1)
	{
		setButtonState(pBRewardRedMoney, vipPower.dwRedPaperStatus);
	}
	else
	{
		setButtonState(pBRewardRedMoney, 0);
	}
	//////////////////////////////////////////////////////////////////////////
	//商城折扣
	UILabel *pLRewardDiscount = static_cast<UILabel *>(pLVVipReward->getItem(2)->getChildByName("LabelVipRewardNum"));

	if (vipPower.VipPowerInfo[index].dwShopping % 10 == 0)
	{
		pLRewardDiscount->setText(CCString::createWithFormat(" VIP享%.0f折优惠 ", vipPower.VipPowerInfo[index].dwShopping / 10.0)->getCString());
	}
	else
	{
		pLRewardDiscount->setText(CCString::createWithFormat(" VIP享%.1f折优惠 ", vipPower.VipPowerInfo[index].dwShopping / 10.0)->getCString());
	}
}
//设置按键状态
void PopDialogBoxVip::setButtonState(UIButton *button,int state){
	int colorH = 125;
	switch (state)
	{
	case 0://无效
	{
		button->setColor(ccc3(colorH, colorH, colorH));
		button->setTitleColor(ccc3(colorH, colorH, colorH));
		button->setTouchEnabled(false);
		button->setTitleText(" 领取奖励 ");
	}
	break;
	case 1://未领取
	{
		button->setColor(ccc3(255, 255, 255));
		button->setTitleColor(ccc3(255, 255, 255));
		button->setTouchEnabled(true);
		button->setTitleText(" 领取奖励 ");
	}
	break;
	case 2://已领取
	{
		button->setColor(ccc3(colorH, colorH, colorH));
		button->setTitleColor(ccc3(colorH, colorH, colorH));
		button->setTouchEnabled(false);
		button->setTitleText(" 已领取 ");
	}
	break;
	default:
		break;
	}
}
//更新
void PopDialogBoxVip::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
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
	setNextVipGrade(pVipPower->dwVipID + 1);
	if (pVipPower->dwVipID==7)
	{
		pLBVipNextGrade->setPercent(100);
		pLVipNextContent->setVisible(false);
	}
	else
	{
		//下一VIP等级进度
		float fNextPercent = (float)pVipPower->dwIngot / pVipPower->dwAllIngot;
		pLBVipNextGrade->setPercent(fNextPercent * 100);
		//设置下一级
		pLVipNextContent->setText(CCString::createWithFormat("还需要充值%ld元宝，您即可享VIP%ld特权", pVipPower->dwAllIngot - pVipPower->dwIngot, pVipPower->dwVipID + 1)->getCString());

	}
	//更新列表
	updateListVip();
	if (pVipPower->dwVipID>0)
	{
		//设置默认选择
		pCBVip[pVipPower->dwVipID - 1]->setSelectedState(true);
		onCheckBoxSelectedStateEvent(pCBVip[pVipPower->dwVipID - 1], CHECKBOX_STATE_EVENT_SELECTED);
	}
	
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
	showTipInfo(GBKToUTF8(pVipLog->szDescribeString).c_str());
	CCLOG(" <<%s>>", __FUNCTION__);
}
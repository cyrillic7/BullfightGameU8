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
	nVipNextRewordTip[0] = 10;
	nVipNextRewordTip[1] = 100;
	nVipNextRewordTip[2] = 500;
	nVipNextRewordTip[3] = 2000;
	nVipNextRewordTip[4] = 5000;
	nVipNextRewordTip[5] = 20000;
	nVipNextRewordTip[6] = 50000;
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
	pIVVipNextGradeIcon = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageVipGrade0"));
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
	//激活
	pIVActivationTip = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageVipInfoBg"));
	pIVActivationTip->setVisible(false);
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
		iCurTag = pCBTemp->getTag();
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

		//UILabel *pLVipInfoContent = static_cast<UILabel*>(pIVActivationTip->getChildByName("LabelVipInfoContent"));
		//pLVipInfoContent->setVisible(false);
		pIVActivationTip->setVisible(false);

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
			/*CCString *strData = (CCString*)pBTemp->getUserData();
			if (strcmp(strData->getCString(), "state3") == 0)
			{
				CCLOG("state3333333 <<%s>>", __FUNCTION__);
			}
			else
			{*/
				setVipActionType(VIP_REWARD);
				setRewardType((RewardType)pBTemp->getTag());
				connectServer();
				connectSuccess();
			/*}*/
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
//领取奖励按键
void PopDialogBoxVip::onMenuActivation(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton*)object;
		switch (pBTemp->getTag())
		{
		case REWARD_GOLD:
		case REWARD_RED_MONEY:
		{
			CCPoint posTip;
			posTip = ccpSub(pBTemp->getWorldPosition(), pIVActivationTip->getParent()->getPosition());
			posTip = ccpAdd(posTip, ccp(-80,42));
			pIVActivationTip->setVisible(true);
			pIVActivationTip->setPosition(posTip);
			UILabel *pLVipInfoContent = static_cast<UILabel*>(pIVActivationTip->getChildByName("LabelVipInfoContent"));
			pLVipInfoContent->setText(CCString::createWithFormat("还需充值%d元宝领福利哦~", nVipNextRewordTip[iCurTag])->getCString());
			pIVActivationTip->setSize(CCSize(pLVipInfoContent->getSize().width+20,pIVActivationTip->getContentSize().height));
			
			pIVActivationTip->setOpacity(255);
			pIVActivationTip->setScale(0);
			CCSequence *pSeq = CCSequence::create(
				CCEaseBackOut::create(CCScaleTo::create(0.2, 1)),
				CCDelayTime::create(1),
				CCFadeOut::create(0.5),
				NULL);
			pIVActivationTip->stopAllActions();
			pIVActivationTip->runAction(pSeq);

			pLVipInfoContent->setOpacity(255);
			pLVipInfoContent->setScale(0);
			CCSequence *pSeq1 = CCSequence::create(
				CCEaseBackOut::create(CCScaleTo::create(0.2, 1)),
				CCDelayTime::create(1),
				CCFadeOut::create(0.5),
				NULL);
			pLVipInfoContent->stopAllActions();
			pLVipInfoContent->runAction(pSeq1);
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
}
//设置VIP等级
void PopDialogBoxVip::setVipGrade(int iGrade){
	pIVVipGradeIcon->loadTexture(CCString::createWithFormat("icon_vip%d.png",iGrade)->getCString(), UI_TEX_TYPE_PLIST);
	/*if (iGrade==0)
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
	}*/
}
//设置VIP等级
void PopDialogBoxVip::setNextVipGrade(int iGrade){
	if (iGrade>=8)
	{
	}
	else
	{
		pIVVipNextGradeIcon->loadTexture(CCString::createWithFormat("icon_vip%d.png", iGrade)->getCString(), UI_TEX_TYPE_PLIST);
	}
	/*if (iGrade >=8)
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
	}*/
}
//更新列表
void PopDialogBoxVip::updateListVip(){
	int colorH = 125;
	//登录金币奖励
	UILabel *pLRewardGold = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("LabelVipRewardNum"));
	pLRewardGold->setText(CCString::createWithFormat("%lld金币", vipPower.lLoginScore)->getCString());
	UIButton *pBRewardGold = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ButtonVipReward"));
	pBRewardGold->setTag(REWARD_GOLD);
	

	if (vipPower.dwVipID == 0)
	{
		setButtonState(pBRewardGold, 3);
	}
	else
	{
		setButtonState(pBRewardGold, vipPower.dwLoginScoreStatus);
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
	
	if (vipPower.dwVipID == 0)
	{
		setButtonState(pBRewardRedMoney, 3);
	}
	else
	{
		setButtonState(pBRewardRedMoney, vipPower.dwLoginScoreStatus);
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
//重设按键类型
void PopDialogBoxVip::resetRewordButtonType(UIButton *button, long dwVipID, long dwLoginScoreStatus){

}
//更新列表
void PopDialogBoxVip::updateListVipByIndex(int index){
	int colorH = 125;
	//登录金币奖励
	UILabel *pLRewardGold = static_cast<UILabel *>(pLVVipReward->getItem(0)->getChildByName("LabelVipRewardNum"));
	pLRewardGold->setText(CCString::createWithFormat("%lld金币", vipPower.VipPowerInfo[index].lLoginScore)->getCString());
	UIButton *pBRewardGold = static_cast<UIButton *>(pLVVipReward->getItem(0)->getChildByName("ButtonVipReward"));
	//金币图标
	UIImageView *pIVGoldIcon = static_cast<UIImageView *>(pLVVipReward->getItem(0)->getChildByName("ImageKuang")->getChildByName("ImageVipGoodsIcon"));
	pIVGoldIcon->loadTexture(CCString::createWithFormat("u_vip_reward_gold%d.png",index)->getCString(), UI_TEX_TYPE_PLIST);
	if (index==vipPower.dwVipID-1)
	{
		setButtonState(pBRewardGold, vipPower.dwLoginScoreStatus);
	}
	else
	{
		if (vipPower.dwVipID==0)
		{
			setButtonState(pBRewardGold, 3);
		}
		else
		{
			if (index < vipPower.dwVipID - 1)
			{
				setButtonState(pBRewardGold, 0);
			}
			else
			{
				setButtonState(pBRewardGold, 3);
			}
		}
		
	}
	//////////////////////////////////////////////////////////////////////////
	//红包奖励
	UILabel *pLRewardRedMoney = static_cast<UILabel *>(pLVVipReward->getItem(1)->getChildByName("LabelVipRewardNum"));
	//红包图标
	UIImageView *pIVRedMoney = static_cast<UIImageView *>(pLVVipReward->getItem(1)->getChildByName("ImageKuang")->getChildByName("ImageVipGoodsIcon"));
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
	
	/*if (index == vipPower.dwVipID - 1)
	{
		setButtonState(pBRewardRedMoney, vipPower.dwRedPaperStatus);
	}
	else
	{
		setButtonState(pBRewardRedMoney, 0);
	}*/
	if (index == vipPower.dwVipID - 1)
	{
		setButtonState(pBRewardRedMoney, vipPower.dwRedPaperStatus);
	}
	else
	{
		if (vipPower.dwVipID == 0)
		{
			setButtonState(pBRewardRedMoney, 3);
		}
		else
		{
			if (index < vipPower.dwVipID - 1)
			{
				setButtonState(pBRewardRedMoney, 0);
			}
			else
			{
				setButtonState(pBRewardRedMoney, 3);
			}
		}

		

	}
	//////////////////////////////////////////////////////////////////////////
	//商城折扣
	UILabel *pLRewardDiscount = static_cast<UILabel *>(pLVVipReward->getItem(2)->getChildByName("LabelVipRewardNum"));
	//vip折扣图标
	UIImageView *pIVDiscountIcon = static_cast<UIImageView *>(pLVVipReward->getItem(2)->getChildByName("ImageKuang")->getChildByName("ImageVipGoodsIcon"));
	pIVDiscountIcon->loadTexture(CCString::createWithFormat("u_vip_discount_%d.png", index+1)->getCString(), UI_TEX_TYPE_PLIST);
	
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
	//button->setUserData(CCString::createWithFormat("state%d",state));
	switch (state)
	{
	case 0://无效
	{
		button->loadTextureNormal("LQJL_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		button->loadTexturePressed("LQJL_Btn_Down.png", UI_TEX_TYPE_PLIST);

		button->setColor(ccc3(colorH, colorH, colorH));
		button->setTitleColor(ccc3(colorH, colorH, colorH));
		button->setTouchEnabled(false);
		//button->setTitleText(" 领取奖励 ");
	}
	break;
	case 1://未领取
	{
		button->loadTextureNormal("LQJL_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		button->loadTexturePressed("LQJL_Btn_Down.png", UI_TEX_TYPE_PLIST);

		button->setColor(ccc3(255, 255, 255));
		button->setTitleColor(ccc3(255, 255, 255));
		button->setTouchEnabled(true);
		button->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxVip::onMenuReward));
		//button->setTitleText(" 领取奖励 ");
	}
	break;
	case 2://已领取
	{
		button->loadTextureNormal("YLQ_Btn_Down.png", UI_TEX_TYPE_PLIST);
		button->loadTexturePressed("YLQ_Btn_Down.png", UI_TEX_TYPE_PLIST);

		button->setColor(ccc3(colorH, colorH, colorH));
		button->setTitleColor(ccc3(colorH, colorH, colorH));
		button->setTouchEnabled(false);
		//button->setTitleText(" 已领取 ");
	}
	break;
	case 3:
	{
		button->loadTextureNormal("JHJL_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		button->loadTexturePressed("JHJL_Btn_Down.png", UI_TEX_TYPE_PLIST);
		button->setColor(ccc3(255, 255, 255));
		button->setTouchEnabled(true);

		button->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxVip::onMenuActivation));
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
		pLVipNextContent->setText(CCString::createWithFormat("还需充值%ld元宝，即可成为VIP%ld，福利翻倍哦", pVipPower->dwAllIngot - pVipPower->dwIngot, pVipPower->dwVipID + 1)->getCString());

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
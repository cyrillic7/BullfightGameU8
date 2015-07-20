/*
 * PopDialogBoxUserInfo.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxUserInfo.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/BaseAttributes.h"
#include "../Tools/Tools.h"
#include "PopDialogBoxRecharge.h"
#include "../Network/MD5/MD5.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxUserInfo::PopDialogBoxUserInfo()
:isShowChange(false)
, eUserInfoType(USER_GET_MONEY)
{
	scheduleUpdate();
}
PopDialogBoxUserInfo::~PopDialogBoxUserInfo() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_USER_INFO);
	//gameSocket.Destroy(true);
}
void PopDialogBoxUserInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUserInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
    
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);

	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//兑换
	UIButton *pBExchange = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonExchange"));
	pBExchange->addTouchEventListener(this, toucheventselector(PopDialogBoxUserInfo::onMenuExchange));
	//充值
	UIButton *pBRecharge = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonAddGold"));
	pBRecharge->addTouchEventListener(this, toucheventselector(PopDialogBoxUserInfo::onMenuRecharge));

	pBRecharge = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonAddBigGold"));
	pBRecharge->addTouchEventListener(this, toucheventselector(PopDialogBoxUserInfo::onMenuRecharge));
	//绑定手机
	pBBindingPhone = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBindPhone"));
	pBBindingPhone->addTouchEventListener(this, toucheventselector(PopDialogBoxUserInfo::onMenuBindingPhone));
	resetBindingButton();
	
	//设置游戏ID
	UILabel *labelUserID=static_cast<UILabel*>(pUILayer->getWidgetByName("LabelUserID"));
	labelUserID->setText(CCString::createWithFormat("ID:%ld",DataModel::sharedDataModel()->userInfo->dwGameID)->getCString());
	//昵称输入框
	pLabelNickName=static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldNickName"));
	pLabelNickName->setText(Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName));
	pLabelNickName->setTouchEnabled(false);
	//性别选择
	pcbSexGirl=static_cast<UICheckBox*>(pUILayer->getWidgetByName("CheckBoxSexGirl"));
	pcbSexGirl->addEventListenerCheckBox(this,SEL_SelectedStateEvent(&PopDialogBoxUserInfo::onCheckBoxSelectedStateEvent));
	plSexBoyInfo=static_cast<UILabel*>(pUILayer->getWidgetByName("LabelSexBoyInfo"));

	pcbSexBoy=static_cast<UICheckBox*>(pUILayer->getWidgetByName("CheckBoxSexBoy"));
	pcbSexBoy->addEventListenerCheckBox(this,SEL_SelectedStateEvent(&PopDialogBoxUserInfo::onCheckBoxSelectedStateEvent));
	//金币数
	pLGoldCount= static_cast<UILabel*>(pUILayer->getWidgetByName("LabelGoldCount"));
	//元宝数
	pLBigGoldCount = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelBigGoldCount"));
	//奖券数
	pLVoucherCount = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelVoucherCount"));
	//修改按键
	bChange=static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonChange"));
	bChange->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxUserInfo::onMenuChange));
	//修改昵称时背景
	piNickNameBg=static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageChangeBg"));

	piSexIcon=static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageSexIcon"));

	ppSexInfo=static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelSexInfo"));
	ppSexSelect=static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelSexSelect"));
	updateSex();
	setShowChangeView();
	//设置大厅不读取数据
	setLobbyReadMessage(false);

	//连接服务
	connectServer();
	connectSuccess();

	playAnimation();
}
void PopDialogBoxUserInfo::onExit(){
	CCLayer::onExit();
}
//重设绑定按键
void PopDialogBoxUserInfo::resetBindingButton(){
	UIImageView *pIVBindingFinish = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageBinding"));
	if (DataModel::sharedDataModel()->sPhone.length() > 0)
	{
		pBBindingPhone->setEnabled(false);
		pIVBindingFinish->setVisible(true);
	}
	else
	{
		pIVBindingFinish->setVisible(false);
	}
}
//关闭绑定手机回调
void PopDialogBoxUserInfo::onCloseBindingPhone(){
	isReadMessage = true;
	resetBindingButton();
}
//关闭窗口回调
void PopDialogBoxUserInfo::onCloseView(){
	isReadMessage = true;
}
//兑换
void PopDialogBoxUserInfo::onMenuExchange(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		if (getIPopDialogBoxAssist())
		{
			getIPopDialogBoxAssist()->onCloseViewToShop();
		}
		this->removeFromParentAndCleanup(true);
	}
}
//充值
void PopDialogBoxUserInfo::onMenuRecharge(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton*)object;
		isReadMessage = false;
		PopDialogBoxRecharge *pRecharge = PopDialogBoxRecharge::create();
		if (strcmp(pBTemp->getName(), "ButtonAddGold") == 0)
		{
			pRecharge->isShowExchangeGold = true;
		}
		else
		{
			pRecharge->isShowExchangeGold = false;
		}
		
		this->addChild(pRecharge);
		pRecharge->setIPopDialogBoxAssistCloseView(this);
	}
}
void PopDialogBoxUserInfo::onMenuChange(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		isShowChange=!isShowChange;
		if (!isShowChange)
		{
			
			setUserInfoType(USER_CHANGE_INFO);
			connectServer();
			connectSuccess();
		}
		else
		{
			
		}
		setShowChangeView();
	}
}
//绑定手机按键
void PopDialogBoxUserInfo::onMenuBindingPhone(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		isReadMessage = false;
		PopDialogBoxBindingPhone *pPopBoxBinding = PopDialogBoxBindingPhone::create();
		pPopBoxBinding->setIPopAssistBindingPhone(this);
		this->addChild(pPopBoxBinding);
	}
}
//设置显示修改窗口
void PopDialogBoxUserInfo::setShowChangeView(){
	
	if (isShowChange)
	{
		//pLabelNickName->setTouchEnabled(true);
		//pLabelNickName->setColor(ccc3(118,65,20));onCheckBoxSelectedStateEvent
		//piNickNameBg->setVisible(true);
		onCheckBoxSelectedStateEvent(pcbSexBoy, DataModel::sharedDataModel()->userInfo->cbGender == 1 ? CHECKBOX_STATE_EVENT_SELECTED : CHECKBOX_STATE_EVENT_UNSELECTED);
		onCheckBoxSelectedStateEvent(pcbSexGirl, DataModel::sharedDataModel()->userInfo->cbGender == 0 ? CHECKBOX_STATE_EVENT_SELECTED : CHECKBOX_STATE_EVENT_UNSELECTED);


		bChange->setTitleText("保存");

		ppSexInfo->setEnabled(false);
		ppSexSelect->setEnabled(true);
		ppSexSelect->setVisible(true);
	}else
	{
		//pLabelNickName->setTouchEnabled(false);
		//pLabelNickName->setColor(ccc3(255,255,255));
		//piNickNameBg->setVisible(false);
		
		bChange->setTitleText("修改");

		ppSexInfo->setEnabled(true);
		ppSexSelect->setEnabled(false);
		
		
	}
	updateSex();
}
void PopDialogBoxUserInfo::updateSex(){
	if (DataModel::sharedDataModel()->userInfo->cbGender==1)
	{
		piSexIcon->loadTexture("u_info_icon_boy.png",UI_TEX_TYPE_PLIST);
		plSexBoyInfo->setText(BaseAttributes::sharedAttributes()->sSexBoyName);
		if (isShowChange)
		{
			pcbSexBoy->setSelectedState(true);
			pcbSexGirl->setSelectedState(false);
			pcbSexBoy->setTouchEnabled(false);
		}
	}else
	{
		piSexIcon->loadTexture("u_info_icon_girl.png",UI_TEX_TYPE_PLIST);
		plSexBoyInfo->setText(BaseAttributes::sharedAttributes()->sSexGirlName);
		if (isShowChange)
		{
			pcbSexBoy->setSelectedState(false);
			pcbSexGirl->setSelectedState(true);
			pcbSexGirl->setTouchEnabled(false);
		}
		
	}
}
void PopDialogBoxUserInfo::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
		{
			UICheckBox *box=(UICheckBox*)pSender;
			box->setTouchEnabled(false);
			if (box->getTag()==1)
			{
				pcbSexGirl->setTouchEnabled(true);
				pcbSexGirl->setSelectedState(false);
			}else{
				pcbSexBoy->setTouchEnabled(true);
				pcbSexBoy->setSelectedState(false);
			}
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
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxUserInfo::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//读取网络消息回调
void PopDialogBoxUserInfo::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
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
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_USER_INFO);
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxUserInfo::connectSuccess(){
	switch (eUserInfoType)
	{
	case PopDialogBoxUserInfo::USER_GET_MONEY:	//获取财富
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
	case PopDialogBoxUserInfo::USER_CHANGE_INFO:
	{
		CMD_GP_ModifyIndividual modifyIndividual;
		modifyIndividual.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		//modifyIndividual.dwUserID = 0;
		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(modifyIndividual.szPassword, md5PassWord.c_str());
		
		modifyIndividual.cbGender = pcbSexBoy->getSelectedState() ? 1 : 0;
		//modifyIndividual.cbGender = DataModel::sharedDataModel()->userInfo->cbGender;

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INDIVIDUAL, &modifyIndividual, sizeof(modifyIndividual));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxUserInfo::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_TREASURE://财富详细 
	{
		onSubTreasure(pDataBuffer, wDataSize);
	}
	break;
	case SUB_GP_OPERATE_SUCCESS:
	{
		CMD_GP_OperateSuccess *pSuccess = (CMD_GP_OperateSuccess*)pDataBuffer;
		showTipInfo(GBKToUTF8(pSuccess->szDescribeString));

		DataModel::sharedDataModel()->userInfo->cbGender = pcbSexBoy->getSelectedState() ? 1 :0;
		setShowChangeView();

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
void PopDialogBoxUserInfo::onSubTreasure(void * pDataBuffer, unsigned short wDataSize){
	if (wDataSize != sizeof(CMD_GP_UserTreasure)) return;
	CMD_GP_UserTreasure * pUserTreasure = (CMD_GP_UserTreasure *)pDataBuffer;
	pLGoldCount->setText(CCString::createWithFormat("%lld",DataModel::sharedDataModel()->userInfo->lScore)->getCString());
	pLBigGoldCount->setText(CCString::createWithFormat("%lld", pUserTreasure->lIngotScore)->getCString());
	pLVoucherCount->setText(CCString::createWithFormat("%lld",pUserTreasure->lLottery)->getCString());
}
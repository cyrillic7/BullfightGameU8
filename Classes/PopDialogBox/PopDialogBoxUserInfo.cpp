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
#include "PopDialogBoxBindingPhone.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxUserInfo::PopDialogBoxUserInfo()
:isShowChange(false)
{
	
}
PopDialogBoxUserInfo::~PopDialogBoxUserInfo() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxUserInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUserInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
    
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);

	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//绑定手机
	UIButton *pBBindingPhone = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBindPhone"));
	pBBindingPhone->addTouchEventListener(this, toucheventselector(PopDialogBoxUserInfo::onMenuBindingPhone));
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
	//修改按键
	UIButton *bChange=static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonChange"));
	bChange->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxUserInfo::menuChange));
	//修改昵称时背景
	piNickNameBg=static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageChangeBg"));

	piSexIcon=static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageSexIcon"));

	ppSexInfo=static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelSexInfo"));
	ppSexSelect=static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelSexSelect"));
	updateSex();
	setShowChangeView();

	playAnimation();
}
void PopDialogBoxUserInfo::onExit(){
	CCLayer::onExit();
}

void PopDialogBoxUserInfo::menuChange(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		isShowChange=!isShowChange;
		if (!isShowChange)
		{
			DataModel::sharedDataModel()->userInfo->cbGender=pcbSexBoy->getSelectedState()?1:2;
		}
		setShowChangeView();
	}
}
//绑定手机按键
void PopDialogBoxUserInfo::onMenuBindingPhone(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		PopDialogBoxBindingPhone *pPopBoxBinding = PopDialogBoxBindingPhone::create();
		this->addChild(pPopBoxBinding);
	}
}
//设置显示修改窗口
void PopDialogBoxUserInfo::setShowChangeView(){
	
	if (isShowChange)
	{
		pLabelNickName->setTouchEnabled(true);
		pLabelNickName->setColor(ccc3(118,65,20));
		piNickNameBg->setVisible(true);


		ppSexInfo->setEnabled(false);
		ppSexSelect->setEnabled(true);
		ppSexSelect->setVisible(true);
	}else
	{
		pLabelNickName->setTouchEnabled(false);
		pLabelNickName->setColor(ccc3(255,255,255));
		piNickNameBg->setVisible(false);

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

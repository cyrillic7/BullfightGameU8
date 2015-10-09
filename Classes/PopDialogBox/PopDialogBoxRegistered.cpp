/*
 * PopDialogBoxRegistered.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxRegistered.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxRegistered::PopDialogBoxRegistered()
{
	
    
}
PopDialogBoxRegistered::~PopDialogBoxRegistered() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxRegistered::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxRegistered.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//帐号
	pTFAccount = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldAccount"));
	addEditBox(pTFAccount, kEditBoxInputModeSingleLine);
	//昵称
	pTFNickname = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldNickname"));
	addEditBox(pTFNickname, kEditBoxInputModeSingleLine);
	//密码
	pTFPassword = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldPassword"));
	addEditBox(pTFPassword, kEditBoxInputModeSingleLine);
	//注册
	UIButton *pBRegistered = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonRegistered"));
	pBRegistered->addTouchEventListener(this, toucheventselector(PopDialogBoxRegistered::onMenuRegistered));
	//帐号
	//播放动画
	playAnimation();
}
void PopDialogBoxRegistered::onExit(){
	CCLayer::onExit();
}
//注册
void PopDialogBoxRegistered::onMenuRegistered(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		CCEditBox *pEBAccount = (CCEditBox*)pTFAccount->getNodeByTag(TAG_INPUT_EDIT_BOX);
		CCEditBox *pEBNickname = (CCEditBox*)pTFNickname->getNodeByTag(TAG_INPUT_EDIT_BOX);
		CCEditBox *pEBPassword = (CCEditBox*)pTFPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string password0 = pEBPassword->getText();
		if (strlen(pEBAccount->getText())<6)
		{
			showTipInfo("游戏帐号必须大于6位以上!");
		}
		else if (isAllChniese(pEBAccount->getText()))
		{
			showTipInfo("游戏帐号不能包含中文!");
		}
		else if (strlen(pEBNickname->getText())<6)
		{
			showTipInfo("游戏昵称必须大于6位以上!");
		}
		else if (strlen(pEBPassword->getText()) < 8)
		{
			showTipInfo("密码必须大于8位以上!");
		}
		else if (isAllChniese(pEBPassword->getText()))
		{
			showTipInfo("密码不能包含中文!");
		}
		else if (password0.find_first_not_of("1234567890") == std::string::npos)
		{
			showTipInfo("密码不能全是数字!");
		}
		else
		{
			pIPopAssistRegistered->onRegistered(pEBAccount->getText(),pEBNickname->getText(),pEBPassword->getText());
		}
	}
}
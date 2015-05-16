/*
 * PopDialogBoxLogonAccount.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxLogonAccount.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../LogonScene/LogonScene.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxLogonAccount::PopDialogBoxLogonAccount()
{
	
}
PopDialogBoxLogonAccount::~PopDialogBoxLogonAccount() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxLogonAccount::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxLogonAccount.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	//获取背景图片容器
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	//设置初始大小
	pWidgetBg->setScale(0.8);
	//绑定关闭按键
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//绑定登录按键
	backButton=static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonLogonAccount"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxLogonAccount::onMenuLogon));
	//绑定帐号文本输入框
	pTAccount=static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldAccount"));
	pTAccount->addEventListenerTextField(this,SEL_TextFieldEvent(&PopDialogBoxLogonAccount::onTextFieldAccount));
	pTAccount->setText(DataModel::sharedDataModel()->sLogonAccount);
	//绑定密码文本输入框
	pTPassword=static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldPassword"));
	pTPassword->addEventListenerTextField(this,SEL_TextFieldEvent(&PopDialogBoxLogonAccount::onTextFieldAccount));
	pTPassword->setText(DataModel::sharedDataModel()->sLogonPassword);
	//播放显示动画
	playAnimation();
}
void PopDialogBoxLogonAccount::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxLogonAccount::onTextFieldAccount(CCObject* obj, TextFiledEventType type){
	switch (type)
	{
	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
		//CCLog("attach<<%s>>",__FUNCTION__);
		break;
	case TEXTFIELD_EVENT_DETACH_WITH_IME:
		{
			UITextField *pTF=(UITextField*)obj;
			if (strcmp(pTF->getName(),"TextFieldAccount")==0)
			{
				DataModel::sharedDataModel()->sLogonAccount=pTF->getStringValue();
			}else if(strcmp(pTF->getName(),"TextFieldPassword")==0)
			{
				DataModel::sharedDataModel()->sLogonPassword=pTF->getStringValue();
			}
		}
		//CCLog("detach<<%s>>",__FUNCTION__);
		break;
	case TEXTFIELD_EVENT_INSERT_TEXT:
		//CCLog("insert<<%s>>",__FUNCTION__);
		break;
	case TEXTFIELD_EVENT_DELETE_BACKWARD:
		//CCLog("delete<<%s>>",__FUNCTION__);
		break;
	default:
		break;
	}
}
void PopDialogBoxLogonAccount::onMenuLogon(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			if (strcmp(pTAccount->getStringValue(),"")==0||strcmp(pTPassword->getStringValue(),"")==0)
			{
				CCLog("帐号、密码不能为空<<%s>>",__FUNCTION__);
			}else
			{
				((LogonScene*)this->getParent())->logonGameByAccount();
			}
		}
		break;
	default:
		break;
	}
}
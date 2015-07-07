/*
 * PopDialogBoxForgetPassword.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxForgetPassword.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxForgetPassword::PopDialogBoxForgetPassword()
	:isInputId(true)
{
}
PopDialogBoxForgetPassword::~PopDialogBoxForgetPassword() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxForgetPassword::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxForgetPassword.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	//输入帐号容器
	pPInputId = static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelInputId"));
	pPInputId->setEnabled(true);
	//找到密码容器
	pPForgetPwd = static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelChangePwd"));
	pPForgetPwd->setEnabled(false);
	//输入帐号框
	UITextField *pTFInputId = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldId")); 
	addEditBox(pTFInputId, kEditBoxInputModeAny);

	playAnimation();
}
void PopDialogBoxForgetPassword::onExit(){
	CCLayer::onExit();
}
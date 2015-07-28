/*
 * PopDialogBoxFeedback.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxFeedback.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxFeedback::PopDialogBoxFeedback()
{
	
    
}
PopDialogBoxFeedback::~PopDialogBoxFeedback() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxFeedback::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxFeedback.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));

	//反馈按键
	UIButton *pBFeedback = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonFeedback"));
	pBFeedback->addTouchEventListener(this, toucheventselector(PopDialogBoxFeedback::onMenuFeedback));
	
	//反馈内容输入框
	UITextField *pTFFeedbackContent = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldContent"));
	addEditBox(pTFFeedbackContent,kEditBoxInputModeAny);
	pEBFeedbackContent = (CCEditBox*)pTFFeedbackContent->getNodeByTag(TAG_INPUT_EDIT_BOX);

	pEBFeedbackContent->setEnabled(false);
	pEBFeedbackContent->setVisible(false);
	
	UIImageView *pIVInputContent = static_cast<UIImageView*>(pTFFeedbackContent->getParent());
	pIVInputContent->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxFeedback::onMenuInputContent));
	//
	

	//联系方式
	UITextField *pTFFeedbackQQ = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldQQ"));
	addEditBox(pTFFeedbackQQ, kEditBoxInputModePhoneNumber);
	pEBFeedbackQQ = (CCEditBox*)pTFFeedbackQQ->getNodeByTag(TAG_INPUT_EDIT_BOX);
	pEBFeedbackQQ->setEnabled(false);
	pEBFeedbackQQ->setVisible(false);

	UIImageView *pIVInputContact = static_cast<UIImageView*>(pTFFeedbackQQ->getParent());
	pIVInputContact->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxFeedback::onMenuInputContact));

	playAnimation();
}
void PopDialogBoxFeedback::onExit(){
	CCLayer::onExit();
}
//反馈////////////////////////////////////////////////////////////////////////
void PopDialogBoxFeedback::onMenuFeedback(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		showTipInfo("敬请期待");
	}
}
//输入内容
void PopDialogBoxFeedback::onMenuInputContent(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		pEBFeedbackContent->touchDownAction(NULL, NULL); //弹出键盘  
	}
}
//输入联系方式
void PopDialogBoxFeedback::onMenuInputContact(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		pEBFeedbackQQ->touchDownAction(NULL, NULL); //弹出键盘  
	}
}
//输入回调
void PopDialogBoxFeedback::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text){
	UITextField *pTFFeedbackContent = (UITextField*)editBox->getParent();
	pTFFeedbackContent->setText(editBox->getText());
}
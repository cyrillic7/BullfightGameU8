/*
 * PopDialogBoxShare.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxShare.h"
#include "../Platform/coPlatform.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxShare::PopDialogBoxShare()
{
	
}
PopDialogBoxShare::~PopDialogBoxShare() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxShare::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxShare.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	//分享朋友圈
	UIButton *pBShareFriend = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonShareFriend"));
	pBShareFriend->addTouchEventListener(this, toucheventselector(PopDialogBoxShare::onMenuShareFriend));
	//分享微信
	UIButton *pBShareWX = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonShareWX"));
	pBShareWX->addTouchEventListener(this, toucheventselector(PopDialogBoxShare::onMenuShareWX));
	
	playAnimation();
}
void PopDialogBoxShare::onExit(){
	CCLayer::onExit();
}
//分享朋友圈////////////////////////////////////////////////////////////////////////
void PopDialogBoxShare::onMenuShareFriend(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		platformAction("{\"act\":900}");
		this->removeFromParentAndCleanup(true);
	}
}
//分享微信
void PopDialogBoxShare::onMenuShareWX(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		platformAction("{\"act\":901}");
		this->removeFromParentAndCleanup(true);
	}
}
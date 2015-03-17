/*
 * PopDialogBoxUserInfo.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxUserInfo.h"
PopDialogBoxUserInfo::PopDialogBoxUserInfo()
{
	
}
PopDialogBoxUserInfo::~PopDialogBoxUserInfo() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxUserInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUserInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	imageBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	imageBg->setScale(0.8);

	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
}
void PopDialogBoxUserInfo::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxUserInfo::playAnimation(){
	imageBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
	//pUILayer
}
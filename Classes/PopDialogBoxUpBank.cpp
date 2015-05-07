/*
 * PopDialogBoxUpBank.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxUpBank.h"
#include "DataModel.h"
#include "GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxUpBank::PopDialogBoxUpBank()
{
	
}
PopDialogBoxUpBank::~PopDialogBoxUpBank() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxUpBank::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUpBank.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();
}
void PopDialogBoxUpBank::onExit(){
	CCLayer::onExit();
}
/*
 * PopDialogBoxFirstRecharge.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxFirstRecharge.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxFirstRecharge::PopDialogBoxFirstRecharge()
{
}
PopDialogBoxFirstRecharge::~PopDialogBoxFirstRecharge() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxFirstRecharge::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxFirstRecharge.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	

	playAnimation();
}
void PopDialogBoxFirstRecharge::onExit(){
	CCLayer::onExit();
}
/*
 * PopDialogBoxRanking.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxRanking.h"
#include "DataModel.h"
#include "GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxRanking::PopDialogBoxRanking()
{
	
}
PopDialogBoxRanking::~PopDialogBoxRanking() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxRanking::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxRanking.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	imageBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	imageBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();
}
void PopDialogBoxRanking::onExit(){
	CCLayer::onExit();
}
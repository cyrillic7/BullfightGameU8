/*
 * PopDialogBoxShop.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxShop.h"
#include "DataModel.h"
#include "GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxShop::PopDialogBoxShop()
{
	
}
PopDialogBoxShop::~PopDialogBoxShop() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxShop::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxShop.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	imageBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	imageBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();
}
void PopDialogBoxShop::onExit(){
	CCLayer::onExit();
}
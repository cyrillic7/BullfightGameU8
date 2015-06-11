/*
 * PopDialogBoxSign.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxSign.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxSign::PopDialogBoxSign()
{
	
    
}
PopDialogBoxSign::~PopDialogBoxSign() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxSign::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxSign.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();
}
void PopDialogBoxSign::onExit(){
	CCLayer::onExit();
}
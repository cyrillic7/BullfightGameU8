/*
 * PopDialogBoxSetUp.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxSetUp.h"
#include "DataModel.h"
#include "GameConfig.h"
#include "BaseAttributes.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxSetUp::PopDialogBoxSetUp()
{
	
}
PopDialogBoxSetUp::~PopDialogBoxSetUp() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxSetUp::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxSetUp.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	imageBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	imageBg->setScale(0.8);

	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
}
void PopDialogBoxSetUp::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxSetUp::playAnimation(){
	imageBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
}
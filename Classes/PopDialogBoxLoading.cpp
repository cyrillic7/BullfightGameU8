/*
 * PopDialogBoxLoading.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxLoading.h"
PopDialogBoxLoading::PopDialogBoxLoading()
{
	
}
PopDialogBoxLoading::~PopDialogBoxLoading() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxLoading::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxLoading.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
}
void PopDialogBoxLoading::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxLoading::playAnimation(){
	imageBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
}
/*
 * PopDialogBoxSetUp.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxSetUp.h"
#include "Tools/DataModel.h"
#include "Tools/GameConfig.h"
#include "Tools/BaseAttributes.h"
#include "PopDialogBoxHelp.h"
#include "PopDialogBoxFeedback.h"
#include "PopDialogBoxAbout.h"
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

	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);

	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//帮助按键
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonHelp"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::menuHelp));
	//意见反馈
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonFeedback"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::menuFeedback));
	//关于我们
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonAbout"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::menuAbout));

	playAnimation();
}
void PopDialogBoxSetUp::onExit(){
	CCLayer::onExit();
}
//帮助
void PopDialogBoxSetUp::menuHelp(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxHelp *help=PopDialogBoxHelp::create();
		this->addChild(help);
	}
}
//意见反馈
void PopDialogBoxSetUp::menuFeedback(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxFeedback *feedback=PopDialogBoxFeedback::create();
		this->addChild(feedback);
	}
}
//关于我们
void PopDialogBoxSetUp::menuAbout(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxAbout *about=PopDialogBoxAbout::create();
		this->addChild(about);
	}
}
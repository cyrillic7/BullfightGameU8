/*
 * PopDialogBoxShop.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxShop.h"
#include "Tools/DataModel.h"
#include "Tools/GameConfig.h"
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
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *bg=CCSprite::create("res/shop_bg.jpg");
	pWidgetBg->addNode(bg,-1);
	bg->setPosition(CCPointZero);
	float scale=deviceSize.height/bg->getContentSize().height;
	bg->setScale(scale);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();
}
void PopDialogBoxShop::onExit(){
	CCLayer::onExit();
}
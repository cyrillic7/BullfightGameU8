/*
 * PopDialogBoxBank.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxBank.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxBank::PopDialogBoxBank()
{
	
}
PopDialogBoxBank::~PopDialogBoxBank() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxBank::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxBank.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	pLTitle0 = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelTitle0"));
	pLTitle1 = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelTitle1"));
	if (DataModel::sharedDataModel()->cbInsurePwd)
	{
		setTitle("创建银行密码");
	}
	else
	{
		setTitle("输入银行密码");
	}
	playAnimation();
}
void PopDialogBoxBank::onExit(){
	CCLayer::onExit();
}
//设置标题
void PopDialogBoxBank::setTitle(const char * sTitle){
	pLTitle0->setText((sTitle));
	pLTitle1->setText((sTitle));
}
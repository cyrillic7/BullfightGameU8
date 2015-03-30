/*
 * PopDialogBoxUserInfo.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxUserInfo.h"
#include "DataModel.h"
#include "GameConfig.h"
PopDialogBoxUserInfo::PopDialogBoxUserInfo()
{
	
}
PopDialogBoxUserInfo::~PopDialogBoxUserInfo() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxUserInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUserInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	imageBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	imageBg->setScale(0.8);

	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//设置游戏ID
	UILabel *labelUserID=static_cast<UILabel*>(pUILayer->getWidgetByName("LabelUserID"));
	labelUserID->setText(CCString::createWithFormat("ID:%d",DataModel::sharedDataModel()->logonSuccessUserInfo->dwGameID)->getCString());
	UILabel *pLabelNickName=static_cast<UILabel*>(pUILayer->getWidgetByName("LabelNickName"));
	pLabelNickName->setText(DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName);
	
	
}
void PopDialogBoxUserInfo::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxUserInfo::playAnimation(){
	imageBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
}
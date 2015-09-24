/*
 * PopDialogBoxUpdateTipInfo.cpp
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#include "PopDialogBoxUpdateTipInfo.h"
#include "../Platform/coPlatform.h"
PopDialogBoxUpdateTipInfo::PopDialogBoxUpdateTipInfo()
{
	// TODO Auto-generated constructor stub

}

PopDialogBoxUpdateTipInfo::~PopDialogBoxUpdateTipInfo() {
	// TODO Auto-generated destructor stub
	
}
void PopDialogBoxUpdateTipInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUpdateTipInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	
	//关闭
	pBClose = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonCancel"));
	pBClose->addTouchEventListener(this, toucheventselector(PopDialogBoxUpdateTipInfo::onMenuCloseTipInfo));

	UIButton *pBSure = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonSure"));
	pBSure->addTouchEventListener(this, toucheventselector(PopDialogBoxUpdateTipInfo::onMenuSure));

	pLInfo = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelInfo"));
	pLInfo->ignoreContentAdaptWithSize(true);
	pLInfo->setTextAreaSize(CCSize(400, 0));
	
	//////////////////////////////////////////////////////////////////////////
	pLInfo->setText(GBKToUTF8(uInfo.strUpdateContent.c_str()).c_str());
	float offsetY = 40 - pLInfo->getSize().height;
	CCSize cSize = CCSize(pWidgetBg->getContentSize().width, pWidgetBg->getContentSize().height - offsetY);
	pWidgetBg->setSize(cSize);
	pWidgetBg->setScale(0.8);
	playAnimation();
}
void PopDialogBoxUpdateTipInfo::onExit(){

	CCLayer::onExit();
}
//关闭////////////////////////////////////////////////////////////////////////
void PopDialogBoxUpdateTipInfo::onMenuCloseTipInfo(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		if (uInfo.iUpdateType==1)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
			CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
#endif
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}
//确定
void PopDialogBoxUpdateTipInfo::onMenuSure(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		std::string url = "{\"act\":700,\"updateUrl\":\"";
		url += uInfo.strUpdateUrl;
		url+="\"}";
		platformAction(url);
	}
}
//设置提示内容
void PopDialogBoxUpdateTipInfo::setTipInfoData(updateInfo upInfo){
	this->uInfo = upInfo;
}
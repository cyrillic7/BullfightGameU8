/*
 * PopDialogBoxOptTipInfo.cpp
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#include "PopDialogBoxOptTipInfo.h"
#include "../Platform/coPlatform.h"
PopDialogBoxOptTipInfo::PopDialogBoxOptTipInfo()
	:iHornMsgAssist(NULL)
{
}
PopDialogBoxOptTipInfo::~PopDialogBoxOptTipInfo() {
}
void PopDialogBoxOptTipInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUpdateTipInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	
	//关闭
	pBClose = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonCancel"));
	pBClose->addTouchEventListener(this, toucheventselector(PopDialogBoxOptTipInfo::onMenuCloseTipInfo));
	
	UIImageView *pIVTitle = static_cast<UIImageView*>(pUILayer->getWidgetByName("Image_8"));
	pIVTitle->loadTexture("TiShi_BiaoTi.png", UI_TEX_TYPE_PLIST);
	/*if (uInfo.iUpdateType == 1)
	{
		pBClose->loadTextureNormal("TuiChu_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		pBClose->loadTexturePressed("TuiChu_Btn_Down.png", UI_TEX_TYPE_PLIST);
	}
	else
	{
		pBClose->loadTextureNormal("QuXiao_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		pBClose->loadTexturePressed("QuXiao_Btn_Down.png", UI_TEX_TYPE_PLIST);
	}*/

	UIButton *pBSure = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonSure"));
	pBSure->addTouchEventListener(this, toucheventselector(PopDialogBoxOptTipInfo::onMenuSure));

	pLInfo = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelInfo"));
	pLInfo->ignoreContentAdaptWithSize(true);
	pLInfo->setTextAreaSize(CCSize(400, 0));
	
	//////////////////////////////////////////////////////////////////////////
	//
	pWidgetBg->setScale(0.8);
	playAnimation();
}
void PopDialogBoxOptTipInfo::onExit(){
	CCLayer::onExit();
}
//关闭////////////////////////////////////////////////////////////////////////
void PopDialogBoxOptTipInfo::onMenuCloseTipInfo(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
//确定
void PopDialogBoxOptTipInfo::onMenuSure(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		if (iHornMsgAssist)
		{
			iHornMsgAssist->onSendSure(this);
			//this->removeFromParentAndCleanup(true);
		}
	}
}
//设置提示内容
void PopDialogBoxOptTipInfo::setTipInfoData(std::string strContent){
	this->strContent = strContent;
	pLInfo->setText(strContent.c_str());
	float offsetY = 40 - pLInfo->getSize().height;
	CCSize cSize = CCSize(pWidgetBg->getContentSize().width, pWidgetBg->getContentSize().height - offsetY);
	pWidgetBg->setSize(cSize);
}
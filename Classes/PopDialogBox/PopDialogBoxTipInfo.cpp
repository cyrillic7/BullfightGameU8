/*
 * PopDialogBoxTipInfo.cpp
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#include "PopDialogBoxTipInfo.h"

PopDialogBoxTipInfo::PopDialogBoxTipInfo() {
	// TODO Auto-generated constructor stub

}

PopDialogBoxTipInfo::~PopDialogBoxTipInfo() {
	// TODO Auto-generated destructor stub
}
void PopDialogBoxTipInfo::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxTipInfo.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));

	pLInfo = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelInfo"));
	pLInfo->ignoreContentAdaptWithSize(true);
	pLInfo->setTextAreaSize(CCSize(400, 0));
	
}
void PopDialogBoxTipInfo::onExit(){
	CCLayer::onExit();
}
//设置提示内容
void PopDialogBoxTipInfo::setTipInfoContent(const char *content){
	pLInfo->setText(content);
	float offsetY = 80-pLInfo->getSize().height;
	CCSize cSize = CCSize(pWidgetBg->getContentSize().width,pWidgetBg->getContentSize().height-offsetY);
	pWidgetBg->setSize(cSize);

	pWidgetBg->setScale(0.8);
	playAnimation();
}
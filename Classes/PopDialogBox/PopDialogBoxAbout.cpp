/*
 * PopDialogBoxAbout.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxAbout.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxAbout::PopDialogBoxAbout()
{
	readAboutText();
}
PopDialogBoxAbout::~PopDialogBoxAbout() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxAbout::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxAbout.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	pSVAbout = static_cast<UIScrollView*>(pUILayer->getWidgetByName("ScrollViewAbout"));

	//帮助内容
	pLAboutText = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelAboutContent"));
	pLAboutText->ignoreContentAdaptWithSize(true);
	pLAboutText->setTextAreaSize(CCSize(614, 0));

	pLAboutText->setText(sAboutContent.c_str());

	pSVAbout->setInnerContainerSize(pLAboutText->getContentSize());
	pSVAbout->scrollToTop(0.1, false);

	playAnimation();
}
void PopDialogBoxAbout::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxAbout::readAboutText(){
	CCString *tempStr = CCString::createWithFormat("data/aboutText.plist");;
	CCDictionary *tempDic = CCDictionary::createWithContentsOfFile(tempStr->getCString());
	
	sAboutContent = dynamic_cast<CCString *>(tempDic->objectForKey("aboutText"))->getCString();

	tempDic->removeAllObjects();
	CC_SAFE_RELEASE_NULL(tempDic);
}
/*
 * PopDialogBoxMakeText.cpp
 *
 *  Created on: 2015年5月30日
 *      Author: xw007
 */

#include "PopDialogBoxMakeText.h"
#include "../Platform/coPlatform.h"
PopDialogBoxMakeText::PopDialogBoxMakeText()
{
	// TODO Auto-generated constructor stub

}

PopDialogBoxMakeText::~PopDialogBoxMakeText() {
	// TODO Auto-generated destructor stub
	
}
void PopDialogBoxMakeText::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxMakeText.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	pIVBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageBg"));
	pIVBg->setCascadeOpacityEnabled(true);
	pLContent = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelMakeText"));
	pLContent->setCascadeOpacityEnabled(true);
	/*pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	
	//关闭
	pBClose = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonCancel"));
	pBClose->addTouchEventListener(this, toucheventselector(PopDialogBoxMakeText::onMenuCloseTipInfo));
	if (uInfo.iUpdateType == 1)
	{
		pBClose->loadTextureNormal("TuiChu_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		pBClose->loadTexturePressed("TuiChu_Btn_Down.png", UI_TEX_TYPE_PLIST);
	}
	else
	{
		pBClose->loadTextureNormal("QuXiao_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		pBClose->loadTexturePressed("QuXiao_Btn_Down.png", UI_TEX_TYPE_PLIST);
	}

	UIButton *pBSure = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonSure"));
	pBSure->addTouchEventListener(this, toucheventselector(PopDialogBoxMakeText::onMenuSure));

	
	pLInfo->ignoreContentAdaptWithSize(true);
	pLInfo->setTextAreaSize(CCSize(400, 0));
	
	//////////////////////////////////////////////////////////////////////////
	pLInfo->setText(GBKToUTF8(uInfo.strUpdateContent.c_str()).c_str());
	float offsetY = 40 - pLInfo->getSize().height;
	CCSize cSize = CCSize(pWidgetBg->getContentSize().width, pWidgetBg->getContentSize().height - offsetY);
	pWidgetBg->setSize(cSize);
	pWidgetBg->setScale(0.8);
	playAnimation();*/
}
void PopDialogBoxMakeText::onExit(){

	CCLayer::onExit();
}
//设置提示内容
void PopDialogBoxMakeText::showMakeText(std::string sContents){
	pLContent->setText(sContents.c_str());
	pIVBg->setSize(pLContent->getContentSize() + ccp(20, 20));
	int time = 3;
	CCSequence *seq = CCSequence::create(
		CCFadeOut::create(time),
		CCCallFunc::create(this, SEL_CallFunc(&PopDialogBoxMakeText::removeSelf)),
		NULL);
	pIVBg->runAction(seq);

	CCSequence *seq1 = CCSequence::create(
		CCFadeOut::create(time),
		NULL);
	pLContent->runAction(seq1);
}
//向上移动plane
void PopDialogBoxMakeText::movePlane(int y){
	pIVBg->setPositionY(pIVBg->getPositionY() + y);
}
//移除自己
void PopDialogBoxMakeText::removeSelf(){
	DataModel::sharedDataModel()->m_aMakeText->removeObject(this);
	this->removeFromParentAndCleanup(true);
}
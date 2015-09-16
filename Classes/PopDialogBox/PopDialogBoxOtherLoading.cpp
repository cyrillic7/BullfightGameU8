/*
 * PopDialogBoxOtherLoading.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */
#include "PopDialogBoxOtherLoading.h"
#include "../Tools/BaseAttributes.h"
#include "../Tools/DataModel.h"
PopDialogBoxOtherLoading::PopDialogBoxOtherLoading()
{
	timerIndex = 0;
}
PopDialogBoxOtherLoading::~PopDialogBoxOtherLoading() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxOtherLoading::onEnter(){
	CCLayer::onEnter();
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationLoading.ExportJson));
	CCArmature *pArmature= CCArmature::create("AnimationLoading");
	this->addChild(pArmature);
	CCPoint pos = ccp(DataModel::sharedDataModel()->deviceSize.width/2,DataModel::sharedDataModel()->deviceSize.height/2+15);
	pArmature->setPosition(pos);
	pArmature->getAnimation()->play("AnimationLoading");

	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxLoading.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	pLLoading = static_cast<UILabel *>(pUILayer->getWidgetByName("LabelLoading"));
	pLLoading->setPositionY(pArmature->getPositionY()-pArmature->getContentSize().height/2-30);
	pLLoading->setVisible(false);
	
	//pLLoading->setText("正在转入.");
	//schedule(SEL_SCHEDULE(&PopDialogBoxOtherLoading::updateLoadingLabel),0.5);
}
void PopDialogBoxOtherLoading::onExit(){
	CCLayer::onExit();
	//unschedule(SEL_SCHEDULE(&PopDialogBoxOtherLoading::updateLoadingLabel));
}
void PopDialogBoxOtherLoading::playAnimation(){
	pWidgetBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
}
//更新加载文字动画
void PopDialogBoxOtherLoading::updateLoadingLabel(float dt){
	if (timerIndex <1)
	{
		pLLoading->setText(CCString::createWithFormat("%s.",BaseAttributes::sharedAttributes()->sLoading.c_str())->getCString());
	}
	else if (timerIndex <2)
	{
		pLLoading->setText(CCString::createWithFormat("%s..", BaseAttributes::sharedAttributes()->sLoading.c_str())->getCString());
	}
	else if (timerIndex < 3){
		pLLoading->setText(CCString::createWithFormat("%s...", BaseAttributes::sharedAttributes()->sLoading.c_str())->getCString());
	}
	else if (timerIndex < 4){
		timerIndex = 0;
		return;
	}
	
	timerIndex++;
}
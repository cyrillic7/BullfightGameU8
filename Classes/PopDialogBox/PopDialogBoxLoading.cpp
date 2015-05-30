/*
 * PopDialogBoxLoading.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxLoading.h"
#include "../Tools/DataModel.h"
#include "../Tools/Tools.h"
#include "../Tools/BaseAttributes.h"
#include "../Network/TCPSocket/TCPSocketControl.h"
#include "PopDialogBoxTipInfo.h"
#define OUT_TIME			10				//超时时长
PopDialogBoxLoading::PopDialogBoxLoading()
{
	timerIndex = 0;
}
PopDialogBoxLoading::~PopDialogBoxLoading() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxLoading::onEnter(){
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
	
	//pLLoading->setText("正在转入.");
	schedule(SEL_SCHEDULE(&PopDialogBoxLoading::updateLoadingLabel),0.5);
	scheduleOnce(SEL_SCHEDULE(&PopDialogBoxLoading::outTimeExit), OUT_TIME);
}
void PopDialogBoxLoading::onExit(){
	CCLayer::onExit();
	unschedule(SEL_SCHEDULE(&PopDialogBoxLoading::updateLoadingLabel));
}

//超时关闭
void PopDialogBoxLoading::outTimeExit(float dt){
	//关闭网络
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(sSocketName);
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->getParent()->addChild(pTipInfo,100);
	pTipInfo->setTipInfoContent(GBKToUTF8("连接超时！"));

	this->removeFromParentAndCleanup(true);
}
void PopDialogBoxLoading::playAnimation(){
	pWidgetBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
}
//更新加载文字动画
void PopDialogBoxLoading::updateLoadingLabel(float dt){
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
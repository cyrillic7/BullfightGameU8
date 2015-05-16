/*
 * PopDialogBoxRanking.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxRanking.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxRanking::PopDialogBoxRanking()
{
	
}
PopDialogBoxRanking::~PopDialogBoxRanking() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxRanking::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxRanking.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//总财富榜、昨日赚金榜 复选框
	for (int i=0;i<2;i++)
	{
		pcbRanking[i]=static_cast<UICheckBox*>(pUILayer->getWidgetByName(CCString::createWithFormat("CheckBoxRanking%d",i)->getCString()));
		pcbRanking[i]->addEventListenerCheckBox(this,SEL_SelectedStateEvent(&PopDialogBoxRanking::onCheckBoxSelectedStateEvent));
	}
	pcbRanking[0]->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pcbRanking[0],CHECKBOX_STATE_EVENT_SELECTED);
	
	playAnimation();
}
void PopDialogBoxRanking::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxRanking::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
		{
			UICheckBox *checkBox=(UICheckBox*)pSender;
			if (strcmp(checkBox->getName(),"CheckBoxRanking0")==0)
			{
				pcbRanking[1]->setSelectedState(false);
				pcbRanking[0]->setTouchEnabled(false);
				pcbRanking[1]->setTouchEnabled(true);
				UILabel *name=(UILabel*)pcbRanking[0]->getChildByName("LabelButtonName");
				name->setColor(ccc3(255,255,255));
				name=(UILabel*)pcbRanking[1]->getChildByName("LabelButtonName");
				name->setColor(ccc3(112,54,8));
			}
			else if (strcmp(checkBox->getName(),"CheckBoxRanking1")==0){
				pcbRanking[0]->setSelectedState(false);
				pcbRanking[0]->setTouchEnabled(true);
				pcbRanking[1]->setTouchEnabled(false);
				UILabel *name=(UILabel*)pcbRanking[0]->getChildByName("LabelButtonName");
				name->setColor(ccc3(112,54,8));
				name=(UILabel*)pcbRanking[1]->getChildByName("LabelButtonName");
				name->setColor(ccc3(255,255,255));
			}
			
		}
		break;
	case CHECKBOX_STATE_EVENT_UNSELECTED:
		{

		}
		break;
	default:
		break;
	}
}
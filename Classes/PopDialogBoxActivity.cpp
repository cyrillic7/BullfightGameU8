/*
 * PopDialogBoxActivity.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxActivity.h"
#include "DataModel.h"
#include "GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxActivity::PopDialogBoxActivity()
{
	
}
PopDialogBoxActivity::~PopDialogBoxActivity() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxActivity::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxActivity.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//获取打赏列表
	plvReward=static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewActivity"));
	//获取登录Panel
	ppLogonReward=static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelReward"));
	//登录活动、打赏活动 复选框
	for (int i=0;i<2;i++)
	{
		pcbActivity[i]=static_cast<UICheckBox*>(pUILayer->getWidgetByName(CCString::createWithFormat("CheckBoxActivity%d",i)->getCString()));
		pcbActivity[i]->addEventListenerCheckBox(this,SEL_SelectedStateEvent(&PopDialogBoxActivity::onCheckBoxSelectedStateEvent));
	}
	pcbActivity[0]->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pcbActivity[0],CHECKBOX_STATE_EVENT_SELECTED);
	
	playAnimation();
}
void PopDialogBoxActivity::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxActivity::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
		{
			UICheckBox *checkBox=(UICheckBox*)pSender;
			if (strcmp(checkBox->getName(),"CheckBoxActivity0")==0)
			{
				plvReward->setEnabled(false);
				ppLogonReward->setEnabled(true);
				pcbActivity[1]->setSelectedState(false);
				pcbActivity[0]->setTouchEnabled(false);
				pcbActivity[1]->setTouchEnabled(true);
				UILabel *name=(UILabel*)pcbActivity[0]->getChildByName("LabelButtonName");
				name->setColor(ccc3(255,255,255));
				name=(UILabel*)pcbActivity[1]->getChildByName("LabelButtonName");
				name->setColor(ccc3(112,54,8));
			}
			else if (strcmp(checkBox->getName(),"CheckBoxActivity1")==0){
				plvReward->setEnabled(true);
				ppLogonReward->setEnabled(false);
				pcbActivity[0]->setSelectedState(false);
				pcbActivity[0]->setTouchEnabled(true);
				pcbActivity[1]->setTouchEnabled(false);
				UILabel *name=(UILabel*)pcbActivity[0]->getChildByName("LabelButtonName");
				name->setColor(ccc3(112,54,8));
				name=(UILabel*)pcbActivity[1]->getChildByName("LabelButtonName");
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
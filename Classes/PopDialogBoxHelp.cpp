/*
 * PopDialogBoxHelp.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxHelp.h"
#include "DataModel.h"
#include "GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxHelp::PopDialogBoxHelp()
{
	
}
PopDialogBoxHelp::~PopDialogBoxHelp() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxHelp::onEnter(){
	CCLayer::onEnter();
	//读取帮助内容
	readHelpText();

	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxHelp.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	imageBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	imageBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//帮助内容
	plHelpText = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelHelpText"));
	plHelpText->ignoreContentAdaptWithSize(true);
	plHelpText->setTextAreaSize(CCSize(409,0));
	
	scrollViewHelp=static_cast<UIScrollView*>(pUILayer->getWidgetByName("ScrollViewHelp"));
	
	for (int i = 0; i < MAX_HELP_COUNT; i++)
	{
		pcbHelp[i]=static_cast<UICheckBox*>(pUILayer->getWidgetByName(CCString::createWithFormat("CheckBoxHelp%d",i+1)->getCString()));
		pcbHelp[i]->addEventListenerCheckBox(this,SEL_SelectedStateEvent(&PopDialogBoxHelp::onCheckBoxSelectedStateEvent));
	}
	pcbHelp[0]->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pcbHelp[0],CHECKBOX_STATE_EVENT_SELECTED);
	
	//updateContent(0);
	playAnimation();
}
void PopDialogBoxHelp::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxHelp::readHelpText(){
	CCString *tempStr = CCString::createWithFormat("data/helpText.plist");;
	CCArray *tempArray = CCArray::createWithContentsOfFile(tempStr->getCString());
	for (int i = 0; i < tempArray->count(); i++) {
		CCDictionary* mDic = dynamic_cast<CCDictionary *>(tempArray->objectAtIndex(i));
		helpText[i] = dynamic_cast<CCString *>(mDic->objectForKey("help"))->getCString();
	}
	tempArray->removeAllObjects();
	CC_SAFE_RELEASE_NULL(tempArray);
}
//复选框回调（选择性别）
void PopDialogBoxHelp::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
		{
			UICheckBox *box=(UICheckBox*)pSender;
			int index=box->getTag()-1;
			updateContent(index);
			for (int i = 0; i < MAX_HELP_COUNT; i++)
			{
				if (index!=i)
				{
					pcbHelp[i]->setSelectedState(false);
					pcbHelp[i]->setTouchEnabled(true);
				}else
				{
					pcbHelp[i]->setTouchEnabled(false);
				}
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
void PopDialogBoxHelp::updateContent(int index){
	plHelpText->setText(helpText[index]);
	scrollViewHelp->setInnerContainerSize(plHelpText->getContentSize());
}
/*
 * PopDialogBoxMore.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxMore.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Platform/coPlatform.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxMore::PopDialogBoxMore()
{
	
    
}
PopDialogBoxMore::~PopDialogBoxMore() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxMore::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxMore.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//更多列表
	pLVMoreList = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewMore"));
	//设置cell模式
	pLVMoreList->setItemModel(pLVMoreList->getItem(0));
	pLVMoreList->removeAllItems();

	vecMoreInfo.push_back("sdd");
	updateListMore();

	playAnimation();
}
void PopDialogBoxMore::onExit(){
	CCLayer::onExit();
}
//打开游戏
void PopDialogBoxMore::onMenuOpenGame(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pButton = (UIButton*)object;
		int tag = pButton->getTag();
		
		CCString *sAction = CCString::createWithFormat("{\"act\":500 ,\"packageName\":\"%s\",\"activity\":\"%s\"}", "com.foxgame.ForestGuard", "ForestGuard");
		platformAction(sAction->getCString());
	}
}
//更新更多游戏列表
void PopDialogBoxMore::updateListMore(){
	UIListView *pLVTemp = pLVMoreList;
	pLVTemp->removeAllItems();

	int tempSize = vecMoreInfo.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		
		//按键
		UIButton *pButton = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonOpenGame"));
		pButton->setTag(inserterPos);
		pButton->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxMore::onMenuOpenGame));
	}
}
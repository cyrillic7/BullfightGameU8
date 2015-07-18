/*
 * PopDialogBoxGISetting.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxGISetting.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxGISetting::PopDialogBoxGISetting()
{
}
PopDialogBoxGISetting::~PopDialogBoxGISetting() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxGISetting::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxSetting.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	//音乐
	UIButton *pBMusic = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonMusic"));
	pBMusic->addTouchEventListener(this, toucheventselector(PopDialogBoxGISetting::onMenuMusic));
	changeMusicButtonState(pBMusic, DataModel::isMusic);
	//音效
	UIButton *pBSound = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonSound"));
	pBSound->addTouchEventListener(this, toucheventselector(PopDialogBoxGISetting::onMenuSound));
	changeMusicButtonState(pBSound, DataModel::isSound);

	playAnimation();
}
void PopDialogBoxGISetting::onExit(){
	CCLayer::onExit();
}

//音乐开关
void PopDialogBoxGISetting::onMenuMusic(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		DataModel::isMusic = !DataModel::isMusic;
		if (DataModel::isMusic)
		{
		}
		else
		{

		}
		changeMusicButtonState((UIButton*)object, DataModel::isMusic);
		Tools::saveBoolByRMS(RMS_IS_MUSIC, DataModel::isMusic);
	}
}
//音效开关
void PopDialogBoxGISetting::onMenuSound(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		DataModel::isSound = !DataModel::isSound;
		if (DataModel::isSound)
		{
		}
		else
		{

		}
		changeMusicButtonState((UIButton*)object, DataModel::isSound);
		Tools::saveBoolByRMS(RMS_IS_SOUND, DataModel::isSound);
	}
}

//更改音乐状态
void PopDialogBoxGISetting::changeMusicButtonState(UIButton *pButton, bool isOpen){
	if (isOpen)
	{
		pButton->loadTextureNormal("on.png", UI_TEX_TYPE_PLIST);
		pButton->loadTexturePressed("on.png", UI_TEX_TYPE_PLIST);
	}
	else
	{
		pButton->loadTextureNormal("off.png", UI_TEX_TYPE_PLIST);
		pButton->loadTexturePressed("off.png", UI_TEX_TYPE_PLIST);
	}
}